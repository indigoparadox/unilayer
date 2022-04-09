
#define ANIMATE_C
#include "unilayer.h"

void animate_draw_RECTANGLE( struct ANIMATION* a ) {
   /* TODO */
}

void animate_draw_CIRCLE( struct ANIMATION* a ) {
   /* TODO */
}

void animate_draw_FIRE( struct ANIMATION* a ) {
   int8_t x = 0,
      next_x = 0,
      y = 0;
   uint16_t
      idx = 0,
      next_idx = 0;

   if( !(a->flags & ANIMATE_FLAG_INIT) ) {
      /* Setup initial "heat line" from which fire is drawn. */
      for( x = 0 ; ANIMATE_TILE_W > x ; x++ ) {
         idx = ((ANIMATE_TILE_H - 1) * ANIMATE_TILE_W) + x;
         a->tile[idx] = graphics_get_random( 70, 101 );
      }

      a->flags |= ANIMATE_FLAG_INIT;
   }

   for( y = 0 ; ANIMATE_TILE_H - 1 > y ; y++ ) {
      /* debug_printf( 3, "%d, %d: %d", 0, y, data[(y * a->w)] ); */
      for( x = 0 ; ANIMATE_TILE_W > x ; x++ ) {
         idx = (y * ANIMATE_TILE_W) + x;

         /* Make sure we don't overflow the buffer. */
         next_x = x + graphics_get_random( -1, 3 );
         if( 0 > next_x ) {
            next_x = ANIMATE_TILE_W - 1;
         } else if( ANIMATE_TILE_W <= next_x ) {
            next_x = 0;
         }

         next_idx = ((y + 1) * ANIMATE_TILE_W) + next_x;

         /* Make sure integers don't rollover. */
         if( ANIMATE_FIRE_COOLING_MAX + 3 >= a->tile[next_idx] ) {
            if( -1 == a->tile[idx] ) {
               /* Presume hiding was done. */
               a->tile[idx] = 0;

            } else if( 0 != a->tile[idx] ) {
               /* Hide previous pixel. */
               a->tile[idx] = -1;
            }
         } else {
            /* Propagate heat. */
            a->tile[idx] = a->tile[next_idx] - graphics_get_random(
               ANIMATE_FIRE_COOLING_MIN, ANIMATE_FIRE_COOLING_MAX );
         }
      }
   }

   /* Only tesselate the bottom row. */
   animate_tesselate( a, a->h - ANIMATE_TILE_H );

}

void animate_draw_SNOW( struct ANIMATION* a ) {
   int16_t
      x = 0,
      y = 0,
      idx = 0,
      new_idx = 0;

   if( !(a->flags & ANIMATE_FLAG_INIT) ) {
      /* Create initial snowflakes along the left side of the tile. */
      for( y = 0 ; ANIMATE_TILE_H > y ; y += 4 ) {
         idx = (y * ANIMATE_TILE_W);
         a->tile[idx] = 1;
      }

      a->flags |= ANIMATE_FLAG_INIT;
   }
 
   for( y = ANIMATE_TILE_H - 1 ; 0 <= y ; y-- ) {
      for( x = ANIMATE_TILE_W - 1 ; 0 <= x ; x-- ) {
         idx = (y * ANIMATE_TILE_W) + x;

         if( -1 == a->tile[idx] ) {
            /* Presume hiding was done. */
            a->tile[idx] = 0;

         } else if( 0 < a->tile[idx] ) {
            /* Hide the snowflake's previous position. */
            a->tile[idx] = -1;

            do {
               /* Move the snowflake down and maybe to the right. */
               new_idx = idx + ANIMATE_TILE_W + graphics_get_random( 0, 3 );

                  /* Wrap the snowflake if it moves off-tile. */
               if( new_idx >= ANIMATE_TILE_SZ ) {
                  new_idx -= ANIMATE_TILE_SZ;
               }

            /* Don't let snowflakes merge over time. */
            } while( a->tile[new_idx] );
            
            /* Show the snowflake at its new position. */
            a->tile[new_idx] = 1;
         }
      }
   }

   animate_tesselate( a, 0 );
}

