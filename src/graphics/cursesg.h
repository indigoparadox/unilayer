#ifndef CURSESG_H
#define CURSESG_H

typedef uint8_t GRAPHICS_COLOR;

struct GRAPHICS_BITMAP {
   RESOURCE_ID id;
   uint16_t flags;
   uint16_t ref_count;

   /* Unused Overrides */
   void* resa;
   void* resb;
   uint32_t res1;
   uint32_t res2;
   uint8_t res3;
   uint8_t res4;
};

struct GRAPHICS_ARGS {
   uint8_t mode;
};

#define GRAPHICS_COLOR_BLACK        0
#define GRAPHICS_COLOR_CYAN         1
#define GRAPHICS_COLOR_MAGENTA      2
#define GRAPHICS_COLOR_WHITE        3

int16_t graphics_curses_char_at( unsigned char c, uint16_t x, uint16_t y );

#endif /* cursesg_h */

