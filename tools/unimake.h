
#ifndef UNIMAKE_H
#define UNIMAKE_H

#include "../src/uprintf.h"

#define UNIMAKE_ERROR_INVALID_ARG 0x01

#define UNIMAKE_ERROR_BAD_UNIFILE_PATH 0x02

#define UNIMAKE_ERROR_TOO_MANY_CODE_FILES 0x04

#define UNIMAKE_ERROR_TOO_MANY_ASSET_FILES 0x08

#define UNIMAKE_ERROR_STRING_TOO_LONG -0x10

#define UNIFILE_STATE_NONE 0

#define UNIFILE_STATE_PARSE 1

#define UNIFILE_LINE_SZ 64

#define UNIMAKE_ASSETS_DIR "assets"

#define UNIMAKE_SRC_DIR "src"

#  define COMPILER_REP_LIB( cc, ld, tgt_inc, rep_inc, tgt_def, rep_def, tgt_lib, rep_lib, tgt_ldr, rep_ldr ) rep_lib,
#  define COMPILER_TGT_LIB( cc, ld, tgt_inc, rep_inc, tgt_def, rep_def, tgt_lib, rep_lib, tgt_ldr, rep_ldr ) tgt_lib,

#  define UNIMAKE_COMPILER_TABLE( f ) \
   f( "gcc", "gcc", "", "", "", "", "", "", "", "" ) \
   f( "wcc", "wcl", "", "", "", "", "-l", "-l=", "", "" ) \
   f( "", "", "", "", "", "", "", "", "", "" )

#  define UNIMAKE_FLAGS( name, bit, cflags, defines, includes, ldflags, libs, libdirs ) bit,
#  define UNIMAKE_NAMES( name, bit, cflags, defines, includes, ldflags, libs, libdirs ) name,
#  define UNIMAKE_FLAG_DEFINES( name, bit, cflags, defines, includes, ldflags, libs, libdirs ) defines,
#  define UNIMAKE_FLAG_LIBS( name, bit, cflags, defines, includes, ldflags, libs, libdirs ) libs,

#define UNIMAKE_PLAT_MASK 0xff000000
#define UNIMAKE_PLAT_TABLE( f ) \
   f( "sdl", 0x01000000, "", "-DPLATFORM_SDL", "", "", "-lsdl", "" ) \
   f( "wsm", 0x02000000, "", "-DPLATFORM_SDL", "", "", "", "" ) \
   f( "w16", 0x04000000, "", "-DPLATFORM_WIN16", "", "", "-lwindows", "" ) \
   f( "w32", 0x08000000, "", "-DPLATFORM_WIN32", "", "", "", "" ) \
   f( "dos", 0x10000000, "", "-DPLATFORM_DOS", "", "", "", "" ) \
   f( "", 0x00000000, "", "", "", "", "", "" )

#define UNIMAKE_GFX_MASK 0x000000ff
#define UNIMAKE_GFX_TABLE( f ) \
   f( "cga", 0x00000001, "", "-DDEPTH_CGA", "", "", "", "" ) \
   f( "mno", 0x00000002, "", "-DDEPTH_MONO", "", "", "", "" ) \
   f( "vga", 0x00000004, "", "-DDEPTH_VGA", "", "", "", "" ) \
   f( "", 0x00000000, "", "", "", "", "", "" )

#define UNIMAKE_FMT_MASK 0x0000ff00
#define UNIMAKE_FMT_TABLE( f ) \
   f( "hdr", 0x00000100, "", "", "", "", "", "" ) \
   f( "jsn", 0x00000200, "", "", "", "", "", "" ) \
   f( "asn", 0x00000400, "", "", "", "", "", "" ) \
   f( "", 0x00000000, "", "", "", "", "", "" )

#define UNIMAKE_MISC_MASK 0x00ff0000
#define UNIMAKE_MISC_TABLE( f ) \
   f( "debug", 0x00010000, "-pg -g", "", "", "-pg -g", "", "" ) \
   f( "", 0x00000000, "", "", "", "", "", "" )

#define UNIFILE_PATH_DEFAULT "Unifile"

#define UNIFILE_PATHS_MAX 32

#define UNIFILE_PATH_SZ_MAX UNIFILE_LINE_SZ

#define UNIFILE_DEFINES_SZ_MAX 255

#define UNIFILE_INCLUDES_SZ_MAX 127

#define UNIFILE_LIBS_SZ_MAX 64

struct unimake_state {
   char code_files[UNIFILE_PATHS_MAX][UNIFILE_PATH_SZ_MAX + 1]; /* +1 NULL */
   int code_files_sz;
   char gfx_files[UNIFILE_PATHS_MAX][UNIFILE_PATH_SZ_MAX + 1]; /* +1 NULL */
   int gfx_files_sz;
   char defines[UNIFILE_DEFINES_SZ_MAX + 1];
   int defines_sz;
   char includes[UNIFILE_INCLUDES_SZ_MAX + 1];
   int includes_sz;
   char libs[UNIFILE_LIBS_SZ_MAX + 1];
   int libs_sz;
   int compiler;
   unsigned long options;
};

#ifdef UNIMAKE_C

char* gc_unimake_compiler_lib_tgt[] = {
   UNIMAKE_COMPILER_TABLE( COMPILER_TGT_LIB )
};

char* gc_unimake_compiler_lib_rep[] = {
   UNIMAKE_COMPILER_TABLE( COMPILER_REP_LIB )
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

/* Misc Flags */

unsigned long gc_unimake_misc_flags[] = {
   UNIMAKE_MISC_TABLE( UNIMAKE_FLAGS )
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

#endif /* UNIMAKE_C */

/**
 * @return 0 if replacement string not found or if it is
 *         found and replaced. UNIMAKE_ERROR_STING_TOO_LONG if replacement is
 *         attempted but fails due to insufficient room in target buffer.
 */
int str_replace(
   char* str, int str_sz_max, const char* str_in,
   const char* substr_replace, const char* substr_replacement,
   int* chars_advanced_tgt, int* chars_advanced_in );

#endif /* !UNIMAKE_H */

