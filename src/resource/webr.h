
#ifndef RESOURCE_WEB_H
#define RESOURCE_WEB_H

#ifndef RESOURCE_PATH_MAX
#define RESOURCE_PATH_MAX 48
#endif /* !RESOURCE_PATH_MAX */

#include <emscripten/fetch.h>

struct RESOURCE_WEBR_HANDLE {
   emscripten_fetch_t* fetch;
};

typedef char RESOURCE_ID[RESOURCE_PATH_MAX];
typedef struct RESOURCE_WEBR_HANDLE* RESOURCE_HANDLE;

#define resource_compare_id( a, b ) (0 == a ? NULL == b : (NULL != a && NULL != b  && 0 == memory_strncmp_ptr( a, b, RESOURCE_PATH_MAX )))
#define resource_assign_id( dest, source ) if( NULL != source && memory_strnlen_ptr( source, RESOURCE_PATH_MAX ) < RESOURCE_PATH_MAX ) { memory_zero_ptr( dest, RESOURCE_PATH_MAX ); memory_strncpy_ptr( dest, source, RESOURCE_PATH_MAX ); } else { error_printf( "attempted to assign invalid resource: %s", source ); }
#define resource_id_valid( id ) ('\0' == id[0])

#define RESOURCE_ID_FMT "%s"

#endif /* RESOURCE_WEB_H */

