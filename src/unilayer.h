
#ifndef UNILAYER_H
#define UNILAYER_H

/*! \file unilayer.h
 *  \brief Combines platform-specific functionality and loads enablers in
 *         the correct order.
 */

#ifdef __GNUC__
#  define WARN_UNUSED __attribute__( (warn_unused_result) )
#  ifndef PLATFORM_NDS
#     define UNILAYER_PACKED __attribute__( (__packed__) )
#  else
#     define UNILAYER_PACKED
#  endif /* !PLATFORM_NDS */
#  define INLINE inline
#elif defined( __WATCOMC__ )
#  define WARN_UNUSED
#  define UNILAYER_PACKED
#  define INLINE
#else
#  define WARN_UNUSED
#  define UNILAYER_PACKED
#  define INLINE
#endif /* __GNUC__ */

#if defined( __GNUC__ ) && !defined( _POSIX_C_SOURCE )
/* For strnlen(). */
#define _POSIX_C_SOURCE 200809L
#endif /* __GNUC__ */

#define stringize_internal( inp ) #inp

#define stringize( inp ) stringize_internal( inp )

#if !defined( PLATFORM_GB ) && !defined( PLATFORM_NDS )
#include <stddef.h>
#endif /* !PLATFORM_GB && !PLATFORM_NDS */

#ifdef ANCIENT_C
/* For Microsoft C 5 and company. */
#  include <stdio.h>
#  define DIO_SILENT
#  define NO_VARGS
#  define NO_I86
#  define NO_CGA_FUNCTIONS
#endif /* ANCIENT_C */

#ifndef RESOURCE_NAME_MAX
/**
 * \warning Changing this may break save compatibility!
 */
#define RESOURCE_NAME_MAX 10
#endif /* !RESOURCE_NAME_MAX */

typedef char RESOURCE_NAME[RESOURCE_NAME_MAX];

/* Sets up platform, including calling resinc.h to determine resource system.
 * It calls it before platform-specific graphics header, but after setting
 * flags like forcing RESOURCE_FILE, so it can't be pulled out for now.
 */
#include "platform.h"

#ifdef PLATFORM_PALM
#define SECTION_WINDOW __attribute__ ((section( "winfns" )))
#define SECTION_SETUP __attribute__ ((section( "setfns" )))
#define SECTION_ASN __attribute__ ((section( "asnfns" )))
#else
#define SECTION_WINDOW
#define SECTION_SETUP
#define SECTION_ASN
#endif

/* = Subsystems and Structures = */

#include "config.h"
#include "uassert.h"
#include "uprintf.h"

/* = Module Includes = */

#ifndef NO_RESEXT
#include <resext.h>
#endif /* !NO_RESEXT */

#include "memory.h"
#include "resource.h"
#ifndef RES_CONST
#define RES_CONST const
#endif /* !RES_CONST */
#include "graphics.h"
#include "animate.h"
#include "input.h"
#include "dio.h"
#ifdef NETWORK_SOCKET
#include "network.h"
#endif /* UNILAYER_NETWORK */
#ifdef RESOURCE_FILE
#include "json.h"
#endif /* RESOURCE_FILE */
#include "asn.h"
#include "save.h"
#include "window.h"

#endif /* UNILAYER_H */

