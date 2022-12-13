
#include "../unilayer.h"

#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <ncurses.h>

#ifdef USE_SOFTWARE_TEXT
#error "platform curses does not support software text"
#endif /* USE_SOFTWARE_TEXT */

uint32_t g_s_launch = 0;
const uint32_t gc_ms_target = 1000 / FPS;
static uint32_t g_ms_start = 0; 

int16_t graphics_platform_init() {
   time_t tm;

   srand( (unsigned int)time( &tm ) );

   initscr();

   return 1;
}

void graphics_platform_shutdown() {
   endwin();
}

void graphics_flip() {
   refresh();
   clear();
}

int16_t graphics_get_random( int16_t start, int16_t range ) {
   return start + (rand() % range);
}

uint32_t graphics_get_ms() {
   struct timespec spec;
   uint32_t ms_out = 0,
      ms_launch_delta = 0;
   
   clock_gettime( CLOCK_MONOTONIC, &spec );

   /* Get the seconds since program launched. Multiply by 1000, so we want a
    * smaller number than seconds since the epoch. */
   ms_launch_delta = spec.tv_sec - g_s_launch;
   ms_out += ms_launch_delta * 1000;
   ms_out += spec.tv_nsec / 1000000;

   return ms_out;
}

void graphics_loop_start() {
   g_ms_start = graphics_get_ms();
}

void graphics_loop_end() {
   int16_t delta = 0;

   do {
      delta = gc_ms_target - (graphics_get_ms() - g_ms_start);
   } while( 0 < delta );  
}

void graphics_draw_block(
   uint16_t x_orig, uint16_t y_orig, uint16_t w, uint16_t h,
   const GRAPHICS_COLOR color
) {
}

void graphics_draw_px( uint16_t x, uint16_t y, const GRAPHICS_COLOR color ) {
}

int16_t graphics_platform_blit_partial_at(
   const struct GRAPHICS_BITMAP* b,
   uint16_t s_x, uint16_t s_y,
   uint16_t d_x, uint16_t d_y, uint16_t w, uint16_t h
) {
   return 1;
}

int16_t graphics_platform_load_bitmap(
   RESOURCE_HANDLE res_handle, struct GRAPHICS_BITMAP* b
) {
   return 'x';
}

int16_t graphics_platform_unload_bitmap( struct GRAPHICS_BITMAP* b ) {
   return 1;
}

void graphics_string_at(
   const char* str, uint16_t str_sz, uint16_t x_orig, uint16_t y_orig,
   GRAPHICS_COLOR color, uint8_t flags
) {
   int i = 0;

   for( i = 0 ; str_sz > i ; i++ ) {
      mvaddch( y_orig, x_orig + i, str[i] );
   }
}

void graphics_string_sz(
   const char* str, uint16_t str_sz, uint8_t flags, struct GRAPHICS_RECT* sz_out
) {
   sz_out->h = 1;
   sz_out->w = memory_strnlen_ptr( str, str_sz );
}

void graphics_draw_rect(
   uint16_t x_orig, uint16_t y_orig, uint16_t w, uint16_t h,
   uint16_t thickness, const GRAPHICS_COLOR color
) {
}

int16_t graphics_cache_load_bitmap( RESOURCE_ID res_id ) {
   return 1;
}

int16_t graphics_cache_blit_at(
   uint16_t bitmap_idx,
   uint16_t s_x, uint16_t s_y, uint16_t d_x, uint16_t d_y,
   uint16_t w, uint16_t h
) {
   return 1;
}

void graphics_curses_char_at( unsigned char c, uint16_t x, uint16_t y ) {
   debug_printf( 3, "blit %c at %d, %d", c, x, y );
   mvaddch( y, x, c );
}

