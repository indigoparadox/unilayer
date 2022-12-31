
#define GRAPHICS_PLATFORM_C
#define NDSG_C
#include "../unilayer.h"

#define BG_TILE_W_PX 8
#define BG_TILE_H_PX 8
#define BG_W_TILES 32

static uint16_t* g_sprite_frames[GRAPHICS_SPRITES_ONSCREEN];
static const struct GRAPHICS_BITMAP* g_oam_entries[GRAPHICS_SPRITES_ONSCREEN];
static int16_t g_oam_x[GRAPHICS_SPRITES_ONSCREEN];
static int16_t g_oam_y[GRAPHICS_SPRITES_ONSCREEN];
static OamState* g_oam_active = NULL;

static const struct GRAPHICS_BITMAP* g_bg_bmp = NULL;
static const struct GRAPHICS_BITMAP* g_window_bmp = NULL;
static int g_bg_id = 0;
static int g_window_id = 0;
static int g_px_id = 0;
static uint16_t g_bg_tiles[1024];
static uint16_t g_window_tiles[1024];

const uint32_t gc_ms_target = 1000 / FPS;
static uint32_t g_ms_start = 0;

/*
 * @return 1 if init was successful and 0 otherwise.
 */
int16_t graphics_platform_init() {
   int i = 0;

   powerOn( POWER_ALL );
   
   videoSetMode( MODE_5_2D );
	videoSetModeSub( MODE_0_2D );

   /* Setup the upper screen for background and sprites. */
	vramSetBankA( VRAM_A_MAIN_BG );
	vramSetBankB( VRAM_B_MAIN_SPRITE );

   /* Setup the lower screen for background and sprites. */
	vramSetBankC( VRAM_C_MAIN_BG );
	vramSetBankD( VRAM_D_SUB_SPRITE );

   bgExtPaletteEnable();

   /* Setup the background engine. */

   /* Put map at base 2, but stow tiles up after the bitmap BG at base 7. */
   g_bg_id = bgInit( 0, BgType_Text8bpp, BgSize_T_256x256, 2, 7 );
   dmaFillWords( 0, g_bg_tiles, sizeof( g_bg_tiles ) );
   bgSetPriority( g_bg_id, 2 );

   /* Put map at base 3, and tiles at base 0. */
   g_window_id = bgInit( 1, BgType_Text8bpp, BgSize_T_256x256, 3, 0 );
   dmaFillWords( 0, g_window_tiles, sizeof( g_window_tiles ) );
   bgSetPriority( g_window_id, 1 );

   /* Put bitmap BG at base 1, leaving map-addressable space at base 0. */
   g_px_id = bgInit( 2, BgType_Bmp16, BgSize_B16_256x256, 1, 0 );
   bgSetPriority( g_px_id, 0 );

   /* Setup the sprite engines. */
	oamInit( &oamMain, SpriteMapping_1D_128, 0 );
	oamInit( &oamSub, SpriteMapping_1D_128, 0 );
   g_oam_active = &oamMain;

   /* Allocate sprite frame memory. */
   for( i = 0 ; GRAPHICS_SPRITES_ONSCREEN > i ; i++ ) {
      g_sprite_frames[i] = oamAllocateGfx(
         &oamMain, SpriteSize_16x16, SpriteColorFormat_256Color );
   }

   /* Setup the timer. */
   TIMER0_CR = TIMER_ENABLE | TIMER_DIV_1024;
   TIMER1_CR = TIMER_ENABLE | TIMER_CASCADE;

   /* glScreen2D(); */

   return 1;
}

void graphics_platform_shutdown() {
   /* TODO */
}

void graphics_lock() {
#ifdef NDS_BG_PX_CLEAR
   uint16_t* px_ptr = NULL;

   /* Clear px layer as well. */
   px_ptr = bgGetGfxPtr( g_px_id );
   dmaFillWords( 0, px_ptr, 256 * 256 );
#endif /* NDS_BG_PX_CLEAR */

   /* Clear sprites at the start of the loop. */
   oamClear( g_oam_active, 0, 0 );
}

