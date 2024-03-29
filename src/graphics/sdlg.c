
#define GRAPHICS_PLATFORM_C
#define SDLG_C
#include "../unilayer.h"

#include <SDL.h>
#include <time.h> /* For time() and time_t. */
#include <stdlib.h> /* For rand(). */

#ifdef PLATFORM_SDL1

SDL_Surface* g_screen = NULL;
int g_screen_locks = 0;

#else

SDL_Window* g_window = NULL;
SDL_Renderer* g_window_renderer = NULL;
SDL_Texture* g_buffer_tex = NULL;

#endif /* PLATFORM_SDL2 */

volatile uint32_t g_ms;

const uint32_t gc_ms_target = 1000 / FPS;
static uint32_t g_ms_start = 0; 

/*
 * @return 1 if init was successful and 0 otherwise.
 */
int16_t graphics_platform_init() {
#ifdef DEBUG_CGA_EMU
   SDL_Rect area;
#endif /* DEBUG_CGA_EMU */
   time_t tm;

   if( SDL_Init( SDL_INIT_EVERYTHING ) ) {
      error_printf( "error initializing SDL: %s", SDL_GetError() );
   }

#ifdef PLATFORM_SDL1

   g_screen = SDL_SetVideoMode( g_screen_real_w, g_screen_real_h, 8,
      SDL_DOUBLEBUF | SDL_HWSURFACE | SDL_ANYFORMAT );
   if( NULL == g_screen ) {
      return 0;
   }

#else

   /* Create the main window. */
   g_window = SDL_CreateWindow( UNILAYER_WINDOW_TITLE,
      SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      g_screen_real_w, g_screen_real_h, SDL_WINDOW_RESIZABLE );
   if( NULL == g_window ) {
      return 0;
   }

   /* Create the renderer. */
   g_window_renderer = SDL_CreateRenderer(
      g_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE );
   if( NULL == g_window_renderer ) {
      return 0;
   }

   g_buffer_tex = SDL_CreateTexture( g_window_renderer,
      SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
      g_screen_real_w, g_screen_real_h );

#endif /* PLATFORM_SDL1 */

   srand( (unsigned int)time( &tm ) );

   return 1;
}

void graphics_platform_shutdown() {
#ifndef PLATFORM_SDL1
   SDL_DestroyWindow( g_window );
#endif /* PLATFORM_SDL1 */
   SDL_Quit();
}

void graphics_lock() {
#ifdef PLATFORM_SDL1
   /* SDL_LockSurface( g_screen ); */
   g_screen_locks++;
   assert( 1 == g_screen_locks );
#else
   SDL_SetRenderTarget( g_window_renderer, g_buffer_tex );
#endif /* PLATFORM_SDL1 */
}

void graphics_release() {

#ifdef PLATFORM_SDL1

   g_screen_locks--;
   /* SDL_UnlockSurface( g_screen ); */
   SDL_Flip( g_screen );

#else

   SDL_Rect d_r;
   uint8_t zoom_factor = 0;

   if( 0 != (GRAPHICS_FLAG_ZOOM_MASK & g_screen_flags) ) {
      /* Zoomed into the center of the screen. */
      zoom_factor = ((GRAPHICS_FLAG_ZOOM_MASK & g_screen_flags) >> 2);
      debug_printf( 3, "zoom: %02x", zoom_factor );
      d_r.x = 0 - ((zoom_factor * SCREEN_W) / 2);
      d_r.y = 0 - ((zoom_factor * SCREEN_H) / 2);
      d_r.w = ((zoom_factor + 1) * SCREEN_W);
      d_r.h = ((zoom_factor + 1) * SCREEN_H);

   } else if( 0 != (GRAPHICS_FLAG_SHAKING_MASK & g_screen_flags) ) {
      /* Random shaking. */
      d_r.x = rand() % (GRAPHICS_FLAG_SHAKING_MASK & g_screen_flags);
      d_r.y = rand() % (GRAPHICS_FLAG_SHAKING_MASK & g_screen_flags);
      d_r.w = g_screen_real_w;
      d_r.h = g_screen_real_h;
   } else {
      /* Normal stretch blit. */
      d_r.x = 0;
      d_r.y = 0;
      d_r.w = g_screen_real_w;
      d_r.h = g_screen_real_h;
   }

   /* Render the buffer texture to the main window. */
   SDL_SetRenderTarget( g_window_renderer, NULL );
   SDL_RenderCopyEx( g_window_renderer, g_buffer_tex, NULL, &d_r, 0, NULL, 0 );
   SDL_RenderPresent( g_window_renderer );
#endif /* PLATFORM_SDL1 */
}

