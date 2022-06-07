
#ifndef UNIMAKE_H
#define UNIMAKE_H

#include "../src/uprintf.h"

#define UNIMAKE_ERROR_INVALID_ARG 0x01

#define UNIMAKE_ERROR_BAD_UNIFILE_PATH 0x02

#define UNIMAKE_ERROR_TOO_MANY_CODE_FILES 0x04

#define UNIMAKE_ERROR_TOO_MANY_ASSET_FILES 0x08

#define UNIMAKE_ERROR_PATH_TOO_LONG 0x10

#define UNIFILE_STATE_NONE 0

#define UNIFILE_STATE_PLATFILES 1

#define UNIFILE_LINE_SZ 255

#define UNIMAKE_ASSETS_DIR "assets"

#define UNIMAKE_SRC_DIR "src"

#define UNIMAKE_PLAT_MASK 0xff000000
#define UNIMAKE_PLAT_TABLE( f ) f( "sdl", 0x01000000, "-DPLATFORM_SDL", "" ) f( "wsm", 0x02000000, "-DPLATFORM_SDL", "" ) f( "w16", 0x04000000, "-DPLATFORM_WIN16", "" ) f( "w32", 0x08000000, "-DPLATFORM_WIN32", "" ) f( "dos", 0x10000000, "-DPLATFORM_DOS", "" ) f( "", 0x00000000, "", "" )

#define UNIMAKE_GFX_MASK 0x000000ff
#define UNIMAKE_GFX_TABLE( f ) f( "cga", 0x00000001, "-DDEPTH_CGA -DDEPTH_SPEC=\"16x16x4\"", "" ) f( "mno", 0x00000002, "-DDEPTH_MONO -DDEPTH_SPEC=\"16x16x2\"", "" ) f( "vga", 0x00000004, "-DDEPTH_VGA -DDEPTH_SPEC=\"16x16x16\"", "" ) f( "", 0x00000000, "", "" )

#define UNIMAKE_FMT_MASK 0x0000ff00
#define UNIMAKE_FMT_TABLE( f ) f( "hdr", 0x00000100, "", "" ) f( "jsn", 0x00000200, "", "" ) f( "asn", 0x00000400, "", "" ) f( "", 0x00000000, "", "" )

#define UNIMAKE_MISC_MASK 0x00ff0000
#define UNIMAKE_MISC_TABLE( f ) f( "release", 0x00010000, "", "" ) f( "", 0x00000000, "", "" )

#define UNIFILE_PATH_DEFAULT "Unifile"

#define UNIFILE_PATHS_MAX 255

#define UNIFILE_PATH_SZ_MAX 255

#define UNIFILE_DEFINES_SZ_MAX 255

#define UNIFILE_INCLUDES_SZ_MAX 255

struct unimake_state {
   char code_files[UNIFILE_PATHS_MAX][UNIFILE_PATH_SZ_MAX + 1]; /* +1 NULL */
   int code_files_sz;
   char gfx_files[UNIFILE_PATHS_MAX][UNIFILE_PATH_SZ_MAX + 1]; /* +1 NULL */
   int gfx_files_sz;
   char defines[UNIFILE_DEFINES_SZ_MAX + 1];
   int defines_sz;
   char includes[UNIFILE_INCLUDES_SZ_MAX + 1];
   int includes_sz;
   unsigned long options;
};

#ifdef UNIMAKE_C

#  define UNIMAKE_FLAGS( name, bit, defines, includes ) bit,
#  define UNIMAKE_NAMES( name, bit, defines, includes ) name,

unsigned long gc_unimake_fmt_flags[] = {
   UNIMAKE_FMT_TABLE( UNIMAKE_FLAGS )
};

char* gc_unimake_fmt_names[] = {
   UNIMAKE_FMT_TABLE( UNIMAKE_NAMES )
};

unsigned long gc_unimake_gfx_flags[] = {
   UNIMAKE_GFX_TABLE( UNIMAKE_FLAGS )
};

char* gc_unimake_gfx_names[] = {
   UNIMAKE_GFX_TABLE( UNIMAKE_NAMES )
};

unsigned long gc_unimake_misc_flags[] = {
   UNIMAKE_MISC_TABLE( UNIMAKE_FLAGS )
};

char* gc_unimake_misc_names[] = {
   UNIMAKE_MISC_TABLE( UNIMAKE_NAMES )
};

char* gc_unimake_plat_names[] = {
   UNIMAKE_PLAT_TABLE( UNIMAKE_NAMES )
};

unsigned long gc_unimake_plat_flags[] = {
   UNIMAKE_PLAT_TABLE( UNIMAKE_FLAGS )
};

#endif /* UNIMAKE_C */

#endif /* !UNIMAKE_H */

