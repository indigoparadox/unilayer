
#ifndef RESOURCE_NDS_H
#define RESOURCE_NDS_H

/* This file is automatically selected by resinc.h. */

struct RESOURCE_NDS_HANDLE {
   const unsigned char* const ptr;
   uint32_t ptr_sz;
   uint32_t locks;
};

/* This is defined in the Makefile.nds-created resext.h. */
typedef struct RESOURCE_GRIT_DEF* RESOURCE_ID;
typedef struct RESOURCE_NDS_HANDLE* RESOURCE_HANDLE;

#define resource_compare_id( a, b ) (a == b)
#define resource_assign_id( dest, source ) dest = source

#endif /* RESOURCE_NDS_H */

