
#define WINDOW_C
#include "unilayer.h"

/* === Static Utility Functions === */

static struct WINDOW* window_get( uint16_t id, struct WINDOW* windows ) {
   int16_t i = 0;

   for( i = 1 ; WINDOWS_MAX > i ; i++ ) {
      if(
         /* Skip live windows looking for free window. */
         (0 == id &&
            WINDOW_FLAG_ACTIVE == (WINDOW_FLAG_ACTIVE & windows[i].flags)) ||
         /* Skip dead windows looking for requested ID. */
         (0 != id &&
            WINDOW_FLAG_ACTIVE != (WINDOW_FLAG_ACTIVE & windows[i].flags))
      ) {
         continue;
      }

      if( 0 == id || windows[i].id == id ) {
         return &(windows[i]);
      }
   }

   return NULL;
}

static void window_placement(
   struct WINDOW* c, struct WINDOW* p, int16_t coord, uint8_t x_y
) {
   int16_t* p_grid = NULL;
   const int16_t* p_coords;

   assert( 2 > x_y );

   if( NULL == p ) {
      /* Position relative to screen. */
#ifdef WINDOW_TRACE
      debug_printf( 1, "window %u rel screen", c->id );
#endif /* WINDOW_TRACE */
      p_coords = g_window_screen_coords;
      p_grid = g_window_screen_grid;
   } else {
      /* Position relative to parent. */
#ifdef WINDOW_TRACE
      debug_printf( 1, "window %u rel window %d", c->id, c->parent_id );
#endif /* WINDOW_TRACE */
      p_coords = g_window_screen_coords;
      p_coords = p->coords;
      p_grid = p->data.grid;
   }

   if( 0 >= p_grid[x_y] ) {
      /* Setup initial grid. */
      p_grid[x_y] = WINDOW_PADDING_OUTSIDE;
      p_grid[x_y + 2] = WINDOW_PADDING_OUTSIDE;
   }

   switch( coord ){
   case WINDOW_PLACEMENT_CENTER:
      /* Window width / 2 - Control width / 2 */
      assert( p_coords[x_y + 2] > 0 );
      c->coords[x_y] = (p_coords[x_y + 2] / 2) - (c->coords[x_y + 2] / 2);
#ifdef WINDOW_TRACE
      debug_printf( 1, "window %u center coord %d (%d / 2) - (%d / 2): %d",
         c->id, x_y, p_coords[x_y + 2], c->coords[x_y + 2], c->coords[x_y] );
#endif /* WINDOW_TRACE */
      break;

   case WINDOW_PLACEMENT_RIGHT_BOTTOM:
      c->coords[x_y] =
         /* Window width - Padding - Control width */
         p_coords[x_y + 2] - WINDOW_PADDING_OUTSIDE - c->coords[x_y + 2];
      break;

   case WINDOW_PLACEMENT_GRID_RIGHT_DOWN:
      p_grid[x_y + 2] = p_grid[x_y];
      p_grid[x_y] += c->coords[x_y + 2] + WINDOW_PADDING_INSIDE;
      /* No break. */

   case WINDOW_PLACEMENT_GRID:
#ifdef WINDOW_TRACE
      debug_printf( 1, " window %u adding control using grid at: %d",
         c->id, p_grid[x_y + 2] );
#endif /* WINDOW_TRACE */
      c->coords[x_y] = p_grid[x_y + 2];
      break;

   default:
      c->coords[x_y] = coord;
      break;
   }
}

