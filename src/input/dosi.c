
#include "../unilayer.h"

#include <dos.h>
#include <conio.h>

uint8_t input_init() {
   return 1;
}

uint8_t input_poll() {
   uint8_t ch_out = 0;
   if( kbhit() ) {
      ch_out = getch();
      while( kbhit() ) {
         getch();
      }
      return ch_out;
   }
   return 0;
}

void input_shutdown() {

}

