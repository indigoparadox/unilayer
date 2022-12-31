
#ifndef DIO_H
#define DIO_H

/**
 * \addtogroup unilayer_dio Unilayer Data Inputs & Operations
 *
 * \{
 */

/*! \file dio.h
 *  \brief General abstractions for common stdlib-related tasks.
 */

/**
 * \addtogroup unilayer_dio_errors Unilayer DIO Errors
 * \brief Error codes from DIO operations.
 *
 * \{
 */

#define DIO_ERROR_NULL_PTR             -1
#define DIO_ERROR_COPY_MISMATCH        -2
#define DIO_ERROR_COULD_NOT_OPEN_FILE  -3
#define DIO_ERROR_INVALID_STREAM       -4

/*! \} */

/**
 * \addtogroup unilayer_dio_streams Unilayer DIO Streams
 * \brief Abstraction for dealing with streams from files or memory.
 *
 * \{
 */

/**
 * \relates DIO_STREAM
 * \brief DIO_STREAM::type indicating stream is a memory buffer.
 */
#define DIO_STREAM_BUFFER  1
/**
 * \relates DIO_STREAM
 * \brief DIO_STREAM::type indicating stream is a file on disk.
 */
#define DIO_STREAM_FILE    2

#ifdef DISABLE_FILESYSTEM

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

#define dio_assert_stream( stream )

#else
#include <stdio.h>

/**
 * \brief Assert ::DIO_STREAM is open and ready for operations.
 * \param stream ::DIO_STREAM to test.
 */
#define dio_assert_stream( stream ) \
   assert( \
      0 <= stream->id && \
      ((DIO_STREAM_BUFFER == stream->type && NULL != stream->buffer.bytes ) || \
      (DIO_STREAM_FILE == stream->type && NULL != stream->buffer.file )) )
#endif /* DISABLE_FILESYSTEM */

union DIO_BUFFER {
#ifndef DISABLE_FILESYSTEM
   FILE* file;
#endif /* !DISABLE_FILESYSTEM */
   uint8_t* bytes;
};

struct DIO_STREAM {
   int32_t id;
   uint8_t type;
   union DIO_BUFFER buffer;
   int32_t buffer_sz;
   int32_t position;
};

/**
 * \addtogroup unilayer_dio_list Unilayer Static Lists
 * \{
 */

#define LIST_ERROR_MAX -100

/**
 * \brief Append a copy of the given node to the end of a static list.
 * \param node Locked ::MEMORY_PTR to a node to copy to the end of the list.
 * \param list Locked ::MEMORY_PTR to the list to manipulate.
 * \param list_sz Number of active items in the list.
 * \param list_max Maximum number of active items the list can hold.
 * \param list_type Type of items in the list.
 */
#define dio_list_append( node, list, list_sz, list_max, list_type ) \
   if( (list_sz) + 1 >= (list_max) ) { \
      g_dio_error = LIST_ERROR_MAX; \
   } else { \
      memory_copy_ptr( (MEMORY_PTR)&(list[list_sz]), (MEMORY_PTR)node, \
         sizeof( list_type ) ); \
      (list_sz)++; \
   }

/**
 * \brief Remove an item from a static list and move the other items up, then
 *        shrink the list.
 * \param idx Index of the item to remove.
 * \param list Locked ::MEMORY_PTR to the list to manipulate.
 * \param list_sz Number of active items in the list.
 * \param list_type Type of items in the list.
 */
#define dio_list_remove( idx, list, list_sz, list_type ) assert( (idx) < (list_sz) ); while( (idx) + 1 < (list_sz) ) { memory_copy_ptr( (MEMORY_PTR)&(list[idx]), (MEMORY_PTR)&(list[idx + 1]), sizeof( list_type ) ); (idx)++; } (list_sz)--;

/*! \} */ /* unilayer_dio_list */

/**
 * \brief Open a stream as a file on disk.
 * \param path Path to the file on disk to open.
 * \param mode Standard FILE mode string (e.g. "r", "w", or "rb" for binary).
 * \param stream ::DIO_STREAM object to open file into.
 * \return
 */
int32_t dio_open_stream_file(
   const char* path, const char* mode, struct DIO_STREAM* stream );
int32_t dio_open_stream_buffer( uint8_t*, uint32_t, struct DIO_STREAM* );
void dio_close_stream( struct DIO_STREAM* );
int32_t dio_seek_stream( struct DIO_STREAM*, int32_t, uint8_t );
int32_t dio_tell_stream( struct DIO_STREAM* );
int32_t dio_sz_stream( struct DIO_STREAM* );
int32_t dio_position_stream( struct DIO_STREAM* );
uint8_t dio_type_stream( struct DIO_STREAM* );
int32_t dio_read_stream( MEMORY_PTR, uint32_t, struct DIO_STREAM* );
int32_t dio_write_stream( const MEMORY_PTR, uint32_t, struct DIO_STREAM* );

/*! \} */

uint32_t dio_reverse_endian_32( uint32_t );
uint16_t dio_reverse_endian_16( uint16_t );
int32_t dio_char_idx( const char*, int32_t, char );
int32_t dio_char_idx_r( const char*, int32_t, char );
int16_t dio_mktemp_path( char*, uint16_t, const char* );
int32_t dio_basename( const char*, uint32_t );
int32_t dio_copy_file( const char*, const char* );
uint32_t dio_read_file( const char*, MEMORY_HANDLE* );
int32_t dio_move_file( const char*, const char* );
int16_t dio_strnchr( const char*, uint16_t, char );
int16_t dio_itoa( char*, uint16_t, int16_t, uint8_t );
int16_t dio_atoi( const char*, uint8_t );
int16_t dio_snprintf( char*, uint16_t, const char*, ... );

#ifdef DIO_C
int8_t g_dio_error = 0;
#else
/*! \brief Holds return errors for macro-based utilities. */
extern int8_t g_dio_error;
#endif /* DIO_C */

/*! \} */ /* unilayer_dio */

#endif /* DIO_H */

