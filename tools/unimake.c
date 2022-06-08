
#include <string.h>
#include <stdio.h>
#include <assert.h>

#define UNIMAKE_C
#include "unimake.h"

int str_replace(
   char* str, int str_sz_max, const char* str_in,
   const char* substr_target, const char* substr_replacement,
   int* chars_advanced_tgt, int* chars_advanced_in
) {
   int retval = 0,
      i = 0,
      substr_replacement_sz = 0,
      substr_target_sz = 0;

   *chars_advanced_tgt = 0;
   *chars_advanced_in = 0;

   if( NULL == substr_target || NULL == substr_replacement ) {
      /* No target/replacement provided! */
      goto cleanup;
   }

   substr_replacement_sz = strlen( substr_replacement );
   substr_target_sz = strlen( substr_target );

   if( 1 >= substr_target_sz || 1 >= substr_replacement_sz ) {
      /* Not necessarily an error, but the search automatically fails. */
      goto cleanup;
   }

   if( substr_target_sz >= str_sz_max ) {
      /* Not necessarily an error, but the search automatically fails. */
      goto cleanup;
   }

   if( 0 != strncmp( str_in, substr_target, substr_target_sz ) ) {
      /* Replace string not found. */
      goto cleanup;
   }

   if( substr_replacement_sz >= str_sz_max ) {
      error_printf( "replacement token \"%s\" too long for string: %s",
         substr_replacement, str_in );
      retval = UNIMAKE_ERROR_STRING_TOO_LONG;
      goto cleanup;
   }

   /* Perform the token replacement. */
   for( i = 0 ; substr_replacement_sz > i ; i++ ) {
      str[i] = substr_replacement[i];
   }

   *chars_advanced_tgt = substr_replacement_sz;
   *chars_advanced_in = substr_target_sz;

cleanup:
   return retval;
}

int str_concat(
   char* str, int* str_sz, int str_sz_max, const char* str_in,
   const char* substr_target, const char* substr_replacement
) {
   int retval = 0,
      i = 0,
      str_in_sz = 0,
      chars_advanced_str = 0,
      chars_advanced_in = 0;

   /* Get string sizes for use later. */
   str_in_sz = strlen( str_in );

   /* Bounds checking. */
   /* TODO: Account for increase due to replacement. */
   /*
   if( *str_sz + str_in_sz + 1 >= str_sz_max ) {
      error_printf( "string (%s) too long to add: %s", str, str_in );
      retval = UNIMAKE_ERROR_STRING_TOO_LONG;
      goto cleanup;
   }
   */

   /* Copy new define(s) into buffer. */
   if( 0 < *str_sz ) {
      str[*str_sz] = ' ';
      (*str_sz)++;
   }
   for( i = 0 ; str_in_sz > i ; i++ ) {
      if( '\r' == str_in[i] || '\n' == str_in[i] ) {
         /* Don't concat newlines. */
         continue;
      }

      /* Attempt token replacement. */
      retval = str_replace(
         &(str[*str_sz]), str_sz_max - *str_sz,
         &(str_in[i]), substr_target, substr_replacement,
         &chars_advanced_str, &chars_advanced_in );
      if( 0 > retval ) {
         /* Error occurred. */
         goto cleanup;

      } else if( 0 < chars_advanced_in ) {
         /* Replacement occurred, advance target counter. */
         debug_printf( 1,
            "advance str_sz (%d), in (%d) of str (%s) by %d in, %d tgt",
            *str_sz, i, str, chars_advanced_in, chars_advanced_str );
         *str_sz += chars_advanced_str;
         i += chars_advanced_in - 1; /* i++ above. */

      } else {
         /* No replacement, just do normal copy. */
         str[*str_sz] = str_in[i];
         (*str_sz)++;
      }
   }
   str[*str_sz] = '\0';

cleanup:
   return retval;
}

int parse_test_arr(
   char* argv, unsigned long* options, unsigned long dupe_mask, int* compiler,
   char* names[], const unsigned long flags[], char* flag_defines[],
   char* flag_libs[],
   char* defines, int* defines_sz, char* libs, int* libs_sz
) {
   int j = 0,
      /* Start out assuming bad arg, check below. */
      retval = UNIMAKE_ERROR_INVALID_ARG;

   while( flags[j] != 0 ) {
      if( 0 == strncmp( names[j], argv, 3 ) ) {
         if( 0 != (*options & dupe_mask) ) {
            /* Option from this category already selected! */
            goto cleanup;
         }
         debug_printf( 3, "enabled option: %s", names[j] );
         *options |= flags[j];
         str_concat(
            defines, defines_sz, UNIFILE_DEFINES_SZ_MAX, flag_defines[j],
            /* TODO: Handle compiler-specific replacements. */
            NULL, NULL );
         str_concat(
            libs, libs_sz, UNIFILE_LIBS_SZ_MAX, flag_libs[j],
            gc_unimake_compiler_lib_tgt[*compiler],
            gc_unimake_compiler_lib_rep[*compiler] );
         retval = 0; /* Arg was good after all! */
         goto cleanup;
      }
      j++;
   }

cleanup:

   return retval;
}

