
#ifndef RESOURCE_H
#define RESOURCE_H

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

