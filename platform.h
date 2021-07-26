
#ifndef PLATFORM_H
#define PLATFORM_H

#if defined( PLATFORM_WIN16 ) || defined( PLATFORM_WIN32 ) || defined( PLATFORM_WINCE )
#define PLATFORM_WIN
#endif

#ifdef PLATFORM_DOS

#  define NEWLINE_STR "\n"
#  define LOG_TO_FILE
#  define LOG_FILE_NAME "logdos.txt"
#  include "types/x86.h"
#  include "memory/fakem.h"
#  if defined RESOURCE_DRC
#     include "resource/drcr.h"
#  elif defined RESOURCE_HEADER
#     include "resource/header.h"
#  else
#     define RESOURCE_FILE
#     include "resource/file.h"
#  endif
#  include "input/dosi.h"
#  include "graphics/dosg.h"

#elif defined( PLATFORM_SDL )

#  ifndef NEWLINE_STR
#    define NEWLINE_STR "\n"
#  endif /* !NEWLINE_STR */
#  include <stdint.h>
#  include "memory/fakem.h"
#  if defined RESOURCE_DRC
#     include "resource/drcr.h"
#  elif defined RESOURCE_HEADER
#     include "resource/header.h"
#  else
#     define RESOURCE_FILE
#     include "resource/file.h"
#  endif
#  include "input/sdli.h"
#  include "graphics/sdlg.h"

#elif defined( PLATFORM_XLIB )

#  ifndef NEWLINE_STR
#    define NEWLINE_STR "\n"
#  endif /* !NEWLINE_STR */
#  include <X11/Xlib.h>
#  include <stdint.h>
#  include "memory/fakem.h"
#  if defined RESOURCE_DRC
#     include "resource/drcr.h"
#  elif defined RESOURCE_HEADER
#     include "resource/header.h"
#  else
#     define RESOURCE_FILE
#     include "resource/file.h"
#  endif
#  include "input/xi.h"
#  include "graphics/xg.h"

#elif defined( PLATFORM_PALM )

#  ifdef RESOURCE_FILE
#     undef RESOURCE_FILE
#  endif
#  ifdef RESOURCE_HEADER
#     undef RESOURCE_HEADER
#  endif
#  ifdef RESOURCE_DRC
#     undef RESOURCE_DRC
#  endif
#  define NEWLINE_STR "\n"
#  define USE_SOFT_ASSERT
#  define DISABLE_FILESYSTEM
#  define LOG_TO_FILE
#  define LOG_FILE_NAME "logpalm.txt"
#  include <PalmOS.h>
#  define platform_file HostFILE*
#  define platform_fprintf HostFPrintF
#  define platform_fopen HostFOpen
#  define platform_fflush HostFFlush
#  define platform_fclose HostFClose
#  include "types/palmt.h"
#  include "memory/palmm.h"
#  include "resource/palmr.h"
#  include "input/palmi.h"
#  include "graphics/palmg.h"

#elif defined( PLATFORM_WIN )

#  ifdef RESOURCE_FILE
#     undef RESOURCE_FILE
#  endif
#  ifdef RESOURCE_HEADER
#     undef RESOURCE_HEADER
#  endif
#  ifdef RESOURCE_DRC
#     undef RESOURCE_DRC
#  endif
#  define NEWLINE_STR "\n"
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
#  include <windows.h>
#  include "types/x86.h"
#  include "memory/winm.h"
#  include "resource/winr.h"
#  include "input/wini.h"
#  include "graphics/wing.h"

#elif defined( PLATFORM_MAC6 )

#  define DISABLE_MAIN_PARMS
#  define NEWLINE_STR "\r"
#  define LOG_TO_FILE
#  define LOG_FILE_NAME "logmac.txt"
#  include <Multiverse.h>
#  include <Quickdraw.h>
#  include <stdint.h>
#  include "memory/mac7m.h"
#  if defined RESOURCE_DRC
#     include "resource/drcr.h"
#  elif defined RESOURCE_HEADER
#     include "resource/header.h"
#  else
#     define RESOURCE_FILE
#     include "resource/file.h"
#  endif
#  include "input/mac6i.h"
#  include "graphics/mac6g.h"

#elif defined( PLATFORM_NDS )

#  ifndef NEWLINE_STR
#    define NEWLINE_STR "\n"
#  endif /* !NEWLINE_STR */
#  define LOG_TO_FILE
#  define LOG_FILE_NAME "lognds.txt"
#  include <nds.h>
#  include <stdint.h>
#  include "memory/fakem.h"
#  if defined RESOURCE_DRC
#     include "resource/drcr.h"
#  elif defined RESOURCE_HEADER
#     include "resource/header.h"
#  else
#     define RESOURCE_FILE
#     include "resource/file.h"
#  endif
#  include "input/ndsi.h"
#  include "graphics/ndsg.h"

#elif defined( PLATFORM_GL )

#  ifndef NEWLINE_STR
#    define NEWLINE_STR "\n"
#  endif /* !NEWLINE_STR */
#  include <stdint.h>
#  include "memory/fakem.h"
#  if defined RESOURCE_DRC
#     include "resource/drcr.h"
#  elif defined RESOURCE_HEADER
#     include "resource/header.h"
#  else
#     define RESOURCE_FILE
#     include "resource/file.h"
#  endif
#  include "input/gli.h"
#  include "graphics/glg.h"

#else

#  ifndef NEWLINE_STR
#    define NEWLINE_STR "\n"
#  endif /* !NEWLINE_STR */
#  include <stdint.h>
#  include "memory/fakem.h"
#  include "resource/nullr.h"
#  include "input/nulli.h"
#  include "graphics/nullg.h"

#endif

#endif /* !PLATFORM_H */

