
#ifndef NDSG_H
#define NDSG_H

typedef int GRAPHICS_COLOR;
struct GRAPHICS_BITMAP {
   RESOURCE_ID id;
   uint16_t flags;
   uint16_t ref_count;
   uint16_t* pixel_data;
   uint8_t* frame_data;

   /* Unused Overrides */
   uint32_t res1;
   uint32_t res2;
   uint8_t res3;
   uint8_t res4;
};

struct GRAPHICS_ARGS {
};

#ifdef DEPTH_VGA

#define GRAPHICS_COLOR_BLACK        RGB15(0,   0,   0)
#define GRAPHICS_COLOR_DARKBLUE     RGB15(0, 0, 170)
#define GRAPHICS_COLOR_DARKGREEN    RGB15(0, 170, 0)
#define GRAPHICS_COLOR_TEAL         RGB15(0, 170, 170)
#define GRAPHICS_COLOR_DARKRED      RGB15(170, 0, 0)
#define GRAPHICS_COLOR_VIOLET       RGB15(170, 0, 170)
#define GRAPHICS_COLOR_BROWN        RGB15(170, 85, 0)
#define GRAPHICS_COLOR_GRAY         RGB15(170, 170, 170)
#define GRAPHICS_COLOR_DARKGRAY     RGB15(85, 85, 85)
#define GRAPHICS_COLOR_BLUE         RGB15(85, 85, 255)
#define GRAPHICS_COLOR_GREEN        RGB15(85, 255, 85)
#define GRAPHICS_COLOR_CYAN         RGB15(85, 255, 255)
#define GRAPHICS_COLOR_RED          RGB15(255, 85, 85)
#define GRAPHICS_COLOR_MAGENTA      RGB15(255, 85, 255)
#define GRAPHICS_COLOR_YELLOW       RGB15(255, 255, 85)
#define GRAPHICS_COLOR_WHITE        RGB15(255, 255, 255)

#else
                                         
#define GRAPHICS_COLOR_BLACK        RGB15(0, 0, 0)
#define GRAPHICS_COLOR_CYAN         RGB15(0, 255, 255)
#define GRAPHICS_COLOR_MAGENTA      RGB15(255, 0, 255)
#define GRAPHICS_COLOR_WHITE        RGB15(255, 255, 255)

#endif /* DEPTH_VGA */


#endif /* NDSG_H */

