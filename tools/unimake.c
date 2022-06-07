
#include <string.h>
#include <stdio.h>
#include <assert.h>

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
      retval = parse_test_arr( argv[i], options, UNIMAKE_PLAT_MASK,
         gc_unimake_plat_names, gc_unimake_plat_flags );
      if( !retval ) {
         /* Arg was platform. */
         continue;
      }
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

int parse_unifile_paths(
   char* unifile_path,
   char files_array[UNIFILE_PATHS_MAX][UNIFILE_PATH_SZ_MAX + 1],
   int* files_array_sz, const char* files_type, const char* files_plat
) {
   FILE* unifile = NULL;
   int retval = 0,
      parse_state = UNIFILE_STATE_NONE,
      line_offset = 0;
   char line[UNIFILE_LINE_SZ + 1] = { 0 },
      * line_start = NULL;

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

      /* Get rid of errant whitespace. */
      line_start = line;
      while(
         ('\t' == line_start[0] || ' ' == line_start[0]) &&
         line_offset < UNIFILE_LINE_SZ - strlen( files_type )
      ) {
         line_start++;
         line_offset++;
      }

      /* Parse the line. */
      if(
         '#' == line_start[0] ||
         ';' == line_start[0] ||
         '\n' == line_start[0] ||
         '\r' == line_start[0]
      ) {
         /* Skip comments and empty lines. */
         /* TODO: Handle errant whitespace lines. */
         continue;

      } else if(
         '[' == line_start[0] &&
         0 == strncmp( files_type, &(line_start[1]), 4 ) && (
            /* Looking for no platform and found it. */
            (NULL == files_plat && ']' == line_start[5]) ||
            /* Looking for a platform and found it. */
            (NULL != files_plat &&
               strlen( line_start ) > 10 &&
               0 == strncmp( files_plat, &(line_start[6]), 3 )))
      ) {
         /* Change state to platform code files. */
         parse_state = UNIFILE_STATE_PLATFILES;
         debug_printf( 1, "unifile state: %d", parse_state );
         
         /* ADD NEW SECTIONS HERE. */

      } else if( '[' == line_start[0] ) {
         /* Unknown section found. */
         parse_state = UNIFILE_STATE_NONE;
         debug_printf( 1, "unifile state: %d", parse_state );
      
      } else if( UNIFILE_STATE_PLATFILES == parse_state ) {
         /* Add code file after making sure we can. */

         if( *files_array_sz + 1 >= UNIFILE_PATHS_MAX ) {
            error_printf( "too many code files" );
            retval = UNIMAKE_ERROR_TOO_MANY_CODE_FILES;
            goto cleanup;
         
         } else if( strlen( line_start ) > UNIFILE_PATH_SZ_MAX ) {
            error_printf( "code path too long: %s", line_start );
            retval = UNIMAKE_ERROR_PATH_TOO_LONG;
            goto cleanup;
         }

         assert( UNIFILE_LINE_SZ <= UNIFILE_PATH_SZ_MAX );

         strncpy(
            files_array[*files_array_sz], line_start, strlen( line_start ) );
         (*files_array_sz)++;

         debug_printf( 1, "%d %s files for %s (added %s)",
            *files_array_sz, files_type, files_plat, line );
      }
   }

cleanup:

   if( NULL != unifile ) {
      fclose( unifile );
   }

   return retval;
}

char* get_plat_name( unsigned long options ) {
   int i = 0;

   while( gc_unimake_plat_flags[i] != (options & UNIMAKE_PLAT_MASK) ) {
      i++;
   }

   /* TODO: Bounds checking. */

   return gc_unimake_plat_names[i];
}

int main( int argc, char** argv ) {
   int retval = 0;
   struct unimake_state um_state;
   char* plat_type = NULL;

   memset( &um_state, '\0', sizeof( struct unimake_state ) );

   retval = parse_args( argc, argv, &(um_state.options) );
   if( retval ) {
      error_printf( "invalid argument specified!" );
      goto cleanup;
   }

   plat_type = get_plat_name( um_state.options );

   /* TODO: Pass specified path to unifile. */
   retval = parse_unifile_paths( NULL,
      um_state.code_files, &(um_state.code_files_sz), "code", NULL );
   retval = parse_unifile_paths( NULL,
      um_state.code_files, &(um_state.code_files_sz), "code", plat_type );
   
cleanup:

   return retval;
}

