
#define GRAPHICS_C
#include "../unilayer.h"

static BitmapType* g_screen = NULL;
static WinHandle g_win;
static uint32_t g_ticks_start = 0;
static int16_t g_ticks_target = 0;

/*
 * @return 1 if init was successful and 0 otherwise.
 */
int16_t graphics_platform_init() {
   g_ticks_target = SysTicksPerSecond();
   return 1;
}

void graphics_platform_shutdown() {
}

void graphics_flip() {
}

uint32_t graphics_get_ms() {
   uint32_t ticks_per_ms = 0;

   ticks_per_ms = SysTicksPerSecond() * 1000;

   return TimGetTicks() * ticks_per_ms;
}

void graphics_loop_start() {
   g_ticks_start = TimGetTicks();
}

void graphics_loop_end() {
   int16_t delta = 0;

   delta = SysTicksPerSecond() - (TimGetTicks() - g_ticks_start);

   if( 0 > delta || 150 < delta ) {
      return;
   }

   /* Sleep until ticks target. */
   SysTaskDelay( delta );
}

void graphics_draw_px( uint16_t x, uint16_t y, const GRAPHICS_COLOR color ) {

   if( SCREEN_H <= y || SCREEN_W <= x ) {
      return;
   }

   WinSetForeColor( color );
   WinPaintPixel( x, y );
}

void graphics_draw_block(
   uint16_t x_orig, uint16_t y_orig, uint16_t w, uint16_t h,
   const GRAPHICS_COLOR color
) {
   /* TODO */
}

int16_t graphics_platform_blit_partial_at(
   const struct GRAPHICS_BITMAP* bmp,
   uint16_t s_x, uint16_t s_y,
   uint16_t d_x, uint16_t d_y, uint16_t w, uint16_t h
) {
   int retval = 1;
   MEMORY_HANDLE rsrc = NULL;
   BitmapPtr ptr = NULL;
   RectangleType screen_rect;

   if( NULL == bmp || !bmp->initialized ) {
      WinDrawChars( "X", 1, d_x, d_y );
      retval = 0;
      goto cleanup;
   }

   rsrc = resource_get_handle( bmp->id );

   if( NULL == rsrc ) {
      WinDrawChars( "Z", 1, d_x, d_y );
      retval = 0;
      goto cleanup;
   }

   screen_rect.topLeft.x = d_x;
   screen_rect.topLeft.y = d_y;
   screen_rect.extent.x = w;
   screen_rect.extent.y = h;
   WinSetClip( &screen_rect );

   ptr = resource_lock_handle( rsrc );
   WinDrawBitmap( ptr, d_x, d_y );
   ptr = resource_unlock_handle( rsrc );

   WinResetClip();

cleanup:

   if( NULL != rsrc ) {
      resource_free_handle( rsrc );
   }

   return retval;
}

int16_t graphics_platform_load_bitmap(
   RESOURCE_HANDLE res_handle, struct GRAPHICS_BITMAP* b
) {
   int retval = 1;

   if( 0 != b->ref_count ) {
      retval = 0;
      goto cleanup;
   }

   /* "Loading" happens in draw routine, since it's coming from RAM anyway. */

cleanup:
   return retval;
}

int16_t graphics_platform_unload_bitmap( struct GRAPHICS_BITMAP* b ) {
   if( NULL == b ) {
      return 0;
   }
   b->ref_count--;
   if( 0 >= b->ref_count ) {
      b->initialized = 0;
      b->id = 0;
      return 1;
   }
   return 0;
}

#ifndef USE_SOFTWARE_TEXT

void graphics_string_at(
   const char* s, uint16_t s_len, uint16_t x_orig, uint16_t y_orig,
   GRAPHICS_COLOR color, uint8_t flags
) {
   WinDrawChars( s, s_len, x_orig, y_orig );
}

void graphics_string_sz(
   const char* str, uint16_t str_sz, uint8_t flags, struct GRAPHICS_RECT* sz_out
) {
   /* TODO */
}

#endif /* !USE_SOFTWARE_TEXT */

