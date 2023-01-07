
#ifndef RESOURCE_FILE_H
#define RESOURCE_FILE_H

/* So RESOURCE_PATH_MAX is available to tools. */
#include "../config.h"

typedef char RESOURCE_ID[RESOURCE_PATH_MAX];
typedef MEMORY_HANDLE RESOURCE_HANDLE;

#define resource_compare_id( a, b ) (0 == a ? NULL == b : (NULL != a && NULL != b  && 0 == memory_strncmp_ptr( a, b, RESOURCE_PATH_MAX )))
#define resource_assign_id( dest, source ) if( NULL != source && memory_strnlen_ptr( source, RESOURCE_PATH_MAX ) < RESOURCE_PATH_MAX ) { memory_zero_ptr( dest, RESOURCE_PATH_MAX ); memory_strncpy_ptr( dest, source, RESOURCE_PATH_MAX ); } else { error_printf( "attempted to assign invalid resource: %s", source ); }
#define resource_id_valid( id ) ('\0' != id[0])

#define RESOURCE_ID_FMT "%s"

/* ! */
#ifdef ANCIENT_C
/* ! */

#  define resource_debug_printf
#  define resource_error_printf

/* ! */
#else
/* ! */

#define resource_debug_printf( lvl, fmt, rsrc, ... ) debug_printf( lvl, "resource %s: " fmt, rsrc, __VA_ARGS__ )
#define resource_error_printf( fmt, rsrc, ... ) error_printf( "resource %s: " fmt, rsrc, __VA_ARGS__ )

/* ! */
#endif
/* ! */

#endif /* RESOURCE_FILE_H */

