
#include "unilayer.h"

/* Private Prototypes w/ Sections */

int8_t asn_get_int_sz( int32_t value ) SECTION_ASN;
int32_t asn_raw_write_int(
   uint8_t* buffer, int32_t idx, int32_t value, int32_t value_sz ) SECTION_ASN;
int16_t asn_raw_read_int(
   const uint8_t* buffer, int32_t idx, int32_t* value_out, int32_t value_sz
) SECTION_ASN;
int32_t asn_ensure_buffer_sz(
   MEMORY_HANDLE* ph_buffer, int32_t idx, int32_t val_sz ) SECTION_ASN;
int32_t asn_write_sz(
   uint8_t* buffer, int32_t idx, int32_t sz, int32_t sz_of_sz ) SECTION_ASN;

/* Function Definitions */

int8_t asn_get_int_sz( int32_t value ) {
   int8_t val_sz = -1;

   if( value <= 0x7f ) {
      /* TODO: 0x7f only for integer types? */
      val_sz = 1;
      
   } else if( value == (value & 0xffff) ) {
      val_sz = 2;

   } else if( value == (value & 0xffffff) ) {
      val_sz = 3;

   } else if( value == (value & 0xffffffff) ) {
      val_sz = 4;
   }

   return val_sz;
}

int32_t asn_raw_write_int(
   uint8_t* buffer, int32_t idx, int32_t value, int32_t value_sz
) {
   int32_t i = 0;

   assert( 0 < value_sz );

   for( i = value_sz - 1 ; 0 <= i ; i-- ) {
      /* Grab the i'th byte of the value from left and place it in the buffer.
       */
      buffer[idx++] = ((value >> (i * 8)) & 0xff);
      debug_printf( 1, "wrote byte #%d to buffer (0x%02x)",
         i, buffer[idx - 1] );
   }

   return idx;
}

int16_t asn_raw_read_int(
   const uint8_t* buffer, int32_t idx, int32_t* value_out, int32_t value_sz
) {
   int32_t i = 0,
      byte_scratch = 0; /* Buffer to hold byte while it is shifted. */
   int16_t field_sz = 0;

   assert( 0 < value_sz );

   for( i = value_sz - 1 ; 0 <= i ; i-- ) {
      /* Grab the buffer and shift it to the left i times. */
      byte_scratch = buffer[idx + field_sz];
      field_sz++;
      byte_scratch <<= (i * 8);

      *value_out |= byte_scratch;

      /*
      n_out |= (asn_buffer[0] << 8);
      n_out |= asn_buffer[1];
      */
   }

   return field_sz;
}

int32_t asn_ensure_buffer_sz(
   MEMORY_HANDLE* ph_buffer, int32_t idx, int32_t val_sz
) {
   int32_t sz_of_sz = 1,
      buffer_sz = 0;

   if( 127 < val_sz ) {
      sz_of_sz = asn_get_int_sz( val_sz ) + 1; /* +1 for 0x80 | sz_of_sz */
   }

   /* Grow the buffer if we need to. */
   buffer_sz = memory_sz( *ph_buffer );
   assert( 0 < buffer_sz );
   while( idx + val_sz + sz_of_sz >= buffer_sz ) {
      debug_printf( 2, "growing buffer from %d to %d bytes",
         buffer_sz, buffer_sz * 2 );
      assert( buffer_sz < buffer_sz * 2 );
      if( 0 == memory_resize( ph_buffer, buffer_sz * 2 ) ) {
         error_printf( "unable to expand buffer" );

         /* We haven't written to the buffer yet, so caller can use same
          * index if there's an error.
          */
         return ASN_ERROR_UNABLE_TO_ALLOCATE;
      }
      buffer_sz = memory_sz( *ph_buffer );
   }

   return memory_sz( *ph_buffer );
}

