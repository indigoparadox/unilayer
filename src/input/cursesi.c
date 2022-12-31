
#define INPUT_PLATFORM_C
#include "../unilayer.h"

#include <ncurses.h>

uint8_t input_platform_init() {
   nodelay( stdscr, TRUE );
   noecho();
   keypad( stdscr, TRUE );
   mousemask( BUTTON1_CLICKED, NULL );
   return 1;
}

INPUT_VAL input_poll( int16_t* x, int16_t* y ) {
   INPUT_VAL in_char = 0;
   MEVENT event;

   in_char = getch();

   if( KEY_MOUSE == in_char ) {
      if( OK == getmouse( &event ) ) {
         debug_printf( 3, "click" );
         *x = event.x;
         *y = event.y;
      }
      return INPUT_CLICK;
   }

   return in_char;
}

void input_shutdown() {
}