static int16_t window_sizing(
   int16_t w_id, int16_t dimension, uint8_t w_h,
   struct WINDOW* windows
) {
   int16_t win_sz[2];
   int16_t retval = 0;
   struct WINDOW* c = NULL;

   assert( 4 > w_h );
   assert( 1 < w_h );

#ifdef WINDOW_TRACE
   debug_printf( 1, "sizing window ID: %u", w_id );
#endif /* WINDOW_TRACE */

   c = window_get( w_id, windows );
   assert( NULL != c );

   /* Width and Height */
   if( WINDOW_SIZE_AUTO != dimension && 0 < dimension ) {
      c->coords[w_h] = dimension;
#ifdef WINDOW_TRACE
      debug_printf( 1, "window %u manual size %d: %d",
         c->id, w_h, dimension );
#endif /* WINDOW_TRACE */
      retval = dimension;

   } else if(
      gc_window_sz_callbacks[c->type]( w_id, windows, win_sz )
   ) {
      c->coords[w_h] = win_sz[w_h - 2];
#ifdef WINDOW_TRACE
      debug_printf( 1, "window %u auto-size %d: %d",
         c->id, w_h, win_sz[w_h - 2] );
#endif /* WINDOW_TRACE */
      retval = win_sz[w_h - 2];

   } else {
      error_printf( "unable to automatically size window" );
      retval = 0;
   }

   /* TODO: Make sure window doesn't exceed parent size. */

   return retval;
}

static void window_draw_text(
   struct WINDOW* c, struct WINDOW* p, const char* str, uint16_t str_sz
) {
   int16_t offset_x = 0,
      offset_y = 0;

   if( NULL != p ) {
      offset_x = p->coords[GUI_X];
      offset_y = p->coords[GUI_Y];
   }

   assert( NULL != c );

#ifdef WINDOW_TRACE
   debug_printf( 1, "window %u drawing string at %d, %d: %s",
      c->id,
      offset_x + c->coords[GUI_X],
      offset_y + c->coords[GUI_Y], str );
#endif /* WINDOW_TRACE */

   graphics_string_at( 
      str, str_sz,
      offset_x + c->coords[GUI_X],
      offset_y + c->coords[GUI_Y], c->fg, c->render_flags );
}

/* === Drawing Callbacks === */

