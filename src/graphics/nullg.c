
#define GRAPHICS_PLATFORM_C
#define NULLG_C
#include "../unilayer.h"

int16_t graphics_platform_init() {
   return 1;
}

void graphics_platform_shutdown() {
}

void graphics_lock() {
}

void graphics_release() {
}

int16_t graphics_get_random( int16_t start, int16_t range ) {
   return 0;
}

uint32_t graphics_get_ms() {
   return 0;
}

void graphics_loop_start() {
}

void graphics_loop_end() {
}

void graphics_draw_px( uint16_t x, uint16_t y, const GRAPHICS_COLOR color ) {
}

void graphics_draw_block(
   uint16_t x_orig, uint16_t y_orig, uint16_t w, uint16_t h,
   const GRAPHICS_COLOR color
) {
}

void graphics_draw_line(
   uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t thickness,
   const GRAPHICS_COLOR color
) {
}

void graphics_draw_rect(
   uint16_t x_orig, uint16_t y_orig, uint16_t w, uint16_t h,
   uint16_t thickness, const GRAPHICS_COLOR color
) {
}

int16_t graphics_platform_blit_at(
   const struct GRAPHICS_BITMAP* bmp,
   uint16_t x, uint16_t y, uint16_t w, uint16_t h
) {
   return 0;
}

int16_t graphics_platform_blit_partial_at(
   const struct GRAPHICS_BITMAP* bmp,
   uint16_t s_x, uint16_t s_y,
   uint16_t d_x, uint16_t d_y, uint16_t w, uint16_t h
) {
   return 0;
}

int16_t graphics_platform_load_bitmap(
   RESOURCE_HANDLE res_handle, struct GRAPHICS_BITMAP* b
) {
   return 0;
}

int16_t graphics_platform_unload_bitmap( struct GRAPHICS_BITMAP* b ) {
   return 0;
}

