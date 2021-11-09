
#define ANIMATE_C
#include "unilayer.h"

void animate_draw_RECTANGLE( struct ANIMATION* a ) {

}

void animate_draw_CIRCLE( struct ANIMATION* a ) {

}

void animate_draw_FIRE( struct ANIMATION* a ) {
   int x = 0,
      y = 0,
      idx = 0;
   uint8_t* data = NULL;

   data = memory_lock( a->data );
   if( NULL == data ) {
      error_printf( "could not lock animation data!" );
      goto cleanup;
   }

   if( !(a->flags & ANIMATE_FLAG_INIT) ) {
      a->flags |= ANIMATE_FLAG_INIT;
      for( x = 0 ; a->w > x ; x++ ) {
         idx = ((a->h - 1) * a->w) + x;
         data[idx] = 100;
      }
   }
   
   for( y = 0 ; a->h - 1 > y ; y++ ) {
      /* debug_printf( 3, "%d, %d: %d", 0, y, data[(y * a->w)] ); */
      for( x = 0 ; a->w > x ; x++ ) {
         idx = (y * a->w) + x;
         if( 3 >= data[idx + a->w] ) {
            data[idx] = 0;
         } else {
            data[idx] = data[idx + a->w] - graphics_get_random( 2, 3 );
         }

         if( 90 < data[idx] ) {
            graphics_draw_px( a->x + x, a->y + y, GRAPHICS_COLOR_WHITE );
         } else if( 60 < data[idx] ) {
            graphics_draw_px( a->x + x, a->y + y, GRAPHICS_COLOR_CYAN );
         } else if( 30 < data[idx] ) {
            graphics_draw_px( a->x + x, a->y + y, GRAPHICS_COLOR_MAGENTA );
         }
      }
   }

cleanup:

   if( NULL != data ) {
      data = memory_unlock( a->data );
   }

   return;
}

int8_t animate_create_animation(
   uint8_t type, int16_t x, int16_t y, int16_t w, int16_t h
) {
   int8_t i = 0,
      idx_out = -1;

   for( i = 0 ; ANIMATE_ANIMATIONS_MAX > i ; i++ ) {
      if( !(g_animations[i].flags & ANIMATE_FLAG_ACTIVE) ) {
         idx_out = i;
         break;
      }
   }

   if( 0 > idx_out ) {
      error_printf( "animation table is full!" );
      goto cleanup;
   }

   g_animations[i].data = memory_alloc( w, h );
   if( (MEMORY_HANDLE)NULL == g_animations[i].data ) {
      error_printf( "unable to allocate animation scratch!" );
      goto cleanup;
   }

   g_animations[i].flags = ANIMATE_FLAG_ACTIVE;
   g_animations[i].x = x;
   g_animations[i].y = y;
   g_animations[i].w = w;
   g_animations[i].h = h;
   g_animations[i].type = type;

cleanup:
   return idx_out;
}

void animation_frame() {
   int i = 0;

   for( i = 0 ; ANIMATE_ANIMATIONS_MAX > i ; i++ ) {
      if( !(g_animations[i].flags & ANIMATE_FLAG_ACTIVE) ) {
         continue;
      }
      gc_animate_draw[g_animations[i].type]( &(g_animations[i]) );
   }

}

