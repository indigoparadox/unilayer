
#ifndef UNIMAKE_H
#define UNIMAKE_H

#include "../src/uprintf.h"

/**
 * \addtogroup unimake Unimake
 * \brief Build system for building on limited/legacy compilers/platforms.
 *
 * Unimake is designed to facilitate building projects using unilayer for
 * portability across legacy platforms. It is somewhat less flexible than other
 * build systems by design.
 *
 * Unimake incorporates knowledge and quirks of old/esoteric compilers to build
 * projects for platforms typically neglected by modern build systems.
 *
 * \{
 */

/**
 * \file unimake.h
 * \brief Unilayer build system.
 */

/**
 * \addtogroup unimake_errors Unimake Error Constants
 * \{
 */

#define UNIMAKE_ERROR_INVALID_ARG 0x01

#define UNIMAKE_ERROR_BAD_UNIFILE_PATH 0x02

#define UNIMAKE_ERROR_TOO_MANY_CODE_FILES 0x04

#define UNIMAKE_ERROR_TOO_MANY_ASSET_FILES 0x08

#define UNIMAKE_ERROR_STRING_TOO_LONG -0x10

/*! \} */

#define UNIFILE_STATE_NONE 0

#define UNIFILE_STATE_PARSE 1

/**
 * \addtogroup unimake_config Unimake Configuration Constants
 * \{
 */

#define UNIFILE_LINE_SZ 64

#define UNIMAKE_ASSETS_DIR "assets"

#define UNIMAKE_SRC_DIR "src"

#define UNIMAKE_OBJ_DIR "obj"

#define UNIMAKE_BIN_DIR "bin"

#define UNIMAKE_GEN_DIR "gen"

#define UNIFILE_PATH_DEFAULT "Unifile"

#define UNIFILE_PATHS_MAX 32

#define UNIFILE_PATH_SZ_MAX UNIFILE_LINE_SZ

#define UNIFILE_DEFINES_SZ_MAX 255

#define UNIFILE_INCLUDES_SZ_MAX 127

#define UNIFILE_LIBS_SZ_MAX 64

#define UNIFILE_CFLAGS_SZ_MAX 32

#define UNIMAKE_CLI_SZ_MAX 1024

/*! \} */

/**
 * \addtogroup unimake_compiler Unimake Compiler
 * \{
 */

#  define COMPILER_CC( cc, ld, rc, tgt_inc, rep_inc, tgt_def, rep_def, tgt_lib, rep_lib, tgt_ldr, rep_ldr, rep_dbg, obj_out, exe_out ) cc,
#  define COMPILER_LD( cc, ld, rc, tgt_inc, rep_inc, tgt_def, rep_def, tgt_lib, rep_lib, tgt_ldr, rep_ldr, rep_dbg, obj_out, exe_out ) ld, 
#  define COMPILER_REP_LIB( cc, ld, rc, tgt_inc, rep_inc, tgt_def, rep_def, tgt_lib, rep_lib, tgt_ldr, rep_ldr, rep_dbg, obj_out, exe_out ) rep_lib,
#  define COMPILER_TGT_LIB( cc, ld, rc, tgt_inc, rep_inc, tgt_def, rep_def, tgt_lib, rep_lib, tgt_ldr, rep_ldr, rep_dbg, obj_out, exe_out ) tgt_lib,
#  define COMPILER_REP_DBG( cc, ld, rc, tgt_inc, rep_inc, tgt_def, rep_def, tgt_lib, rep_lib, tgt_ldr, rep_ldr, rep_dbg, obj_out, exe_out ) rep_dbg,
#  define COMPILER_TGT_INC( cc, ld, rc, tgt_inc, rep_inc, tgt_def, rep_def, tgt_lib, rep_lib, tgt_ldr, rep_ldr, rep_dbg, obj_out, exe_out ) tgt_inc,
#  define COMPILER_REP_INC( cc, ld, rc, tgt_inc, rep_inc, tgt_def, rep_def, tgt_lib, rep_lib, tgt_ldr, rep_ldr, rep_dbg, obj_out, exe_out ) rep_inc,
#  define COMPILER_OBJ_OUT( cc, ld, rc, tgt_inc, rep_inc, tgt_def, rep_def, tgt_lib, rep_lib, tgt_ldr, rep_ldr, rep_dbg, obj_out, exe_out ) obj_out,
#  define COMPILER_EXE_OUT( cc, ld, rc, tgt_inc, rep_inc, tgt_def, rep_def, tgt_lib, rep_lib, tgt_ldr, rep_ldr, rep_dbg, obj_out, exe_out ) exe_out,

