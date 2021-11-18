
#ifndef UNILAYER_H
#define UNILAYER_H

/*! \file unilayer.h
 *  \brief Combines platform-specific functionality and loads enablers in
 *         the correct order.
 */

#ifdef __GNUC__
#define WARN_UNUSED __attribute__( (warn_unused_result) )
#define PACKED __attribute__( (__packed__) )
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

#include <stddef.h>

#include <errno.h>

#if !defined( offsetof )
#warn "offsetof not defined in stddef.h; using internal version..."
#define offsetof( obj, field ) ((int)(&(((obj*)(0))->field)))
#endif /* !offsetof */

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
#include "graphics.h"
#include "animate.h"
#include "input.h"
#include "dio.h"

/**
 * \brief Setup the main loop function and data arguments.
 * \param iter Function pointer to the main loop iteration function.
 * \param data MEMORY_HANDLE for implementation-specific data/state struct.
 * \param gargs MEMORY_PTR for platform-specific graphics args struct.
 */
#define loop_set( iter, data, gargs ) g_loop_iter = (loop_iter)iter; g_loop_data = (void*)data; g_loop_gargs = gargs;

#endif /* UNILAYER_H */

