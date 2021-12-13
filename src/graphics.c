
#define GRAPHICS_C
#include "unilayer.h"

#ifdef USE_SOFTWARE_TEXT
#include "data/font8x8.h"
#endif /* USE_SOFTWARE_TEXT */

static MEMORY_HANDLE gs_graphics_cache_handle = (MEMORY_HANDLE)NULL;
static int16_t gs_graphics_cache_sz = 0;

int16_t graphics_init() {
   int16_t retval = 1;

   retval = graphics_platform_init();
   if( !retval ) {
      goto cleanup;
   }

   gs_graphics_cache_handle = memory_alloc(
      GRAPHICS_CACHE_INITIAL_SZ, sizeof( struct GRAPHICS_BITMAP ) );
   gs_graphics_cache_sz = GRAPHICS_CACHE_INITIAL_SZ;

cleanup:
   return retval;
}

void graphics_clear_cache() {
   int16_t i = 0,
      dropped_count = 0;
   struct GRAPHICS_BITMAP* bitmaps = NULL;

   bitmaps = (struct GRAPHICS_BITMAP*)memory_lock( gs_graphics_cache_handle );
   for( i = 0 ; gs_graphics_cache_sz > i ; i++ ) {
      if( 1 == bitmaps[i].initialized ) {
         graphics_platform_unload_bitmap( &(bitmaps[i]) );
         dropped_count++;
      }
   }
   memory_zero_ptr( (MEMORY_PTR)bitmaps,
      sizeof( struct GRAPHICS_BITMAP ) * gs_graphics_cache_sz );
   bitmaps = (struct GRAPHICS_BITMAP*)memory_unlock( gs_graphics_cache_handle );
   
   debug_printf( 2, "graphics cache cleared (%d of %d items)",
      dropped_count, gs_graphics_cache_sz );
}

void graphics_shutdown() {
   graphics_clear_cache();
   memory_free( gs_graphics_cache_handle );
   graphics_platform_shutdown();
}

#ifdef USE_SOFTWARE_TEXT

void graphics_char_at(
   char c, uint16_t x_orig, uint16_t y_orig, GRAPHICS_COLOR color,
   uint8_t flags
) {
	int16_t x = 0,
	   y = 0;
	uint8_t bitmask = 0;
	GRAPHICS_COLOR pixel = GRAPHICS_COLOR_BLACK;

   assert( '~' >= c );
   assert( ' ' <= c );

   if(
      GRAPHICS_STRING_FLAG_ALL_CAPS == (GRAPHICS_STRING_FLAG_ALL_CAPS & flags)
   ) {
      c &= ~ 0x20; /* XOR ASCII all-caps trick. */
   }

   /* Draw the char from our built-in font. */
	for( y = 0 ; FONT_H > y ; y++ ) {
      if(
         GRAPHICS_STRING_FLAG_FONT_SCRIPT ==
         (GRAPHICS_STRING_FLAG_FONT_SCRIPT & flags)
      ) {
   		bitmask = gc_font8x8_script[c - ' '][y];
      } else {
   		bitmask = gc_font8x8_basic[c - ' '][y];
      }

		for( x = 0 ; FONT_W > x ; x++ ) {
			if( bitmask & 0x01 ) {
				pixel = color;
            graphics_draw_px( x_orig + x, y_orig + y, pixel );
			}
			bitmask >>= 1;
		}
	}
}

void graphics_string_at(
   const char* str, uint16_t str_sz, uint16_t x_orig, uint16_t y_orig,
   GRAPHICS_COLOR color, uint8_t flags
) {
   int16_t i = 0,
      x_o = 0, /* X offset. */
      y_o = 0;

   while(
      '\0' != str[i]
#if defined( SCREEN_REAL_W ) && defined( SCREEN_REAL_H )
      &&
      x_orig + FONT_W < SCREEN_REAL_W && /* On-screen (x-axis). */
      y_orig + FONT_H < SCREEN_REAL_H    /* On-screen (y-axis). */
#endif /* SCREEN_REAL_W && SCREEN_REAL_H */
   ) {
      if( '\n' == str[i] ) {
         /* Shift the "cursor" down and back. */
         x_o = 0;
         y_o += FONT_H + FONT_SPACE;

      } else if( graphics_char_is_printable( str[i] ) ) {
         graphics_char_at( str[i], x_orig + x_o, y_orig + y_o, color, flags );

         /* Shift the "cursor" to the right. */
         x_o += FONT_W + FONT_SPACE;
      }
      i++;
   }
}

