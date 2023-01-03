
#ifndef RESOURCE_NDS_H
#define RESOURCE_NDS_H

/* This file is automatically selected by resinc.h. */

struct RESOURCE_GRIT_DEF {
   const char* name;
   const unsigned int* tiles;
   const unsigned short* palette;
   const uint32_t tiles_sz;
   const uint32_t palette_sz;
   int32_t tile_offset;
   uint32_t ptr_sz;
   uint32_t locks;
};

/* This is defined in the Makefile.nds-created resext.h. */
typedef uint32_t RESOURCE_ID;
typedef struct RESOURCE_GRIT_DEF* RESOURCE_HANDLE;

#define resource_compare_id( a, b ) (a == b)
#define resource_assign_id( dest, source ) dest = source
#define resource_id_valid( id ) (0 < id)

#define RESOURCE_ID_FMT "%d"

#endif /* RESOURCE_NDS_H */