void graphics_release() {

   /* Setup bank E to receive extended palettes. */
   /* TODO: Palette/BMP dirty bit so we don't have to keep copying. */
   vramSetBankE( VRAM_E_LCD );

   /* Update background tiles. */
   if( NULL != g_bg_tiles && NULL != g_bg_bmp ) {
      dmaCopy(
         g_bg_bmp->id->tiles, bgGetGfxPtr( g_bg_id ), g_bg_bmp->id->tiles_sz );
      dmaCopy( g_bg_tiles, bgGetMapPtr( g_bg_id ),  sizeof( g_bg_tiles ) );

      /* Using extended palettes as a workaround for ImageMagick palette
       * issues.
       */
      dmaCopy( g_bg_bmp->id->palette, &VRAM_E_EXT_PALETTE[0][0],
         g_bg_bmp->id->palette_sz );
   }

   /* Update window tiles. */
   if( NULL != g_window_tiles && NULL != g_window_bmp ) {
      dmaCopy( g_window_bmp->id->tiles, bgGetGfxPtr( g_window_id ),
         g_window_bmp->id->tiles_sz );
      dmaCopy( g_window_tiles, bgGetMapPtr( g_window_id ),
         sizeof( g_window_tiles ) );

      /* Using extended palettes as a workaround for ImageMagick palette
       * issues.
       */
      dmaCopy( g_window_bmp->id->palette, &VRAM_E_EXT_PALETTE[1][0],
         g_window_bmp->id->palette_sz );
   }

   /* Tell bank E it can use the extended palettes, now. */
   vramSetBankE( VRAM_E_BG_EXT_PALETTE );

   /* Update sprite engines. */
   oamUpdate( &oamMain );
   oamUpdate( &oamSub );
}

int16_t graphics_get_random( int16_t start, int16_t range ) {
   /* TODO */
   return 0;
}

uint32_t graphics_get_ms() {
   return ((TIMER1_DATA * (1 << 16)) + TIMER0_DATA) / 32;
}

void graphics_loop_start() {
   g_ms_start = graphics_get_ms();
}

void graphics_loop_end() {
   int16_t delta = 0;
   
   /* Wait for FPS timer. */
   do {
      swiWaitForVBlank();
      delta = gc_ms_target - (graphics_get_ms() - g_ms_start);
   } while( 0 < delta );  
}

void graphics_draw_px( uint16_t x, uint16_t y, const GRAPHICS_COLOR color ) {
   uint16_t* px_ptr = NULL;

   px_ptr = bgGetGfxPtr( g_px_id );
   px_ptr[(y * 256) + x] = color;
}

int16_t graphics_platform_blit_partial_at(
   const struct GRAPHICS_BITMAP* bmp, int16_t instance_id,
   uint16_t s_x, uint16_t s_y,
   uint16_t d_x, uint16_t d_y, uint16_t w, uint16_t h
) {
   int tile_idx = 0,
      tile_x = 0,
      tile_y = 0;
   uint16_t* bg_tiles = NULL;

   /* TODO: Draw crops and items. */

   if( 0 <= instance_id && GRAPHICS_SPRITES_ONSCREEN > instance_id ) {
      /* Blitting a sprite. */

      /* TODO: Take s_x/s_y into account for caching! */
#if 0
      if( g_oam_entries[instance_id] == bmp ) {
         /* Bitmap already loaded, so just move it and avoid a dmaCopy(). */
         oamSetXY( g_oam_active, instance_id, d_x, d_y );
         g_oam_x[instance_id] = d_x;
         g_oam_y[instance_id] = d_y;
         return 1;
      }
#endif

      /* On the DS, the RESOURCE_ID in bmp->id that would be a filename or w/e
       * on other platforms is just a pointer directly to the data from the
       * structs/arrays assembled by grit in Makefile.nds. So just pull right
       * from there.
       */
      dmaCopy( bmp->id->palette, SPRITE_PALETTE, bmp->id->palette_sz );

      /* 2 = spritesheet width of one row in sprites. */
      tile_idx = ((s_y / SPRITE_H) * 2) + (s_x / SPRITE_W);
      dmaCopy(
         bmp->id->tiles + (tile_idx * (BG_TILE_W_PX * BG_TILE_H_PX)),
         g_sprite_frames[instance_id], (TILE_W * TILE_H) );

      oamSet(
         g_oam_active, instance_id, d_x, d_y, 0, 0,
         SpriteSize_16x16, SpriteColorFormat_256Color, 
         g_sprite_frames[instance_id], -1, false, false, false, false, false );

   } else if(
      GRAPHICS_INSTANCE_TILEMAP == instance_id ||
      GRAPHICS_INSTANCE_WINDOW == instance_id
   ) {
      /* Blitting a background or window tile. */
      if( GRAPHICS_INSTANCE_TILEMAP == instance_id ) {
         bg_tiles = g_bg_tiles;
         g_bg_bmp = bmp;
      } else {
         bg_tiles = g_window_tiles;
         g_window_bmp = bmp;
      }

      /* DS tiles are 8x8, so each tile is split up into 4, so compensate! */
      tile_idx = bmp->id->tile_offset * 4;

      /* The tile's physical location on the tilemap. */
      /* Divide by 8 rather than 16 since DS tiles are 8x8. */
      tile_y = d_y / BG_TILE_H_PX;
      tile_x = d_x / BG_TILE_W_PX;

      if( GRAPHICS_INSTANCE_TILEMAP == instance_id ) {
         /* Hide window tiles if a tilemap tile was drawn more recently. */
         g_window_tiles[(tile_y * BG_W_TILES) + tile_x] = 0;
         g_window_tiles[(tile_y * BG_W_TILES) + tile_x + 1] = 0;
         g_window_tiles[((tile_y + 1) * BG_W_TILES) + tile_x] = 0;
         g_window_tiles[((tile_y + 1) * BG_W_TILES) + tile_x + 1] = 0;

         /* Fill block with transparency on px layer in front. */
         graphics_draw_block( d_x, d_y, TILE_W, TILE_H, 0 );
      }

      bg_tiles[(tile_y * BG_W_TILES) + tile_x] = tile_idx;
      bg_tiles[(tile_y * BG_W_TILES) + tile_x + 1] = tile_idx + 1;
      bg_tiles[((tile_y + 1) * BG_W_TILES) + tile_x] = tile_idx + 2;
      bg_tiles[((tile_y + 1) * BG_W_TILES) + tile_x + 1] = tile_idx + 3;
   }

   return 1;
}

