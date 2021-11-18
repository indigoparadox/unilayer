
#ifndef MEMORY_H
#define MEMORY_H

/**
 * \addtogroup unilayer_memory Unilayer Memory Layer
 * \brief Simple abstract unified memory management layer designed
 *        for resource-constrained systems.
 *
 * \{
 */

/*! \file memory.h
 *  \brief Functions and prototypes for managing memory.
 */

/*! \brief An emulated memory handle for modern systems. Overridden on most
 *         platforms.
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
 * \return A MEMORY_HANDLE to allocated memory. This handle **MUST BE** locked
 *         to receive a MEMORY_PTR that can be dereferenced safely.
 */
MEMORY_HANDLE memory_alloc( uint32_t sz, uint32_t count );

/**
 * \brief Free a block of dynamic memory previously allocated with
 *        memory_alloc().
 * \param handle ::MEMORY_HANDLE to a previously allocated memory block.
 */
void memory_free( MEMORY_HANDLE handle );
uint32_t memory_sz( MEMORY_HANDLE );
uint32_t memory_resize( MEMORY_HANDLE*, uint32_t );
void memory_copy_ptr( MEMORY_PTR, CONST_MEMORY_PTR, uint32_t );

/**
 * \brief Fill a block of memory with zeros.
 * \param ptr ::MEMORY_PTR to a block of memory to overwrite. May be on the
 *            stack or a ::MEMORY_PTR returned from locking a ::MEMORY_HANDLE.
 * \param sz Size of the block of memory to overwrite.
 */
void memory_zero_ptr( MEMORY_PTR ptr, uint32_t sz );

/**
 * \brief Lock a dynamic memory handle and return a ::MEMORY_PTR that can be
 *        dereferenced safely.
 * \param handle A handle that has been successfully allocated with
 *        memory_alloc().
 * \return A typical C-style pointer that can be dereferenced as normal.
 */
WARN_UNUSED MEMORY_PTR memory_lock( MEMORY_HANDLE handle );

/**
 * \brief Unlock a dynamic memory handle so the system can relocate it on the
 *        heap to relieve congestion.
 * \param handle A handle that has been previously locked with memory_lock().
 * \return NULL if the unlock was successful.
 */
WARN_UNUSED MEMORY_PTR memory_unlock( MEMORY_HANDLE handle );
char* memory_strncpy_ptr( char*, const char*, uint16_t );
int16_t memory_strncmp_ptr( const char*, const char*, uint16_t );

/**
 * \brief Get the size of a string, up to a specified maximum.
 * \param s ::MEMORY_PTR to a string to get the size of.
 * \param l Maximum possible size of string s.
 * \return Position of the NULL terminator in string s, or if none is found
 *         under l characters, l.
 */
int16_t memory_strnlen_ptr( const char* s, uint16_t l );

/*! \} */

#endif /* !MEMORY_H */

