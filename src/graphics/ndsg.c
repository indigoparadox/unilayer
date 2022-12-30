
#define GRAPHICS_PLATFORM_C
#define NDSG_C
#include "../unilayer.h"

# define BG_TILE_TW 32
# define BG_TILE_TH 32

static uint16_t* g_sprite_frames[GRAPHICS_SPRITES_ONSCREEN];

static const struct GRAPHICS_BITMAP* g_bg_bmp = NULL;
static const struct GRAPHICS_BITMAP* g_window_bmp = NULL;
static int g_bg_id = 0;
static int g_window_id = 0;
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
   
   videoSetMode( MODE_0_2D );
	videoSetModeSub( MODE_0_2D );

   /* Setup the upper screen for background and sprites. */
	vramSetBankA( VRAM_A_MAIN_BG );
	vramSetBankB( VRAM_A_MAIN_SPRITE );

   /* Setup the lower screen for background and sprites. */
	vramSetBankC( VRAM_C_MAIN_BG );
	vramSetBankD( VRAM_D_SUB_SPRITE );

   bgExtPaletteEnable();

   /* Setup the background engine. */

   /* Put map at base 7, after tiles at base 0, leaving room for 224 tiles. */
   g_bg_id = bgInit( 0, BgType_Text8bpp, BgSize_T_256x256, 7, 0 );
   memset( g_bg_tiles, 0, sizeof( g_bg_tiles ) );

   /* Put window map at base 10, after tiles at base 1, leaving room for 64. */
   g_window_id = bgInit( 1, BgType_Text8bpp, BgSize_T_256x256, 9, 2 );
   memset( g_window_tiles, 0, sizeof( g_window_tiles ) );

   /* Setup the sprite engines. */
	oamInit( &oamMain, SpriteMapping_1D_128, 0 );
	oamInit( &oamSub, SpriteMapping_1D_128, 0 );

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

}

void graphics_release() {

   /* Setup bank E to receive extended palettes. */
   /* TODO: Palette/BMP dirty bit. */
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

   /* Clear sprites at the start of the loop. */
   oamClear( &oamMain, 0, 0 );
}

void graphics_loop_end() {
   int16_t delta = 0;
   
   /* Wait for FPS timer. */
   do {
      delta = gc_ms_target - (graphics_get_ms() - g_ms_start);
   } while( 0 < delta );  
   swiWaitForVBlank();
}

void graphics_draw_px( uint16_t x, uint16_t y, const GRAPHICS_COLOR color ) {
   /* VRAM_A[(y * SCREEN_H) + x] = color; */
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

   if( 0 <= instance_id && GRAPHICS_SPRITES_ONSCREEN > instance_id ) {
      /* Blitting a sprite. */
      
      /* On the DS, the RESOURCE_ID in bmp->id that would be a filename or w/e
       * on other platforms is just a pointer directly to the data from the
       * structs/arrays assembled by grit in Makefile.nds. So just pull right
       * from there.
       */
      dmaCopy( bmp->id->palette, SPRITE_PALETTE, bmp->id->palette_sz );

      /* 2 = spritesheet width of one row in sprites. */
      tile_idx = ((s_y / SPRITE_H) * 2) + (s_x / SPRITE_W);
      dmaCopy(
         bmp->id->tiles + (tile_idx * (8 * 8)),
         g_sprite_frames[instance_id], (TILE_W * TILE_H) );

      oamSet(
         &oamMain, instance_id, d_x, d_y, 0, 0,
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
      tile_y = d_y / 8;
      tile_x = d_x / 8;

      bg_tiles[(tile_y * BG_TILE_TW) + tile_x] = tile_idx;
      bg_tiles[(tile_y * BG_TILE_TW) + tile_x + 1] = tile_idx + 1;
      bg_tiles[((tile_y + 1) * BG_TILE_TW) + tile_x] = tile_idx + 2;
      bg_tiles[((tile_y + 1) * BG_TILE_TW) + tile_x + 1] = tile_idx + 3;
   }
}

void graphics_draw_block(
   uint16_t x_orig, uint16_t y_orig, uint16_t w, uint16_t h,
   GRAPHICS_COLOR color
) {
   /* TODO */
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
   oamClear( &oamMain, 0, 0 );
}

#ifndef USE_SOFTWARE_TEXT
#error "platform nds does not support hardware text"
#endif /* !USE_SOFTWARE_TEXT */

