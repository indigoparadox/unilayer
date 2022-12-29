
#define GRAPHICS_PLATFORM_C
#define NDSG_C
#include "../unilayer.h"

/*
 * @return 1 if init was successful and 0 otherwise.
 */
int16_t graphics_platform_init() {
   powerOn( POWER_ALL );
   
   videoSetMode( MODE_0_2D );
	videoSetModeSub( MODE_0_2D );

	vramSetBankA( VRAM_A_MAIN_SPRITE );
	vramSetBankD( VRAM_D_SUB_SPRITE );

   /* Setup the sprite engines. */
	oamInit( &oamMain, SpriteMapping_1D_128, 0 );
	oamInit( &oamSub, SpriteMapping_1D_128, 0 );

   glScreen2D();

   lcdSwap();
   return 1;
}

void graphics_platform_shutdown() {
   /* TODO */
}

void graphics_lock() {

}

void graphics_release() {
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
   VRAM_A[(y * SCREEN_H) + x] = color;
}

int16_t graphics_platform_blit_partial_at(
   const struct GRAPHICS_BITMAP* bmp, int16_t instance_id,
   uint16_t s_x, uint16_t s_y,
   uint16_t d_x, uint16_t d_y, uint16_t w, uint16_t h
) {
   if( 0 <= instance_id ) {
      /* Blitting a sprite. */
      
      /* On the DS, the RESOURCE_ID in bmp->id that would be a filename or w/e
       * on other platforms is just a pointer directly to the data from the
       * structs/arrays assembled by grit in Makefile.nds. So just pull right
       * from there.
       */
      dmaCopy( bmp->id->palette, SPRITE_PALETTE, bmp->id->palette_sz );

      oamSet(
         &oamMain, instance_id, d_x, d_y, 0, 0,
         SpriteSize_16x16, SpriteColorFormat_256Color, 
         bmp->frame_data, -1, false, false, false, false, false );

      /* TODO */
      dmaCopy( bmp->id->tiles, bmp->frame_data, TILE_W * TILE_H );
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
	b->frame_data = oamAllocateGfx(
      &oamMain, SpriteSize_16x16, SpriteColorFormat_256Color );
}

int16_t graphics_platform_unload_bitmap( struct GRAPHICS_BITMAP* b ) {
   /* TODO */
}

#ifndef USE_SOFTWARE_TEXT
#error "platform nds does not support hardware text"
#endif /* !USE_SOFTWARE_TEXT */

