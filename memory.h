
#ifndef MEMORY_H
#define MEMORY_H

/*! \file memory.h
 *  \brief Functions and prototypes for managing memory.
 */

struct FAKE_MEMORY_HANDLE {
#if 0
   uint32_t sz;
   uint32_t offset;
#endif
   MEMORY_PTR ptr;
   uint32_t ptr_sz;
   uint32_t locks;
   struct FAKE_MEMORY_HANDLE* next;
};

/**
 * \brief Request dynamic memory from operating system while protecting from
 *        overflow conditions.
 * \param sz Size of blocks to request.
 * \param count Number of blocks to request.
 * \return A MEMORY_HANDLE to allocated memory. This handle MUST BE locked to
 *         receive a MEMORY_PTR that can be dereferenced safely.
 */
MEMORY_HANDLE memory_alloc( uint32_t sz, uint32_t count );

void memory_free( MEMORY_HANDLE );
uint32_t memory_sz( MEMORY_HANDLE );
uint32_t memory_resize( MEMORY_HANDLE*, uint32_t );
void memory_copy_ptr( MEMORY_PTR, CONST_MEMORY_PTR, uint32_t );
void memory_zero_ptr( MEMORY_PTR, uint32_t );

/**
 * \brief Lock a dynamic memory handle and return a MEMORY_PTR that can be
 *        dereferenced safely.
 * \param handle A handle that has been successfully allocated with
 *        memory_alloc()
 * \return A typical C-style pointer that can be dereferenced as normal.
 */
WARN_UNUSED MEMORY_PTR memory_lock( MEMORY_HANDLE handle );
WARN_UNUSED MEMORY_PTR memory_unlock( MEMORY_HANDLE );
char* memory_strncpy_ptr( char*, const char*, uint16_t );
int16_t memory_strncmp_ptr( const char*, const char*, uint16_t );
int16_t memory_strnlen_ptr( const char*, uint16_t );

#endif /* !MEMORY_H */