static int16_t window_draw_WINDOW( uint16_t w_id, struct WINDOW* windows ) {
   struct WINDOW_FRAME* frames = NULL;
   int16_t x = 0,
      y = 0,
      i = 0,
      x_max = 0,
      y_max = 0,
      blit_retval = 0;
   struct WINDOW* c = NULL;

#ifdef WINDOW_TRACE
   debug_printf( 1, "window %u drawing...", w_id );
#endif /* WINDOW_TRACE */

   c = window_get( w_id, windows );
   assert( NULL != c );

   frames = (struct WINDOW_FRAME*)memory_lock( g_frames_handle );

#ifdef WINDOW_TRACE
   debug_printf(
      1, "window %u min: %d, %d; max: %d, %d",
      c->id, c->coords[GUI_X], c->coords[GUI_Y], x_max, y_max );
#endif /* WINDOW_TRACE */

   x_max = c->coords[GUI_X] + c->coords[GUI_W];
   y_max = c->coords[GUI_Y] + c->coords[GUI_H];

#ifdef WINDOW_TRACE
   debug_printf( 1, "window %u drawing with frame %d...",
      c->id, c->render_flags );
#endif /* WINDOW_TRACE */

   /* Draw the window background. */
   for( y = c->coords[GUI_Y] ; y < y_max ; y += WINDOW_PATTERN_H ) {
      for( x = c->coords[GUI_X] ; x < x_max ; x += WINDOW_PATTERN_W ) {
         if(
            c->coords[GUI_X] == x &&
            c->coords[GUI_Y] == y
         ) {
            /* Top Left */
            blit_retval = graphics_blit_tile_at(
               frames[c->render_flags].tl, 0, 0, x, y,
               WINDOW_PATTERN_W, WINDOW_PATTERN_H );

         } else if(
            x_max - WINDOW_PATTERN_W == x && c->coords[GUI_Y] == y
         ) {
            /* Top Right */
            blit_retval = graphics_blit_tile_at(
               frames[c->render_flags].tr, 0, 0, x, y,
               WINDOW_PATTERN_W, WINDOW_PATTERN_H );

         } else if(
            c->coords[GUI_X] == x && y_max - WINDOW_PATTERN_H == y
         ) {
            /* Bottom Left */
            blit_retval = graphics_blit_tile_at(
               frames[c->render_flags].bl, 0, 0, x, y,
               WINDOW_PATTERN_W, WINDOW_PATTERN_H );
         
         } else if( x_max - WINDOW_PATTERN_W == x && y_max - WINDOW_PATTERN_H == y ) {
            /* Bottom Right */
            blit_retval = graphics_blit_tile_at(
               frames[c->render_flags].br, 0, 0, x, y,
               WINDOW_PATTERN_W, WINDOW_PATTERN_H );
         
         } else if( x_max - WINDOW_PATTERN_W == x ) {
            /* Right */
            blit_retval = graphics_blit_tile_at(
               frames[c->render_flags].r, 0, 0, x, y,
               WINDOW_PATTERN_W, WINDOW_PATTERN_H );
         
         } else if( c->coords[GUI_X] == x ) {
            /* Left */
            blit_retval = graphics_blit_tile_at(
               frames[c->render_flags].l, 0, 0, x, y,
               WINDOW_PATTERN_W, WINDOW_PATTERN_H );
         
         } else if( c->coords[GUI_Y] == y ) {
            /* Top */
            blit_retval = graphics_blit_tile_at(
               frames[c->render_flags].t, 0, 0, x, y,
               WINDOW_PATTERN_W, WINDOW_PATTERN_H );
         
         } else if( y_max - WINDOW_PATTERN_H == y ) {
            /* Bottom */
            blit_retval = graphics_blit_tile_at(
               frames[c->render_flags].b, 0, 0, x, y,
               WINDOW_PATTERN_W, WINDOW_PATTERN_H );
         
         } else {
            blit_retval = graphics_blit_tile_at(
               frames[c->render_flags].c, 0, 0, x, y,
               WINDOW_PATTERN_W, WINDOW_PATTERN_H );
         }

         if( !blit_retval ) {
            /* error_printf( "window blit failed" ); */
            goto cleanup;
         }
      }
   }

   /* Draw the window's children. */
   for( i = 1 ; WINDOWS_MAX > i ; i++ ) {
      /* Skip inactive and non-children. */
      if(
         WINDOW_FLAG_ACTIVE != (WINDOW_FLAG_ACTIVE & windows[i].flags) ||
         windows[i].parent_id != w_id
      ) {
         continue;
      }

      blit_retval = gc_window_draw_callbacks[windows[i].type](
         windows[i].id, windows );
      if( !blit_retval ) {
         error_printf( "error drawing window children!" );
         goto cleanup;
      }

      windows[i].flags &= ~WINDOW_FLAG_DIRTY;
   }

cleanup:

   if( NULL != frames ) {
      frames = (struct WINDOW_FRAME*)memory_unlock( g_frames_handle );
   }

   return blit_retval;
}

static int16_t window_draw_BUTTON( uint16_t w_id, struct WINDOW* windows ) {
   /* TODO */
   return 1;
}

static int16_t window_draw_CHECK( uint16_t w_id, struct WINDOW* windows ) {
   /* TODO */
   return 1;
}

int16_t window_get_text(
   struct WINDOW* c, char* buffer, uint16_t buffer_sz
) {
   char* str_ptr = NULL;
   int16_t sz_out = 0;

   if( WINDOW_FLAG_TEXT_PTR == (WINDOW_FLAG_TEXT_PTR & c->flags) ) {
      /* Get the string from a directly passed pointer. */
      str_ptr = (char*)memory_lock( c->data.string );
      sz_out = memory_strnlen_ptr( str_ptr, buffer_sz - 1 );
#ifdef WINDOW_TRACE
      debug_printf( 1, "window %u text str: %s (%d)",
         c->id, str_ptr, sz_out );
#endif /* WINDOW_TRACE */
      memory_strncpy_ptr( buffer, str_ptr, sz_out );
      str_ptr = (char*)memory_unlock( c->data.string );

   } else if( WINDOW_FLAG_TEXT_NUM == (WINDOW_FLAG_TEXT_NUM & c->flags) ) {
      sz_out = dio_itoa(
         buffer, buffer_sz, c->data.scalar, 10 );
#ifdef WINDOW_TRACE
      debug_printf( 1, "window %u text num: %s (%d)",
         c->id, buffer, sz_out );
#endif /* WINDOW_TRACE */
   
   }

   return sz_out;
}

