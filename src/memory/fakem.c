
#define MEMORY_PLATFORM_C
#include "../unilayer.h"

#include <stdlib.h>
#include <string.h>

static struct FAKE_MEMORY_HANDLE* g_block_first = NULL;

#if 0
static const uint32_t gc_sentinal = 1212;

#define MEMORY_BLOCK_REAL_SZ( data_sz ) \
   (sizeof( struct FAKE_MEMORY_HANDLE ) + (data_sz) + sizeof( gc_sentinal ))
#endif

int32_t memory_init() {
#if 0
   g_fake_heap = calloc( MEMORY_FAKE_HEAP_SZ, 1 );
#endif
   return 1;
}

void memory_shutdown() {
#if 0
   free( g_fake_heap );
#endif
}

MEMORY_HANDLE memory_alloc( uint32_t sz, uint32_t count ) {
   struct FAKE_MEMORY_HANDLE* iter = NULL;
   uint32_t new_sz;
#if 0
   uint32_t new_real_sz = 0,
      new_addr = 0,
      new_top = 0,
      new_sz = 0;
#endif

   /* TODO: Detect overflow. */
   assert( sz * count >= sz );
   new_sz = sz * count;

   if( NULL == g_block_first ) {
#if 0
      g_block_first = (struct FAKE_MEMORY_HANDLE*)g_fake_heap;
      g_block_first->next = NULL;
      g_block_first->offset = 0;
      g_block_first->allocated = 1;
#endif
      g_block_first = calloc( 1, sizeof( struct FAKE_MEMORY_HANDLE ) );
      iter = g_block_first;
   } else {
      iter = g_block_first;
      while( NULL != iter->next ) {
         iter = iter->next;
      }
      iter->next = calloc( 1, sizeof( struct FAKE_MEMORY_HANDLE ) );
      iter = iter->next;

#if 0
      new_addr = iter->offset + MEMORY_BLOCK_REAL_SZ( iter->sz );
      new_top = new_addr + MEMORY_BLOCK_REAL_SZ( new_sz );
      if( new_top >= FAKE_HEAP_SIZE ) {
         /* Not enough memory. */
         iter = NULL;
      } else {
         iter->next = (struct FAKE_MEMORY_HANDLE*)&(g_fake_heap[new_addr]);
         last_block = iter;
         iter = iter->next;
         iter->offset = new_addr;

         /* TODO: Place/check sentinal. */
      }
#endif
   }

   if( NULL == iter ) {
      error_printf( "unable to allocate handle" );
   } else {
      iter->ptr_sz = new_sz;
#if 0
      g_fake_heap_top = iter->offset + MEMORY_BLOCK_REAL_SZ( iter->sz );
#endif

      iter->ptr = calloc( 1, new_sz );

      assert( iter->ptr_sz == sz * count );
   }

   return iter;
}

void memory_free( MEMORY_HANDLE handle ) {
   struct FAKE_MEMORY_HANDLE* handle_iter;
   
   if( NULL == handle ) {
      error_printf( "attempted to free NULL handle" );
      return;
   }

   assert( 0 == handle->locks );

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

   free( handle->ptr );
   free( handle );
}

uint32_t memory_sz( MEMORY_HANDLE handle ) {
   return handle->ptr_sz;
}

/**
 * \return New size of handle; either given size or old size if unsuccessful.
 */
uint32_t memory_resize( MEMORY_HANDLE* handle, uint32_t sz ) {
   MEMORY_PTR new_ptr = NULL;

   if( NULL == handle || NULL == *handle || NULL == (*handle)->ptr ) {
      return 0;
   }

   debug_printf( 1, "reallocating %u-byte block to %u bytes",
      (*handle)->ptr_sz, sz );

   new_ptr = realloc( (*handle)->ptr, sz );
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

#ifdef MEMORY_DEBUG_LOCKS

MEMORY_PTR memory_lock_wrapper(
   MEMORY_HANDLE handle, const char* handle_name, const char* caller
) {
   debug_printf( 1, "handle %s (%p) LOCKED by %s (from %d locks)",
      handle_name, (void*)handle, caller, handle->locks );

   return memory_lock( handle );
}

MEMORY_PTR memory_unlock_wrapper(
   MEMORY_HANDLE handle, const char* handle_name, const char* caller
) {
   debug_printf( 1, "handle %s (%p) UNLOCKED by %s (from %d locks)",
      handle_name, (void*)handle, caller, handle->locks );

   return memory_unlock( handle );
}

void memory_free_wrapper(
   MEMORY_HANDLE handle, const char* handle_name, const char* caller
) {
   debug_printf( 1, "handle %s (%p) FREED by %s",
      handle_name, (void*)handle, caller );

   memory_free( handle );
}

#endif /* MEMORY_DEBUG_LOCKS */

MEMORY_PTR memory_lock( MEMORY_HANDLE handle ) {
   handle->locks++;
   assert( 1 == handle->locks );
#if 0
   return &(g_fake_heap[handle->offset]);
#endif
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
#ifdef __GNUC__
   /* TODO: None-GNUC alternative. */
   if( 0 < sz ) {
      return strnlen( s1, sz );
   } else {
#endif /* __GNUC__ */
      return strlen( s1 );
#ifdef __GNUC__
   }
#endif /* __GNUC__ */
}

