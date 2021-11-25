
#ifndef RESOURCE_HEADER_H
#define RESOURCE_HEADER_H

struct RESOURCE_HEADER_HANDLE {
   const unsigned char* const ptr;
   uint32_t ptr_sz;
   uint32_t locks;
};

typedef uint32_t RESOURCE_ID;
typedef struct RESOURCE_HEADER_HANDLE* RESOURCE_HANDLE;

#define resource_compare_id( a, b ) (a == b)
#define resource_assign_id( dest, source ) dest = source

#endif /* RESOURCE_HEADER_H */

