
#include "../unilayer.h"

extern Display* g_display;
extern Window g_window;

uint8_t input_init() {
   XSelectInput( g_display, g_window, ExposureMask | KeyPressMask );
   return 1;
}

uint8_t input_poll() {
   XEvent event;
   uint16_t mask = 0;

   mask = ExposureMask | KeyPressMask | KeyReleaseMask;

   /* XNextEvent( g_display, &event ); */
   if( (
      XCheckWindowEvent( g_display, g_window, mask, &event ) ||
      XCheckTypedWindowEvent( g_display, g_window, KeyPress, &event )
   ) && KeyPress == event.type ) {
      /* printf( "%lu\n", event.xkey.keycode ); */
      return event.xkey.keycode;
   }
   return 0;
}

