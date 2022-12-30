
#include "../unilayer.h"

uint8_t input_init() {
   return 1;
}

uint8_t input_poll( int16_t* x, int16_t* y ) {
   int keys = 0;

   scanKeys();
   keys = keysDown();
		
   if( KEY_A == (keys & KEY_A) ) {
      return INPUT_KEY_OK;
   } else if( KEY_UP == (keys & KEY_UP) ) {
      return INPUT_KEY_UP;
   } else if( KEY_DOWN == (keys & KEY_DOWN) ) {
      return INPUT_KEY_DOWN;
   } else if( KEY_LEFT == (keys & KEY_LEFT) ) {
      return INPUT_KEY_LEFT;
   } else if( KEY_RIGHT == (keys & KEY_RIGHT) ) {
      return INPUT_KEY_RIGHT;
   }

   return 0;
}

void input_shutdown() {

}

