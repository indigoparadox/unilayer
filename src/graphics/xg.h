
#ifndef XG_H
#define XG_H

typedef uint32_t GRAPHICS_COLOR;
struct GRAPHICS_BITMAP {
   RESOURCE_ID id;
   uint8_t initialized;
   uint16_t ref_count;
   XImage* pixmap;
   char* bits;

   /* Unused Overrides */
   uint32_t res1;
   uint32_t res2;
   uint8_t res3;
   uint8_t res4;
};

struct GRAPHICS_ARGS {
};

#define GRAPHICS_COLOR_BLACK     0x00000000
#define GRAPHICS_COLOR_CYAN      0x0000ffff
#define GRAPHICS_COLOR_MAGENTA   0x00ff00ff
#define GRAPHICS_COLOR_WHITE     0x00ffffff

#endif /* XG_H */

