
#include "../unilayer.h"

#include <dos.h>
#include <conio.h>

uint8_t input_platform_init() {
   union REGS regs;

   regs.x.ax = 0;

   int86( 0x33, &regs, &regs );

   if( 0xffff == regs.x.ax ) {
      debug_printf( 3, "mouse support enabled" );
   }

   return 1;
}

/* = SCREEN DRAWING = */

INPUT_VAL input_poll( int16_t* x, int16_t* y ) {
   uint8_t ch_out = 0;
   union REGS regs;
   static int16_t last_x = -1;
   static int16_t last_y = -1;

   /* Poll the mouse. */
   regs.x.ax = 3;
   int86( 0x33, &regs, &regs );

   if( kbhit() ) {
      /* Poll the keyboard. */
      ch_out = getch();
      while( kbhit() ) {
         getch();
      }
      return ch_out;
   } else if( 1 == regs.x.bx /* Left button clicked. */ ) {
	   if(
         regs.x.cx != last_x &&
         regs.x.dx != last_y
      ) { 
	      *x = regs.x.cx;
   	   *y = regs.x.dx;

         /* Prevent repeated clicks. */
         last_x = *x;
         last_y = *y;

         return INPUT_CLICK;
      }
   } else {
      last_x = -1;
      last_y = -1;
   }

   return 0;
}

void input_shutdown() {

}

