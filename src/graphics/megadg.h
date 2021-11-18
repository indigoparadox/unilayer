
#ifndef MEGADG_H
#define MEGADG_H

#include <vdp.h>

typedef uint16_t GRAPHICS_COLOR;
struct GRAPHICS_BITMAP {
   RESOURCE_ID id;
   uint8_t initialized;
   uint16_t ref_count;

   /* Unused Overrides */
   void* ptr1;
   void* ptr2;
   uint32_t res1;
   uint32_t res2;
   uint8_t res3;
   uint8_t res4;
};

struct GRAPHICS_ARGS {
};

#define GRAPHICS_COLOR_BLACK RGB24_TO_VDPCOLOR( 0x000000 )
#define GRAPHICS_COLOR_CYAN RGB24_TO_VDPCOLOR( 0x00ffff )
#define GRAPHICS_COLOR_MAGENTA RGB24_TO_VDPCOLOR( 0xff00ff )
#define GRAPHICS_COLOR_WHITE RGB24_TO_VDPCOLOR( 0xffffff )

#endif /* !MEGADG_H */

