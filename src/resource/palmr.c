
#define RESOURCE_C
#include "../unilayer.h"

static RESOURCE_HANDLE resource_get_handle( RESOURCE_ID id, char type[4] ) {
   debug_printf( 2, "loading resource %u of type %u", id, type );

   return DmGetResource( type, id );
}

RESOURCE_HANDLE resource_get_handle( RESOURCE_ID id ) {
   return resource_get_handle( id, 'Tbmp' );
}

MEMORY_PTR resource_lock_handle( RESOURCE_HANDLE handle ) {
   /* Designed to work with palm memory, which locks resources. */
   return memory_lock( (MEMORY_HANDLE)handle );
}

MEMORY_PTR resource_unlock_handle( RESOURCE_HANDLE handle ) {
   /* Designed to work with palm memory, which locks resources. */
   return memory_unlock( (MEMORY_HANDLE)handle );
}

int32_t resource_sz_handle( RESOURCE_HANDLE handle ) {
   /* TODO */
   return 0;
}

void resource_free_handle( RESOURCE_HANDLE handle ) {
   DmReleaseResource( handle );
}

