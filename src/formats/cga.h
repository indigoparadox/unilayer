
#ifndef CGA_FORMAT_H
#define CGA_FORMAT_H

struct UNILAYER_PACKED CGA_HEADER {
   uint16_t id;
   uint16_t version;
   uint16_t width;
   uint16_t height;
   uint16_t bpp;
   uint16_t plane1_offset;
   uint16_t plane1_sz;
   uint16_t plane2_offset;
   uint16_t plane2_sz;
   uint16_t palette;
   uint16_t endian;
   uint16_t reserved1;
   uint16_t reserved2;
   uint16_t reserved3;
   uint16_t reserved4;
};

#endif /* CGA_FORMAT_H */

