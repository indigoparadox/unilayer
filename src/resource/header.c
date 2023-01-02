
#define RESOURCE_C
#include "../unilayer.h"

#ifdef RESOURCE_HEADER

RESOURCE_HANDLE resource_get_handle( RESOURCE_ID id ) {
   return (RESOURCE_HANDLE)gsc_resources[id];
}

MEMORY_PTR resource_lock_handle( RESOURCE_HANDLE handle ) {
   return (MEMORY_PTR)handle->ptr;
}

MEMORY_PTR resource_unlock_handle( RESOURCE_HANDLE handle ) {
   return NULL;
}

int32_t resource_sz_handle( RESOURCE_HANDLE handle ) {
   return handle->ptr_sz;
}

void resource_free_handle( RESOURCE_HANDLE handle ) {
}

uint8_t resource_id_from_name(
   RESOURCE_ID* id, RESOURCE_NAME name, const char* ext
) {
   RESOURCE_ID i = 1;

   while( '\0' != gsc_resource_names[i][0] ) {
      debug_printf( 1, "\"%s\" vs \"%s\" (%d)",
         name, gsc_resource_names[i], i );
      if( 0 == memory_strncmp_ptr(
         gsc_resource_names[i], name, RESOURCE_NAME_MAX
      ) ) {
         debug_printf( 1, "found header ID for resource \"%s\": %d",
            name, i );
         *id = i;
         return 1;
      }
      i++;
   }

   error_printf( "did not find header ID for resource \"%s\"!", name );
   *id = 0;

   return 0;
}

#endif /* RESOURCE_HEADER */

