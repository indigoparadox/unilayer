
#ifndef RESOURCE_H
#define RESOURCE_H

/**
 * \addtogroup unilayer_resource Unilayer Resource Layer
 * \brief Simple abstract unified resource loading layer designed
 *        for various obsolete systems.
 *
 * \{
 */

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

RESOURCE_HANDLE resource_get_handle( const RESOURCE_ID id ) SECTION_SETUP;

/**
 * \brief Lock the given handle so that it cannot be relocated in memory by
 *        the operating system.
 */
MEMORY_PTR resource_lock_handle( RESOURCE_HANDLE handle ) SECTION_SETUP;

/**
 * \brief Unlock the given handle so that it may be relocated/recovered by
 *        the operating system.
 */
MEMORY_PTR resource_unlock_handle( RESOURCE_HANDLE handle ) SECTION_SETUP;

int32_t resource_sz_handle( RESOURCE_HANDLE handle ) SECTION_SETUP;
void resource_free_handle( RESOURCE_HANDLE ) SECTION_SETUP;

uint8_t resource_id_from_name(
   RESOURCE_ID* id, RESOURCE_NAME name, const char* ext ) SECTION_SETUP;

/*! \} */

#endif /* RESOURCE_H */