/*     cc,   ld,    rc,    ti, ri, td, rd, */
#  define UNIMAKE_COMPILER_TABLE( f ) \
   f( "gcc", "gcc", "", "", "", "", "", "", "", "", "", "-g -pg", "-o $FILE$", "-o $FILE$" ) \
   f( "wcc", "wcl", "wrc", "-I", "-i=", "", "", "-l", "-l=", "", "", "", "-fo=$FILE$", "-fe=$FILE$" ) \
   f( "m68k-gcc-palmos", "m68k-gcc-palmos", "", "", "", "", "", "", "", "", "", "-g", "-o $FILE$", "-o $FILE$" ) \
   f( "", "", "", "", "", "", "", "", "", "", "", "", "", "" )

/*! \} */

/**
 * \addtogroup unimake_flags Unimake Flags
 * \{
 */

#  define UNIMAKE_FLAGS( name, bit, cflags, defines, includes, ldflags, libs, libdirs ) bit,
#  define UNIMAKE_CFLAGS( name, bit, cflags, defines, includes, ldflags, libs, libdirs ) cflags,
#  define UNIMAKE_NAMES( name, bit, cflags, defines, includes, ldflags, libs, libdirs ) name,
#  define UNIMAKE_FLAG_DEFINES( name, bit, cflags, defines, includes, ldflags, libs, libdirs ) defines,
#  define UNIMAKE_FLAG_LIBS( name, bit, cflags, defines, includes, ldflags, libs, libdirs ) libs,
#  define UNIMAKE_FLAG_INCLUDES( name, bit, cflags, defines, includes, ldflags, libs, libdirs ) includes,

#define UNIMAKE_PLAT_MASK 0xff000000
#define UNIMAKE_PLAT_TABLE( f ) \
   f( "sdl", 0x01000000, "", "-DPLATFORM_SDL", "", "", "-lsdl", "" ) \
   f( "wsm", 0x02000000, "", "-DPLATFORM_SDL", "", "", "", "" ) \
   f( "w16", 0x04000000, "-bt=windows -bw -zp=1", "-DPLATFORM_WIN16", "-I$INCLUDE/win", "", "-lwindows", "" ) \
   f( "w32", 0x08000000, "", "-DPLATFORM_WIN32", "", "", "", "" ) \
   f( "dos", 0x10000000, "", "-DPLATFORM_DOS", "", "", "", "" ) \
   f( "plm", 0x20000000, "", "-DPLATFORM_PALM", "", "", "", "" ) \
   f( "", 0x00000000, "", "", "", "", "", "" )

#define UNIMAKE_GFX_MASK 0x0000000f
#define UNIMAKE_GFX_TABLE( f ) \
   f( "cga", 0x00000000, "", "-DDEPTH_CGA", "", "", "", "" ) \
   f( "mno", 0x00000001, "", "-DDEPTH_MONO", "", "", "", "" ) \
   f( "vga", 0x00000002, "", "-DDEPTH_VGA", "", "", "", "" ) \
   f( "", 0x00000000, "", "", "", "", "", "" )

