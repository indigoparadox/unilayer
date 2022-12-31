
#ifndef NDSG_H
#define NDSG_H

typedef int32_t GRAPHICS_COLOR;
struct GRAPHICS_BITMAP {
   RESOURCE_ID id;
   uint16_t flags;
   uint16_t ref_count;
   uint16_t* pixel_data;

   /* Unused Overrides */
   void* res0;
   uint32_t res1;
   uint32_t res2;
   uint8_t res3;
   uint8_t res4;
};

struct GRAPHICS_ARGS {
};

/* Need to clear multiple drawing mechanisms (oam, bitmap, tile BG). */
#define graphics_clear_screen() \
   graphics_nds_clear_region( 0, 0, SCREEN_W, SCREEN_H );
#define graphics_clear_block( x, y, w, h ) \
   graphics_nds_clear_region( x, y, w, h );

#ifdef DEPTH_VGA

#define GRAPHICS_COLOR_BLACK        ARGB16(1, 0,   0,   0)
#define GRAPHICS_COLOR_DARKBLUE     ARGB16(1, 0, 0, 170)
#define GRAPHICS_COLOR_DARKGREEN    ARGB16(1, 0, 170, 0)
#define GRAPHICS_COLOR_TEAL         ARGB16(1, 0, 170, 170)
#define GRAPHICS_COLOR_DARKRED      ARGB16(1, 170, 0, 0)
#define GRAPHICS_COLOR_VIOLET       ARGB16(1, 170, 0, 170)
#define GRAPHICS_COLOR_BROWN        ARGB16(1, 170, 85, 0)
#define GRAPHICS_COLOR_GRAY         ARGB16(1, 170, 170, 170)
#define GRAPHICS_COLOR_DARKGRAY     ARGB16(1, 85, 85, 85)
#define GRAPHICS_COLOR_BLUE         ARGB16(1, 85, 85, 255)
#define GRAPHICS_COLOR_GREEN        ARGB16(1, 85, 255, 85)
#define GRAPHICS_COLOR_CYAN         ARGB16(1, 85, 255, 255)
#define GRAPHICS_COLOR_RED          ARGB16(1, 255, 85, 85)
#define GRAPHICS_COLOR_MAGENTA      ARGB16(1, 255, 85, 255)
#define GRAPHICS_COLOR_YELLOW       ARGB16(1, 255, 255, 85)
#define GRAPHICS_COLOR_WHITE        ARGB16(1, 255, 255, 255)

#else
                                         
#define GRAPHICS_COLOR_BLACK        ARGB16(1, 0, 0, 0)
#define GRAPHICS_COLOR_CYAN         ARGB16(1, 0, 255, 255)
#define GRAPHICS_COLOR_MAGENTA      ARGB16(1, 255, 0, 255)
#define GRAPHICS_COLOR_WHITE        ARGB16(1, 255, 255, 255)

#endif /* DEPTH_VGA */

void graphics_nds_clear_region(
   uint16_t x_orig, uint16_t y_orig, uint16_t w, uint16_t h );

#endif /* NDSG_H */

