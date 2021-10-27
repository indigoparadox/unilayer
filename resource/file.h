
#ifndef RESOURCE_FILE_H
#define RESOURCE_FILE_H

#ifndef RESOURCE_PATH_MAX
#define RESOURCE_PATH_MAX 48
#endif /* !RESOURCE_PATH_MAX */

typedef char RESOURCE_ID[RESOURCE_PATH_MAX];
typedef MEMORY_HANDLE RESOURCE_BITMAP_HANDLE;
typedef MEMORY_HANDLE RESOURCE_JSON_HANDLE;

#define resource_compare_id( a, b ) (NULL != a && NULL != b  && 0 == memory_strncmp_ptr( a, b, RESOURCE_PATH_MAX ))
#define resource_assign_id( dest, source ) if( NULL != source && memory_strnlen_ptr( source, RESOURCE_PATH_MAX ) < RESOURCE_PATH_MAX ) { memory_strncpy_ptr( dest, source, RESOURCE_PATH_MAX ); } else { error_printf( "attempted to assign invalid resource: %s", source ); }

#endif /* RESOURCE_FILE_H */

