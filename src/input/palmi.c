
#include "../unilayer.h"

#include <PalmOS.h>

static UInt32 g_next_input = 0;

uint8_t input_init() {
   return 1;
}

uint8_t input_poll( int16_t* x, int16_t* y ) {
   EventType event;
   UInt16 allow_input = 0;

   /* Doze until an event arrives. */
   EvtGetEvent( &event, 100 );

   if( 0 == g_next_input || TimGetTicks() > g_next_input ) {
      g_next_input = TimGetTicks() + (SysTicksPerSecond());
      allow_input = 0xffff;
   }

   switch( event.eType ) {
   case keyDownEvent:
      if( event.data.keyDown.chr == hard2Chr ) {
         return allow_input & INPUT_KEY_LEFT;

      } else if( event.data.keyDown.chr == pageDownChr ) {
         return allow_input & INPUT_KEY_DOWN;

      } else if( event.data.keyDown.chr == pageUpChr ) {
         return allow_input & INPUT_KEY_UP;

      } else if( event.data.keyDown.chr == hard3Chr ) {
         return allow_input & INPUT_KEY_RIGHT;

      } else if( event.data.keyDown.chr == hard4Chr ) {
         return allow_input & INPUT_KEY_OK;

      } else {
         /* System gets a chance to handle the event. */
         SysHandleEvent( &event );
      }
      break;

   case appStopEvent:
      /* Suspend the program. */
      return INPUT_KEY_QUIT;

   case menuOpenEvent:
      /* TODO: Broken. */
      return INPUT_KEY_MENU;

   case penDownEvent:
      *x = event.screenX;
      *y = event.screenY;
      return INPUT_CLICK;

   default:
      /* System gets a chance to handle the event. */
      SysHandleEvent( &event );
      break;
   }

   return 0;
}

void input_shutdown() {

}