int16_t graphics_get_random( int16_t start, int16_t range ) {
   return start + (rand() % range);
}

uint32_t graphics_get_ms() {
   return SDL_GetTicks();
}

void graphics_loop_start() {
   g_ms_start = SDL_GetTicks();
}

void graphics_loop_end() {
   int16_t delta = 0;
   
   do {
      delta = gc_ms_target - (SDL_GetTicks() - g_ms_start);
   } while( 0 < delta );  
}

void graphics_draw_px( uint16_t x, uint16_t y, const GRAPHICS_COLOR color ) {
#ifdef PLATFORM_SDL1
   int offset = 0;
   uint8_t* px_1 = NULL;
   uint16_t* px_2 = NULL;
   uint32_t* px_4 = NULL;

   offset = (y * target->surface->pitch) +
      (x * target->surface->format->BytesPerPixel);

   switch( target->surface->format->BytesPerPixel ) {
   case 4:
      px_4 = (uint32_t*)&(((uint8_t*)(target->surface->pixels))[offset]);
      *px_4 =
         SDL_MapRGB( target->surface->format, color->r, color->g, color->b );
      break;

   case 2:
      px_2 = (uint16_t*)&(((uint8_t*)(target->surface->pixels))[offset]);
      *px_2 =
         SDL_MapRGB( target->surface->format, color->r, color->g, color->b );
      break;

   case 1:
      px_1 = (uint8_t*)&(((uint8_t*)(target->surface->pixels))[offset]);
      *px_1 =
         SDL_MapRGB( target->surface->format, color->r, color->g, color->b );
      break;
   }
#else
   SDL_SetRenderDrawColor(
      g_window_renderer,  color->r, color->g, color->b, 255 );
   SDL_RenderDrawPoint( g_window_renderer, x, y );
#endif /* PLATFORM_SDL1 */
}

int16_t graphics_platform_blit_partial_at(
   const struct GRAPHICS_BITMAP* bmp, int16_t instance_id,
   uint16_t s_x, uint16_t s_y,
   uint16_t d_x, uint16_t d_y, uint16_t w, uint16_t h
) {
   SDL_Rect dest_rect = { d_x, d_y, w, h };
   SDL_Rect src_rect = { s_x, s_y, w, h };

   assert(
      NULL != bmp
#ifndef PLATFORM_SDL1
      && NULL != bmp->texture
#endif /* !PLATFORM_SDL1 */
   );

   resource_debug_printf( 0, "blitting to %d, %d x %d, %d...",
      bmp->id, d_x, d_y, w, h );

#ifdef PLATFORM_SDL1
   SDL_BlitSurface( bmp->surface, &src_rect, g_screen, &dest_rect );
#else
   SDL_RenderCopy( g_window_renderer, bmp->texture, &src_rect, &dest_rect );
#endif /* PLATFORM_SDL1 */

   return 1;
}

void graphics_draw_block(
   uint16_t x_orig, uint16_t y_orig, uint16_t w, uint16_t h,
   const GRAPHICS_COLOR color
) {
   SDL_Rect area;

   area.x = x_orig;
   area.y = y_orig;
   area.w = w;
   area.h = h;

#ifdef PLATFORM_SDL1
   SDL_FillRect( g_screen, &area, SDL_MapRGB(
      g_screen->format, color->r, color->g, color->b ) );
#else
   SDL_SetRenderDrawColor(
      g_window_renderer,  color->r, color->g, color->b, 255 );
   SDL_RenderFillRect( g_window_renderer, &area );
#endif /* PLATFORM_SDL1 */
}

#if !defined( PLATFORM_SDL1 ) && !defined( USE_SOFTWARE_LINES )

void graphics_draw_rect(
   uint16_t x_orig, uint16_t y_orig, uint16_t w, uint16_t h,
   uint16_t thickness, const GRAPHICS_COLOR color
) {
   SDL_Rect area;

   area.x = x_orig;
   area.y = y_orig;
   area.w = w;
   area.h = h;

   /* TODO: Handle thickness. */

   SDL_SetRenderDrawColor(
      g_window_renderer,  color->r, color->g, color->b, 255 );
   SDL_RenderDrawRect( g_window_renderer, &area );
}