int32_t asn_write_sz(
   uint8_t* buffer, int32_t idx, int32_t sz, int32_t sz_of_sz
) {

   if( 127 < sz ) {
      /* 0x80 | size of size, followed by size. */
      buffer[idx++] = 0x80 | sz_of_sz;
      idx = asn_raw_write_int( buffer, idx, sz, sz_of_sz );
   } else {
      buffer[idx++] = sz;
   }

   return idx;
}

int32_t asn_write_int( MEMORY_HANDLE* ph_buffer, int32_t idx, int32_t value ) {
   int32_t val_sz = 0;
   uint8_t* buffer = NULL;
   uint8_t type_val = ASN_INT;
   int32_t retval = 0;
   
   val_sz = asn_get_int_sz( value );
   if( 0 >= val_sz ) {
      error_printf( "invalid value size" );
      idx = ASN_ERROR_INVALID_VALUE_SZ;
      goto cleanup;
   }
   debug_printf( 1, "(offset 0x%02x) value %d (0x%02x) is %d byte(s)",
      idx, value, value, val_sz );

   /* Grow the buffer if we need to. */
   retval = asn_ensure_buffer_sz( ph_buffer, idx, val_sz );
   if( 0 >= retval ) {
      error_printf( "unable to grow buffer" );
      idx = retval;
      goto cleanup;
   }

   /* >32-bit ints not supported. */
   assert( 0 < val_sz );

   /* Start writing to the buffer! */

   buffer = memory_lock( *ph_buffer );
   if( NULL == buffer ) {
      error_printf( "unable to lock buffer" );
      idx = ASN_ERROR_UNABLE_TO_LOCK;
      goto cleanup;
   }

   /* Write the integer type to the buffer. */
   if( 0 > value ) {
      type_val |= 0x40;
      value *= -1;
   }

   debug_printf( 1,
      "(offset 0x%02x) value %d (0x%02x) type is %02x",
      idx, value, value, type_val );

   buffer[idx++] = type_val;

   /* Write the size of the integer to the buffer. */
   buffer[idx++] = val_sz;

   /* Write the actual value to the buffer. */
   idx = asn_raw_write_int( buffer, idx, value, val_sz );

cleanup:

   if( NULL != buffer ) {
      buffer = memory_unlock( *ph_buffer );
   }

   return idx;
}

int32_t asn_write_string(
   MEMORY_HANDLE* ph_buffer, int32_t idx, const char* source, int32_t source_sz
) {
   int8_t sz_of_sz = 1;
   int32_t source_len = 0,
      i = 0;
   uint8_t* buffer = NULL;
   int32_t retval = 0;

   source_len = memory_strnlen_ptr( source, source_sz );

   /* Get the size of the size. */
   sz_of_sz = asn_get_int_sz( source_len );
   if( 0 >= sz_of_sz ) {
      error_printf( "invalid value size" );
      idx = ASN_ERROR_INVALID_VALUE_SZ;
      goto cleanup;
   }
   debug_printf( 1, "string length %d (0x%02x) takes %d bytes",
      source_len, source_len, sz_of_sz );

   /* Grow the buffer if we need to. */
   retval = asn_ensure_buffer_sz( ph_buffer, idx, source_len );
   if( 0 >= retval ) {
      error_printf( "unable to grow buffer" );
      idx = retval;
      goto cleanup;
   }

   /* Start writing to the buffer! */

   buffer = memory_lock( *ph_buffer );
   if( NULL == buffer ) {
      error_printf( "unable to lock buffer" );
      idx = ASN_ERROR_UNABLE_TO_LOCK;
      goto cleanup;
   }

   buffer[idx++] = ASN_STRING;

   if( 127 < source_len ) {
      /* 0x80 | size of size, followed by size. */
      buffer[idx++] = 0x80 | sz_of_sz;
      idx = asn_raw_write_int( buffer, idx, source_len, sz_of_sz );
   } else {
      buffer[idx++] = source_len;
   }

   for( i = idx ; idx + source_len > i ; i++ ) {
      buffer[i] = 0;
   }
   if( 0 < source_len ) {
      memory_copy_ptr(
         (MEMORY_PTR)&(buffer[idx]), (CONST_MEMORY_PTR)source, source_len );
      idx += source_len;
   }

cleanup:

   if( NULL != buffer ) {
      buffer = memory_unlock( *ph_buffer );
   }

   return idx;
}

