
#include <string.h>
#include <stdio.h>

#define UNIMAKE_C
#include "unimake.h"

int parse_test_arr(
   char* argv, unsigned long* options, unsigned long dupe_mask,
   char* names[], const unsigned long flags[]
) {
   int j = 0,
      /* Start out assuming bad arg, check below. */
      retval = UNIMAKE_ERROR_INVALID_ARG;

   while( flags[j] != 0 ) {
      debug_printf( 1, "test option: %s", names[j] );
      if( 0 == strncmp( names[j], argv, 3 ) ) {
         if( 0 != (*options & dupe_mask) ) {
            /* Option from this category already selected! */
            goto cleanup;
         }
         debug_printf( 3, "enabled option: %s", names[j] );
         *options |= flags[j];
         retval = 0; /* Arg was good after all! */
         goto cleanup;
      }
      j++;
   }

cleanup:

   return retval;
}

int parse_args( int argc, char** argv, unsigned long* options ) {
   int retval = 0,
      i = 0,
      j = 0;

   for( i = 1 ; argc > i ; i++ ) {
      retval = parse_test_arr( argv[i], options, UNIMAKE_FMT_MASK,
         gc_unimake_fmt_names, gc_unimake_fmt_flags );
      if( !retval ) {
         /* Arg was a format. */
         continue;
      }
      retval = parse_test_arr( argv[i], options, UNIMAKE_GFX_MASK,
         gc_unimake_gfx_names, gc_unimake_gfx_flags );
      if( !retval ) {
         /* Arg was gfx. */
         continue;
      }
      retval = parse_test_arr( argv[i], options, 0x0, /* Allow dupes. */
         gc_unimake_misc_names, gc_unimake_misc_flags );
      if( !retval ) {
         /* Arg was misc. */
         continue;
      }

      /* Bad argument! */
      break;
   }
   return retval;
}

int main( int argc, char** argv ) {
   int retval = 0;
   unsigned long options = 0;

   retval = parse_args( argc, argv, &options );
   if( retval ) {
      error_printf( "invalid argument specified!" );
      goto cleanup;
   }
   
cleanup:

   return retval;
}

