
#ifndef SAVE_H
#define SAVE_H

/**
 * \addtogroup unilayer_save Unilayer Save Layer
 * \brief Low-level layer for saving and loading binary blobs of user data.
 * \{
 *
 * \file save.
 * \brief Platform-general abstractions for disk I/O.
 */

/**
 * \brief Given an unlocked ::MEMORY_HANDLE, write the contents of that handle
 *        to disk (or other long-term storage media) with the given name.
 * \param save_buffer_sz Only write this many bytes from the buffer to disk.
 */
int32_t save_write(
   const char* save_name, MEMORY_HANDLE save_buffer, int32_t save_buffer_sz
) SECTION_SETUP;

/**
 * \brief Given the name of a save, load that save from the disk (or other
 *        long-term storage media) and return a ::MEMORY_HANDLE containing
 *        a blob with that save's data.
 */
MEMORY_HANDLE save_read( const char* save_name ) SECTION_SETUP;

/*! \} */

#endif /* !SAVE_H */

