
#define RESOURCE_C
#include "../unilayer.h"

#include <errno.h>

#ifdef RESOURCE_FILE

RESOURCE_HANDLE resource_get_handle( RESOURCE_ID id ) {
   FILE* res_file = NULL;
   uint8_t* buffer = NULL;
#ifdef PLATFORM_DOS
   int16_t extension_idx = 0;
#endif /* PLATFORM_DOS */
   int32_t res_sz = 0,
      read = 0;
   RESOURCE_HANDLE res_handle = (RESOURCE_HANDLE)0;
   char asset_path[RESOURCE_PATH_MAX];

   debug_printf( 2, "requested resource: %s", id );

   dio_snprintf( asset_path, RESOURCE_PATH_MAX, "%s", id );

#ifdef PLATFORM_DOS
   extension_idx = dio_char_idx_r( asset_path,
      memory_strnlen_ptr( asset_path, RESOURCE_PATH_MAX ), '.' );

   #if 0
   for( i = 0 ; strlen( asset_path ) > i ; i++ ) {
      if( '/' == asset_path[i] ) {
         asset_path[i] = PLATFORM_DIR_SEP;
      }
   }
   debug_printf( 3, "asset path: %s\n", asset_path );
   #endif

   /* Use CGA-converted assets in DOS. */
   debug_printf( 2, "resource extension: %s", &(asset_path[extension_idx]) );
   if( 0 == strncmp( &(asset_path[extension_idx]), ".bmp", 3 ) ) {
      asset_path[extension_idx + 1] = 'c';
      asset_path[extension_idx + 2] = 'g';
      asset_path[extension_idx + 3] = 'a';
   }
#endif /* PLATFORM_DOS */

   res_file = fopen( asset_path, "rb" );
   if( NULL == res_file ) {
      error_printf( "unable to load resource: %s (error: %d)",
         asset_path, errno );
      return (RESOURCE_HANDLE)0;
   }

   fseek( res_file, 0, SEEK_END );
   res_sz = ftell( res_file );
   fseek( res_file, 0, SEEK_SET );
   if( 0 >= res_sz ) {
      error_printf( "invalid resource size" );
      goto cleanup;
   }

   debug_printf( 2, "opened resource: %s (%d bytes)", asset_path, res_sz );

   res_handle = (RESOURCE_HANDLE)memory_alloc( res_sz, 1 );
   if( (RESOURCE_HANDLE)0 == (RESOURCE_HANDLE)res_handle ) {
      error_printf( "could not allocate resource buffer of %d bytes", res_sz );
      goto cleanup;
   }

   debug_printf( 1, "allocated resource buffer: %s", asset_path );

   buffer = memory_lock( (MEMORY_HANDLE)res_handle );
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

   if( NULL != res_file ) {
      fclose( res_file );
   }

   if( NULL != buffer ) {
      buffer = memory_unlock( (MEMORY_HANDLE)res_handle );
   }

   return res_handle;
}

MEMORY_PTR resource_lock_handle( RESOURCE_HANDLE handle ) {
   return memory_lock( handle );
}

MEMORY_PTR resource_unlock_handle( RESOURCE_HANDLE handle ) {
   return memory_unlock( handle );
}

int32_t resource_sz_handle( RESOURCE_HANDLE handle ) {
   return memory_sz( handle );
}

void resource_free_handle( RESOURCE_HANDLE handle ) {
   memory_free( handle );
}

#endif /* RESOURCE_FILE */

