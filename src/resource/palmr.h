
#ifndef PALMR_H
#define PALMR_H

typedef uint32_t RESOURCE_ID;
typedef MEMORY_HANDLE RESOURCE_HANDLE;

#define resource_compare_id( a, b ) ((a) == (b))
#define resource_assign_id( dest, source ) dest = source
#define resource_id_valid( id ) (0 < id)

#define RESOURCE_ID_FMT "%d"

#endif /* PALMR_H */