static int16_t window_draw_LABEL( uint16_t w_id, struct WINDOW* windows ) {
   int16_t str_sz = 0;
   char str_ptr[WINDOW_STRING_SZ_MAX] = { 0 };
   struct WINDOW* c = NULL,
      * p = NULL;

   c = window_get( w_id, windows );
   assert( NULL != c );

   p = window_get( c->parent_id, windows );

   str_sz = window_get_text(
      window_get( w_id, windows ), str_ptr, WINDOW_STRING_SZ_MAX );

   window_draw_text( c, p, str_ptr, str_sz );

   return 1;
}

static int16_t window_draw_SPRITE( uint16_t w_id, struct WINDOW* windows ) {
   int16_t offset_sprite = 0,
      offset_x = 0,
      offset_y = 0;
   struct WINDOW* c = NULL,
      * p = NULL;
   uint8_t dir = 0;

   c = window_get( w_id, windows );
   assert( NULL != c );

   p = window_get( c->parent_id, windows );
   if( NULL != p ) {
      offset_x = p->coords[GUI_X];
      offset_y = p->coords[GUI_Y];
   }

#ifdef WINDOW_TRACE
   debug_printf(
      1, "window %u screen offset: %d, %d", w_id, offset_x, offset_y );
#endif /* WINDOW_TRACE */

   /* Set offset_sprite based on dir flags. */
   dir = ((c->flags & WINDOW_FLAG_SPRITE_DIR_MASK) >> 4);
#ifdef WINDOW_TRACE
   debug_printf( 1, "window %u sprite flags 0x%02x dir 0x%02x",
      w_id, c->flags, dir );
#endif /* WINDOW_TRACE */
   offset_sprite = dir * WINDOW_SPRITE_H;

   graphics_blit_sprite_at(
      c->data.scalar,
      0, offset_sprite,
      offset_x + c->coords[GUI_X] + 2,
      offset_y + c->coords[GUI_Y] + 2,
      WINDOW_SPRITE_W, WINDOW_SPRITE_H );

   if(
      WINDOW_FLAG_SPRITE_BORDER_SINGLE ==
      (WINDOW_FLAG_SPRITE_BORDER_SINGLE & c->flags)
   ) {
      graphics_draw_rect(
         offset_x + c->coords[GUI_X],
         offset_y + c->coords[GUI_Y],
         WINDOW_SPRITE_W + 4, WINDOW_SPRITE_H + 4,
         1, GRAPHICS_COLOR_BLACK );

      graphics_draw_rect(
         offset_x + c->coords[GUI_X] + 1,
         offset_y + c->coords[GUI_Y] + 1,
         WINDOW_SPRITE_W + 3, WINDOW_SPRITE_H + 3,
         1, GRAPHICS_COLOR_WHITE );
   }

   return 1;
}

/* === Sizing Callbacks === */

static uint8_t window_sz_WINDOW(
   uint16_t w_id, struct WINDOW* windows, int16_t r[2]
) {
   /* TODO */
   return 0;
}

static uint8_t window_sz_BUTTON(
   uint16_t w_id, struct WINDOW* windows, int16_t r[2]
) {
   /* TODO */
   return 0;
}

static uint8_t window_sz_CHECK(
   uint16_t w_id, struct WINDOW* windows, int16_t r[2]
) {
   /* TODO */
   return 0;
}

static uint8_t window_sz_LABEL(
   uint16_t w_id, struct WINDOW* windows, int16_t r[2]
) {
   int16_t str_sz = 0;
   struct GRAPHICS_RECT sz;
   struct WINDOW* c = NULL;
   char str_ptr[WINDOW_STRING_SZ_MAX] = { 0 };

   c = window_get( w_id, windows );
   assert( NULL != c );

   str_sz = window_get_text( c, str_ptr, WINDOW_STRING_SZ_MAX );
   graphics_string_sz( str_ptr, str_sz, 0, &sz );

   r[0] = sz.w;
   r[1] = sz.h;

   return 1;
}

