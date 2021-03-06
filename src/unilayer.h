
#ifndef UNILAYER_H
#define UNILAYER_H

/*! \file unilayer.h
 *  \brief Combines platform-specific functionality and loads enablers in
 *         the correct order.
 */

#ifdef __GNUC__
#define WARN_UNUSED __attribute__( (warn_unused_result) )
#ifndef PLATFORM_NDS
#define PACKED __attribute__( (__packed__) )
#endif /* !PLATFORM_NDS */
#else
#define WARN_UNUSED
#define PACKED
#endif /* __GNUC__ */

#if defined( __GNUC__ ) && !defined( _POSIX_C_SOURCE )
/* For strnlen(). */
#define _POSIX_C_SOURCE 200809L
#endif /* __GNUC__ */

#define stringize_internal( inp ) #inp

#define stringize( inp ) stringize_internal( inp )

#ifndef PLATFORM_GB
#include <stddef.h>
#endif /* !PLATFORM_GB */

#ifdef ANCIENT_C
/* For Microsoft C 5 and company. */
#  include <stdio.h>
#  define DIO_SILENT
#  define NO_VARGS
#  define NO_I86
#  define NO_CGA_FUNCTIONS
#endif /* ANCIENT_C */

#include "platform.h"

/* = Subsystems and Structures = */

#include "config.h"
#include "uassert.h"
#include "uprintf.h"

/* = Module Includes = */

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
#include "asn.h"
#endif /* RESOURCE_FILE */
#include "save.h"
#include "window.h"

#endif /* UNILAYER_H */

