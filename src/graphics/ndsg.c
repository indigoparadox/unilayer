
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
static uint16_t g_bg_tiles[BG_TILE_TW * BG_TILE_TH];
static int g_window_tiles[BG_TILE_TW * BG_TILE_TH];

/*
 * @return 1 if init was successful and 0 otherwise.
 */
int16_t graphics_platform_init() {
   int i = 0;

   powerOn( POWER_ALL );
   
   videoSetMode( MODE_3_2D );
	videoSetModeSub( MODE_0_2D );

   /* Setup the upper screen for background and sprites. */
	vramSetBankA( VRAM_A_MAIN_BG );
	vramSetBankB( VRAM_A_MAIN_SPRITE );

   /* Setup the lower screen for background and sprites. */
	vramSetBankC( VRAM_C_MAIN_BG );
	vramSetBankD( VRAM_D_SUB_SPRITE );

   /* Setup the background engine. */
   memset( g_bg_tiles, 0, sizeof( g_bg_tiles ) );
   g_bg_id = bgInit( 0, BgType_Text8bpp, BgSize_T_256x256, 0, 1 );

   memset( g_window_tiles, 0, sizeof( g_window_tiles ) );
   g_window_id = bgInit( 1, BgType_Text8bpp, BgSize_T_256x256, 0, 1 );

   /* Setup the sprite engines. */
	oamInit( &oamMain, SpriteMapping_1D_128, 0 );
	oamInit( &oamSub, SpriteMapping_1D_128, 0 );

   /* Allocate sprite frame memory. */
   for( i = 0 ; GRAPHICS_SPRITES_ONSCREEN > i ; i++ ) {
      g_sprite_frames[i] = oamAllocateGfx(
         &oamMain, SpriteSize_16x16, SpriteColorFormat_256Color );
   }

   /* glScreen2D(); */

   return 1;
}

void graphics_platform_shutdown() {
   /* TODO */
}

void graphics_lock() {

}

void graphics_release() {

   /* Update background tiles. */
   if( NULL != g_bg_tiles && NULL != g_bg_bmp ) {
      dmaCopy(
         g_bg_bmp->id->tiles, bgGetGfxPtr( g_bg_id ), g_bg_bmp->id->tiles_sz );
      dmaCopy( g_bg_bmp->id->palette, BG_PALETTE, g_bg_bmp->id->palette_sz );
      dmaCopy( g_bg_tiles, bgGetMapPtr( g_bg_id ),  sizeof( g_bg_tiles ) );
   }

   /* Update window tiles. */
   if( NULL != g_window_tiles && NULL != g_window_bmp ) {
      dmaCopy( g_window_bmp->id->tiles, bgGetGfxPtr( g_window_id ),
         g_window_bmp->id->tiles_sz );
      dmaCopy(
         g_window_bmp->id->palette, BG_PALETTE, g_window_bmp->id->palette_sz );
      dmaCopy( g_window_tiles, bgGetMapPtr( g_window_id ),
         sizeof( g_window_tiles ) );
   }

   /* Update sprite engines. */
   oamUpdate( &oamMain );
   oamUpdate( &oamSub );
}

int16_t graphics_get_random( int16_t start, int16_t range ) {
   /* TODO */
   return 0;
}

uint32_t graphics_get_ms() {
   /* TODO */
   return 0;
}

void graphics_loop_start() {
   /* TODO */
}

void graphics_loop_end() {
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

   if( 0 <= instance_id && GRAPHICS_SPRITES_ONSCREEN > instance_id ) {
      /* Blitting a sprite. */
      
      /* On the DS, the RESOURCE_ID in bmp->id that would be a filename or w/e
       * on other platforms is just a pointer directly to the data from the
       * structs/arrays assembled by grit in Makefile.nds. So just pull right
       * from there.
       */
      dmaCopy( bmp->id->palette, SPRITE_PALETTE, bmp->id->palette_sz );

      /* TODO */
      /* 2 = spritesheet width of one row in sprites. */
      tile_idx = ((s_y / SPRITE_H) * 2) + (s_x / SPRITE_W);
      dmaCopy(
         bmp->id->tiles + (tile_idx * (8 * 8)),
         g_sprite_frames[instance_id], (TILE_W * TILE_H) );

      oamSet(
         &oamMain, instance_id, d_x, d_y, 0, 0,
         SpriteSize_16x16, SpriteColorFormat_256Color, 
         g_sprite_frames[instance_id], -1, false, false, false, false, false );

   } else if( GRAPHICS_INSTANCE_TILEMAP == instance_id ) {
      /* Blitting a background tile. */

      /* DS tiles are 8x8, so each tile is split up into 4, so compensate! */
      tile_idx = bmp->id->tile_offset * 4;

      /* The tile's physical location on the tilemap. */
      /* Divide by 8 rather than 16 since DS tiles are 8x8. */
      tile_y = d_y / 8;
      tile_x = d_x / 8;

      g_bg_tiles[(tile_y * BG_TILE_TW) + tile_x] = tile_idx;
      g_bg_tiles[(tile_y * BG_TILE_TW) + tile_x + 1] = tile_idx + 1;
      g_bg_tiles[((tile_y + 1) * BG_TILE_TW) + tile_x] = tile_idx + 2;
      g_bg_tiles[((tile_y + 1) * BG_TILE_TW) + tile_x + 1] = tile_idx + 3;
      g_bg_bmp = bmp;
      /* scroll(bg, 256, 256); */

   } else if( GRAPHICS_INSTANCE_WINDOW == instance_id ) {
   /*
      g_window_tiles[((d_y / TILE_W) * BG_TILE_TW) + (d_x / TILE_H)] = -1;
      g_window_bmp = bmp;
      */
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
}

#ifndef USE_SOFTWARE_TEXT
#error "platform nds does not support hardware text"
#endif /* !USE_SOFTWARE_TEXT */

