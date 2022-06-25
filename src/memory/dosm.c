
#define MEMORY_PLATFORM_C
#include "../unilayer.h"

#include <stdlib.h>
#include <string.h>

#include <malloc.h>

static struct FAKE_MEMORY_HANDLE* g_block_first = NULL;

void memory_debug_dump() {
   struct _heapinfo h_info;
   int heap_status = _HEAPOK;
   size_t heap_total_used = 0,
      heap_total_free = 0;
   char* status = NULL;

   h_info._pentry = NULL;
   for(;;) {
      heap_status = _nheapwalk( &h_info );
      if( heap_status != _HEAPOK ) {
         break;
      }
      if( _USEDENTRY == h_info._useflag ) {
         status = "used";
         heap_total_used += h_info._size;
      } else {
         status = "free";
         heap_total_free += h_info._size;
      }  
      debug_printf( 3, "%s block at %Fp (%u bytes)",
         status, h_info._pentry, h_info._size );
   }

   switch( heap_status ) {
   case _HEAPEND:
      debug_printf( 3, "end of heap (%u bytes used, %u bytes free)",
         heap_total_used, heap_total_free );
      debug_printf( 3, "memory available: %u", _memavl() );
      break;
   case _HEAPEMPTY:
      debug_printf( 3, "heap empty" );
      break;
   case _HEAPBADBEGIN:
      error_printf( "heap is damaged" );
      break;
   case _HEAPBADPTR:
      error_printf( "bad pointer to heap" );
      break;
   case _HEAPBADNODE:
      error_printf( "bad node in heap" );
      break;
   }
}

int32_t memory_init() {
   debug_printf( 3, "memory available before growth: %u", _memavl() );
   _nheapgrow();
   debug_printf( 3, "memory available after growth: %u", _memavl() );
   return 1;
}

void memory_shutdown() {
}

MEMORY_HANDLE memory_alloc( uint32_t sz, uint32_t count ) {
   struct FAKE_MEMORY_HANDLE* iter = NULL;
   uint32_t new_sz;

   /* TODO: Detect overflow. */
   assert( sz * count >= sz );
   new_sz = sz * count;

   if( NULL == g_block_first ) {
      g_block_first = _ncalloc( 1, sizeof( struct FAKE_MEMORY_HANDLE ) );
      iter = g_block_first;
   } else {
      iter = g_block_first;
      while( NULL != iter->next ) {
         iter = iter->next;
      }
      iter->next = _ncalloc( 1, sizeof( struct FAKE_MEMORY_HANDLE ) );
      iter = iter->next;
   }

   if( NULL == iter ) {
      error_printf( "unable to allocate handle" );
   } else {
      iter->ptr_sz = new_sz;
      iter->ptr = _ncalloc( 1, new_sz );
      assert( iter->ptr_sz == sz * count );
   }

#ifdef MEMORY_HANDLE_SENTINAL
   iter->sentinal = MEMORY_HANDLE_SENTINAL;
#endif /* MEMORY_HANDLE_SENTINAL */

   return iter;
}

void memory_free( MEMORY_HANDLE handle ) {
   struct FAKE_MEMORY_HANDLE* handle_iter;
   
   if( NULL == handle ) {
      error_printf( "attempted to free NULL handle" );
      return;
   }

#ifdef MEMORY_HANDLE_SENTINAL
   assert( MEMORY_HANDLE_SENTINAL == handle->sentinal );
#endif /* MEMORY_HANDLE_SENTINAL */

   assert( 0 == handle->locks );

#ifdef MEMORY_HANDLE_SENTINAL
   handle->sentinal = 0;
#endif /* MEMORY_HANDLE_SENTINAL */

   /* Find the handle block and remove it from the chain. */
   if( handle == g_block_first ) {
      g_block_first = handle->next;
   } else {
      handle_iter = g_block_first;
      while( handle != handle_iter->next ) {
         handle_iter = handle_iter->next;
      }
      handle_iter->next = handle->next;
   }

   _nfree( handle->ptr );
   _nfree( handle );
}

uint32_t memory_sz( MEMORY_HANDLE handle ) {
   return handle->ptr_sz;
}

uint32_t memory_resize( MEMORY_HANDLE* handle, uint32_t sz ) {
   MEMORY_PTR new_ptr = NULL;

   if( NULL == handle || NULL == *handle || NULL == (*handle)->ptr ) {
      return 0;
   }

   debug_printf( 1, "reallocating %u-byte block to %u bytes",
      (*handle)->ptr_sz, sz );

   new_ptr = _nrealloc( (*handle)->ptr, sz );
   if( NULL == new_ptr ) {
      error_printf( "unable to reallocate handle" );
      return 0;
   }

   (*handle)->ptr = new_ptr;
   (*handle)->ptr_sz = sz;

   return (*handle)->ptr_sz;
}

void memory_copy_ptr( MEMORY_PTR dest, CONST_MEMORY_PTR src, uint32_t sz ) {
   memcpy( dest, src, sz );
}

void memory_zero_ptr( MEMORY_PTR ptr, uint32_t sz ) {
   memset( ptr, 0, sz );
}

MEMORY_PTR memory_lock( MEMORY_HANDLE handle ) {
   handle->locks++;
   assert( 1 == handle->locks );
   return handle->ptr;
}

MEMORY_PTR memory_unlock( MEMORY_HANDLE handle ) {
   handle->locks--;
   assert( 0 == handle->locks );
   return NULL;
}

char* memory_strncpy_ptr( char* dest, const char* src, uint16_t sz ) {
   return strncpy( dest, src, sz );
}

int16_t memory_strncmp_ptr( const char* s1, const char* s2, uint16_t sz ) {
   return strncmp( s1, s2, sz );
}

int16_t memory_strnlen_ptr( const char* s1, uint16_t sz ) {
   return strlen( s1 );
}

