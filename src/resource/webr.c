
#define RESOURCE_C
#include "../unilayer.h"

RESOURCE_HANDLE resource_get_handle( const RESOURCE_ID id ) {
   RESOURCE_HANDLE handle_out = NULL;
   emscripten_fetch_attr_t attr;
   emscripten_fetch_t* fetcher;

   debug_printf( 2, "downloading %s...", id );

   strncpy( attr.requestMethod, "GET", 4 );
   attr.attributes = 
      EMSCRIPTEN_FETCH_LOAD_TO_MEMORY | EMSCRIPTEN_FETCH_SYNCHRONOUS |
      EMSCRIPTEN_FETCH_REPLACE;

   fetcher = emscripten_fetch( &attr, id );

   /*
   while( 4 > fetcher->readyState ) {
      debug_printf( 2, "downloading... stage %d", fetcher->readyState );
   }
   */

   if( 200 == fetcher->status ) {
      debug_printf( 2, "download successful" );
   } else {
      error_printf( "download failed with code: %d", fetcher->status );
      return NULL;
   }

   /* Create the handle to export the data. */
   handle_out = calloc( 1, sizeof( struct RESOURCE_WEBR_HANDLE ) );
   assert( NULL != handle_out );
   handle_out->fetch = fetcher;

   return handle_out;
}

MEMORY_PTR resource_lock_handle( RESOURCE_HANDLE handle ) {
   return handle->fetch->data;
}

MEMORY_PTR resource_unlock_handle( RESOURCE_HANDLE handle ) {
   return NULL;
}

int32_t resource_sz_handle( RESOURCE_HANDLE handle ) {
   /* TODO */
   return 0;
}

void resource_free_handle( RESOURCE_HANDLE handle ) {
   emscripten_fetch_close( handle->fetch );
   free( handle );
}

uint8_t resource_id_from_name(
   RESOURCE_ID* id, RESOURCE_NAME name, const char* ext
) {
   if(
      NULL != name && 
      memory_strnlen_ptr( name, RESOURCE_NAME_MAX ) < RESOURCE_NAME_MAX
   ) {
      memory_zero_ptr( (*id), RESOURCE_PATH_MAX );
      /* TODO: Add in resource URL. */
      dio_snprintf( (*id), RESOURCE_PATH_MAX,
         /* TODO: Use ASSETS_DIR def. */
         "http://localhost:8000/assets/%s/%s.%s",
         DEPTH_SPEC, name, ext );
      return 1;
   } else {
      error_printf( "attempted to assign invalid resource: %s", name );
   }
   return 0;
}