void animate_draw_CLOUDS( struct ANIMATION* a ) {
   int8_t row_start_idx = 0,
      row_start_last = 0,
      row_col_end_buffer = 0, /* Last pixel wrapped off row end. */
      row_col_offset = 0,
      prev_row_col_offset = 0;
   int16_t
      x = 0,
      y = 0,
      idx = 0,
      prev_idx = 0;

   if( !(a->flags & ANIMATE_FLAG_INIT) ) {
      /* Create initial cloud lines along the left side of the tile. */
      for( y = 0 ; ANIMATE_TILE_H > y ; y++ ) {
         
         /* Get new non-repeating offset for each row. */
         do {
            row_start_idx = graphics_get_random( 0, ANIMATE_TILE_W / 4 );
         } while( row_start_idx == row_start_last );

         /* Draw the row's initial state. */
         for( x = 0 ; ANIMATE_TILE_W > x ; x++ ) {
            if(
               ANIMATE_FLAG_CLOUDS_ROTATE !=
               (ANIMATE_FLAG_CLOUDS_ROTATE & a->flags)
            ) {
               idx = (y * ANIMATE_TILE_W) + x;
            } else {
               idx = (x * ANIMATE_TILE_W) + y;
            }
            if(
               x > row_start_idx && x < row_start_idx + ANIMATE_CLOUD_WISP_LEN
            ) {
               a->tile[idx] = 100;
            } else {
               a->tile[idx] = 33;
            }
         }

         /* Save the row length to compare on next loop. */
         row_start_last = row_start_idx;
      }

      a->flags |= ANIMATE_FLAG_INIT;
   }

   /* TODO: Add a parallax effect, maybe? */

#if 0
   if(
      ANIMATE_FLAG_CLOUDS_ROTATE ==
      (ANIMATE_FLAG_CLOUDS_ROTATE & a->flags)
   ) {
      for( y = ANIMATE_TILE_H - 1 ; 0 <= y ; y-- ) {

         * Do we advance this wisp on this iteration? Not always. */
         row_col_offset = graphics_get_random( 0, 30 );
         if( 5 < row_col_offset ) {
            continue;
         }

         /* Iterate each row. */
         row_col_idx = (y * ANIMATE_TILE_W);
         row_col_end_buffer = a->tile[row_col_idx + (ANIMATE_TILE_W - 1)];

         for( x = ANIMATE_TILE_W - 1 ; 0 <= x ; x-- ) {
            idx = row_col_idx + x;

            if( 0 == x ) {
               /* Wrap-around. */
               a->tile[row_col_idx] = row_col_end_buffer;

            } else {
               /* Cloud advance. */
               a->tile[idx] = a->tile[idx - 1];
            }
         }
      }

   } else {

      for( x = ANIMATE_TILE_W - 1 ; 0 <= x ; x-- ) {

         /* Do we advance this wisp on this iteration? Not always. */
         row_col_offset = graphics_get_random( 0, 30 );
         if( 5 < row_col_offset ) {
            continue;
         }

         /* Iterate each row. */
         for( y = ANIMATE_TILE_H - 1 ; 0 <= y ; y-- ) {
            idx = (y * ANIMATE_TILE_W) + x;
            row_col_end_buffer = a->tile[((ANIMATE_TILE_H - 1) * y) + x];
            prev_idx = idx - ANIMATE_TILE_W;

            if( 0 == y ) {
               /* Wrap-around. */
               a->tile[idx] = row_col_end_buffer;

            } else {
               /* Cloud advance. */
               a->tile[idx] = a->tile[prev_idx];
            }
         }
      }

   }
#endif

   for( y = ANIMATE_TILE_H - 1 ; 0 <= y ; y-- ) {

      /* TODO: Adapt this for rotated orientation... somewhat more complicated.
       */
      if(
         ANIMATE_FLAG_CLOUDS_ROTATE !=
         (ANIMATE_FLAG_CLOUDS_ROTATE & a->flags)
      ) {
         /* Do we advance this wisp on this iteration? Not always. */
         prev_row_col_offset = row_col_offset;
         row_col_offset = graphics_get_random( 0, 70 );
         if( 45 > row_col_offset || 45 > prev_row_col_offset ) {
            continue;
         }
      }

      /* Iterate each row. */
      for( x = ANIMATE_TILE_W - 1 ; 0 <= x ; x-- ) {

         idx = (y * ANIMATE_TILE_W) + x;
         if(
            ANIMATE_FLAG_CLOUDS_ROTATE ==
            (ANIMATE_FLAG_CLOUDS_ROTATE & a->flags)
         ) {
            row_col_end_buffer = 
               /* Grab the pixel in this column off the bottom. */
               a->tile[((ANIMATE_TILE_H - 1) * ANIMATE_TILE_W) + x];
            prev_idx = idx - ANIMATE_TILE_W;
         } else {
            row_col_end_buffer =
               /* Grab the pixel in this row off the right. */
               a->tile[(y * ANIMATE_TILE_W) + (ANIMATE_TILE_W - 1)];
            prev_idx = idx - 1;
         }

         if( 
            (ANIMATE_FLAG_CLOUDS_ROTATE ==
               (ANIMATE_FLAG_CLOUDS_ROTATE & a->flags) && 0 == y) ||
            (ANIMATE_FLAG_CLOUDS_ROTATE !=
               (ANIMATE_FLAG_CLOUDS_ROTATE & a->flags) && 0 == x)
         ) {
            /* Wrap-around. */
            a->tile[idx] = row_col_end_buffer;

         } else {
            /* Cloud advance. */
            a->tile[idx] = a->tile[prev_idx];
         }
      }
   }

   animate_tesselate( a, 0 );

}