void graphics_nds_clear_region(
   uint16_t x_orig, uint16_t y_orig, uint16_t w, uint16_t h
) {
   int16_t i = 0,
      tile_x = 0,
      tile_y = 0,
      x = 0,
      y = 0;

   assert( 0 == x_orig % TILE_W );
   assert( 0 == y_orig % TILE_H );

   /* Zero out bitmap region. */
   graphics_draw_block( x_orig, y_orig, w, h, 0 );

   /* Clear background in the region. */
   for( y = y_orig ; y_orig + h > y ; y += TILE_H ) {
      for( x = x_orig ; x_orig + w > x ; x += TILE_W ) {
         tile_y = y / BG_TILE_H_PX;
         tile_x = x / BG_TILE_W_PX;

         /* Erase window tiles. */
         g_window_tiles[(tile_y * BG_W_TILES) + tile_x] = 0;
         g_window_tiles[(tile_y * BG_W_TILES) + tile_x + 1] = 0;
         g_window_tiles[((tile_y + 1) * BG_W_TILES) + tile_x] = 0;
         g_window_tiles[((tile_y + 1) * BG_W_TILES) + tile_x + 1] = 0;

         /* Erase tilemap tiles. */
         g_bg_tiles[(tile_y * BG_W_TILES) + tile_x] = 0;
         g_bg_tiles[(tile_y * BG_W_TILES) + tile_x + 1] = 0;
         g_bg_tiles[((tile_y + 1) * BG_W_TILES) + tile_x] = 0;
         g_bg_tiles[((tile_y + 1) * BG_W_TILES) + tile_x + 1] = 0;
      }
   }

   /* Clear out sprites in the region. */
   for( i = 0 ; GRAPHICS_SPRITES_ONSCREEN > i ; i++ ) {
      if(
         NULL != g_oam_entries[i] &&
         x_orig < g_oam_x[i] &&
         x_orig + w > g_oam_x[i] &&
         y_orig < g_oam_y[i] &&
         y_orig + h > g_oam_y[i]
      ) {
         /* Sprite in the cleared region, so hide! */
         oamSetHidden( g_oam_active, i, true );
      }
   }
}

void graphics_draw_block(
   uint16_t x_orig, uint16_t y_orig, uint16_t w, uint16_t h,
   GRAPHICS_COLOR color
) {
   uint16_t* px_ptr = NULL;
   int16_t x = 0,
      y = 0;

   px_ptr = bgGetGfxPtr( g_px_id );
   for( y = y_orig ; y < y_orig + h ; y++ ) {
      for( x = x_orig ; x < x_orig + w ; x++ ) {
         px_ptr[(y * 256) + x] = color;
      }
      /* dmaFillHalfWords( color, &(px_ptr[(y * 256) + x_orig]), w ); */
   }
}

#ifndef USE_SOFTWARE_LINES

void graphics_draw_rect(
   uint16_t x_orig, uint16_t y_orig, uint16_t w, uint16_t h,
   uint16_t thickness, const GRAPHICS_COLOR color
) {
   /* TODO */
}

void graphics_draw_line(
   uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t thickness,
   const GRAPHICS_COLOR color
) {
   /* TODO */
}

#endif /* !USE_SOFTWARE_LINES */

int16_t graphics_platform_load_bitmap(
   RESOURCE_HANDLE res_handle, struct GRAPHICS_BITMAP* b
) {
   /* TODO */
}

int16_t graphics_platform_unload_bitmap( struct GRAPHICS_BITMAP* b ) {
   /* TODO */
   /* If we're unloading one then we're probably unloading them all? */
   /* oamClear( g_oam_active, 0, 0 ); */
}

#ifndef USE_SOFTWARE_TEXT
#error "platform nds does not support hardware text"
#endif /* !USE_SOFTWARE_TEXT */