void graphics_draw_line(
   uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t thickness,
   const GRAPHICS_COLOR color
) {
   int16_t i = 0;

   SDL_SetRenderDrawColor( g_window_renderer, color->r, color->g, color->b, 255 );
   /* TODO: Handle thickness. */
   for( i = 0 ; thickness > i ; i++ ) {
      SDL_RenderDrawLine( g_window_renderer,
         (x1 + i),
         (y1 + i),
         (x2 + i),
         (y2 + i) );
   }
}

#endif /* !PLATFORM_SDL1 && !USE_SOFTWARE_LINES */

int16_t graphics_platform_load_bitmap(
   RESOURCE_HANDLE res_handle, struct GRAPHICS_BITMAP* b
) {
   uint8_t* buffer = NULL;
   int16_t retval = 1;
   uint32_t buffer_sz = 0;
   SDL_RWops* bmp_stream;
#ifdef DEPTH_VGA
   int cc_flags = 0;
#endif /* DEPTH_VGA */
#ifdef PLATFORM_SDL1
   SDL_Surface* tmp_surface = NULL;
#endif /* PLATFORM_SDL1 */

   assert( NULL != b );
   assert( 0 == b->ref_count );

   buffer_sz = resource_sz_handle( res_handle );
   buffer = resource_lock_handle( res_handle );

   /* Parse buffered resource into SDL. */
   debug_printf( 0, "SDL loading surface for bitmap... (%d bytes)", buffer_sz );
   bmp_stream = SDL_RWFromMem( buffer, buffer_sz );

#ifdef PLATFORM_SDL1
   tmp_surface = SDL_LoadBMP_RW( bmp_stream, 1 ); /* Free stream on close. */
   if( NULL == tmp_surface ) {
      error_printf( "SDL unable to load bitmap: %s", SDL_GetError() );
      retval = 0;
      goto cleanup;
   }

   b->surface = SDL_DisplayFormat( tmp_surface );

#  ifdef DEPTH_VGA
   cc_flags = SDL_RLEACCEL | SDL_SRCCOLORKEY;
#  endif /* DEPTH_VGA */
#else
   b->surface = SDL_LoadBMP_RW( bmp_stream, 1 ); /* Free stream on close. */

#  ifdef DEPTH_VGA
   cc_flags = SDL_TRUE;
#  endif /* DEPTH_VGA */
#endif /* PLATFORM_SDL1 */

   if( NULL == b->surface ) {
      error_printf( "SDL unable to load bitmap: %s", SDL_GetError() );
      retval = 0;
      goto cleanup;
   }

#ifdef DEPTH_VGA
   /* Setup transparency if we're using VGA. */
   /* TODO: Color key from constants. */
   SDL_SetColorKey( b->surface, cc_flags, SDL_MapRGB( b->surface->format,
      GRAPHICS_TXP_R, GRAPHICS_TXP_G, GRAPHICS_TXP_B ) );
#endif /* DEPTH_VGA */

#ifndef PLATFORM_SDL1
   b->texture = SDL_CreateTextureFromSurface( g_window_renderer, b->surface );
   if( NULL == b->texture ) {
      error_printf( "SDL unable to create texture: %s", SDL_GetError() );
      retval = 0;
      if( NULL != b->surface ) {
         SDL_FreeSurface( b->surface );
         b->surface = NULL;
      }
      goto cleanup;
   }
   debug_printf( 1, "SDL created texture for bitmap" );
#endif /* !PLATFORM_SDL1 */

cleanup:

#ifdef PLATFORM_SDL1
   if( NULL != tmp_surface ) {
      SDL_FreeSurface( tmp_surface );
   }
#endif /* PLATFORM_SDL1 */

   if( NULL != buffer ) {
      buffer = resource_unlock_handle( res_handle );
   }

   if( NULL != res_handle ) {
      resource_free_handle( res_handle );
   }

   return retval;
}

/*
 * @return 1 if bitmap is unloaded and 0 otherwise.
 */
int16_t graphics_platform_unload_bitmap( struct GRAPHICS_BITMAP* b ) {
   if( NULL == b ) {
      return 0;
   }
   b->ref_count--;
   if( 0 == b->ref_count ) {
      resource_debug_printf( 2, "unloading texture and surface for %s",
         b->id, "bitmap" );
#ifndef PLATFORM_SDL1
      SDL_DestroyTexture( b->texture );
      b->texture = NULL;
#endif /* !PLATFORM_SDL1 */
      SDL_FreeSurface( b->surface );
      b->surface = NULL;
      b->flags &= ~GRAPHICS_BMP_FLAG_INIT;
      return 1;
   } else {
      resource_debug_printf( 2, "bitmap ref count greater than zero for %s",
         b->id, "bitmap" );
   }
   return 0;
}

