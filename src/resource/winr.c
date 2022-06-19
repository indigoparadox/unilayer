
#define RESOURCE_C
#include "../unilayer.h"

extern HINSTANCE g_instance;

RESOURCE_HANDLE resource_get_handle( uint32_t id ) {
   debug_printf( 2, "loading bitmap resource %u", id );
   return LoadBitmap( g_instance, MAKEINTRESOURCE( id ) );
}

MEMORY_PTR resource_lock_handle( RESOURCE_HANDLE handle ) {
   return LocalLock( handle );
}

MEMORY_PTR resource_unlock_handle( RESOURCE_HANDLE handle ) {
   return LocalUnlock( handle );
}

int32_t resource_sz_handle( RESOURCE_HANDLE handle ) {
   /* TODO */
   return 0;
}

void resource_free_handle( RESOURCE_HANDLE handle ) {
   LocalFree( handle );
}

