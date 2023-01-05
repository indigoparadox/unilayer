
#include <check.h>

#include "../src/unilayer.h"

#define TEST_STRUCT_SZ 35

uint8_t gc_test_struct[TEST_STRUCT_SZ] = {
   0x30, 0x21, 0x16, 0x1b, 0x61, 0x73, 0x73, 0x65, 0x74, 0x73,
   0x2f, 0x31, 0x36, 0x78, 0x31, 0x36, 0x78, 0x31, 0x36, 0x2f,
   0x74, 0x5f, 0x67, 0x72, 0x61, 0x73, 0x73, 0x2e, 0x62, 0x6d,
   0x70, 0x02, 0x02, 0x00, 0x80 };

#define TEST_NEG_16_SZ 3

int16_t gc_test_neg_16_val = -128;
uint8_t gc_test_neg_16[TEST_NEG_16_SZ] = { 0x42, 0x01, 0x80 };

#define TEST_NEG_16_1_SZ 3

int16_t gc_test_neg_16_1_val = -1;
uint8_t gc_test_neg_16_1[TEST_NEG_16_SZ] = { 0x42, 0x01, 0x01 };

#define TEST_POS_16_128_SZ 4

int16_t gc_test_pos_16_128_val = 128;
uint8_t gc_test_pos_16_128[TEST_POS_16_128_SZ] = { 0x02, 0x01, 0x00, 0x80 };

START_TEST( test_asn_write_neg_int_16 ) {
   int16_t written = 0;
   uint8_t* buffer = NULL;
   MEMORY_HANDLE h_buffer = NULL;

   h_buffer = memory_alloc( TEST_NEG_16_SZ, 1 );
   written = asn_write_int( &h_buffer, 0, gc_test_neg_16_val );
   buffer = memory_lock( h_buffer );
   ck_assert_int_eq( gc_test_neg_16[_i], buffer[_i] );
   buffer = memory_unlock( h_buffer );
}
END_TEST

/* Write -1 to a 16-bit signed int. */
START_TEST( test_asn_write_neg_int_16_1 ) {
   int16_t written = 0;
   uint8_t* buffer = NULL;
   MEMORY_HANDLE h_buffer = NULL;

   h_buffer = memory_alloc( TEST_NEG_16_1_SZ, 1 );
   written = asn_write_int( &h_buffer, 0, gc_test_neg_16_1_val );
   buffer = memory_lock( h_buffer );
   ck_assert_int_eq( gc_test_neg_16_1[_i], buffer[_i] );
   buffer = memory_unlock( h_buffer );
}
END_TEST

START_TEST( test_asn_read_neg_int_16 ) {
   int16_t int_sz = 0;
   int16_t int_val = 0;

   int_sz = asn_read_int(
      (uint8_t*)(&int_val), 2, ASN_FLAG_SIGNED, gc_test_neg_16, 0 );

   ck_assert_int_eq( int_val, gc_test_neg_16_val );
   ck_assert_int_eq( int_sz, 3 ); /* type byte, sz byte, 1x val bytes */
}
END_TEST

START_TEST( test_asn_read_int_16 ) {
   int int_sz = 0;
   uint16_t int_val = 0;

   int_sz = asn_read_int(
      (uint8_t*)(&int_val), 2, 0, gc_test_struct, 31 );

   ck_assert_int_eq( int_val, 128 );
   ck_assert_int_eq( int_sz, 4 ); /* type byte, sz byte, 2x val bytes */
}
END_TEST

START_TEST( test_asn_read_str ) {
   int str_sz = 0;
   char buffer[1025] = { 0 };

   str_sz = asn_read_string( buffer, 1024, gc_test_struct, 2 );

   ck_assert_int_eq( str_sz, 29 );
   ck_assert_str_eq( "assets/16x16x16/t_grass.bmp", buffer ); 
}
END_TEST

Suite* asn_suite( void ) {
   Suite* s;
   TCase* tc_read;

   s = suite_create( "asn" );

   /* Core test case */
   tc_read = tcase_create( "Read" );

   tcase_add_loop_test( tc_read, test_asn_write_neg_int_16, 0, TEST_NEG_16_SZ );
   tcase_add_loop_test( tc_read, test_asn_write_neg_int_16_1, 0, TEST_NEG_16_1_SZ );
   tcase_add_test( tc_read, test_asn_read_neg_int_16 );
   tcase_add_test( tc_read, test_asn_read_int_16 );
   tcase_add_test( tc_read, test_asn_read_str );

   suite_add_tcase( s, tc_read );

   return s;
}

