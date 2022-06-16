
#define SAVE_C
#include "../unilayer.h"

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

int32_t save_write(
   const char* save_name, uint8_t* save_buffer, int32_t save_buffer_sz
) {
   FILE* save_file = NULL;
   int32_t written_bytes = 0;

   debug_printf( 3, "writing to save file: %s", save_name );

   save_file = fopen( save_name, "wb" );
   if( NULL == save_file ) {
      error_printf( "could not open save file: %s", save_name );
      written_bytes = -1;
      goto cleanup;
   }

   fwrite( save_buffer, 1, save_buffer_sz, save_file );

cleanup:

   if( NULL != save_file ) {
      fclose( save_file );
   }

   return written_bytes;
}

