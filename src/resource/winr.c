
#define RESOURCE_C
#include "../unilayer.h"

extern HINSTANCE g_instance;

typedef HBITMAP RESOURCE_BITMAP_HANDLE;

RESOURCE_BITMAP_HANDLE resource_get_bitmap_handle( uint32_t id ) {
   debug_printf( 2, "loading bitmap resource %u", id );
   return LoadBitmap( g_instance, MAKEINTRESOURCE( id ) );
}

MEMORY_PTR resource_lock_handle( MEMORY_HANDLE handle ) {
   return memory_lock( handle );
}

MEMORY_PTR resource_unlock_handle( MEMORY_HANDLE handle ) {
   return memory_unlock( handle );
}

void resource_free_handle( MEMORY_HANDLE handle ) {
   memory_free( handle );
}

