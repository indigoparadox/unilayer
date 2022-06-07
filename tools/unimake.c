
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
      i = 0;

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

int parse_unifile_paths( char* unifile_path, struct unimake_state* um_state ) {
   FILE* unifile = NULL;
   int retval = 0,
      parse_state = UNIFILE_STATE_NONE;
   char line[UNIFILE_LINE_SZ + 1] = { 0 };

   if( NULL == unifile_path ) {
      unifile_path = UNIFILE_PATH_DEFAULT;
   }

   /* Try to open unifile. */
   unifile = fopen( unifile_path, "r" );
   if( NULL == unifile ) {
      retval = UNIMAKE_ERROR_BAD_UNIFILE_PATH;
      goto cleanup;
   }

   while( NULL != fgets( line, UNIFILE_LINE_SZ, unifile ) ) {
      if(
         '#' == line[0] || ';' == line[0] || '\n' == line[0] || '\r' == line[0]
      ) {
         /* Skip comments and empty lines. */
         /* TODO: Handle errant whitespace lines. */
         continue;

      } else if( 0 == strncmp( "[code", line, 5 ) ) {
         /* Change state to platform code files. */
         parse_state = UNIFILE_STATE_PLATFILES;
         debug_printf( 1, "unifile state: %d", parse_state );
         
      } else if( UNIFILE_STATE_PLATFILES == parse_state ) {
         /* Add code file after making sure we can. */

         if( um_state->code_files_sz + 1 >= UNIFILE_PATHS_MAX ) {
            error_printf( "too many code files" );
            retval = UNIMAKE_ERROR_TOO_MANY_CODE_FILES;
            goto cleanup;
         
         } else if( strlen( line ) > UNIFILE_PATH_SZ_MAX ) {
            error_printf( "code path too long: %s", line );
            retval = UNIMAKE_ERROR_PATH_TOO_LONG;
            goto cleanup;
         }

         strncpy(
            um_state->code_files[um_state->code_files_sz], line,
            /* We always null-term line above (there's a +1), so worry more
             * about dest array. */
            UNIFILE_PATH_SZ_MAX );
         um_state->code_files_sz++;

         debug_printf( 1, "%d code files (added %s)",
            um_state->code_files_sz, line );
      }
   }

cleanup:

   if( NULL != unifile ) {
      fclose( unifile );
   }

   return retval;
}

int main( int argc, char** argv ) {
   int retval = 0;
   unsigned long options = 0;
   struct unimake_state um_state;

   memset( &um_state, '\0', sizeof( struct unimake_state ) );

   retval = parse_args( argc, argv, &options );
   if( retval ) {
      error_printf( "invalid argument specified!" );
      goto cleanup;
   }

   /* TODO: Pass specified path to unifile. */
   retval = parse_unifile( NULL, &um_state );
   
cleanup:

   return retval;
}

