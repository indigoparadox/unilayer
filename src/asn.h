
#ifndef ASN_H
#define ASN_H

/**
 * \addtogroup unilayer_asn ASN.1 Format
 * \brief General binary format for storing and retrieving data.
 *
 * \{
 */

/*! \file asn.h
 *  \brief Functions and macros for dealing with ASN.1 formatted data.
 */

/**
 * \addtogroup unilayer_asn_types ASN.1 Data Types
 *
 * \{
 */

#define ASN_STRING     0x16
#define ASN_INT        0x02
#define ASN_BLOB       0x04
#define ASN_SEQUENCE   0x30

/*! \} */

/**
 * \addtogroup unilayer_asn_flags ASN.1 Function Flags
 * \brief These may be passed to certain functions to modify their behavior.
 * \{
 */

/*! \brief Tread parsed integers as signed. */
#define ASN_FLAG_SIGNED 0x01

/*! \} */

/**
 * \addtogroup unilayer_asn_errors ASN.1 Error Codes
 * \brief These error codes may be returned by ASN.1-related functions.
 * \{
 */

/*! \brief Value/buffer size mismatch or overflow occurred. */
#define ASN_ERROR_INVALID_VALUE_SZ -1
#define ASN_ERROR_UNABLE_TO_ALLOCATE -2
#define ASN_ERROR_UNABLE_TO_LOCK -3
/*! \brief Invalid \ref unilayer_asn_types encountered. */
#define ASN_ERROR_INVALID_TYPE -4

/*! \} */

/**
 * \addtogroup unilayer_asn_write Writing ASN.1 Data
 * \{
 */

int32_t asn_write_int(
   MEMORY_HANDLE* h_buffer, int32_t idx, int32_t value ) SECTION_ASN;
int32_t asn_write_string(
   MEMORY_HANDLE* h_buffer, int32_t idx,
   const char* source, int32_t source_sz ) SECTION_ASN;
int32_t asn_write_blob(
   MEMORY_HANDLE* h_buffer, int32_t idx, uint8_t* source, int32_t source_sz
) SECTION_ASN;
int32_t asn_write_seq_start(
   MEMORY_HANDLE* ph_buffer, int32_t idx, int32_t* mark ) SECTION_ASN;
int32_t asn_write_seq_end(
   MEMORY_HANDLE* ph_buffer, int32_t idx, int32_t* mark ) SECTION_ASN;

/*! \} */

/**
 * \addtogroup unilayer_asn_read Reading ASN.1 Data
 * \brief Functions for reasing ASN.1 data back into C structs.
 *
 * These functions generally take a locked ::MEMORY_PTR to the ASN.1 data.
 * This may seem inconsistent with \ref unilayer_asn_write, but since the
 * ASN.1 buffer is finalized and won't change size, locking the buffer once
 * and passing it around is faster and simpler.
 *
 * \{
 */

/**
 * \brief Read short object in asn_buffer.
 * \param int_buffer Buffer in which to place parsed integer.
 * \param asn_buffer Buffer containing ASN data to parse.
 * \param idx Index of integer object (starting at type byte) in asn_buffer.
 * \return Count of bytes parsed in asn_buffer or \ref unilayer_asn_errors.
 * \deprecated Should be worked into asn_read_int using flags at some point.
 */
int16_t asn_read_short( const uint8_t* asn_buffer, int32_t idx ) SECTION_ASN;

/**
 * \brief Read int object in asn_buffer into buffer.
 * \param int_buffer Buffer in which to place parsed integer.
 * \param buffer_sz Size of output integer buffer in bytes.
 * \param flags \ref unilayer_asn_flags to alter function behavior.
 * \param asn_buffer Buffer containing ASN data to parse.
 * \param idx Index of integer object (starting at type byte) in asn_buffer.
 * \return Count of bytes parsed in asn_buffer or \ref unilayer_asn_errors.
 */
int16_t asn_read_int(
   uint8_t* int_buffer, uint8_t buffer_sz, uint8_t flags,
   const uint8_t* asn_buffer, int32_t idx ) SECTION_ASN;

/**
 * \brief Read string object in asn_buffer into str_buffer.
 * \param str_buffer Buffer in which to place parsed string.
 * \param str_buffer_sz Size of output string buffer in bytes.
 * \param asn_buffer Buffer containing ASN data to parse.
 * \param idx Index of string object (starting at type byte) in asn_buffer.
 * \return Count of bytes parsed in asn_buffer or \ref unilayer_asn_errors.
 */
int16_t asn_read_string(
   char* str_buffer, int16_t str_buffer_sz,
   const uint8_t* asn_buffer, int32_t idx ) SECTION_ASN;

/**
 * \brief Get metadata for the object at idx in buffer.
 * \param buffer
 * \param idx
 * \param type_out
 * \param sz_out
 * \return
 */
int16_t asn_read_meta_ptr(
   const uint8_t* buffer, int32_t idx, uint8_t* type_out, int32_t* sz_out
) SECTION_ASN;

/*! \} */

/*! \} */

#endif /* !ASN_H */

