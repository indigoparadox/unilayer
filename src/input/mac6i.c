
#include "../unilayer.h"

uint8_t input_platform_init() {
   FlushEvents( everyEvent, 0 );
   return 1;
}

INPUT_VAL input_poll( int16_t* x, int16_t* y ) {
   EventRecord event;
   INPUT_VAL key = 0;

   /* SystemTask(); */

   if( GetNextEvent( everyEvent, &event ) ) {
      SystemTask();
      if( keyDown == event.what ) {
         key = (uint8_t)((event.message & keyCodeMask) >> 8);
         switch( key ) {
         case 0x0c: /* Q */
            return INPUT_KEY_QUIT;

         case 0x06: /* Z */
            return INPUT_KEY_OK;

         default:
            break;
         }
      }
   }

   return 0;
}

void input_shutdown() {

}

