
#include "../unilayer.h"

#include <string.h>

/* WIN16_FAKE_DS is an attempt to set the DS register to get another local
 * heap for more space. It doesn't work very well (at all).
 */

#if defined( WINM_MOVEABLE ) && defined( WIN16_FAKE_DS )
#  define WINM_FLAG() LMEM_MOVEABLE
#elif !defined( WINM_MOVEABLE ) && defined( WIN16_FAKE_DS )
#  define WINM_FLAG() LMEM_FIXED
#elif defined( WINM_MOVEABLE ) && !defined( WIN16_FAKE_DS )
#  define WINM_FLAG() GMEM_MOVEABLE
#elif !defined( WINM_MOVEABLE ) && !defined( WIN16_FAKE_DS )
#  define WINM_FLAG() GMEM_FIXED
#endif

#ifdef WIN16_FAKE_DS

#define SEGMENT 4096

static HGLOBAL g_segment_handle = 0;
static uint8_t* g_segment_ptr = NULL;

extern uint16_t memory_get_ds();
#pragma aux memory_get_ds = \
   "mov ax,ds" \
   modify [ ax ] \
   value [ ax ];

extern void memory_set_ds( uint16_t ds );
#pragma aux memory_set_ds = \
   "mov ds,ax" \
   parm [ ax ]

#endif /* WIN16_FAKE_DS */

void memory_debug_dump() {
}

int32_t memory_init() {
   
#ifdef WIN16_FAKE_DS

   /* Create another local heap for some breathing room. */
   g_segment_handle = GlobalAlloc(
      GMEM_DDESHARE | WINM_FLAG() | GMEM_ZEROINIT, SEGMENT );
   assert( 0 != g_segment_handle );
   g_segment_ptr = GlobalLock( g_segment_handle );
   debug_printf( 1, "new global segment at %p\n", g_segment_handle );
   LocalInit(
      HIWORD( (LONG)g_segment_ptr ), 0,
      (WORD)GlobalSize( g_segment_handle ) - 16 );
   UnlockSegment( HIWORD( (LONG)g_segment_ptr ) );

#endif /* WIN16_FAKE_DS */

   return 1;
}

void memory_shutdown() {
}

MEMORY_HANDLE memory_alloc( uint32_t sz, uint32_t count ) {
   MEMORY_HANDLE alloc_out = 0;

   /* TODO: Check sz * count for overflow. */

#ifdef WIN16_FAKE_DS
   uint16_t ds = 0;

   ds = memory_get_ds();
   debug_printf( 1, "alloc ds: %02x to: %02x\n",
      ds, HIWORD( (LONG)g_segment_ptr ) );

   memory_set_ds( g_segment_handle );
   alloc_out = LocalAlloc( WINM_FLAG() | LMEM_ZEROINIT, sz * count );
   memory_set_ds( ds );
#elif defined WIN32_HEAP
   alloc_out = HeapAlloc( GetProcessHeap(), HEAP_ZERO_MEMORY, sz * count );
#else
   alloc_out = GlobalAlloc( WINM_FLAG() | GMEM_ZEROINIT, sz * count );
#endif /* WIN16_FAKE_DS */

   return alloc_out;
}

void memory_free( MEMORY_HANDLE handle ) {
#ifdef WIN16_FAKE_DS
   uint16_t ds = 0;

   ds = memory_get_ds();
   debug_printf( 1, "free ds: %02x to: %02x\n",
      ds, HIWORD( (LONG)g_segment_ptr ) );

   memory_set_ds( g_segment_handle );
   LocalFree( handle );
   memory_set_ds( ds );
#elif defined WIN32_HEAP
   HeapFree( GetProcessHeap(), 0, handle );
#else
   GlobalFree( handle );
#endif /* WIN16_FAKE_DS */
}

