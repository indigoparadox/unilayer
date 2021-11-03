
#include "../unilayer.h"

int input_init() {
   return 1;
}

int input_poll() {
   SDL_Event event;
   int eres = 0,
      sym_out = 0;

   memory_zero_ptr( &event, sizeof( SDL_Event ) );

   SDL_PollEvent( &event );

   if( SDL_KEYDOWN == event.type ) {
      sym_out = event.key.keysym.sym;
      
      /* Flush key buffer to improve responsiveness. */
      while( eres = SDL_PollEvent( &event ) );

      return sym_out;
   } else if( SDL_QUIT == event.type ) {
      return INPUT_KEY_QUIT;
   }
   return 0;
}

