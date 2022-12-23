
#ifndef DOSG_H
#define DOSG_H

typedef uint8_t GRAPHICS_COLOR;

struct GRAPHICS_BITMAP {
   RESOURCE_ID id;
   uint8_t initialized;
   uint16_t ref_count;

   MEMORY_PTR plane_1;
   MEMORY_PTR plane_2;
   uint32_t w;
   uint32_t h;
   uint8_t palette;

   /* Unused Overrides */
   uint8_t res4;
};

struct GRAPHICS_ARGS {
   uint8_t mode;
};

#if defined( DEPTH_VGA )

/* VGA colors. These are the values directly written to each octet in VGA
 * memory.
 */

#define GRAPHICS_COLOR_BLACK        0
#define GRAPHICS_COLOR_DARKBLUE     1
#define GRAPHICS_COLOR_DARKGREEN    2
#define GRAPHICS_COLOR_CYAN         3
#define GRAPHICS_COLOR_DARKRED      4
#define GRAPHICS_COLOR_MAGENTA      5
#define GRAPHICS_COLOR_BROWN        6
#define GRAPHICS_COLOR_GRAY         7
#define GRAPHICS_COLOR_DARKGRAY     8
#define GRAPHICS_COLOR_BLUE         9
#define GRAPHICS_COLOR_GREEN        10
#define GRAPHICS_COLOR_TEAL         11
#define GRAPHICS_COLOR_RED          12
#define GRAPHICS_COLOR_VIOLET       13
#define GRAPHICS_COLOR_YELLOW       14
#define GRAPHICS_COLOR_WHITE        15

#elif defined( DEPTH_CGA )

/* CGA colors. These are the values directly written to each 2-bit
 * quarter-octet in the CGA planes.
 */

#define GRAPHICS_COLOR_BLACK        0
#define GRAPHICS_COLOR_CYAN         1
#define GRAPHICS_COLOR_MAGENTA      2
#define GRAPHICS_COLOR_WHITE        3

#else
#endif /* DEPTH_VGA || DEPTH_CGA */

#endif /* DOSG_H */

