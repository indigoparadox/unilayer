
#define RESOURCE_C
#include "../unilayer.h"

RESOURCE_HANDLE resource_get_handle( RESOURCE_ID id ) {
   return (RESOURCE_HANDLE)id;
}

MEMORY_PTR resource_lock_handle( RESOURCE_HANDLE handle ) {
   /* return (MEMORY_PTR)handle->ptr; */
   return NULL;
}

MEMORY_PTR resource_unlock_handle( RESOURCE_HANDLE handle ) {
   return NULL;
}

int32_t resource_sz_handle( RESOURCE_HANDLE handle ) {
   return handle->ptr_sz;
}

void resource_free_handle( RESOURCE_HANDLE handle ) {
}

