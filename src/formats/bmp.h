
#ifndef BMP_FORMAT_H
#define BMP_FORMAT_H

struct PACKED BITMAP_FILE_HEADER {
   char id[2];
   uint32_t file_sz;
   uint16_t reserved1;
   uint16_t reserved2;
   uint32_t bmp_offset;
};

struct PACKED BITMAP_DATA_HEADER {
   uint32_t header_sz;
   int32_t bitmap_w;
   int32_t bitmap_h;
   uint16_t planes;
   uint16_t bpp;
   uint32_t compression;
   uint32_t image_sz;
   int32_t hres;
   int32_t vres;
   uint32_t colors;
   uint32_t colors_important;
};

#endif /* BMP_FORMAT_H */

