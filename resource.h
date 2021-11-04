
#ifndef RESOURCE_H
#define RESOURCE_H

#ifndef NO_RESEXT
#include <resext.h>
#endif /* !NO_RESEXT */

/* ! */
#ifdef ANCIENT_C
/* ! */

#  define resource_debug_printf
#  define resource_error_printf

/* ! */
#else
/* ! */

#ifndef resource_debug_printf
#define resource_debug_printf( lvl, fmt, rsrc, ... ) debug_printf( lvl, "resource %d: " fmt, rsrc, __VA_ARGS__ )
#endif /* !resource_printf */

#ifndef resource_error_printf
#define resource_error_printf( fmt, rsrc, ... ) error_printf( "resource %d: " fmt, rsrc, __VA_ARGS__ )
#endif /* !resource_printf */

/* ! */
#endif
/* ! */

/*! \file resource.h
 *  \brief Platform-general function prototypes for loading assets.
 */

RESOURCE_BITMAP_HANDLE resource_get_bitmap_handle( RESOURCE_ID id );
RESOURCE_JSON_HANDLE resource_get_json_handle( RESOURCE_ID id );
void resource_free_handle( MEMORY_HANDLE );

/**
 * \brief Lock the given handle so that it cannot be relocated in memory by
 *        the operating system.
 */
MEMORY_PTR resource_lock_handle( MEMORY_HANDLE handle );

/**
 * \brief Unlock the given handle so that it may be relocated/recovered by
 *        the operating system.
 */
MEMORY_PTR resource_unlock_handle( MEMORY_HANDLE handle );

#endif /* RESOURCE_H */

