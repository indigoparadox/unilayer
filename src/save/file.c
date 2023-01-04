
#define SAVE_C
#include "../unilayer.h"

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

int32_t save_write(
   const char* save_name, MEMORY_HANDLE save_h, int32_t save_buffer_sz
) {
   FILE* save_file = NULL;
   int32_t written_bytes = 0;
   MEMORY_PTR save_buffer;

   debug_printf( 3, "writing to save file: %s", save_name );

   assert( 0 < save_buffer_sz );
   save_buffer = memory_lock( save_h );
   assert( NULL != save_buffer );

   save_file = fopen( save_name, "wb" );
   if( NULL == save_file ) {
      error_printf( "could not open save file: %s", save_name );
      /* TODO: errno */
      written_bytes = -1;
      goto cleanup;
   }

   fwrite( save_buffer, 1, save_buffer_sz, save_file );

cleanup:

   if( NULL != save_buffer ) {
      save_h = memory_unlock( save_h );
   }

   if( NULL != save_file ) {
      fclose( save_file );
   }

   return written_bytes;
}

MEMORY_HANDLE save_read( const char* save_name ) {
   MEMORY_HANDLE h_out = (MEMORY_HANDLE)NULL;
   FILE* save_file = NULL;
   int32_t save_file_sz = 0;
   int32_t bytes_read = 0;
   MEMORY_PTR buffer_out = NULL;

   save_file = fopen( save_name, "rb" );
   if( NULL == save_file ) {
      error_printf( "could not open save file: %s", save_name );
      /* TODO: errno */
      goto cleanup;
   }

   fseek( save_file, 0, SEEK_END );
   save_file_sz = ftell( save_file );
   fseek( save_file, 0, SEEK_SET );

   h_out = memory_alloc( save_file_sz, 1 );
   if( (MEMORY_HANDLE)NULL == h_out ) {
      error_printf( "could not allocate save buffer" );
      goto cleanup;
   }

   buffer_out = memory_lock( h_out );
   assert( NULL != buffer_out );

   bytes_read = fread( buffer_out, 1, save_file_sz, save_file );
   buffer_out = memory_unlock( h_out );

   if( bytes_read != save_file_sz ) {
      error_printf( "%d bytes read, but file was %d bytes!",
         bytes_read, save_file_sz );
      memory_free( h_out );
      h_out = (MEMORY_HANDLE)NULL;
      goto cleanup;
   }

cleanup:

   if( NULL != save_file ) {
      fclose( save_file );
   }

   return h_out;
}

