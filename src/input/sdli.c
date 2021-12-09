
#include "../unilayer.h"

uint8_t input_init() {
   return 1;
}

uint8_t input_poll() {
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
   } else if( SDL_QUIT == event.type ) {
      return INPUT_KEY_QUIT;
   }
   return 0;
}

