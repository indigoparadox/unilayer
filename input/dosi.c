
#include "../unilayer.h"

#include <dos.h>
#include <conio.h>

int input_init() {
   return 1;
}

int input_poll() {
   int ch_out = 0;
   if( kbhit() ) {
      ch_out = getch();
      while( kbhit() ) {
         getch();
      }
      return ch_out;
   }
   return 0;
}