#define UNIMAKE_FMT_MASK 0x000000f0
#define UNIMAKE_FMT_TABLE( f ) \
   f( "hdr", 0x00000000, "", "", "", "", "", "" ) \
   f( "jsn", 0x00000010, "", "-DNO_RESEXT", "", "", "", "" ) \
   f( "asn", 0x00000020, "", "-DNO_RESEXT", "", "", "", "" ) \
   f( "", 0x00000000, "", "", "", "", "", "" )

#define UNIMAKE_MISC_MASK 0x00ff0000
#define UNIMAKE_MISC_TABLE( f ) \
   f( "dbg", 0x00010000, "$DEBUG$", "", "", "$DEBUG$", "", "" ) \
   f( "", 0x00000000, "", "", "", "", "", "" )

/*! \} */

struct unimake_state {
   /*! \brief List of code units to compile. */
   char code_files[UNIFILE_PATHS_MAX][UNIFILE_PATH_SZ_MAX + 1]; /* +1 NULL */
   /*! \brief Number of code units in unimake_state::code_files. */
   int code_files_sz;
   char gfx_files[UNIFILE_PATHS_MAX][UNIFILE_PATH_SZ_MAX + 1]; /* +1 NULL */
   int gfx_files_sz;
   /*! \brief Defines arg to pass to compiler. */
   char defines[UNIFILE_DEFINES_SZ_MAX + 1];
   /*! \brief Number of chars in unimake_state::defines. */
   int defines_sz;
   /*! \brief Includes arg to pass to compiler. */
   char includes[UNIFILE_INCLUDES_SZ_MAX + 1];
   /*! \brief Number of chars in unimake_state::includes. */
   int includes_sz;
   /*! \brief Libs arg to pass to linker. */
   char libs[UNIFILE_LIBS_SZ_MAX + 1];
   /*! \brief Number of chars in unimake_state::libs. */
   int libs_sz;
   char cflags[UNIFILE_CFLAGS_SZ_MAX + 1];
   int cflags_sz;
   /*! \brief Index of selected compiler from ::UNIMAKE_COMPILER_TABLE. */
   int compiler;
   /*! \brief Selected build flags from \ref unimake_flags. */
   unsigned long options;
};

#ifdef UNIMAKE_C

char* gc_unimake_compiler_cc[] = {
   UNIMAKE_COMPILER_TABLE( COMPILER_CC )
};

char* gc_unimake_compiler_ld[] = {
   UNIMAKE_COMPILER_TABLE( COMPILER_LD )
};

char* gc_unimake_compiler_lib_tgt[] = {
   UNIMAKE_COMPILER_TABLE( COMPILER_TGT_LIB )
};

char* gc_unimake_compiler_lib_rep[] = {
   UNIMAKE_COMPILER_TABLE( COMPILER_REP_LIB )
};

char* gc_unimake_compiler_inc_tgt[] = {
   UNIMAKE_COMPILER_TABLE( COMPILER_TGT_INC )
};

char* gc_unimake_compiler_inc_rep[] = {
   UNIMAKE_COMPILER_TABLE( COMPILER_REP_INC )
};

char* gc_unimake_compiler_dbg_rep[] = {
   UNIMAKE_COMPILER_TABLE( COMPILER_REP_DBG )
};

char* gc_unimake_compiler_obj_out[] = {
   UNIMAKE_COMPILER_TABLE( COMPILER_OBJ_OUT )
};

char* gc_unimake_compiler_exe_out[] = {
   UNIMAKE_COMPILER_TABLE( COMPILER_EXE_OUT )
};

/* Format Flags */

unsigned long gc_unimake_fmt_flags[] = {
   UNIMAKE_FMT_TABLE( UNIMAKE_FLAGS )
};

char* gc_unimake_fmt_names[] = {
   UNIMAKE_FMT_TABLE( UNIMAKE_NAMES )
};

char* gc_unimake_fmt_defines[] = {
   UNIMAKE_FMT_TABLE( UNIMAKE_FLAG_DEFINES )
};

char* gc_unimake_fmt_libs[] = {
   UNIMAKE_FMT_TABLE( UNIMAKE_FLAG_LIBS )
};

