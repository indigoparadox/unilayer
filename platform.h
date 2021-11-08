
#ifndef PLATFORM_H
#define PLATFORM_H

/*! \file platform.h
 *  \brief Contains platform-specific overrides and implementations for engine
 *         functionality.
 */

#if defined( PLATFORM_WIN16 ) || defined( PLATFORM_WIN32 ) || defined( PLATFORM_WINCE )
#define PLATFORM_WIN
#endif

struct GRAPHICS_ARGS;

/*! \brief Implementation-specific main loop iteration function. */
typedef int (*loop_iter)( void*, struct GRAPHICS_ARGS* );

#ifdef MAIN_C
#define loop_globals() uint8_t g_running = 1; loop_iter g_loop_iter = NULL; void* g_loop_data = NULL; struct GRAPHICS_ARGS* g_loop_gargs = NULL;
#else
/*! \brief Declare the globals that hold the main loop pointer and args. */
#define loop_globals() extern uint8_t g_running; extern loop_iter g_loop_iter; extern void* g_loop_data; extern struct GRAPHICS_ARGS* g_loop_gargs;
#endif /* MAIN_C */

#ifdef PLATFORM_DOS

#  define PLATFORM_DIR_SEP '\\'
#  define LOG_TO_FILE
#  define LOG_FILE_NAME "logdos.txt"
#  include "types/x86.h"
loop_globals();
#  include "memory/fakem.h"
#  include "resinc.h"
#  include "input/dosi.h"
#  include "graphics/dosg.h"

#elif defined( PLATFORM_SDL )

#  include <stdint.h>
loop_globals();
#  include "memory/fakem.h"
#  include "resinc.h"
#  include "input/sdli.h"
#  include "graphics/sdlg.h"

#elif defined( PLATFORM_XLIB )

#  include <X11/Xlib.h>
#  include <stdint.h>
loop_globals();
#  include "memory/fakem.h"
#  include "resinc.h"
#  include "input/xi.h"
#  include "graphics/xg.h"

#elif defined( PLATFORM_PALM )

#  define USE_SOFT_ASSERT
#  define DISABLE_FILESYSTEM
#  define LOG_TO_FILE
#  define LOG_FILE_NAME "logpalm.txt"
#  define CODE_SECTION( name ) __attribute__( (section( name )) )
#  include <PalmOS.h>
#  define platform_file HostFILE*
#  define platform_fprintf HostFPrintF
#  define platform_fopen HostFOpen
#  define platform_fflush HostFFlush
#  define platform_fclose HostFClose
#  define unilayer_main() UInt32 PilotMain( UInt16 cmd, MemPtr cmdPBP, UInt16 launchFlags )
#  define platform_init( graphics_arg, icon ) if( cmd == sysAppLaunchCmdNormalLaunch ) {
#  define platform_shutdown() }
#  include "types/palmt.h"
loop_globals();
#  include "memory/palmm.h"
#  include "resinc.h"
#  include "input/palmi.h"
#  include "graphics/palmg.h"

#elif defined( PLATFORM_WIN )

#  define LOG_TO_FILE
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
#  define unilayer_main() int PLATFORM_API WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
#  define unilayer_loop_iter() win_process_messages()
#  define platform_init( graphics_args, icon ) g_instance = hInstance; graphics_args.cmd_show = nCmdShow; graphics_args.icon_res = icon; if( hPrevInstance ) { error_printf( "previous instance detected" ); return 1; }
#  include <windows.h>
#  include "types/x86.h"
loop_globals();
#  include "memory/winm.h"
#  include "resinc.h"
#  include "input/wini.h"
#  include "graphics/wing.h"

#ifdef MAIN_C
HINSTANCE g_instance = (HINSTANCE)NULL;
HWND g_window = (HWND)NULL;
#endif /* MAIN_C */

#elif defined( PLATFORM_MAC6 )

#  define DISABLE_MAIN_PARMS
#  define NEWLINE_STR "\r"
#  define LOG_TO_FILE
#  define LOG_FILE_NAME "logmac.txt"
#  include <Multiverse.h>
#  include <Quickdraw.h>
#  include <stdint.h>
loop_globals();
#  include "memory/mac6m.h"
#  include "resinc.h"
#  include "input/mac6i.h"
#  include "graphics/mac6g.h"

#elif defined( PLATFORM_NDS )

#  define LOG_TO_FILE
#  define LOG_FILE_NAME "lognds.txt"
#  include <nds.h>
#  include <stdint.h>
loop_globals();
#  include "memory/fakem.h"
#  include "resinc.h"
#  include "input/ndsi.h"
#  include "graphics/ndsg.h"

#elif defined( PLATFORM_MEGADRIVE )

#define DISABLE_FILESYSTEM

#  include <stdint.h>
#  include <types.h>
loop_globals();
#  include "memory/fakem.h"
#  include "resinc.h"
#  include "input/megadi.h"
#  include "graphics/megadg.h"

#else

#  include <stdint.h>
loop_globals();
#  include "memory/fakem.h"
#  include "resinc.h"
#  include "input/nulli.h"
#  include "graphics/nullg.h"

#endif

#ifndef NEWLINE_STR
/*! \brief Newline string on the current platform. */
#  define NEWLINE_STR "\n"
#endif /* !NEWLINE_STR */

#ifndef PLATFORM_DIR_SEP
#define PLATFORM_DIR_SEP '/'
#endif /* !PLATFORM_DIR_SEP */

#if !defined( unilayer_main ) && defined( DISABLE_MAIN_PARMS )
#  define unilayer_main() void main()
#elif !defined( unilayer_main ) && !defined( DISABLE_MAIN_PARMS )
/*! \brief Replaces main() and calls the appropriate platform-specific
 *         entrypoint. */
#  define unilayer_main() int main( int argc, char* argv[] )
#endif /* !unilayer_main() */

#ifndef unilayer_loop_iter
/*! \brief Call the main loop for one iteration. */
#define unilayer_loop_iter() g_running = g_loop_iter( g_loop_data, g_loop_gargs )
#endif /* !unilayer_loop_iter() */

#ifndef platform_init
/*! \brief Platform-specific setup function (e.g. create window). */
#define platform_init( graphics_args, icon )
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

