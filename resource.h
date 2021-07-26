
#ifndef RESOURCE_H
#define RESOURCE_H

RESOURCE_BITMAP_HANDLE resource_get_bitmap_handle( RESOURCE_ID );
#ifdef USE_JSON_MAPS
RESOURCE_JSON_HANDLE resource_get_json_handle( RESOURCE_ID );
#endif /* USE_JSON_MAPS */
void resource_free_handle( MEMORY_HANDLE );
MEMORY_PTR resource_lock_handle( MEMORY_HANDLE );
MEMORY_PTR resource_unlock_handle( MEMORY_HANDLE );

#endif /* RESOURCE_H */

