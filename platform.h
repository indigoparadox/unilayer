
#ifndef PLATFORM_H
#define PLATFORM_H

#ifdef PLATFORM_DOS

#  include "types/x86.h"
#  include "memory/fakem.h"
#  if defined RESOURCE_DRC
#     include "resource/drcr.h"
#  elif defined RESOURCE_HEADER
#     include "resource/header.h"
#  else
#     include "resource/file.h"
#  endif
#  include "input/dosi.h"
#  include "graphics/dosg.h"

#elif defined( PLATFORM_SDL )

#  include <stdint.h>
#  include "memory/fakem.h"
#  if defined RESOURCE_DRC
#     include "resource/drcr.h"
#  elif defined RESOURCE_HEADER
#     include "resource/header.h"
#  else
#     include "resource/file.h"
#  endif
#  include "input/sdli.h"
#  include "graphics/sdlg.h"

#elif defined( PLATFORM_XLIB )

#  include <stdint.h>
#  include "memory/fakem.h"
#  if defined RESOURCE_DRC
#     include "resource/drcr.h"
#  elif defined RESOURCE_HEADER
#     include "resource/header.h"
#  else
#     include "resource/file.h"
#  endif
#  include "input/xi.h"
#  include "graphics/xg.h"

#elif defined( PLATFORM_PALM )

#  include <PalmOS.h>
#  include "types/palmt.h"
#  include "memory/palmm.h"
#  include "resource/palmr.h"
#  include "input/palmi.h"
#  include "graphics/palmg.h"

#elif defined( PLATFORM_WIN )

#  include <windows.h>
#  include "types/x86.h"
#  include "memory/winm.h"
#  include "resource/winr.h"
#  include "input/wini.h"
#  include "graphics/wing.h"

#elif defined( PLATFORM_MAC6 )

#  include <Multiverse.h>
#  include <Quickdraw.h>
#  include <stdint.h>
#  include "memory/mac7m.h"
#  if defined RESOURCE_DRC
#     include "resource/drcr.h"
#  elif defined RESOURCE_HEADER
#     include "resource/header.h"
#  else
#     include "resource/file.h"
#  endif
#  include "input/mac6i.h"
#  include "graphics/mac6g.h"

#elif defined( PLATFORM_NDS )

#  include <nds.h>
#  include <stdint.h>
#  include "memory/fakem.h"
#  if defined RESOURCE_DRC
#     include "resource/drcr.h"
#  elif defined RESOURCE_HEADER
#     include "resource/header.h"
#  else
#     include "resource/file.h"
#  endif
#  include "input/ndsi.h"
#  include "graphics/ndsg.h"

#elif defined( PLATFORM_GL )

#  include <stdint.h>
#  include "memory/fakem.h"
#  if defined RESOURCE_DRC
#     include "resource/drcr.h"
#  elif defined RESOURCE_HEADER
#     include "resource/header.h"
#  else
#     include "resource/file.h"
#  endif
#  include "input/gli.h"
#  include "graphics/glg.h"

#else

#  include <stdint.h>
#  include "memory/fakem.h"
#  include "resource/nullr.h"
#  include "input/nulli.h"
#  include "graphics/nullg.h"

#endif

#endif /* !PLATFORM_H */