void graphics_string_sz(
   const char* str, uint16_t str_sz, uint8_t flags, struct GRAPHICS_RECT* sz_out
) {
   int16_t row_x_w = 0,
      i = 0;

   #if 0
   sz_out->w = memory_strnlen_ptr( str, str_sz ) * (FONT_W + FONT_SPACE);
   #endif

   sz_out->h = FONT_H + FONT_SPACE; /* At least one line high. */
   sz_out->w = 0;
   while( '\0' != str[i] ) {
      if( '\n' == str[i] ) {
         /* Shift the "cursor" down and reset row width. */
         sz_out->h += FONT_H + FONT_SPACE;
         row_x_w = 0;

      } else if( graphics_char_is_printable( str[i] ) ) {
         row_x_w += FONT_W + FONT_SPACE;
         if( row_x_w > sz_out->w ) {
            /* Shift the "cursor" to the right if row is widest so far. */
            sz_out->w = row_x_w;
         }
      }
      i++;
   }

   debug_printf( 2, "string %s is %d x %x", str, sz_out->w, sz_out->h );

   if( sz_out->w > 0 ) {
      sz_out->w -= FONT_SPACE; /* Remove trailing space. */
   }
   sz_out->h -= FONT_SPACE;
}

#endif /* USE_SOFTWARE_TEXT */

#ifdef USE_SOFTWARE_LINES

void graphics_draw_rect(
   uint16_t x_orig, uint16_t y_orig, uint16_t w, uint16_t h,
   uint16_t thickness, const GRAPHICS_COLOR color
) {
   /* TODO: Handle thickness. */

   /* Left Wall */
   graphics_draw_line( x_orig, y_orig, x_orig, y_orig + h, thickness, color );
   /* Bottom Wall */
   graphics_draw_line(
      x_orig, y_orig + h, x_orig + w, y_orig + h, thickness, color );
   /* Right Wall */
   graphics_draw_line(
      x_orig + w, y_orig + h, x_orig + w, y_orig, thickness, color );
   /* Top Wall */
   graphics_draw_line( x_orig, y_orig, x_orig + w, y_orig, thickness, color );
}

void graphics_draw_line(
   uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t thickness,
   const GRAPHICS_COLOR color
) {
   #define LINE_AXIS_X 0
   #define LINE_AXIS_Y 1

   int16_t delta[2],
      start[2],
      end[2],
      iter[2],
      delta_p = 0;
   uint8_t inc_axis = 0,
      off_axis = 0;

   /* TODO: Handle thickness. */

   /* Keep coordinates positive. */
   if( x1 > x2 ) {
      start[LINE_AXIS_X] = x2;
      end[LINE_AXIS_X] = x1;
   } else {
      start[LINE_AXIS_X] = x1;
      end[LINE_AXIS_X] = x2;
   }
   if( y1 > y2 ) {
      start[LINE_AXIS_Y] = y2;
      end[LINE_AXIS_Y] = y1;
   } else {
      start[LINE_AXIS_Y] = y1;
      end[LINE_AXIS_Y] = y2;
   }

   /* Select an axis that will DEFINITELY be incrementing. */
   if( end[LINE_AXIS_X] > start[LINE_AXIS_X] ) {
      inc_axis = LINE_AXIS_X;
      off_axis = LINE_AXIS_Y;
   } else {
      inc_axis = LINE_AXIS_Y;
      off_axis = LINE_AXIS_X;
   }

   /* Bresenham's line algorithm. */

   delta[LINE_AXIS_X] = end[LINE_AXIS_X] - start[LINE_AXIS_X];
   delta[LINE_AXIS_Y] = end[LINE_AXIS_Y] - start[LINE_AXIS_Y];
   delta_p = (2 * delta[off_axis]) - delta[inc_axis];
   iter[off_axis] = start[off_axis];
   
   /* Iterate the incremented axis. */
   for(
      iter[inc_axis] = start[inc_axis] ;
      end[inc_axis] > iter[inc_axis] ;
      iter[inc_axis]++
   ) {
      debug_printf(
         0, "px %d, %d (%d)", iter[LINE_AXIS_X], iter[LINE_AXIS_Y], delta_p );
      graphics_draw_px( iter[LINE_AXIS_X], iter[LINE_AXIS_Y], color );
      if( 0 < delta_p ) {
         iter[off_axis] += 1;
         delta_p += (2 * (delta[off_axis] - delta[inc_axis]));
      } else {
         delta_p += (2 * delta[off_axis]);
      }
   }
}

