
#define ANIMATE_C
#include "unilayer.h"

#if defined( PLATFORM_PALM ) || defined( PLATFORM_MAC6 )

void animate_draw_RECTANGLE( struct ANIMATION* a ) {
}

void animate_draw_CIRCLE( struct ANIMATION* a ) {
}

void animate_draw_FIRE( struct ANIMATION* a ) {
}

void animate_draw_SNOW( struct ANIMATION* a ) {
}

void animate_draw_FRAMES( struct ANIMATION* a ) {
}

int8_t animate_create_animation(
   uint8_t type, int16_t x, int16_t y, int16_t w, int16_t h
) {
   return 0;
}

void animation_frame() {
}

void animation_stop( int8_t idx ) {
}

void animation_stop_all() {
}

#else

void animate_draw_RECTANGLE( struct ANIMATION* a ) {
   /* TODO */
}

void animate_draw_CIRCLE( struct ANIMATION* a ) {
   /* TODO */
}

void animate_draw_FIRE( struct ANIMATION* a ) {
   int x = 0,
      y = 0,
      idx = 0,
      next_idx = 0;
   uint8_t* data = NULL;

   if( !(a->flags & ANIMATE_FLAG_INIT) ) {
      a->data = memory_alloc( a->w, a->h );
      if( (MEMORY_HANDLE)NULL == a->data ) {
         error_printf( "unable to allocate animation scratch!" );
         goto cleanup;
      }

      data = memory_lock( a->data );

      for( x = 0 ; a->w > x ; x++ ) {
         idx = ((a->h - 1) * a->w) + x;
         data[idx] = 100;
      }

      data = memory_unlock( a->data );

      a->flags |= ANIMATE_FLAG_INIT;
   }
   
   data = memory_lock( a->data );
   if( NULL == data ) {
      error_printf( "could not lock animation data!" );
      goto cleanup;
   }

   for( y = 0 ; a->h - 1 > y ; y++ ) {
      /* debug_printf( 3, "%d, %d: %d", 0, y, data[(y * a->w)] ); */
      for( x = 0 ; a->w > x ; x++ ) {
         idx = (y * a->w) + x;

         /* Make sure we don't overflow the buffer. */
         if( 2 < x && a->w - 2 > x ) {
            next_idx = idx + a->w + graphics_get_random( -1, 3 );
         } else {
            next_idx = idx + a->w;
         }

         /* Make sure integers don't rollover. */
         if( 3 >= data[next_idx] ) {
            data[idx] = 0;
         } else {
            /* Propagate heat. */
            data[idx] = data[next_idx] - graphics_get_random( 2, 3 );
         }

#ifdef DEPTH_VGA
         if( 90 < data[idx] ) {
            graphics_draw_px( a->x + x, a->y + y, GRAPHICS_COLOR_WHITE );
         } else if( 60 < data[idx] ) {
            graphics_draw_px( a->x + x, a->y + y, GRAPHICS_COLOR_YELLOW );
         } else if( 30 < data[idx] ) {
            graphics_draw_px( a->x + x, a->y + y, GRAPHICS_COLOR_RED );
         }
#else
         if( 90 < data[idx] ) {
            graphics_draw_px( a->x + x, a->y + y, GRAPHICS_COLOR_WHITE );
         } else if( 60 < data[idx] ) {
            graphics_draw_px( a->x + x, a->y + y, GRAPHICS_COLOR_CYAN );
         } else if( 30 < data[idx] ) {
            graphics_draw_px( a->x + x, a->y + y, GRAPHICS_COLOR_MAGENTA );
         }
#endif /* DEPTH_VGA */
      }
   }

cleanup:

   if( NULL != data ) {
      data = memory_unlock( a->data );
   }
}

void animate_draw_SNOW( struct ANIMATION* a ) {
   uint8_t* data = NULL;

   if( !(a->flags & ANIMATE_FLAG_INIT) ) {
      a->flags |= ANIMATE_FLAG_INIT;
   }
 
cleanup:

   if( NULL != data ) {
      data = memory_unlock( a->data );
   }
}

void animate_draw_FRAMES( struct ANIMATION* a ) {
   /* TODO */
}

int8_t animate_create_animation(
   uint8_t type, int16_t x, int16_t y, int16_t w, int16_t h
) {
   int8_t i = 0,
      idx_out = ANIMATE_ERROR;

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

   g_animations[i].flags = ANIMATE_FLAG_ACTIVE;
   g_animations[i].x = x;
   g_animations[i].y = y;
   g_animations[i].w = w;
   g_animations[i].h = h;
   g_animations[i].type = type;
   g_animations[i].data = NULL;

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

void animation_stop( int8_t idx ) {
   if( (MEMORY_HANDLE)NULL != g_animations[idx].data ) {
      memory_free( g_animations[idx].data );
      g_animations[idx].data = (MEMORY_HANDLE)NULL;
   }

   memory_zero_ptr( &(g_animations[idx]), sizeof( struct ANIMATION ) );
}

void animation_stop_all() {
   int8_t i = 0;

   for( i = 0 ; ANIMATE_ANIMATIONS_MAX > i ; i++ ) {
      animation_stop( i );
   }
}

#endif /* PLATFORM_PALM */

