
#ifndef MEMORY_FAKEM_H
#define MEMORY_FAKEM_H

/*! \file fakem.h
 *  \brief Platform-overridden typedefs and macros for handling memory.
 */

struct FAKE_MEMORY_HANDLE;

/*! \brief A reference to a block of memory which the operating system may
 *         move around on its own to free up space. **MUST BE** locked with
 *         memory_lock() to receive a MEMORY_PTR that can be dereferenced.
 */
typedef struct FAKE_MEMORY_HANDLE* MEMORY_HANDLE;

#ifdef ANCIENT_C

typedef char* MEMORY_PTR;
typedef const char* CONST_MEMORY_PTR;
#  ifdef PLATFORM_DOS
typedef char far * MEMORY_FAR_PTR;
typedef const char far * CONST_MEMORY_FAR_PTR;
#  else
typedef char* MEMORY_FAR_PTR;
typedef const char* CONST_MEMORY_FAR_PTR;
#  endif

#else

/*! \brief A C-style memory pointer that can be safely dereferenced.
 *
 * Interchangeable with pointers to arbitrary types (e.g. char\*, int\*, etc).
 */
typedef void* MEMORY_PTR;

/*! \brief A read-only MEMORY_PTR. */
typedef const void* CONST_MEMORY_PTR;
#  ifdef PLATFORM_DOS
typedef void far * MEMORY_FAR_PTR;
typedef const void far * CONST_MEMORY_FAR_PTR;
#  else
/*! \brief On certain platforms, a MEMORY_PTR that exists outside of the current page. */
typedef void* MEMORY_FAR_PTR;
/*! \brief A read-only MEMORY_FAR_PTR. */
typedef const void* CONST_MEMORY_FAR_PTR;
#  endif

#endif /* ANCIENT_C */


#endif /* !MEMORY_FAKEM_H */

