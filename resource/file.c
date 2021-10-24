
#define RESOURCE_C
#include "../unilayer.h"

#ifdef RESOURCE_FILE

static MEMORY_HANDLE resource_get_handle( RESOURCE_ID id ) {
   FILE* res_file = NULL;
   uint8_t* buffer = NULL;
   int16_t i = 0;
   int32_t res_sz = 0,
      read = 0;
   MEMORY_HANDLE res_handle = (MEMORY_HANDLE)0;
   char asset_path[JSON_PATH_SZ];

   dio_snprintf( asset_path, JSON_PATH_SZ, "%s%s", ASSETS_PATH, id );

   #if 0
   for( i = 0 ; strlen( asset_path ) > i ; i++ ) {
      if( '/' == asset_path[i] ) {
         asset_path[i] = PLATFORM_DIR_SEP;
      }
   }
   debug_printf( 3, "asset path: %s\n", asset_path );
   #endif

   res_file = fopen( asset_path, "rb" );
   res_file = fopen( id, "rb" );
   if( NULL == res_file ) {
      error_printf( "unable to load resource: %s", asset_path );
      return (MEMORY_HANDLE)0;
   }

   fseek( res_file, 0, SEEK_END );
   res_sz = ftell( res_file );
   fseek( res_file, 0, SEEK_SET );
   if( 0 >= res_sz ) {
      error_printf( "invalid resource size" );
      goto cleanup;
   }

   debug_printf( 2, "opened resource: %s (%d bytes)", asset_path, res_sz );

   res_handle = memory_alloc( res_sz, 1 );
   if( (MEMORY_HANDLE)0 == res_handle ) {
      error_printf( "could not allocate resource buffer of %d bytes", res_sz );
      goto cleanup;
   }

   debug_printf( 1, "allocated resource buffer: %s", asset_path );

   buffer = memory_lock( res_handle );
   if( NULL == buffer ) {
      error_printf( "could not lock resource buffer" );
      goto cleanup;
   }

   debug_printf( 1, "locked resource buffer: %s", asset_path );

   read = fread( buffer, 1, res_sz, res_file );
   if( read != res_sz ) {
      error_printf( "read invalid resource data (%u bytes vs %u bytes)",
         read, res_sz );
      goto cleanup;
   }

   debug_printf( 1, "read resource into memory: %s", asset_path );

cleanup:

   if( NULL != buffer ) {
      buffer = memory_unlock( res_handle );
   }

   return res_handle;
}

RESOURCE_BITMAP_HANDLE resource_get_bitmap_handle( RESOURCE_ID id ) {
   return resource_get_handle( id );
}

RESOURCE_JSON_HANDLE resource_get_json_handle( RESOURCE_ID id ) {
   return resource_get_handle( id );
}

MEMORY_PTR resource_lock_handle( MEMORY_HANDLE handle ) {
   return memory_lock( handle );
}

MEMORY_PTR resource_unlock_handle( MEMORY_HANDLE handle ) {
   return memory_unlock( handle );
}

void resource_free_handle( MEMORY_HANDLE handle ) {
   memory_free( handle );
}

#endif /* RESOURCE_FILE */

