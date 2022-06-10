
#include <check.h>

#include "../src/unilayer.h"

#define TEST_STRUCT_SZ 35

uint8_t gc_test_struct[TEST_STRUCT_SZ] = {
   0x30, 0x21, 0x16, 0x1b, 0x61, 0x73, 0x73, 0x65, 0x74, 0x73,
   0x2f, 0x31, 0x36, 0x78, 0x31, 0x36, 0x78, 0x31, 0x36, 0x2f,
   0x74, 0x5f, 0x67, 0x72, 0x61, 0x73, 0x73, 0x2e, 0x62, 0x6d,
   0x70, 0x02, 0x02, 0x00, 0x80 };

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

   tcase_add_test( tc_read, test_asn_read_int_16 );
   tcase_add_test( tc_read, test_asn_read_str );

   suite_add_tcase( s, tc_read );

   return s;
}

