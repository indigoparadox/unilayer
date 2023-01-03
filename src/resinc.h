
#ifndef RESINC_H
#define RESINC_H

/**
 * \addtogroup unilayer_resource Unilayer Resource Layer
 * \{
 *
 * \file resinc.h
 * \brief Determines which resource-loading system to use at compile.
 */

#if 0
#if defined PLATFORM_DOS && defined RESOURCE_FILE
#error "DOS has insufficent memory to use JSON parser."
#endif
#endif

#if defined RESOURCE_HEADER
#   define RESOURCES "HEADER"
#   include "resource/header.h"
#elif defined PLATFORM_WASM && !defined RESOURCE_FILE
#   define RESOURCES "WASM"
#   include "resource/webr.h"
#elif defined PLATFORM_NDS && !defined RESOURCE_FILE
#   define RESOURCES "NDS"
#   include "resource/ndsr.h"
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

/*! \} */ /* unilayer_resource */

#endif /* !RESINC_H */

