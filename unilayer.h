
#ifndef UNILAYER_H
#define UNILAYER_H

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

#include <stddef.h>

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
#include "input.h"
#include "dio.h"
#include "drc.h"

#define loop_set( iter, data ) g_loop_iter = (loop_iter)iter; g_loop_data = (void*)data;

#endif /* UNILAYER_H */

