
#ifndef PLATFORM_H
#define PLATFORM_H

/*! \file platform.h
 *  \brief Contains platform-specific overrides and implementations for engine
 *         functionality.
 */

#if defined( PLATFORM_WIN16 ) || defined( PLATFORM_WIN32 ) || defined( PLATFORM_WINCE )
#  define PLATFORM_WIN
#endif /* PLATFORM_WIN16 || PLATFORM_WIN32 || PLATFORM_WINCE */

#if defined( PLATFORM_SDL1 ) || defined( PLATFORM_SDL2 )
#  define PLATFORM_SDL
#endif /* PLATFORM_SDL1 || PLATFORM_SDL2 */

#if defined( DEPTH_VGA )
#  define DEPTH_SPEC "16x16x16"
#elif defined( DEPTH_CGA )
#  define DEPTH_SPEC "16x16x4"
#elif defined( DEPTH_MONO )
#  define DEPTH_SPEC "16x16x2"
#else
#  error "No depth spec defined!"
#endif /* DEPTH_VGA || DEPTH_CGA || DEPTH_MONO */

struct GRAPHICS_ARGS;

/*! \brief Implementation-specific main loop iteration function. */
typedef int (*loop_iter)( void* );

#ifdef MAIN_C
#define loop_globals() uint8_t g_running = 1; loop_iter g_loop_iter = NULL; void* g_loop_data = NULL;
#else
/*! \brief Declare the globals that hold the main loop pointer and args. */
#define loop_globals() extern uint8_t g_running; extern loop_iter g_loop_iter; extern void* g_loop_data;
#endif /* MAIN_C */

/* Allow overriding which memory system is used with a define. */
#if defined( MEMORY_FAKE )
#  define include_memory( default_h ) "memory/fakem.h"
#elif defined( MEMORY_WIN )
#  define include_memory( default_h ) "memory/winm.h"
#else
#  define include_memory( default_h ) default_h
#endif

#ifdef PLATFORM_DOS

#  ifndef SCREEN_W
#     define SCREEN_W 320
#  endif /* !SCREEN_W */

#  ifndef SCREEN_H
#     define SCREEN_H 200
#  endif /* !SCREEN_H */

#  ifdef DEPTH_VGA
#     define RESOURCE_EXT_GRAPHICS "vga"
#  else
#     define RESOURCE_EXT_GRAPHICS "cga"
#  endif

#  define PLATFORM_DIR_SEP '\\'
#  define LOG_FILE_NAME "logdos.txt"
#  include <mtypes.h>
loop_globals();
#  include "memory/dosm.h"
#  include "resinc.h"
#  include "input/dosi.h"
#  include "graphics/dosg.h"

#elif defined( PLATFORM_SDL )

#  ifndef SCREEN_W
#     define SCREEN_W 160
#  endif /* !SCREEN_W */

#  ifndef SCREEN_H
#     define SCREEN_H 160
#  endif /* !SCREEN_H */

#  ifdef PLATFORM_SDL1
   /* SDL1 doesn't have SDL2's primative routines. */
#     define USE_SOFTWARE_LINES
#  endif /* PLATFORM_SDL1 */

#  include <mtypes.h>
loop_globals();
#  include "memory/fakem.h"
#  include "resinc.h"
#  include "input/sdli.h"
#  include "graphics/sdlg.h"

#  ifdef PLATFORM_WASM

#include <emscripten.h>

#define unilayer_loop_set( iter, data ) emscripten_cancel_main_loop(); emscripten_set_main_loop_arg( iter, data, 0, 0 );

#  endif /* PLATFORM_WASM */

#elif defined( PLATFORM_XLIB )

#  ifndef SCREEN_W
#     define SCREEN_W 320
#  endif /* !SCREEN_W */

#  ifndef SCREEN_H
#     define SCREEN_H 200
#  endif /* !SCREEN_H */

#  include <X11/Xlib.h>
#  include <mtypes.h>
loop_globals();
#  include "memory/fakem.h"
#  include "resinc.h"
#  include "input/xi.h"
#  include "graphics/xg.h"