int parse_args(
   int argc, char** argv, unsigned long* options, int* compiler,
   char* defines, int* defines_sz,
   char* libs, int* libs_sz
) {
   int retval = 0,
      i = 0;

   /* TODO: Compiler selection. */

   for( i = 1 ; argc > i ; i++ ) {
      retval = parse_test_arr( argv[i], options, UNIMAKE_PLAT_MASK, compiler,
         gc_unimake_plat_names, gc_unimake_plat_flags, gc_unimake_plat_defines,
         gc_unimake_plat_libs, defines, defines_sz, libs, libs_sz );
      if( UNIMAKE_ERROR_INVALID_ARG != retval ) {
         /* Arg was platform. */
         continue;
      }
      retval = parse_test_arr( argv[i], options, UNIMAKE_FMT_MASK, compiler,
         gc_unimake_fmt_names, gc_unimake_fmt_flags, gc_unimake_fmt_defines,
         gc_unimake_fmt_libs, defines, defines_sz, libs, libs_sz );
      if( UNIMAKE_ERROR_INVALID_ARG != retval ) {
         /* Arg was a format. */
         continue;
      }
      retval = parse_test_arr( argv[i], options, UNIMAKE_GFX_MASK, compiler,
         gc_unimake_gfx_names, gc_unimake_gfx_flags, gc_unimake_gfx_defines,
         gc_unimake_gfx_libs, defines, defines_sz, libs, libs_sz );
      if( UNIMAKE_ERROR_INVALID_ARG != retval ) {
         /* Arg was gfx. */
         continue;
      }
      retval = parse_test_arr( argv[i], options, 0x0, /* Allow dupes. */
         compiler,
         gc_unimake_misc_names, gc_unimake_misc_flags, gc_unimake_misc_defines,
         gc_unimake_misc_libs, defines, defines_sz, libs, libs_sz );
      if( UNIMAKE_ERROR_INVALID_ARG != retval ) {
         /* Arg was misc. */
         continue;
      }

      /* Bad argument! */
      break;
   }
   return retval;
}

FILE* get_unifile( const char* unifile_path ) {
   FILE* unifile = NULL;

   if( NULL == unifile_path ) {
      unifile_path = UNIFILE_PATH_DEFAULT;
   }

   /* Try to open unifile. */
   unifile = fopen( unifile_path, "r" );

   return unifile;
}

char* get_unifile_line_start(
   char* line, int* line_offset, int line_offset_max
) {
   char* line_start = line;

   /* Get rid of errant whitespace. */
   while(
      ('\t' == line_start[0] || ' ' == line_start[0]) &&
      *line_offset < line_offset_max
   ) {
      line_start++;
      (*line_offset)++;
   }

   /* Parse the line. */
   if(
      '#' == line_start[0] ||
      ';' == line_start[0] ||
      '\n' == line_start[0] ||
      '\r' == line_start[0]
   ) {
      /* Skip comments and empty lines. */
      line_start = NULL;
   }

   return line_start;
}

int test_unifile_line_header(
   const char* line, int line_sz,
   const char* header_type, const char* header_plat
) {
   int retval = 1,
      header_type_sz = 0,
      header_plat_sz = 0;

   /* header_type and header_plat only come from our own code,
    * so we can strlen() them here. */
   header_type_sz = strlen( header_type );
   header_plat_sz = strlen( header_plat );

   if( '[' != line[0] ) {
      /* Not a header. */
      retval = 0;
      goto cleanup;
   }

   if( 0 != strncmp( header_type, &(line[1]), header_type_sz ) ) {
      /* Not the header we're looking for. */
      retval = 0;
      goto cleanup;
   }

   if( NULL == header_plat && ']' != line[header_type_sz + 1] ) {
      /* Looking for no platform but this is a platform. */
      retval = 0;
      goto cleanup;
   }

   if( 
      NULL != header_plat && (
         line_sz <= header_plat_sz + header_type_sz + 2 ||
         0 != strncmp(
            header_plat, &(line[header_type_sz + 2]), header_plat_sz ))
   ) {
      /* Looking for a platform but this isn't it. */
      retval = 0;
      goto cleanup;
   }

cleanup:
   return retval;
}

