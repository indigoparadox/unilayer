
#define RESOURCE_C
#include "../unilayer.h"

#ifdef RESOURCE_HEADER

RESOURCE_HANDLE resource_get_handle( RESOURCE_ID id ) {
   return (RESOURCE_HANDLE)gsc_resources[id];
}

MEMORY_PTR resource_lock_handle( RESOURCE_HANDLE handle ) {
   return handle->ptr;
}

MEMORY_PTR resource_unlock_handle( RESOURCE_HANDLE handle ) {
   return NULL;
}

int32_t resource_sz_handle( RESOURCE_HANDLE handle ) {
   return handle->ptr_sz;
}

void resource_free_handle( RESOURCE_HANDLE handle ) {
}

#endif /* RESOURCE_HEADER */