int32_t asn_write_blob(
   MEMORY_HANDLE* ph_buffer, int32_t idx, uint8_t* source, int32_t source_sz
) {
   int8_t sz_of_sz = 1;
   uint8_t* buffer = NULL;
   int32_t retval = 0,
      i = 0;

   /* Get the size of the size. */
   sz_of_sz = asn_get_int_sz( source_sz );
   if( 0 >= sz_of_sz ) {
      error_printf( "invalid value size" );
      idx = ASN_ERROR_INVALID_VALUE_SZ;
      goto cleanup;
   }
   debug_printf( 1, "blob length %d (0x%02x) takes %d bytes",
      source_sz, source_sz, sz_of_sz );

   /* Grow the buffer if we need to. */
   retval = asn_ensure_buffer_sz( ph_buffer, idx, source_sz );
   if( 0 >= retval ) {
      error_printf( "unable to grow buffer" );
      idx = retval;
      goto cleanup;
   }

   /* Start writing to the buffer! */

   buffer = memory_lock( *ph_buffer );
   if( NULL == buffer ) {
      error_printf( "unable to lock buffer" );
      idx = ASN_ERROR_UNABLE_TO_LOCK;
      goto cleanup;
   }

   buffer[idx++] = ASN_BLOB;

   if( 127 < source_sz ) {
      /* 0x80 | size of size, followed by size. */
      buffer[idx++] = 0x80 | sz_of_sz;
      idx = asn_raw_write_int( buffer, idx, source_sz, sz_of_sz );
   } else {
      buffer[idx++] = source_sz;
   }

   for( i = idx ; idx + source_sz > i ; i++ ) {
      buffer[i] = 0;
   }
   if( 0 < source_sz ) {
      memory_copy_ptr( &(buffer[idx]), source, source_sz );
      idx += source_sz;
   }

cleanup:

   if( NULL != buffer ) {
      buffer = memory_unlock( *ph_buffer );
   }

   return idx;
}

int32_t asn_write_seq_start(
   MEMORY_HANDLE* ph_buffer, int32_t idx, int32_t* mark
) {
   /* Note space to insert the metadata in asn_write_seq_end() later. */
   *mark = idx;

   return idx;
}

int32_t asn_write_seq_end(
   MEMORY_HANDLE* ph_buffer, int32_t idx, int32_t* mark
) {
   int32_t idx_diff = idx - *mark,
      sz_of_sz = 0,
      meta_idx = 0,
      shift_offset = 0,
      retval = 0,
      i = 0;
   uint8_t* buffer = NULL;
   
   /* Get the size of the difference (body of the sequence). */
   sz_of_sz = asn_get_int_sz( idx_diff );
   if( 0 >= sz_of_sz ) {
      error_printf( "invalid value size" );
      idx = ASN_ERROR_INVALID_VALUE_SZ;
      goto cleanup;
   }
   debug_printf( 1, "sequence length %d (0x%02x) takes %d bytes",
      idx_diff, idx_diff, sz_of_sz );

   /* Grow the buffer if we need to. */
   retval = asn_ensure_buffer_sz( ph_buffer, idx, idx_diff );
   if( 0 >= retval ) {
      error_printf( "unable to grow buffer" );
      idx = retval;
      goto cleanup;
   }

   /* Start writing to the buffer! */

   buffer = memory_lock( *ph_buffer );
   if( NULL == buffer ) {
      error_printf( "unable to lock buffer" );
      idx = ASN_ERROR_UNABLE_TO_LOCK;
      goto cleanup;
   }

   shift_offset = sz_of_sz + 1 + (127 < idx_diff ? 1 : 0);
   debug_printf( 1, "shifting sequence contents by %d bytes...", shift_offset );
   for( i = idx + shift_offset ; (*mark) + shift_offset <= i ; i-- ) {
      buffer[i] = buffer[i - shift_offset];
   }

   meta_idx = *mark;
   buffer[meta_idx++] = ASN_SEQUENCE;
   meta_idx = asn_write_sz( buffer, meta_idx, idx_diff, sz_of_sz );

   idx += shift_offset;

cleanup:

   if( NULL != buffer ) {
      buffer = memory_unlock( *ph_buffer );
   }

   return idx;
}