void animate_draw_STRING( struct ANIMATION* a ) {
   int8_t* y_offset = (int8_t*)&(a->tile[ANIMATE_TEXT_HEADER_Y_OFFSET]);
   uint8_t str_sz = (uint8_t)(a->tile[ANIMATE_TEXT_HEADER_STR_SZ]),
      color_idx = (uint8_t)(a->tile[ANIMATE_TEXT_HEADER_COLOR_IDX]),
      * y_count = (uint8_t*)&(a->tile[ANIMATE_TEXT_HEADER_Y_COUNT]);
   char* str = (char*)&(a->tile[ANIMATE_TEXT_HEADER_STR]);
   GRAPHICS_COLOR color;

#ifdef DEPTH_VGA
   assert( color_idx <= 16 );
#else
   assert( color_idx <= 3 );
#endif /* DEPTH_VGA */

   /* Select the color and draw the animation text. */
   color = gc_animation_colors[color_idx];
   graphics_string_at(
      str, str_sz, a->x, a->y + a->h - (*y_offset), color, 0 );

   /* Frame advancement delay. */
   if( *y_count < 2 ) {
      (*y_count)++;
      return;
   } else {
      *y_count = 0;
   }

   /* Move the text up half a line until it would leave the animation. */
   *y_offset += (FONT_H / 2);
   if( *y_offset > ANIMATE_TILE_H ) {
      a->flags &= ~ANIMATE_FLAG_ACTIVE;
   }
}

void animate_draw_FRAMES( struct ANIMATION* a ) {
   /* TODO */
}

void animate_set_string(
   int8_t a_idx, char* str_in, uint8_t str_sz_in, uint8_t color_idx_in
) {
   struct ANIMATION* a = &(g_animations[a_idx]);
   int8_t* y_offset = (int8_t*)&(a->tile[ANIMATE_TEXT_HEADER_Y_OFFSET]);
   uint8_t* str_sz = (uint8_t*)&(a->tile[ANIMATE_TEXT_HEADER_STR_SZ]),
      * color_idx = (uint8_t*)&(a->tile[ANIMATE_TEXT_HEADER_COLOR_IDX]);
   char* str = (char*)&(a->tile[ANIMATE_TEXT_HEADER_STR]);

   assert( 0 <= a_idx );
   assert( ANIMATE_ANIMATIONS_MAX > a_idx );
   assert( ANIMATE_TEXT_MAX_SZ > *str_sz );
   assert( ANIMATE_TYPE_STRING == a->type );

   memory_copy_ptr( str, str_in, str_sz_in );
   *str_sz = str_sz_in;
   *color_idx = color_idx_in;
   *y_offset = FONT_H;
}

int8_t animate_create(
   uint8_t type, uint16_t flags, int16_t x, int16_t y, int16_t w, int16_t h
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

   g_animations[i].flags = ANIMATE_FLAG_ACTIVE | flags;
   g_animations[i].x = x;
   g_animations[i].y = y;
   g_animations[i].w = w;
   g_animations[i].h = h;
   g_animations[i].type = type;
   memory_zero_ptr( &(g_animations[i].tile), ANIMATE_TILE_SZ );

cleanup:
   return idx_out;
}

