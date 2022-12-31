
#define INPUT_C
#include "unilayer.h"

uint8_t input_init() {
   uint8_t retval = 0;

   retval = input_platform_init();

   /* TODO: Load mappings from config. */
#ifdef PLATFORM_PALM
   input_assign_key( &g_input_key_up, "INPUT_KEY_PGUP", 11 );
   input_assign_key( &g_input_key_down, "INPUT_KEY_PGDOWN", 11 );
   input_assign_key( &g_input_key_left, "INPUT_KEY_ADDRESS", 11 );
   input_assign_key( &g_input_key_right, "INPUT_KEY_TODO", 11 );
   input_assign_key( &g_input_key_quit, "INPUT_KEY_CALENDAR", 16 );
   input_assign_key( &g_input_key_ok, "INPUT_KEY_MEMO", 11 );
   input_assign_key( &g_input_key_menu, "INPUT_KEY_MENU", 11 );
#else
   input_assign_key( &g_input_key_up, "INPUT_KEY_W", 11 );
   input_assign_key( &g_input_key_down, "INPUT_KEY_S", 11 );
   input_assign_key( &g_input_key_left, "INPUT_KEY_A", 11 );
   input_assign_key( &g_input_key_right, "INPUT_KEY_D", 11 );
   input_assign_key( &g_input_key_quit, "INPUT_KEY_ESCAPE", 16 );
   input_assign_key( &g_input_key_ok, "INPUT_KEY_Z", 11 );
   input_assign_key( &g_input_key_menu, "INPUT_KEY_E", 11 );
#endif /* PLATFORM_PALM */

   return retval;
}

uint8_t input_assign_key( INPUT_VAL* key, const char* name, uint8_t name_sz ) {
   uint8_t retval = 0;
   int i = 0;

   /* Search the list of const names for the given key name. */
   while( NULL != gc_input_keys_names[i] ) {
      if(
         0 == memory_strncmp_ptr( name, gc_input_keys_names[i], name_sz + 1 )
      ) {
         /* Found it! */
         *key = gc_input_keys_vals[i];
         debug_printf( 3, "assigned key %s (%d)", gc_input_keys_names[i],
            gc_input_keys_vals[i] );
         retval = 1;
         break;
      }

      /* Keep looking. */
      i++;
   }

   return retval;
}

