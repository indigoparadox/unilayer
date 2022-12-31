
#include "../unilayer.h"

#include <PalmOS.h>

static UInt32 g_next_input = 0;

uint8_t input_platform_init() {
   return 1;
}

INPUT_VAL input_poll( int16_t* x, int16_t* y ) {
   EventType event;
   INPUT_VAL allow_input = 0;

   /* Doze until an event arrives. */
   EvtGetEvent( &event, 100 );

   if( 0 == g_next_input || TimGetTicks() > g_next_input ) {
      g_next_input = TimGetTicks() + (SysTicksPerSecond());
      allow_input |= g_input_key_left | g_input_key_right | g_input_key_up | g_input_key_down | g_input_key_quit | g_input_key_ok | g_input_key_menu;
   }

   switch( event.eType ) {
   case keyDownEvent:
      if( allow_input & event.data.keyDown.chr == hard2Chr ) {
         return allow_input & event.data.keyDown.chr;

      } else {
         /* System gets a chance to handle the event. */
         SysHandleEvent( &event );
      }
      break;

   case appStopEvent:
      /* Suspend the program. */
      return g_input_key_quit;

   case menuOpenEvent:
      /* TODO: Broken. */
      return g_input_key_menu;

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

