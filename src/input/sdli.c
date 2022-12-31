
#define INPUT_PLATFORM_C
#include "../unilayer.h"

SDL_Joystick* g_joystick = NULL;
int g_joystick_idx = -1;
int g_joystick_deadzone = 8000;

uint8_t input_platform_init() {
   /* TODO: Load selected joystick from config. */

   if( 1 > SDL_NumJoysticks() ) {
      error_printf( "no joysticks present!" );
   } else {
      debug_printf( 3, "found %d joysticks...", SDL_NumJoysticks() );
      g_joystick = SDL_JoystickOpen( 0 );
      g_joystick_idx = 0;
   }

   if( NULL == g_joystick ) {
      error_printf( "no joystick selected." );
   }

   return 1;
}

INPUT_VAL input_poll( int16_t* x, int16_t* y ) {
   SDL_Event event;
   int8_t eres = 0;
   uint8_t sym_out = 0;

   memory_zero_ptr( &event, sizeof( SDL_Event ) );

   SDL_PollEvent( &event );

   if( SDL_KEYDOWN == event.type ) {
      sym_out = event.key.keysym.sym;
      
      /* Flush key buffer to improve responsiveness. */
      while( (eres = SDL_PollEvent( &event )) );

      return sym_out;

   } else if(
      SDL_MOUSEBUTTONDOWN == event.type &&
      NULL != x && NULL != y
   ) {
      
      /* Always return scaled cordinates to compensate for zoom. */
      *x = event.button.x * SCREEN_W / g_screen_real_w;
      *y = event.button.y * SCREEN_H / g_screen_real_h;

      /* Flush key buffer to improve responsiveness. */
      while( (eres = SDL_PollEvent( &event )) );

      return INPUT_CLICK;

   } else if( SDL_JOYAXISMOTION == event.type ) {
      if( g_joystick_idx != event.jaxis.which ) {
         return 0;
      }

      /* Convert axis motions to key presses. */

      switch( event.jaxis.axis ) {
      case 0:
         /* X Axis */
         if( (-1 * g_joystick_deadzone) > event.jaxis.value ) {
            return g_input_key_left;
         } else if( g_joystick_deadzone < event.jaxis.value ) {
            return g_input_key_right;
         }
         break;

      case 1:
         /* Y Axis */
         if( (-1 * g_joystick_deadzone) > event.jaxis.value ) {
            return g_input_key_up;
         } else if( g_joystick_deadzone < event.jaxis.value ) {
            return g_input_key_down;
         }
         break;
      }

   } else if( SDL_JOYBUTTONDOWN == event.type ) {
      /* debug_printf( 3, "joystick button: %d", event.jbutton.button ); */
      if( g_joystick_idx != event.jbutton.which ) {
         return 0;
      }

      /* Convert button presses to key presses. */

      /* TODO: Remappable joystick input. */
      if( 0 == event.jbutton.button ) {
         return g_input_key_ok;
      } else if( 1 == event.jbutton.button ) {
         return g_input_key_menu;
      }

   } else if( SDL_QUIT == event.type ) {
      return g_input_key_quit;

#ifdef PLATFORM_SDL2
   } else if( SDL_WINDOWEVENT == event.type ) {
      switch( event.window.event ) {
      case SDL_WINDOWEVENT_RESIZED:
         graphics_on_resize( event.window.data1, event.window.data2 );
      }
#endif /* PLATFORM_SDL2 */
   }

   return 0;
}

void input_shutdown() {
   if( 0 <= g_joystick_idx && NULL != g_joystick ) {
      SDL_JoystickClose( g_joystick );
      g_joystick = NULL;
      g_joystick_idx = -1;
   }
}

