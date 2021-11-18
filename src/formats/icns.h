
#ifndef ICNS_FORMAT_H
#define ICNS_FORMAT_H

struct PACKED ICNS_FILE_HEADER {
   char     id[4];
   uint32_t file_sz;
};

struct PACKED ICNS_DATA_HEADER {
   char     icon_type[4];
   uint32_t data_sz;
};

#endif /* ICNS_FORMAT_H */

