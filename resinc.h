
#ifndef RESINC_H
#define RESINC_H

#if defined RESOURCE_DRC
#   define RESOURCES "DRC"
#   include "resource/drcr.h"
#elif defined RESOURCE_HEADER
#   define RESOURCES "HEADER"
#   include "resource/header.h"
#elif defined PLATFORM_WIN && !defined RESOURCE_FILE
#   define RESOURCES "WIN"
#   include "resource/winr.h"
#elif defined PLATFORM_PALM && !defined RESOURCE_FILE
#   define RESOURCES "PALM"
#   include "resource/palmr.h"
#elif !defined RESOURCE_FILE
/* Nothing else has been defined, so fall back to file. */
#   define RESOURCE_FILE
#endif

#ifdef RESOURCE_FILE
#   define RESOURCES "FILE"
#   include "resource/file.h"
#endif /* RESOURCE_FILE */

#endif /* !RESINC_H */