int parse_unifile_compiler_args(
   const char* unifile_path,
   char* compiler_args, int* compiler_args_sz, int compiler_args_sz_max,
   const char* args_type, const char* args_plat
) {
   FILE* unifile = NULL;
   int retval = 0,
      parse_state = UNIFILE_STATE_NONE,
      line_offset = 0;
   char line[UNIFILE_LINE_SZ + 1] = { 0 },
      * line_start = NULL;

   debug_printf( 1, "parsing unifile compiler args..." );

   unifile = get_unifile( unifile_path );
   if( NULL == unifile ) {
      retval = UNIMAKE_ERROR_BAD_UNIFILE_PATH;
      goto cleanup;
   }

   while( NULL != fgets( line, UNIFILE_LINE_SZ, unifile ) ) {

      line_start = get_unifile_line_start(
         line, &line_offset, UNIFILE_LINE_SZ - strlen( args_type ) );
      
      if( NULL == line_start ) {
         /* Invalid line. */
         continue;

      } else if(
         test_unifile_line_header(
            line_start, UNIFILE_LINE_SZ - line_offset, args_type, args_plat )
      ) {
         /* Change state to platform code files. */
         parse_state = UNIFILE_STATE_PARSE;
         debug_printf( 1, "unifile state: %d", parse_state );
         
      } else if( '[' == line_start[0] ) {
         /* Unknown section found. */
         parse_state = UNIFILE_STATE_NONE;
         debug_printf( 1, "unifile state: %d", parse_state );
      
      } else if( UNIFILE_STATE_PARSE == parse_state ) {
         /* Add code file after making sure we can. */

         retval = str_concat(
            compiler_args, compiler_args_sz, compiler_args_sz_max,
            line,
            /* TODO: Handle compiler-specific replacements. */
            NULL, NULL );
         if( retval ) {
            goto cleanup;
         }
      }
   }

cleanup:

   if( NULL != unifile ) {
      fclose( unifile );
   }

   return retval;
}

int parse_unifile_paths(
   const char* unifile_path,
   char files_array[UNIFILE_PATHS_MAX][UNIFILE_PATH_SZ_MAX + 1],
   int* files_array_sz, const char* files_type, const char* files_plat
) {
   FILE* unifile = NULL;
   int retval = 0,
      parse_state = UNIFILE_STATE_NONE,
      line_offset = 0;
   char line[UNIFILE_LINE_SZ + 1] = { 0 },
      * line_start = NULL;

   debug_printf( 1, "parsing unifile paths..." );

   unifile = get_unifile( unifile_path );
   if( NULL == unifile ) {
      retval = UNIMAKE_ERROR_BAD_UNIFILE_PATH;
      goto cleanup;
   }

   while( NULL != fgets( line, UNIFILE_LINE_SZ, unifile ) ) {

      line_start = get_unifile_line_start(
         line, &line_offset, UNIFILE_LINE_SZ - strlen( files_type ) );
      
      if( NULL == line_start ) {
         /* Invalid line. */
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
         parse_state = UNIFILE_STATE_PARSE;
         debug_printf( 1, "unifile state: %d", parse_state );
         
      } else if( '[' == line_start[0] ) {
         /* Unknown section found. */
         parse_state = UNIFILE_STATE_NONE;
         debug_printf( 1, "unifile state: %d", parse_state );
      
      } else if( UNIFILE_STATE_PARSE == parse_state ) {
         /* Add code file after making sure we can. */

         if( *files_array_sz + 1 >= UNIFILE_PATHS_MAX ) {
            error_printf( "too many code files" );
            retval = UNIMAKE_ERROR_TOO_MANY_CODE_FILES;
            goto cleanup;
         
         } else if( strlen( line_start ) > UNIFILE_PATH_SZ_MAX ) {
            error_printf( "code path too long: %s", line_start );
            retval = UNIMAKE_ERROR_STRING_TOO_LONG;
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

   /* Parse CLI args. */
   retval = parse_args( argc, argv, &(um_state.options), &(um_state.compiler),
      um_state.defines, &(um_state.defines_sz),
      um_state.libs, &(um_state.libs_sz) );
   if( retval ) {
      error_printf( "invalid argument specified!" );
      goto cleanup;
   }

   debug_printf( 1, "args parsed. flags: %08lx, defines: %s",
      um_state.options, um_state.defines );

   /* Parse Unifile. */

   plat_type = get_plat_name( um_state.options );

   /* TODO: Pass specified path to unifile. */
   retval = parse_unifile_paths( NULL,
      um_state.code_files, &(um_state.code_files_sz), "code", NULL );
   if( retval ) {
      goto cleanup;
   }

   retval = parse_unifile_paths( NULL,
      um_state.code_files, &(um_state.code_files_sz), "code", plat_type );
   if( retval ) {
      goto cleanup;
   }

   retval = parse_unifile_compiler_args( NULL,
      um_state.defines, &(um_state.defines_sz), UNIFILE_DEFINES_SZ_MAX,
      "defines", plat_type );
   if( retval ) {
      goto cleanup;
   }

   retval = parse_unifile_compiler_args( NULL,
      um_state.libs, &(um_state.libs_sz), UNIFILE_LIBS_SZ_MAX,
      "libs", plat_type );
   if( retval ) {
      goto cleanup;
   }

   debug_printf( 1, "unifile parsed. flags: %08lx, defines: %s, libs: %s",
      um_state.options, um_state.defines, um_state.libs );
 
cleanup:

   return retval;
}

