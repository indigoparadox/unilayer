
#include "../unilayer.h"

#include <ncurses.h>

uint8_t input_init() {
   return 1;
}

uint8_t input_poll( int16_t* x, int16_t* y ) {
   int in_char = 0;

   in_char = getch();

   debug_printf( 3, "c: %d", in_char );

   switch( in_char ) {
   case 'w':
      return INPUT_KEY_UP;
   case 's':
      return INPUT_KEY_DOWN;
   case 'z':
      return INPUT_KEY_OK;
   case 'q':
      return INPUT_KEY_QUIT;
   }

   return 0;
}

void input_shutdown() {
}