static uint8_t window_sz_SPRITE(
   uint16_t w_id, struct WINDOW* windows, int16_t r[2]
) {
   /* TODO: Verify sprite exists. */
   r[0] = WINDOW_SPRITE_W + 4; /* For border. */
   r[1] = WINDOW_SPRITE_H + 4; /* For border. */
   return 1;
}

/* === General Functions === */

int16_t window_init( uint16_t auto_w, uint16_t auto_h ) {
   int16_t retval = 1;

   if( 0 < auto_w ) {
      g_window_screen_coords[2] = auto_w;
   }

   if( 0 < auto_w ) {
      g_window_screen_coords[3] = auto_h;
   }

   debug_printf( 1, "initalizing windowing system..." );

   g_windows_handle =
      memory_alloc( sizeof( struct WINDOW ), WINDOWS_MAX );
   assert( (MEMORY_HANDLE)NULL != g_windows_handle );

   g_frames_handle = memory_alloc( sizeof( struct WINDOW_FRAME ), 1 );
   assert( (MEMORY_HANDLE)NULL != g_frames_handle );

   window_reload_frames();

   return retval;
}

int16_t window_reload_frames() {
   struct WINDOW_FRAME* frames = NULL;
   int16_t retval = 1;

   if( (MEMORY_HANDLE)NULL == g_frames_handle ) {
      return 0;
   }

   frames = (struct WINDOW_FRAME*)memory_lock( g_frames_handle );

   /* TODO: Check for frame load success. */

#ifdef RESOURCE_FILE
   frames[0].tr =
      graphics_cache_load_bitmap( ASSETS_PATH DEPTH_SPEC "/p_chk_tr.bmp" );
   frames[0].tl =
      graphics_cache_load_bitmap( ASSETS_PATH DEPTH_SPEC "/p_chk_tl.bmp" );
   frames[0].br =
      graphics_cache_load_bitmap( ASSETS_PATH DEPTH_SPEC "/p_chk_br.bmp" );
   frames[0].bl =
      graphics_cache_load_bitmap( ASSETS_PATH DEPTH_SPEC "/p_chk_bl.bmp" );
   frames[0].t  =
      graphics_cache_load_bitmap( ASSETS_PATH DEPTH_SPEC "/p_chk_t.bmp" );
   frames[0].b  =
      graphics_cache_load_bitmap( ASSETS_PATH DEPTH_SPEC "/p_chk_b.bmp" );
   frames[0].r  =
      graphics_cache_load_bitmap( ASSETS_PATH DEPTH_SPEC "/p_chk_r.bmp" );
   frames[0].l  =
      graphics_cache_load_bitmap( ASSETS_PATH DEPTH_SPEC "/p_chk_l.bmp" );
   frames[0].c  =
      graphics_cache_load_bitmap( ASSETS_PATH DEPTH_SPEC "/p_chk_c.bmp" );
#else
   frames[0].tr = graphics_cache_load_bitmap( p_chk_tr );
   frames[0].tl = graphics_cache_load_bitmap( p_chk_tl );
   frames[0].br = graphics_cache_load_bitmap( p_chk_br );
   frames[0].bl = graphics_cache_load_bitmap( p_chk_bl );
   frames[0].t  = graphics_cache_load_bitmap( p_chk_t );
   frames[0].b  = graphics_cache_load_bitmap( p_chk_b );
   frames[0].r  = graphics_cache_load_bitmap( p_chk_r );
   frames[0].l  = graphics_cache_load_bitmap( p_chk_l );
   frames[0].c  = graphics_cache_load_bitmap( p_chk_c );
#endif

   frames = (struct WINDOW_FRAME*)memory_unlock( g_frames_handle );

   return retval;
}

