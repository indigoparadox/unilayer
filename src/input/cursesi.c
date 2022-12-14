
#include "../unilayer.h"

#include <ncurses.h>

uint8_t input_init() {
   nodelay( stdscr, TRUE );
   noecho();
   keypad( stdscr, TRUE );
   mousemask( BUTTON1_CLICKED, NULL );
   return 1;
}

uint8_t input_poll( int16_t* x, int16_t* y ) {
   int in_char = 0;
   MEVENT event;

   in_char = getch();

   switch( in_char ) {
   case 'w':
      return INPUT_KEY_UP;
   case 'a':
      return INPUT_KEY_LEFT;
   case 's':
      return INPUT_KEY_DOWN;
   case 'd':
      return INPUT_KEY_RIGHT;
   case 'z':
      return INPUT_KEY_OK;
   case 'q':
      return INPUT_KEY_QUIT;
   case KEY_MOUSE:
      if( OK == getmouse( &event ) ) {
         debug_printf( 3, "click" );
         *x = event.x;
         *y = event.y;
      }
      return INPUT_CLICK;
   }

   return 0;
}

void input_shutdown() {
}

