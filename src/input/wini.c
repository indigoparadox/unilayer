
#include "../unilayer.h"

/* This is set by the window message processor in win16g.c. */
uint32_t g_last_key = 0;
uint16_t g_last_mouse = 0;
uint16_t g_last_mouse_x = 0;
uint16_t g_last_mouse_y = 0;

uint8_t input_platform_init() {
   return 1;
}

INPUT_VAL input_poll( int16_t* x, int16_t* y ) {
   INPUT_VAL last_key = g_last_key;
   uint16_t last_mouse = g_last_mouse;
   uint16_t last_mouse_x = g_last_mouse_x;
   uint16_t last_mouse_y = g_last_mouse_y;

   /* Reset these here so we can quickly return below. */
   g_last_key = 0;
   g_last_mouse = 0;
   g_last_mouse_x = 0;
   g_last_mouse_y = 0;

   if( last_key ) {
      return last_key;

   } else if( last_mouse ) {
      if( MK_LBUTTON == (MK_LBUTTON & last_mouse) ) {
         *x = last_mouse_x * SCREEN_W / g_screen_real_w;
         *y = last_mouse_y * SCREEN_H / g_screen_real_h;
         return INPUT_CLICK;
      }
   }

   return 0;
}

void input_shutdown() {

}