void window_shutdown() {
   /* TODO: Verify all windows have closed/freed their resources. */
   memory_free( g_windows_handle );
   g_windows_handle = (MEMORY_HANDLE)NULL;
   memory_free( g_frames_handle );
   g_frames_handle = (MEMORY_HANDLE)NULL;
}

int16_t window_draw_all() {
   int16_t i = 0,
      blit_retval = 1;
   struct WINDOW* windows = NULL;

   windows = (struct WINDOW*)memory_lock( g_windows_handle );
   assert( NULL != windows );

   debug_printf( 0, "starting window drawing..." );

   for( i = WINDOWS_MAX - 1; 1 /* TODO */ <= i ; i-- ) {
      /* Only draw active windows. Those windows will recurse to draw their
       * children.
       */
      if(
         WINDOW_TYPE_WINDOW != windows[i].type ||
         WINDOW_FLAG_ACTIVE != (WINDOW_FLAG_ACTIVE & windows[i].flags) ||
         WINDOW_FLAG_DIRTY != (WINDOW_FLAG_DIRTY & windows[i].flags)
      ) {
         continue;
      }

      assert( 0 == windows[i].coords[GUI_W] % WINDOW_PATTERN_W );
      assert( 0 == windows[i].coords[GUI_H] % WINDOW_PATTERN_H );

      assert( NULL != windows );
      blit_retval = gc_window_draw_callbacks[windows[i].type](
         windows[i].id, windows );
      if( !blit_retval ) {
         error_printf( "error drawing windows!" );
         goto cleanup;
      }

      windows[i].flags &= ~WINDOW_FLAG_DIRTY;
   }

cleanup:

   if( NULL != windows ) {
      windows = (struct WINDOW*)memory_unlock( g_windows_handle );
   }

   return blit_retval;
}

int16_t window_push(
   uint16_t id, uint16_t parent_id, uint8_t type, uint8_t flags,
   int16_t x, int16_t y, int16_t w, int16_t h,
   GRAPHICS_COLOR fg, GRAPHICS_COLOR bg, uint8_t render_flags,
   int32_t data_scalar, const char* data_string
) {
   int16_t retval = 0,
      string_sz = 0;
   struct WINDOW* window_new = NULL,
      * parent = NULL,
      * windows = NULL;
   char* str_ptr = NULL;

   windows = (struct WINDOW*)memory_lock( g_windows_handle );
   assert( NULL != windows );

   assert( 0 != id );
   assert( id != parent_id );

   /* Check for dupe ID. */
   window_new = window_get( id, windows );
   if( NULL != window_new ) {
      error_printf( "window with ID %u already exists", id );
      retval = 0;
      goto cleanup;
   }

   /* Get parent window for placement, etc. */
   if( 0 != parent_id ) {
      parent = window_get( parent_id, windows );
      assert( NULL != parent );
      if( WINDOW_TYPE_WINDOW != parent->type ) {
         error_printf( "parent window is not WINDOW type" );
         retval = 0;
         goto cleanup;
      }
   }

   /* Find a free window slot. */
   window_new = window_get( 0, windows );
   if( NULL == window_new ) {
      error_printf( "unable to find free window!" );
      retval = 0;
      goto cleanup;
   }

   assert( NULL != window_new );

   memory_zero_ptr( (MEMORY_PTR)window_new, sizeof( struct WINDOW ) );

   /* Sizing callbacks below might need these. */
   if( NULL != data_string ) {
      string_sz = memory_strnlen_ptr( data_string, WINDOW_STRING_SZ_MAX );
      
      /* Allocate new string handle. */
      window_new->data.string = memory_alloc( 1, string_sz + 1 );
      assert( (MEMORY_HANDLE)NULL != window_new->data.string );

      /* Copy incoming string into handle. */
      str_ptr = (char*)memory_lock( window_new->data.string );
      assert( NULL != str_ptr );
      memory_strncpy_ptr( str_ptr, data_string, string_sz );
      str_ptr = (char*)memory_unlock( window_new->data.string );

      /* Ensure flag consistency. */
      flags &= ~WINDOW_FLAG_TEXT_MASK;
      flags |= WINDOW_FLAG_TEXT_PTR;
   } else if( 0 != data_scalar ) {
      window_new->data.scalar = data_scalar;

      if( WINDOW_TYPE_LABEL == type ) {
         /* Ensure flag consistency. */
         flags &= ~WINDOW_FLAG_TEXT_MASK;
         flags |= WINDOW_FLAG_TEXT_NUM;
      }
   }

   window_new->flags = flags | WINDOW_FLAG_DIRTY | WINDOW_FLAG_ACTIVE;
   window_new->render_flags = render_flags;
   window_new->type = type;
   window_new->id = id;
   window_new->fg = fg;
   window_new->bg = bg;
   window_new->parent_id = parent_id;

   /* X/Y sizing and placement. Sizing comes first, used for centering. */
   window_sizing( id, w, GUI_W, windows );
   window_sizing( id, h, GUI_H, windows );
   window_placement( window_new, parent, x, GUI_X );
   window_placement( window_new, parent, y, GUI_Y );

cleanup:

   windows = (struct WINDOW*)memory_unlock( g_windows_handle );

   return retval;
}