char* gc_unimake_fmt_includes[] = {
   UNIMAKE_FMT_TABLE( UNIMAKE_FLAG_INCLUDES )
};

/* Graphics Flags */

unsigned long gc_unimake_gfx_flags[] = {
   UNIMAKE_GFX_TABLE( UNIMAKE_FLAGS )
};

char* gc_unimake_gfx_names[] = {
   UNIMAKE_GFX_TABLE( UNIMAKE_NAMES )
};

char* gc_unimake_gfx_defines[] = {
   UNIMAKE_GFX_TABLE( UNIMAKE_FLAG_DEFINES )
};

char* gc_unimake_gfx_libs[] = {
   UNIMAKE_GFX_TABLE( UNIMAKE_FLAG_LIBS )
};

char* gc_unimake_gfx_includes[] = {
   UNIMAKE_GFX_TABLE( UNIMAKE_FLAG_INCLUDES )
};

/* Misc Flags */

unsigned long gc_unimake_misc_flags[] = {
   UNIMAKE_MISC_TABLE( UNIMAKE_FLAGS )
};

char* gc_unimake_misc_cflags[] = {
   UNIMAKE_MISC_TABLE( UNIMAKE_CFLAGS )
};

char* gc_unimake_misc_names[] = {
   UNIMAKE_MISC_TABLE( UNIMAKE_NAMES )
};

char* gc_unimake_misc_defines[] = {
   UNIMAKE_MISC_TABLE( UNIMAKE_FLAG_DEFINES )
};

char* gc_unimake_misc_libs[] = {
   UNIMAKE_MISC_TABLE( UNIMAKE_FLAG_LIBS )
};

char* gc_unimake_misc_includes[] = {
   UNIMAKE_MISC_TABLE( UNIMAKE_FLAG_INCLUDES )
};

/* Platform Flags */

char* gc_unimake_plat_names[] = {
   UNIMAKE_PLAT_TABLE( UNIMAKE_NAMES )
};

unsigned long gc_unimake_plat_flags[] = {
   UNIMAKE_PLAT_TABLE( UNIMAKE_FLAGS )
};

char* gc_unimake_plat_defines[] = {
   UNIMAKE_PLAT_TABLE( UNIMAKE_FLAG_DEFINES )
};

char* gc_unimake_plat_libs[] = {
   UNIMAKE_PLAT_TABLE( UNIMAKE_FLAG_LIBS )
};

char* gc_unimake_plat_includes[] = {
   UNIMAKE_PLAT_TABLE( UNIMAKE_FLAG_INCLUDES )
};

#endif /* UNIMAKE_C */

#define str_concat_char( str, str_sz_p, str_max, c, retval ) \
   if( str_max > *(str_sz_p) + 1 ) { \
      str[*(str_sz_p)] = c; \
      (*(str_sz_p))++; \
      str[*(str_sz_p)] = '\0'; \
   } else { \
      error_printf( "line buffer exceeded" ); \
      retval = UNIMAKE_ERROR_STRING_TOO_LONG; \
      goto cleanup; \
   }

/**
 * @return Index of string in given array if it is found, or -1 otherwise.
 */
int str_in_array( const char* str, int str_sz, char* array[] );

/**
 * @return 0 if replacement string not found or if it is
 *         found and replaced. UNIMAKE_ERROR_STRING_TOO_LONG if replacement is
 *         attempted but fails due to insufficient room in target buffer.
 */
int str_replace(
   char* str, int str_sz_max, const char* str_in,
   const char* substr_replace, const char* substr_replacement,
   int* chars_advanced_tgt, int* chars_advanced_in );

/**
 * \brief Concatenate str_in to str, replacing token substr_target with
 *        substr_replacement as needed.
 */
int str_concat(
   char* str, int* str_sz, int str_sz_max, const char* str_in,
   const char* substr_target, const char* substr_replacement );

/*! \} */

#endif /* !UNIMAKE_H */

