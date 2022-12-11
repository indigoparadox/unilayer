
#include "../unilayer.h"

SDL_Joystick* g_joystick = NULL;
int g_joystick_idx = -1;
int g_joystick_deadzone = 8000;

uint8_t input_init() {
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

uint8_t input_poll( int16_t* x, int16_t* y ) {
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
      
      *x = event.button.x;  
      *y = event.button.y;  

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
            return INPUT_KEY_LEFT;
         } else if( g_joystick_deadzone < event.jaxis.value ) {
            return INPUT_KEY_RIGHT;
         }
         break;

      case 1:
         /* Y Axis */
         if( (-1 * g_joystick_deadzone) > event.jaxis.value ) {
            return INPUT_KEY_UP;
         } else if( g_joystick_deadzone < event.jaxis.value ) {
            return INPUT_KEY_DOWN;
         }
         break;
      }

   } else if( SDL_JOYBUTTONDOWN == event.type ) {
      /* debug_printf( 3, "joystick button: %d", event.jbutton.button ); */
      if( g_joystick_idx != event.jbutton.which ) {
         return 0;
      }

      /* Convert button presses to key presses. */

      if( 0 == event.jbutton.button ) {
         return INPUT_KEY_OK;
      } else if( 1 == event.jbutton.button ) {
         return INPUT_KEY_QUIT;
      }

   } else if( SDL_QUIT == event.type ) {
      return INPUT_KEY_QUIT;
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