#endif /* !USE_SOFTWARE_LINES */

static
int16_t graphics_load_bitmap( RESOURCE_ID id, struct GRAPHICS_BITMAP* b ) {
   int16_t retval = 0;
   RESOURCE_HANDLE bitmap_handle = (RESOURCE_HANDLE)NULL;
   
   /* Load resource into buffer. */
   bitmap_handle = resource_get_handle( id );
   if( (RESOURCE_HANDLE)NULL == bitmap_handle ) {
      retval = 0;
      error_printf( "unable to get resource handle" );
      goto cleanup;
   }

   assert( NULL != b );

   resource_assign_id( b->id, id );
   retval = graphics_platform_load_bitmap( bitmap_handle, b );
   if( !retval ) {
      error_printf( "failed to lazy load bitmap" );
      goto cleanup;
   }

   /* Assume the load was a success. */
   b->ref_count++;
   b->initialized = 1;

cleanup:

   /* The platform-specific loader should dispose of the resource if needed.
    * This is because some platforms use the resource directly.
    */

   return retval;
}

int16_t graphics_blit_at(
   RESOURCE_ID res_id,
   uint16_t s_x, uint16_t s_y, uint16_t d_x, uint16_t d_y,
   uint16_t w, uint16_t h
) {
   int16_t retval = 0,
      i = 0;
   struct GRAPHICS_BITMAP* bitmaps = NULL,
      * bitmap_blit = NULL;

   bitmaps = (struct GRAPHICS_BITMAP*)memory_lock( gs_graphics_cache_handle );

   /* Try to find the bitmap already in the cache. */
   for( i = 0 ; gs_graphics_cache_sz > i ; i++ ) {
      if( resource_compare_id( bitmaps[i].id, res_id ) ) {
         bitmap_blit = &(bitmaps[i]);
         break;
      }
   }

   if( NULL == bitmap_blit ) {
      /* Bitmap not found. */
      debug_printf( 1, "bitmap not found in cache; loading..." );
      for( i = 0 ; gs_graphics_cache_sz > i ; i++ ) {
         if( 0 == bitmaps[i].initialized ) {
            if( graphics_load_bitmap( res_id, &(bitmaps[i]) ) ) {
               bitmap_blit = &(bitmaps[i]);
            }
            break;
         }
      }
   }

   if( NULL == bitmap_blit ) {
      error_printf( "unable to load bitmap; cache full" );
      goto cleanup;
   }

   /*
   if( 0 == s_x && 0 == s_y ) {
      retval = graphics_platform_blit_at( bitmap_blit, d_x, d_y, w, h );
   } else {
   */
      retval = graphics_platform_blit_partial_at(
         bitmap_blit, s_x, s_y, d_x, d_y, w, h );
   /* } */
   if( !retval ) {
      /* error_printf( "failed to blit bitmap" ); */
      goto cleanup;
   }

cleanup:

   if( NULL != bitmaps ) {
      bitmaps = (struct GRAPHICS_BITMAP*)memory_unlock(
         gs_graphics_cache_handle );
   }

   return retval;
}