int16_t asn_read_short( const uint8_t* asn_buffer, int32_t idx ) {
   int16_t n_out = 0;
   n_out |= (asn_buffer[idx++] << 8);
   debug_printf( 1, "(offset 0x%02x) value byte is 0x%02x", idx, n_out );
   debug_printf( 1, "(offset 0x%02x) value byte is 0x%02x", idx, asn_buffer[idx] );
   n_out |= asn_buffer[idx++];
   return n_out;
}

int16_t asn_read_int(
   uint8_t* int_buffer, uint8_t int_buffer_sz, uint8_t flags,
   const uint8_t* asn_buffer, int32_t idx
) {
   int16_t field_sz = 0;
   volatile int16_t* int16_buffer = (int16_t*)int_buffer;
   volatile uint16_t* uint16_buffer = (uint16_t*)int_buffer;
   volatile int8_t* int8_buffer = NULL; /* Assigned below. */
   volatile uint8_t* uint8_buffer = NULL; /* Assigned below. */
   uint8_t negative = 0,
      buffer_offset = 0; /* How deep into the buffer to write? */
   uint8_t type_buf = asn_buffer[idx];

   if( 0x02 != type_buf ) {
      if( 0x42 == type_buf ) {
         /* It's not invalid, just negative. */
         negative = 1;
         if( ASN_FLAG_SIGNED != (flags & ASN_FLAG_SIGNED) ) {
            error_printf( "negative number found in non-negative type" );
            field_sz = ASN_ERROR_INVALID_TYPE;
            goto cleanup;
         }
      } else {
         error_printf( "invalid integer type byte: 0x%02x (%d)",
            type_buf, idx );
         field_sz = ASN_ERROR_INVALID_TYPE;
         goto cleanup;
      }
   }

   /* Make sure the buffer is clean and properly sized. */

   if( 2 < int_buffer_sz ) {
      error_printf( "integer buffer too large" );
      field_sz = ASN_ERROR_INVALID_VALUE_SZ;
      goto cleanup;
   }

   /* Do the reading based on actual size of stored number. */

   if( asn_buffer[idx + 1] == 1 ) {
      /* Stored number is 1 byte. */

      /* Make sure pointer is pointing to right part of the buffer. */
      if( 2 == int_buffer_sz ) {
         /* Provided buffer is 2 bytes but stored value is 1 byte. */
         buffer_offset = 1;

         /* Zero out both bytes. */
         *uint16_buffer = 0;
         debug_printf( 1, "zero buffer to %d", *uint16_buffer );
      } else if( 1 == int_buffer_sz ) {
         buffer_offset = 0;
      } else {
         error_printf( "field mismatch error!" ); /* Shouldn't happen! */
         field_sz = ASN_ERROR_INVALID_VALUE_SZ;
         goto cleanup;
      }

      debug_printf( 1, "buffer offset is %d", buffer_offset );
   
      if( 0x42 == type_buf ) {
         /* Stored number is negative. */

         /* Setup the buffer and assign the value. */
         int8_buffer = (int8_t*)(int_buffer + buffer_offset);
         *int8_buffer = asn_buffer[idx + 2];

         debug_printf( 1, "assigning %d to become %d or %d or %d",
            asn_buffer[idx + 2], *int8_buffer, *int16_buffer, *uint16_buffer );
         
         /* Negative check based on type byte above. */
         if( negative ) {
            (*int8_buffer) *= -1;
            debug_printf( 1, "now it's %d or %d or %d",
               *int8_buffer, *int16_buffer, *uint16_buffer );
         }
         assert(
            (negative && 0 > *int8_buffer) ||
            (!negative && 0 <= *int8_buffer) );

         debug_printf( 1, "(offset 0x%02x) value byte is 0x%02x",
            idx + 2, *int8_buffer );
      } else {
         uint8_buffer = (uint8_t*)(int_buffer + buffer_offset);
         *uint8_buffer = asn_buffer[idx + 2];

         debug_printf( 1, "(offset 0x%02x) value byte is 0x%02x",
            idx + 2, *uint8_buffer );
      }
      field_sz += 1;

   } else if( asn_buffer[idx + 1] == 2 ) {
      if( ASN_FLAG_SIGNED == (flags & ASN_FLAG_SIGNED) ) {
         *int16_buffer = asn_read_short( asn_buffer, idx + 2 );
         if( negative ) {
            (*int16_buffer) *= -1;
         }
         assert(
            (negative && 0 > *int16_buffer) ||
            (!negative && 0 <= *int16_buffer) );
      } else {
         /* TODO: This causes store to misaligned address! */
         *uint16_buffer = asn_read_short( asn_buffer, idx + 2 );
         if( negative ) {
            error_printf( "negative number found in non-negative type" );
            /* TODO: Error value for field_sz? */
            goto cleanup;
         }
      }
      field_sz += 2;
   } else {
      /* TODO: Handle larger integers. */
      error_printf( "unable to process integer: size 0x%02x",
         asn_buffer[idx + 1] );
      idx = ASN_ERROR_INVALID_VALUE_SZ;
      goto cleanup;
   }

   field_sz += 2; /* type and size bytes */

cleanup:

   return field_sz;
}