#elif defined( PLATFORM_CURSES )

#  ifndef SCREEN_W
#     define SCREEN_W 80
#  endif /* !SCREEN_W */

#  ifndef SCREEN_H
#     define SCREEN_H 25
#  endif /* !SCREEN_H */

#  define SCREEN_MAP_W (10)
#  define SCREEN_MAP_H (8)
#  define TILE_W 1
#  define TILE_H 1
#  define SPRITE_W 1
#  define SPRITE_H 1
#  define WINDOW_PATTERN_W 1
#  define WINDOW_PATTERN_H 1
#  define STATUS_WINDOW_H 2
#  define WINDOW_PADDING_INSIDE 0
#  define WINDOW_PADDING_OUTSIDE 1

#  define GFX_ASCII

#  define graphics_blit_sprite_at( ascii, s_x, s_y, d_x, d_y, w, h ) graphics_curses_char_at( ascii, d_x, d_y )
#  define graphics_blit_tile_at( ascii, s_x, s_y, d_x, d_y, w, h ) graphics_curses_char_at( ascii, d_x, d_y )

#  define LOG_TO_FILE
#  define LOG_FILE_NAME "logcurs.txt"
#  include <X11/Xlib.h>
#  include <mtypes.h>
loop_globals();
#  include "memory/fakem.h"
#  include "resinc.h"
#  include "input/cursesi.h"
#  include "graphics/cursesg.h"

#elif defined( PLATFORM_PALM )

#  ifndef SCREEN_W
#     define SCREEN_W 160
#  endif /* !SCREEN_W */

#  ifndef SCREEN_H
#     define SCREEN_H 160
#  endif /* !SCREEN_H */

#  define USE_SOFT_ASSERT
#  define DISABLE_FILESYSTEM
#  define LOG_FILE_NAME "logpalm.txt"
#  define CODE_SECTION( name ) __attribute__( (section( name )) )
#  include <PalmOS.h>
#  define platform_file HostFILE*
#  define platform_fprintf HostFPrintF
#  define platform_fopen HostFOpen
#  define platform_fflush HostFFlush
#  define platform_fclose HostFClose
#  define after_unilayer_main() UInt32 PilotMain( UInt16 cmd, MemPtr cmdPBP, UInt16 launchFlags ) { if( sysAppLaunchCmdNormalLaunch == cmd ) { return unilayer_main( 0, NULL ); } }
#  include <mtypes.h>
loop_globals();
#  include "memory/palmm.h"
#  include "resinc.h"
#  include "input/palmi.h"
#  include "graphics/palmg.h"

#elif defined( PLATFORM_WIN )

#  ifndef SCREEN_W
#     define SCREEN_W 160
#  endif /* !SCREEN_W */

#  ifndef SCREEN_H
#     define SCREEN_H 160
#  endif /* !SCREEN_H */

#  ifndef SCREEN_SCALE
#     define SCREEN_SCALE 2
#  endif /* !SCREEN_SCALE */

#  ifdef PLATFORM_WIN16
#     define LOG_FILE_NAME "logwin16.txt"
#     define PLATFORM_API PASCAL
#  elif defined( PLATFORM_WINCE )
#     define LOG_FILE_NAME "logwince.txt"
#     define PLATFORM_API WINAPI
#  elif defined( PLATFORM_WIN32 )
#     define LOG_FILE_NAME "logwin32.txt"
#     define PLATFORM_API WINAPI
#  endif /* PLATFORM_WIN16, PLATFORM_WIN32 */
#  include <windows.h>
#  include <mtypes.h>
/* TODO: Why aren't these being defined? */
#  ifdef __argc
#     define PLATFORM_ARGS __argc, __argv
#  else
#     define PLATFORM_ARGS 0, NULL
#  endif /* __argc */
#  define after_unilayer_main() \
   extern HWND g_window; \
   extern int __argc; \
   extern char** __argv; \
   extern HINSTANCE g_instance; \
   int PLATFORM_API WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) { \
      g_graphics_args.cmd_show = nCmdShow; \
      g_instance = hInstance; \
      if( hPrevInstance ) { \
         error_printf( "previous instance detected" ); \
         return 1; \
      } \
      return unilayer_main( __argc, __argv ); \
   }
