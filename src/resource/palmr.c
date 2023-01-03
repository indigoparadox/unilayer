
#define RESOURCE_C
#include "../unilayer.h"

#define RES_ARRAYS
#include <resarr.h>

RESOURCE_HANDLE resource_get_handle( RESOURCE_ID id ) {
   return DmGetResource( 'Tbmp', id );
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

uint8_t resource_id_from_name(
   RESOURCE_ID* id, RESOURCE_NAME name, const char* ext
) {
   RESOURCE_ID i = 0;

   while( '\0' != g_resource_names[i][0] ) {
      debug_printf( 1, "\"%s\" vs \"%s\" (%d)",
         name, g_resource_names[i], i );
      if( 0 == memory_strncmp_ptr(
         g_resource_names[i], name, RESOURCE_NAME_MAX
      ) ) {
         debug_printf( 1, "found header ID for resource \"%s\": %d",
            name, g_resource_ids[i] );
         *id = g_resource_ids[i];
         return 1;
      }
      i++;
   }

   error_printf( "did not find header ID for resource \"%s\"!", name );
   *id = 0;

   return 0;
}

