
#define INPUT_PLATFORM_C
#include "../unilayer.h"

uint8_t input_platform_init() {
   return 1;
}

INPUT_VAL input_poll( int16_t* x, int16_t* y ) {
   int keys = 0;

   scanKeys();
   keys = keysDown();
		
   /* These are the arbitrary numbers chosen in ndsi.h. Here's where we map
    * the gamepad controls to them.
    */
   if( KEY_A == (keys & KEY_A) ) {
      return 6;
   } else if( KEY_UP == (keys & KEY_UP) ) {
      return 5;
   } else if( KEY_DOWN == (keys & KEY_DOWN) ) {
      return 4;
   } else if( KEY_LEFT == (keys & KEY_LEFT) ) {
      return 1;
   } else if( KEY_RIGHT == (keys & KEY_RIGHT) ) {
      return 2;
   } else if( KEY_START == (keys & KEY_START) ) {
      return 7;
   }

   /* TODO: Tap support. */

   return 0;
}

void input_shutdown() {

}