#  define unilayer_loop_iter() win_process_messages()
#  define platform_init( icon ) g_graphics_args.icon_res = icon;
loop_globals();
#  include include_memory( "memory/winm.h" )
#  include "resinc.h"
#  include "input/wini.h"
#  include "graphics/wing.h"

#ifdef MAIN_C
#endif /* MAIN_C */

#elif defined( PLATFORM_MAC6 )

#  ifndef SCREEN_W
#     define SCREEN_W 160
#  endif /* !SCREEN_W */

#  ifndef SCREEN_H
#     define SCREEN_H 160
#  endif /* !SCREEN_H */

#  define DISABLE_MAIN_PARMS
#  define NEWLINE_STR "\r"
#  define LOG_FILE_NAME "logmac.txt"
#  include <Multiverse.h>
#  include <Quickdraw.h>
#  include <mtypes.h>
loop_globals();
#  include "memory/mac6m.h"
#  include "resinc.h"
#  include "input/mac6i.h"
#  include "graphics/mac6g.h"

#elif defined( PLATFORM_NDS )

#  ifndef SCREEN_W
#     define SCREEN_W 256
#  endif /* !SCREEN_W */

#  ifndef SCREEN_H
#     define SCREEN_H 192
#  endif /* !SCREEN_H */

#  ifndef LOG_TO_FILE
#     define LOG_TO_FILE
#  endif /* !LOG_TO_FILE */
#  define LOG_FILE_NAME "lognds.txt"
#  include <nds.h>
#  include <mtypes.h>
loop_globals();
#  include "memory/fakem.h"
#  include "resinc.h"
#  include "input/ndsi.h"
#  include "graphics/ndsg.h"

#else

#  include <mtypes.h>
loop_globals();
#  include "memory/fakem.h"
#  include "resinc.h"
#  include "input/nulli.h"
#  include "graphics/nullg.h"

#endif

#ifndef RESOURCE_EXT_GRAPHICS
#  define RESOURCE_EXT_GRAPHICS "bmp"
#endif /* !RESOURCE_EXT_GRAPHICS */

#ifndef NEWLINE_STR
/*! \brief Newline string on the current platform. */
#  define NEWLINE_STR "\n"
#endif /* !NEWLINE_STR */

#ifndef PLATFORM_DIR_SEP
#define PLATFORM_DIR_SEP '/'
#endif /* !PLATFORM_DIR_SEP */

#if !defined( after_unilayer_main )
/*! \brief Replaces main() and calls the appropriate platform-specific
 *         entrypoint. */
#  define after_unilayer_main() int main( int argc, char* argv[] ) { return unilayer_main( argc, argv ); }
#endif /* !unilayer_main() */

#ifndef unilayer_loop_iter
/*! \brief Call the main loop for one iteration. */
#define unilayer_loop_iter() g_running = g_loop_iter( g_loop_data )
#endif /* !unilayer_loop_iter() */

#ifndef unilayer_loop_set
/**
 * \brief Setup the main loop function and data arguments.
 * \param iter Function pointer to the main loop iteration function.
 * \param data MEMORY_HANDLE for implementation-specific data/state struct.
 */
#define unilayer_loop_set( iter, data ) g_loop_iter = (loop_iter)iter; g_loop_data = (void*)data;
#endif /* !unilayer_loop_set */

#ifndef platform_init
/*! \brief Platform-specific setup function (e.g. create window). */
#define platform_init( icon )
#endif /* !platform_init() */

#ifndef platform_shutdown
/*! \brief Platform-specific cleanup function. */
#define platform_shutdown()
#endif /* !platform_shutdown */

#ifndef CODE_SECTION
/*! \brief Define which code resource a function should be placed in. */
#define CODE_SECTION
#endif

#endif /* !PLATFORM_H */

