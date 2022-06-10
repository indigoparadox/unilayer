
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

#define ASN_ERROR_INVALID_VALUE_SZ -1
#define ASN_ERROR_UNABLE_TO_ALLOCATE -2
#define ASN_ERROR_UNABLE_TO_LOCK -3

int32_t asn_write_int( MEMORY_HANDLE* h_buffer, int32_t idx, int32_t value );
int32_t asn_write_string(
   MEMORY_HANDLE* h_buffer, int32_t idx,
   const char* source, int32_t source_sz );
int32_t asn_write_blob(
   MEMORY_HANDLE* h_buffer, int32_t idx, uint8_t* source, int32_t source_sz );
int32_t asn_write_seq_start(
   MEMORY_HANDLE* ph_buffer, int32_t idx, int32_t* mark );
int32_t asn_write_seq_end(
   MEMORY_HANDLE* ph_buffer, int32_t idx, int32_t* mark );

/**
 * \brief Read short object in asn_buffer.
 */
int16_t asn_read_short( const uint8_t* asn_buffer, int32_t idx );

/**
 * \brief Read int object in asn_buffer into buffer.
 */
int16_t asn_read_int(
   uint8_t* buffer, uint8_t buffer_sz, uint8_t sign,
   const uint8_t* asn_buffer, int32_t idx );

/**
 * \brief Read string object in asn_buffer into str_buffer.
 */
int16_t asn_read_string(
   char* str_buffer, int16_t buffer_sz,
   const uint8_t* asn_buffer, int32_t idx );

/**
 * \brief Get metadata for the object at idx in buffer.
 * \param buffer
 * \param idx
 * \param type_out
 * \param sz_out
 * \return
 */
int32_t asn_read_meta_ptr(
   const uint8_t* buffer, int32_t idx, uint8_t* type_out, int32_t* sz_out );

/*! \} */

#endif /* !ASN_H */