void animate_tesselate( struct ANIMATION* a, int16_t y_orig ) {
   int8_t
      /* Address of the current pixel rel to top-left corner of tile. */
      x = 0,
      y = 0;
   int16_t
      idx = 0,
      /* Address of the current tile's top-left corner rel to animation. */
      t_x = 0,
      t_y = 0,
      /* Address of the current pixel rel to screen. */
      p_x = 0,
      p_y = 0;

   /* Iterate over every tile covered by the animation's screen area. */
   for( t_y = y_orig ; a->h > t_y ; t_y += ANIMATE_TILE_H ) {
      for( t_x = 0 ; a->w > t_x ; t_x += ANIMATE_TILE_W ) {
         /* Iterate over every pixel of the animation grid. */
         for( y = 0 ; ANIMATE_TILE_H > y ; y++ ) {
            for( x = 0 ; ANIMATE_TILE_W > x ; x++ ) {
               idx = (y * ANIMATE_TILE_W) + x;

               /* TODO: Try to trim animation to its area. */

               p_x = a->x + t_x + x;
               p_y = a->y + t_y + y;

               if(
                  -1 == a->tile[idx] &&
                  ANIMATE_FLAG_CLEANUP == (ANIMATE_FLAG_CLEANUP & a->flags)
               ) {
                  graphics_draw_px( p_x, p_y, GRAPHICS_COLOR_BLACK );

               } else if( 0 < a->tile[idx] && ANIMATE_TYPE_SNOW == a->type ) {
                  graphics_draw_px( p_x, p_y, GRAPHICS_COLOR_WHITE );
#ifdef DEPTH_VGA
               } else if( 90 < a->tile[idx] ) {
                  graphics_draw_px( p_x, p_y, GRAPHICS_COLOR_WHITE );
               } else if( 60 < a->tile[idx] ) {
                  graphics_draw_px( p_x, p_y, GRAPHICS_COLOR_YELLOW );
               } else if( 30 < a->tile[idx] ) {
                  graphics_draw_px( p_x, p_y, GRAPHICS_COLOR_RED );
               }
#else
               } else if( 90 < a->tile[idx] ) {
                  graphics_draw_px( p_x, p_y, GRAPHICS_COLOR_WHITE );
               } else if( 60 < a->tile[idx] ) {
                  graphics_draw_px( p_x, p_y, GRAPHICS_COLOR_CYAN );
               } else if( 30 < a->tile[idx] ) {
                  graphics_draw_px( p_x, p_y, GRAPHICS_COLOR_MAGENTA );
               }
#endif /* DEPTH_VGA */
            }
         }
      }
   }
}

void animate_frame( uint16_t flags ) {
   int8_t i = 0;

   for( i = 0 ; ANIMATE_ANIMATIONS_MAX > i ; i++ ) {
      if(
         ANIMATE_FLAG_ACTIVE != (g_animations[i].flags & ANIMATE_FLAG_ACTIVE) ||
         ANIMATE_FLAG_PAUSED == (g_animations[i].flags & ANIMATE_FLAG_PAUSED) ||
         flags != (flags & g_animations[i].flags)
         
      ) {
         continue;
      }
      gc_animate_draw[g_animations[i].type]( &(g_animations[i]) );
   }
}

void animate_pause( uint16_t flags ) {
   int8_t i = 0;

   for( i = 0 ; ANIMATE_ANIMATIONS_MAX > i ; i++ ) {
      if( flags == (g_animations[i].flags & flags) ) {
         g_animations[i].flags |= ANIMATE_FLAG_PAUSED;
      }
   }
}

void animate_resume( uint16_t flags ) {
   int8_t i = 0;

   for( i = 0 ; ANIMATE_ANIMATIONS_MAX > i ; i++ ) {
      if( flags == (g_animations[i].flags & flags) ) {
         g_animations[i].flags &= ~ANIMATE_FLAG_PAUSED;
      }
   }
}

void animate_stop( int8_t idx ) {
   memory_zero_ptr( &(g_animations[idx]), sizeof( struct ANIMATION ) );
}

void animate_stop_all() {
   int8_t i = 0;

   for( i = 0 ; ANIMATE_ANIMATIONS_MAX > i ; i++ ) {
      animate_stop( i );
   }
}