int16_t asn_read_string(
   char* str_buffer, int16_t str_buffer_sz,
   const uint8_t* asn_buffer, int32_t idx
) {
   int16_t field_sz = 0;

   if( 0x16 != asn_buffer[idx] ) {
      error_printf( "invalid string type byte: 0x%02x (%d)",
         asn_buffer[idx], idx );
      field_sz = ASN_ERROR_INVALID_TYPE;
      goto cleanup;
   }
   
   /* TODO: Variable-length field size? */
   field_sz = asn_buffer[idx + 1];
   if( str_buffer_sz <= field_sz ) {
      error_printf( "string too long to fit in buffer: %d", field_sz );
      field_sz = ASN_ERROR_INVALID_VALUE_SZ;
      goto cleanup;
   }

   memory_copy_ptr( (MEMORY_PTR)str_buffer,
      (const MEMORY_PTR)&(asn_buffer[idx + 2]), field_sz );
   debug_printf( 1, "parsed string: %s (%d)", str_buffer, field_sz );

   field_sz += 2; /* type and length bytes */

cleanup:

   return field_sz;
}


int16_t asn_read_meta_ptr(
   const uint8_t* buffer, int32_t idx, uint8_t* type_out, int32_t* sz_out
) {
   uint8_t sz_of_sz = 0;
   int16_t read_sz = 0;
   int16_t idx_start = idx;

   *type_out = buffer[idx++];

   /* TODO: Handle invalid data. */

   if( 0x80 == (0x80 & buffer[idx]) ) {
      /* Strip high bit. */
      sz_of_sz = (buffer[idx++] & 0x7f);
      *sz_out = 0;
      read_sz = asn_raw_read_int( buffer, idx, sz_out, sz_of_sz );
      /* TODO: Error checking. */
      idx += read_sz;

   } else {
      /* Size of size is one byte long. */
      *sz_out = buffer[idx++];
   }

   debug_printf(
      1, "sequence type is %02x, %d bytes long", *type_out, *sz_out );

   return idx - idx_start;
}