static void window_pop_internal( uint16_t id, struct WINDOW* windows ) {
   int16_t i = 0,
      id_recurse = 0;
   struct WINDOW* window_out = NULL;

   /* Deallocate the window resources. */
   window_out = window_get( id, windows );
   
#ifdef WINDOW_TRACE
   debug_printf( 1, "window %d popping...", id );
#endif /* WINDOW_TRACE */

   if( NULL != window_out ) {
      if(
         WINDOW_TYPE_LABEL == window_out->type &&
         WINDOW_FLAG_TEXT_PTR == (WINDOW_FLAG_TEXT_MASK & window_out->flags)
      ) {
#ifdef WINDOW_TRACE
         debug_printf( 1, "window %u freeing memory for flags %02x: %p...",
            id, (WINDOW_FLAG_TEXT_MASK & window_out->flags),
            window_out->data.string );
#endif /* WINDOW_TRACE */
         memory_free( window_out->data.string );
         window_out->data.string = (MEMORY_HANDLE)NULL;
      }
      window_out->flags &= ~WINDOW_FLAG_ACTIVE;
      window_out->id = 0;
   }

   /* Deactivate any children of the window. */
   for( i = 1 ; WINDOWS_MAX > i ; i++ ) {
      if(
         WINDOW_FLAG_ACTIVE == (WINDOW_FLAG_ACTIVE & windows[i].flags) &&
         id == windows[i].parent_id
      ) {
         id_recurse = windows[i].id;
         window_pop_internal( id_recurse, windows );
      }
   }

}

void window_pop( uint16_t id ) {
   struct WINDOW* windows = NULL;

   windows = (struct WINDOW*)memory_lock( g_windows_handle );
   assert( NULL != windows );

   window_pop_internal( id, windows );

   windows = (struct WINDOW*)memory_unlock( g_windows_handle );
}

void window_refresh( uint16_t w_id ) {
   struct WINDOW* windows = NULL,
      * w = NULL;

   windows = (struct WINDOW*)memory_lock( g_windows_handle );
   assert( NULL != windows );

   w = window_get( w_id, windows );
   if( NULL != w ) {
      w->flags |= WINDOW_FLAG_DIRTY;
   }

   windows = (struct WINDOW*)memory_unlock( g_windows_handle );
}

int16_t window_modal() {
   int i = 0;
   int16_t modal = 0;
   struct WINDOW* windows = NULL;

   windows = (struct WINDOW*)memory_lock( g_windows_handle );
   assert( NULL != windows );

   for( i = 1 ; WINDOWS_MAX > i ; i++ ) {
      if(
         WINDOW_FLAG_ACTIVE == (WINDOW_FLAG_ACTIVE & windows[i].flags) &&
         WINDOW_FLAG_MODAL == (WINDOW_FLAG_MODAL & windows[i].flags)
      ) {
         modal++;
      }
   }

   windows = (struct WINDOW*)memory_unlock( g_windows_handle );

   return modal;
}

