
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

#define UNIMAKE_C
#include "unimake.h"

int str_in_array( const char* str, int str_sz, char* array[] ) {
   int retval = -1,
      i = 0;

   while( 1 < strlen( array[i] ) ) {
      if( 0 == strncmp( array[i], str, str_sz ) ) {
         retval = i;
         goto cleanup;
      }
      i++;
   }

cleanup:
   return retval;
}

int str_find_char_r( const char* str, int str_sz, char c ) {
   int i = 0;

   for( i = str_sz ; 0 <= i ; i-- ) {
      if( str[i] == c ) {
         return i;
      }
   }

   return i;
}

int str_replace(
   char* str, int str_sz_max, const char* str_in,
   const char* substr_target, const char* substr_replacement,
   int* chars_advanced_tgt, int* chars_advanced_in
) {
   int retval = 0,
      i = 0,
      substr_replacement_sz = 0,
      substr_target_sz = 0;

   if( NULL != chars_advanced_tgt ) {
      *chars_advanced_tgt = 0;
   }
   if( NULL != chars_advanced_in ) {
      *chars_advanced_in = 0;
   }

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

   if( NULL != chars_advanced_tgt ) {
      *chars_advanced_tgt = substr_replacement_sz;
   }
   if( NULL != chars_advanced_in ) {
      *chars_advanced_in = substr_target_sz;
   }

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

   /* Copy new define(s) into buffer. */
   if( 0 < *str_sz ) {
      /* Add a space before we start. */
      /*
      str[*str_sz] = ' ';
      (*str_sz)++;
      */
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
   retval = 0;

cleanup:
   return retval;
}

int apply_flag(
   int flag_idx, int compiler,
   char* names[], const unsigned long flags[], char* flag_defines[],
   char* flag_libs[], char* flag_includes[],
   char* defines, int* defines_sz, char* libs, int* libs_sz,
   char* cflags, int* cflags_sz, char* includes, int* includes_sz
) {
   int dbg_idx = 0,
      retval = 0;

   if( 1 < strlen( flag_defines[flag_idx] ) ) {
      str_concat_char(
         defines, defines_sz, UNIFILE_DEFINES_SZ_MAX, ' ', retval );
      retval = str_concat(
         defines, defines_sz, UNIFILE_DEFINES_SZ_MAX, flag_defines[flag_idx],
         /* TODO: Handle compiler-specific replacements. */
         NULL, NULL );
      if( 0 > retval ) {
         goto cleanup;
      }
   }

   if( 1 < strlen( flag_includes[flag_idx] ) ) {
      str_concat_char(
         includes, includes_sz, UNIFILE_INCLUDES_SZ_MAX, ' ', retval );
      str_concat(
         includes, includes_sz, UNIFILE_INCLUDES_SZ_MAX,
         flag_includes[flag_idx],
         gc_unimake_compiler_inc_tgt[compiler],
         gc_unimake_compiler_inc_rep[compiler] );
      if( 0 > retval ) {
         goto cleanup;
      }
   }

   if( 1 < strlen( flag_libs[flag_idx] ) ) {
      str_concat_char( libs, libs_sz, UNIFILE_LIBS_SZ_MAX, ' ', retval );
      str_concat(
         libs, libs_sz, UNIFILE_LIBS_SZ_MAX, flag_libs[flag_idx],
         gc_unimake_compiler_lib_tgt[compiler],
         gc_unimake_compiler_lib_rep[compiler] );
      if( 0 > retval ) {
         goto cleanup;
      }
   }

   /* Special case: if this is the debug flag. */
   dbg_idx = str_in_array( "dbg", 3, gc_unimake_misc_names );
   if(
      gc_unimake_misc_flags[dbg_idx] == flags[flag_idx] &&
      1 < strlen( gc_unimake_compiler_dbg_rep[compiler] )
   ) {
      str_concat_char( cflags, cflags_sz, UNIFILE_CFLAGS_SZ_MAX, ' ', retval );
      retval = str_concat(
         cflags, cflags_sz, UNIFILE_CFLAGS_SZ_MAX,
         gc_unimake_misc_cflags[flag_idx],
         "$DEBUG$", gc_unimake_compiler_dbg_rep[compiler] );
      if( 0 > retval ) {
         goto cleanup;
      }
   }

cleanup:
   return retval;
}

int parse_test_arr(
   char* argv, unsigned long* options, unsigned long dupe_mask, int* compiler,
   char* names[], const unsigned long flags[], char* flag_defines[],
   char* flag_libs[], char* flag_includes[],
   char* defines, int* defines_sz, char* libs, int* libs_sz,
   char* cflags, int* cflags_sz, char* includes, int* includes_sz
) {
   int argv_idx = -1,
      /* Start out assuming bad arg, check below. */
      retval = UNIMAKE_ERROR_INVALID_ARG;

   argv_idx = str_in_array( argv, 3, names );
   if( 0 > argv_idx ) {
      goto cleanup;
   }

   if( 0 != (*options & dupe_mask) ) {
      /* Option from this category already selected! */
      goto cleanup;
   }

   debug_printf( 3, "enabled option: %s", names[argv_idx] );
   *options |= flags[argv_idx];
   apply_flag( argv_idx, *compiler,
      names, flags, flag_defines, flag_libs, flag_includes,
      defines, defines_sz, libs, libs_sz, cflags, cflags_sz,
      includes, includes_sz );
   retval = 0; /* Arg was good after all! */

cleanup:

   return retval;
}

int parse_args(
   int argc, char** argv, unsigned long* options, int* compiler,
   char* defines, int* defines_sz,
   char* libs, int* libs_sz, char* cflags, int* cflags_sz,
   char* includes, int* includes_sz
) {
   int retval = 0,
      i = 0;

   /* Compiler selection is a special case first-pass, as it affects how the
    * args below are added/parsed. */
   for( i = 1 ; *compiler < 0 && argc > i ; i++ ) {
      if( 0 > *compiler ) {
         *compiler = str_in_array(
            argv[i], strlen( argv[i] ), gc_unimake_compiler_cc );
         if( 0 <= *compiler ) {
            debug_printf( 3, "selected compiler %d: %s",
               *compiler, gc_unimake_compiler_cc[*compiler] );
         }
      }
   }

   if( 0 > *compiler ) {
      *compiler = 0;
      debug_printf(
         3, "defaulting to compiler: %s", gc_unimake_compiler_cc[0] );
   }

   for( i = 1 ; argc > i ; i++ ) {
      /* The compiler arg is fine, too. */
      retval = str_in_array(
         argv[i], strlen( argv[i] ), gc_unimake_compiler_cc );
      if( 0 <= retval ) {
         retval = 0;
         continue;
      }

      /* Parse build options. */
      retval = parse_test_arr( argv[i], options, UNIMAKE_PLAT_MASK, compiler,
         gc_unimake_plat_names, gc_unimake_plat_flags, gc_unimake_plat_defines,
         gc_unimake_plat_libs, gc_unimake_plat_includes,
         defines, defines_sz, libs, libs_sz, cflags, cflags_sz,
         includes, includes_sz );
      if( UNIMAKE_ERROR_INVALID_ARG != retval ) {
         /* Arg was platform. */
         continue;
      }
      retval = parse_test_arr( argv[i], options, UNIMAKE_FMT_MASK, compiler,
         gc_unimake_fmt_names, gc_unimake_fmt_flags, gc_unimake_fmt_defines,
         gc_unimake_fmt_libs, gc_unimake_fmt_includes,
         defines, defines_sz, libs, libs_sz, cflags, cflags_sz,
         includes, includes_sz );
      if( UNIMAKE_ERROR_INVALID_ARG != retval ) {
         /* Arg was a format. */
         continue;
      }
      retval = parse_test_arr( argv[i], options, UNIMAKE_GFX_MASK, compiler,
         gc_unimake_gfx_names, gc_unimake_gfx_flags, gc_unimake_gfx_defines,
         gc_unimake_gfx_libs, gc_unimake_gfx_includes,
         defines, defines_sz, libs, libs_sz, cflags, cflags_sz,
         includes, includes_sz );
      if( UNIMAKE_ERROR_INVALID_ARG != retval ) {
         /* Arg was gfx. */
         continue;
      }
      retval = parse_test_arr( argv[i], options, 0x0, /* Allow dupes. */
         compiler,
         gc_unimake_misc_names, gc_unimake_misc_flags, gc_unimake_misc_defines,
         gc_unimake_misc_libs, gc_unimake_misc_includes,
         defines, defines_sz, libs, libs_sz, cflags, cflags_sz,
         includes, includes_sz );
      if( UNIMAKE_ERROR_INVALID_ARG != retval ) {
         /* Arg was misc. */
         continue;
      }

      /* Bad argument! */
      goto cleanup;
   }

   /* Apply defaults for unspecified categories. */
   if( 0 == (UNIMAKE_GFX_MASK & *options) ) {
      debug_printf(
         3, "applying default graphics: %s", gc_unimake_gfx_names[0] );
      apply_flag( 0, *compiler,
         gc_unimake_gfx_names, gc_unimake_gfx_flags,
         gc_unimake_gfx_defines, gc_unimake_gfx_libs, gc_unimake_gfx_includes,
         defines, defines_sz, libs, libs_sz, cflags, cflags_sz,
         includes, includes_sz );
   }

   if( 0 == (UNIMAKE_FMT_MASK & *options) ) {
      debug_printf(
         3, "applying default format: %s", gc_unimake_fmt_names[0] );
      apply_flag( 0, *compiler,
         gc_unimake_fmt_names, gc_unimake_fmt_flags,
         gc_unimake_fmt_defines, gc_unimake_fmt_libs, gc_unimake_fmt_includes,
         defines, defines_sz, libs, libs_sz, cflags, cflags_sz,
         includes, includes_sz);
   }

cleanup:
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
   if( NULL != header_plat ) {
      header_plat_sz = strlen( header_plat );
   }

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
   const char* args_type, const char* args_plat,
   char* arg_target, char* arg_replace
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

         str_concat_char(
            compiler_args, compiler_args_sz, compiler_args_sz_max, ' ',
            retval );
         retval = str_concat(
            compiler_args, compiler_args_sz, compiler_args_sz_max,
            line, arg_target, arg_replace );
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

char* get_flag_name(
   unsigned long options, unsigned long flags[], int flag_mask, char* names[]
) {
   int i = 0;

   while( flags[i] != (options & flag_mask) ) {
      i++;
   }

   /* TODO: Bounds checking. */

   return names[i];
}

int build_dir( const char* dir_path, int dir_path_sz ) {
   char dir_path_tmp[UNIFILE_PATH_SZ_MAX] = { 0 };
   int recurse_dir_sep_pos = 0,
      retval = -1;
   struct stat dir_info;

   /* Copy dir_path to the temp buffer so we can insert a NULL at dir_path_sz.
    */
   strncpy( dir_path_tmp, dir_path, dir_path_sz );
   dir_path_tmp[dir_path_sz + 1] = '\0';

   /* See if we need to recurse to create this directory's parent. */
   recurse_dir_sep_pos = str_find_char_r( dir_path_tmp, dir_path_sz, '/' );
   if( 0 <= recurse_dir_sep_pos ) {
      build_dir( dir_path_tmp, recurse_dir_sep_pos );
   }

   if( 0 == stat( dir_path_tmp, &dir_info ) ) {
      debug_printf( 1, "directory %s exists", dir_path_tmp );
      retval = 0;
      goto cleanup;
   }

   /* TODO: Checking for GNU C is probably good enough here, but we're really
    *       checking to distinguish vs e.g. Turbo C w/ 1-parm mkdir().
    */
#ifdef __GNUC__
   retval = mkdir( dir_path_tmp, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH );
#endif /* __GNUC__ */
   
   debug_printf( 1, "%s", dir_path_tmp );

cleanup:
   return retval;
}

#if 0
#define unimake_concat_path( path, path_sz, path_sz_max, name, flags, mask, options
   name = get_flag_name(
      options, flags, mask, names );
   retval = str_concat(
      path, &(path_sz), UNIFILE_PATH_SZ_MAX, plat_name, NULL, NULL );
   str_concat_char(
      obj_path, &obj_path_sz, UNIFILE_PATH_SZ_MAX, '/', retval );
   if( 0 > retval ) {
      /* Error occurred. */
      goto cleanup;
   }
#endif

int build_obj_path(
   char* obj_path, int* obj_path_sz, char* src_path, unsigned long options,
   const char* root
) {
   int retval = 0;
   char* plat_name = NULL,
      * gfx_name = NULL,
      * fmt_name = NULL;

   retval = str_concat(
      obj_path, obj_path_sz, UNIFILE_PATH_SZ_MAX, root, NULL, NULL );
   if( 0 > retval ) {
      /* Error occurred. */
      goto cleanup;
   }
   str_concat_char(
      obj_path, obj_path_sz, UNIFILE_PATH_SZ_MAX, '/', retval );

   /* Object Path: Platform Name */
   plat_name = get_flag_name(
      options, gc_unimake_plat_flags, UNIMAKE_PLAT_MASK,
      gc_unimake_plat_names );
   retval = str_concat(
      obj_path, obj_path_sz, UNIFILE_PATH_SZ_MAX,
      plat_name, NULL, NULL );
   if( 0 > retval ) {
      /* Error occurred. */
      goto cleanup;
   }
   str_concat_char(
      obj_path, obj_path_sz, UNIFILE_PATH_SZ_MAX, '/', retval );

   /* Object Path: Graphics Name */
   gfx_name = get_flag_name(
      options, gc_unimake_gfx_flags, UNIMAKE_GFX_MASK,
      gc_unimake_gfx_names );
   retval = str_concat(
      obj_path, obj_path_sz, UNIFILE_PATH_SZ_MAX,
      gfx_name, NULL, NULL );
   if( 0 > retval ) {
      /* Error occurred. */
      goto cleanup;
   }
   str_concat_char(
      obj_path, obj_path_sz, UNIFILE_PATH_SZ_MAX, '/', retval );

   /* Object Path: Format Name */
   fmt_name = get_flag_name(
      options, gc_unimake_fmt_flags, UNIMAKE_FMT_MASK,
      gc_unimake_fmt_names );
   retval = str_concat(
      obj_path, obj_path_sz, UNIFILE_PATH_SZ_MAX,
      fmt_name, NULL, NULL );
   if( 0 > retval ) {
      /* Error occurred. */
      goto cleanup;
   }
   str_concat_char(
      obj_path, obj_path_sz, UNIFILE_PATH_SZ_MAX, '/', retval );

   retval = str_concat(
      obj_path, obj_path_sz, UNIFILE_PATH_SZ_MAX, src_path, ".c", ".o" );
   if( 0 > retval ) {
      /* Error occurred. */
      goto cleanup;
   }

cleanup:
   return retval;
}

int build_obj( struct unimake_state* um_state, char* src_path ) {
   char cmd_line[UNIMAKE_CLI_SZ_MAX + 1] = { 0 },
      obj_path[UNIFILE_PATH_SZ_MAX + 1] = { 0 };
   int cmd_line_sz = 0,
      obj_path_sz = 0,
      retval = 0;
   
   retval = str_concat(
      cmd_line, &cmd_line_sz, UNIMAKE_CLI_SZ_MAX,
      gc_unimake_compiler_cc[um_state->compiler],
      NULL, NULL );
   if( 0 > retval ) {
      /* Error occurred. */
      goto cleanup;
   }

   retval = str_concat(
      cmd_line, &cmd_line_sz, UNIMAKE_CLI_SZ_MAX,
      um_state->defines,
      NULL, NULL );
   if( 0 > retval ) {
      /* Error occurred. */
      goto cleanup;
   }

   retval = str_concat(
      cmd_line, &cmd_line_sz, UNIMAKE_CLI_SZ_MAX,
      um_state->includes,
      NULL, NULL );
   if( 0 > retval ) {
      /* Error occurred. */
      goto cleanup;
   }

   retval = str_concat(
      cmd_line, &cmd_line_sz, UNIMAKE_CLI_SZ_MAX,
      um_state->cflags,
      NULL, NULL );
   if( 0 > retval ) {
      /* Error occurred. */
      goto cleanup;
   }

   /* Build the object path. */
   retval = build_obj_path(
      obj_path, &obj_path_sz, src_path, um_state->options,
      UNIMAKE_OBJ_DIR );
   if( 0 > retval ) {
      /* Error occurred. */
      goto cleanup;
   }

   retval = build_dir(
      obj_path, str_find_char_r( obj_path, obj_path_sz, '/' ) );
   if( 0 > retval ) {
      /* Error occurred. */
      goto cleanup;
   }

   str_concat_char(
      cmd_line, &cmd_line_sz, UNIMAKE_CLI_SZ_MAX, ' ', retval );
   retval = str_concat(
      cmd_line, &cmd_line_sz, UNIMAKE_CLI_SZ_MAX,
      gc_unimake_compiler_obj_out[um_state->compiler],
      "$FILE$", obj_path );
   if( 0 > retval ) {
      /* Error occurred. */
      goto cleanup;
   }

   str_concat_char(
      cmd_line, &cmd_line_sz, UNIMAKE_CLI_SZ_MAX, ' ', retval );
   retval = str_concat(
      cmd_line, &cmd_line_sz, UNIMAKE_CLI_SZ_MAX,
      src_path, NULL, NULL );
   if( 0 > retval ) {
      /* Error occurred. */
      goto cleanup;
   }

   debug_printf( 1, "%s", cmd_line );

   retval = system( cmd_line );

cleanup:
   return retval;
}

int build_exe( struct unimake_state* um_state ) {
   char cmd_line[UNIMAKE_CLI_SZ_MAX + 1] = { 0 },
      exe_path[UNIFILE_PATH_SZ_MAX + 1] = { 0 },
      obj_path[UNIFILE_PATH_SZ_MAX + 1] = { 0 };
   int retval = 0,
      obj_path_sz = 0,
      exe_path_sz = 0,
      cmd_line_sz = 0,
      i = 0;

   retval = str_concat(
      cmd_line, &cmd_line_sz, UNIMAKE_CLI_SZ_MAX,
      gc_unimake_compiler_ld[um_state->compiler],
      NULL, NULL );
   if( 0 > retval ) {
      /* Error occurred. */
      goto cleanup;
   }

   /* Build the binary path. */
   retval = build_obj_path(
      exe_path, &exe_path_sz, "a.exe", um_state->options,
      UNIMAKE_BIN_DIR );
   if( 0 > retval ) {
      /* Error occurred. */
      goto cleanup;
   }

   str_concat_char(
      cmd_line, &cmd_line_sz, UNIMAKE_CLI_SZ_MAX, ' ', retval );
   retval = str_concat(
      cmd_line, &cmd_line_sz, UNIMAKE_CLI_SZ_MAX,
      gc_unimake_compiler_exe_out[um_state->compiler],
      "$FILE$", exe_path );
   if( 0 > retval ) {
      /* Error occurred. */
      goto cleanup;
   }

   for( i = 0 ; um_state->code_files_sz > i ; i++ ) {
      /* Reset path size for each iteration. */
      obj_path_sz = 0;

      retval = build_obj_path(
         obj_path, &obj_path_sz, um_state->code_files[i], um_state->options,
         UNIMAKE_OBJ_DIR );
      if( 0 > retval ) {
         /* Error occurred. */
         goto cleanup;
      }

      str_concat_char(
         cmd_line, &cmd_line_sz, UNIMAKE_CLI_SZ_MAX, ' ', retval );
      retval = str_concat(
         cmd_line, &cmd_line_sz, UNIMAKE_CLI_SZ_MAX, obj_path, NULL, NULL );
      if( 0 > retval ) {
         /* Error occurred. */
         goto cleanup;
      }
   }

   /* TODO: LDFLAGS */

   retval = system( cmd_line );

   debug_printf( 1, "%s", cmd_line );

cleanup:

   return retval;
}

int main( int argc, char** argv ) {
   int retval = 0,
      i = 0;
   struct unimake_state um_state;
   char* plat_type = NULL;

   memset( &um_state, '\0', sizeof( struct unimake_state ) );
   um_state.compiler = -1;

   /* Parse CLI args. */
   retval = parse_args( argc, argv, &(um_state.options), &(um_state.compiler),
      um_state.defines, &(um_state.defines_sz),
      um_state.libs, &(um_state.libs_sz),
      um_state.cflags, &(um_state.cflags_sz),
      um_state.includes, &(um_state.includes_sz) );
   if( retval ) {
      error_printf( "invalid argument specified!" );
      goto cleanup;
   }

   debug_printf( 1, "args parsed. flags: %08lx, defines: %s",
      um_state.options, um_state.defines );

   /* Parse Unifile. */

   plat_type = get_flag_name(
      um_state.options, gc_unimake_plat_flags, UNIMAKE_PLAT_MASK,
      gc_unimake_plat_names );

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

   /* defines */

   retval = parse_unifile_compiler_args( NULL,
      um_state.defines, &(um_state.defines_sz), UNIFILE_DEFINES_SZ_MAX,
      "defines", plat_type,
      NULL, NULL /* TODO */);
   if( retval ) {
      goto cleanup;
   }

   /* libs */

   retval = parse_unifile_compiler_args( NULL,
      um_state.libs, &(um_state.libs_sz), UNIFILE_LIBS_SZ_MAX,
      "libs", plat_type,
      gc_unimake_compiler_lib_tgt[um_state.compiler],
      gc_unimake_compiler_lib_rep[um_state.compiler] );
   if( retval ) {
      goto cleanup;
   }

   /* includes */

   retval = parse_unifile_compiler_args( NULL,
      um_state.includes, &(um_state.includes_sz), UNIFILE_INCLUDES_SZ_MAX,
      "includes", NULL,
      gc_unimake_compiler_inc_tgt[um_state.compiler],
      gc_unimake_compiler_inc_rep[um_state.compiler] );
   if( retval ) {
      goto cleanup;
   }

   retval = parse_unifile_compiler_args( NULL,
      um_state.includes, &(um_state.includes_sz), UNIFILE_INCLUDES_SZ_MAX,
      "includes", plat_type,
      gc_unimake_compiler_inc_tgt[um_state.compiler],
      gc_unimake_compiler_inc_rep[um_state.compiler] );
   if( retval ) {
      goto cleanup;
   }

   /* Build objects. */

   debug_printf( 1, "unifile parsed. flags: %08lx, defines: %s, libs: %s",
      um_state.options, um_state.defines, um_state.libs );

   for( i = 0 ; um_state.code_files_sz > i ; i++ ) {
      retval = build_obj( &um_state, um_state.code_files[i] );
      if( 0 > retval ) {
         goto cleanup;
      }
   }

   /* Link */

   retval = build_exe( &um_state );  
 
cleanup:

   return retval;
}