uint32_t memory_sz( MEMORY_HANDLE handle ) {
   uint32_t sz_out = 0;
#ifdef WIN16_FAKE_DS
   uint16_t ds = 0;

   ds = memory_get_ds();
   debug_printf( 1, "sz ds: %02x to: %02x\n",
      ds, HIWORD( (LONG)g_segment_ptr ) );

   memory_set_ds( g_segment_handle );
   sz_out = LocalSize( handle );
   memory_set_ds( ds );
#elif defined WIN32_HEAP
   sz_out = HeapSize( GetProcessHeap(), 0, handle );
#else
   sz_out = GlobalSize( handle );
#endif /* WIN16_FAKE_DS */

   return sz_out;
}

/**
 * \return New size of handle; either given size or old size if unsuccessful.
 */
uint32_t memory_resize( MEMORY_HANDLE* handle, uint32_t sz ) {
   MEMORY_HANDLE new_handle = (MEMORY_HANDLE)NULL;

   debug_printf( 1, "reallocating %u-byte block to %u bytes",
      memory_sz( handle ), sz );

#ifdef WIN16_FAKE_DS
   uint16_t ds = 0;
#endif /* WIN16_FAKE_DS */

   if( (MEMORY_PTR)NULL == handle || (MEMORY_HANDLE)NULL == *handle ) {
      return 0;
   }

#ifdef WIN16_FAKE_DS
   ds = memory_get_ds();
   debug_printf( 1, "sz ds: %02x to: %02x\n",
      ds, HIWORD( (LONG)g_segment_ptr ) );

   memory_set_ds( g_segment_handle );
   new_handle = LocalReAlloc( *handle, sz, WINM_FLAG() );
   memory_set_ds( ds );
#elif defined WIN32_HEAP
   new_handle = HeapReAlloc( GetProcessHeap(), 0, *handle, sz );
#else
   new_handle = GlobalReAlloc( *handle, sz, WINM_FLAG() );
#endif /* WIN16_FAKE_DS */

   if( (MEMORY_HANDLE)NULL == new_handle ) {
      error_printf( "unable to resize handle" );
      return sz;
   }
   *handle = new_handle;

   return memory_sz( *handle );
}

void memory_copy_ptr( MEMORY_PTR dest, CONST_MEMORY_PTR src, uint32_t sz ) {
   memcpy( dest, src, sz );
}

void memory_zero_ptr( MEMORY_PTR ptr, uint32_t sz ) {
   memset( ptr, '\0', sz );
}

MEMORY_PTR memory_lock( MEMORY_HANDLE handle ) {
   MEMORY_PTR ptr_out = NULL;

#ifdef WINM_MOVEABLE
#  ifdef WIN16_FAKE_DS

   uint16_t ds = 0;

   ds = memory_get_ds();
   debug_printf( 1, "lock ds: %02x to: %02x\n",
      ds, HIWORD( (LONG)g_segment_ptr ) );

   memory_set_ds( g_segment_handle );
   ptr_out = LocalLock( handle );
   memory_set_ds( ds );

#  elif defined WIN32_HEAP

   /* No lock. */
   ptr_out = (MEMORY_PTR)handle;

#  else

   ptr_out = GlobalLock( handle );  

#  endif /* WIN16_FAKE_DS */

   return ptr_out;
#else
   return (MEMORY_PTR)handle;
#endif
}

MEMORY_PTR memory_unlock( MEMORY_HANDLE handle ) {
#ifdef WINM_MOVEABLE
#  ifdef WIN16_FAKE_DS

   uint16_t ds = 0;

   ds = memory_get_ds();
   debug_printf( 1, "unlock ds: %02x to: %02x\n",
      ds, HIWORD( (LONG)g_segment_ptr ) );

   memory_set_ds( g_segment_handle );
   LocalUnlock( handle );
   memory_set_ds( ds );

#elif defined WIN32_HEAP

   /* No lock. */

#  else

   GlobalUnlock( handle );

#  endif /* WIN16_FAKE_DS */
#endif
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

