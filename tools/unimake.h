
#ifndef UNIMAKE_H
#define UNIMAKE_H

#include "../src/uprintf.h"

#define UNIMAKE_ERROR_INVALID_ARG 1

#define UNIMAKE_ASSETS_DIR "assets"

#define UNIMAKE_SRC_DIR "src"

#define UNIMAKE_GFX_MASK 0x000000ff
#define UNIMAKE_GFX_TABLE( f ) f( "cga", 0x00000001 ) f( "mno", 0x00000002 ) f( "vga", 0x00000004 ) f( "", 0x00000000 )

#define UNIMAKE_FMT_MASK 0x0000ff00
#define UNIMAKE_FMT_TABLE( f ) f( "hdr", 0x00000100 ) f( "jsn", 0x00000200 ) f( "asn", 0x00000400 ) f( "", 0x00000000 )

#define UNIMAKE_MISC_MASK 0x00ff0000
#define UNIMAKE_MISC_TABLE( f ) f( "release", 0x00010000 ) f( "", 0x00000000 )

#ifdef UNIMAKE_C

#  define UNIMAKE_FLAGS( name, bit ) bit,
#  define UNIMAKE_NAMES( name, bit ) name,

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

#endif /* UNIMAKE_C */

#endif /* !UNIMAKE_H */

