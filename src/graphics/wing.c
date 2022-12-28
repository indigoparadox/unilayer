
#define GRAPHICS_PLATFORM_C
#define WING_C
#include "../unilayer.h"

/* For rand() */
#include <time.h>
#include <stdlib.h>

#include <string.h>

extern uint8_t g_running;
extern uint8_t g_last_key;
extern HINSTANCE g_instance;
extern HWND g_window;
extern MEMORY_HANDLE g_state_handle;

struct GRAPHICS_BITMAP g_screen;
static HDC g_hdc_buffer = (HDC)NULL;
static HBITMAP g_old_hbm_buffer = (HBITMAP)NULL;
volatile uint32_t g_ms;

struct GRAPHICS_ARGS g_graphics_args;

const uint32_t gc_ms_target = 1000 / FPS;
static uint32_t g_ms_start = 0; 

#define bmp_int( type, buf, offset ) *((type*)&(buf[offset]))

#define bmp_get_hdc( bmp, hdc, old_hbm, hdc_master, cleanup ) hdc = CreateCompatibleDC( hdc_master ); if( (HDC)NULL == hdc ) { error_printf( "HDC is NULL" ); goto cleanup; } old_hbm = SelectObject( hdc, bmp );

#define bmp_cleanup_hdc( hdc, old_hbm ) if( (HDC)NULL != hdc ) { SelectObject( hdc, old_hbm ); DeleteDC( hdc ); hdc = (HDC)NULL; }

#define bmp_create_pen( pen, thickness, color, hdc, old_pen, cleanup ) pen = CreatePen( PS_SOLID, thickness, color ); if( (HPEN)NULL == pen ) { error_printf( "pen is NULL" ); goto cleanup; } old_pen = SelectObject( hdc, pen );

#define bmp_cleanup_pen( pen, hdc, old_pen ); if( (HPEN)NULL != pen ) { SelectObject( hdc, old_pen ); DeleteObject( pen ); pen = (HPEN)NULL; }

static LRESULT CALLBACK WndProc(
   HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam
) {
   PAINTSTRUCT ps;
   HDC hdc_screen = (HDC)NULL;
   BITMAP srcBitmap;

   switch( message ) {
      case WM_CREATE:
         break;

      case WM_PAINT:

         /* Create HDC for screen. */
         hdc_screen = BeginPaint( hWnd, &ps );
         if( (HDC)NULL == hdc_screen ) {
            error_printf( "screen HDC is NULL" );
            break;
         }

         if( 0 == g_screen.initialized ) {
            debug_printf( 2, "creating screen buffer..." );
            g_screen.bitmap = CreateCompatibleBitmap( hdc_screen,
               SCREEN_W, SCREEN_H );
            g_screen.initialized = 1;
            bmp_get_hdc(
               g_screen.bitmap, g_hdc_buffer, g_old_hbm_buffer,
               (HDC)NULL, cleanup_paint );
         }
         if( (HBITMAP)NULL == g_screen.bitmap ) {
            error_printf( "screen buffer bitmap is NULL" );
            break;
         }

         /* Load parameters of the buffer into info object (srcBitmap). */
         GetObject( g_screen.bitmap, sizeof( BITMAP ), &srcBitmap );

         debug_printf( 0, "blitting buffer bitmap (%d x %d)",
            srcBitmap.bmWidth, srcBitmap.bmHeight );

         StretchBlt(
            hdc_screen,
            0, 0,
            g_screen_real_w, g_screen_real_h,
            g_hdc_buffer,
            0, 0,
            srcBitmap.bmWidth,
            srcBitmap.bmHeight,
            SRCCOPY
         );

      cleanup_paint:

         DeleteDC( hdc_screen );

         EndPaint( hWnd, &ps );
         break;

      case WM_ERASEBKGND:
         return 1;

      case WM_KEYDOWN:
         g_last_key = wParam;
         break;

      case WM_DESTROY:
         if( 0 != g_screen.initialized ) {
            debug_printf( 2, "destroying screen buffer..." );
            DeleteObject( g_screen.bitmap );
         }
         PostQuitMessage( 0 );
         break;

      case WM_SIZE:
         graphics_on_resize( LOWORD( lParam ), HIWORD( lParam ) );
         break;

      case WM_TIMER:
         g_running = g_loop_iter( g_loop_data );

         /* Kind of a hack so that we can have a cheap timer. */
         g_ms += 100;
         break;

      default:
         return DefWindowProc( hWnd, message, wParam, lParam );
   }

   return 0;
}

int16_t graphics_platform_init() {
   WNDCLASS wc = { 0 };
   RECT wr = { 0, 0, 0, 0 };

   memset( &g_screen, '\0', sizeof( struct GRAPHICS_BITMAP ) );
   memset( &wc, '\0', sizeof( WNDCLASS ) );

   wc.lpfnWndProc   = (WNDPROC)&WndProc;
   wc.hInstance     = g_instance;
   wc.hIcon         = LoadIcon( g_instance, MAKEINTRESOURCE(
      g_graphics_args.icon_res ) );
   wc.hCursor       = LoadCursor( 0, IDC_ARROW );
   wc.hbrBackground = (HBRUSH)( COLOR_BTNFACE + 1 );
   /* wc.lpszMenuName  = MAKEINTRESOURCE( IDR_MAINMENU ); */
   wc.lpszClassName = UNILAYER_WINDOW_CLASS "WindowClass";

   if( !RegisterClass( &wc ) ) {
      error_printf( "unable to register main window class" );
      return 0;
   }

   /* Get client area size. */
   wr.right = g_screen_real_w;
   wr.bottom = g_screen_real_h;
   AdjustWindowRect( &wr, WS_OVERLAPPEDWINDOW, FALSE );

   g_window = CreateWindowEx(
      0, UNILAYER_WINDOW_CLASS "WindowClass", UNILAYER_WINDOW_TITLE,
      WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT,
      wr.right - wr.left, wr.bottom - wr.top, 0, 0, g_instance, 0
   );

   if( !g_window ) {
      error_printf( "unable to create main window" );
      return 0;
   }

   assert( (HWND)NULL != g_window );

   if( !SetTimer( g_window, WIN_GFX_TIMER_ID, (int)(1000 / FPS), NULL ) ) {
      error_printf( "could not set graphics update timer\n" );
      return 0;
   }
   debug_printf( 2, "set graphics update timer %d to %dms",
      WIN_GFX_TIMER_ID, (int)(1000 / FPS) );

   srand( (unsigned int)time( NULL ) );

   ShowWindow( g_window, g_graphics_args.cmd_show );

   return 1;
}

void graphics_platform_shutdown() {
   bmp_cleanup_hdc( g_hdc_buffer, g_old_hbm_buffer );
}

void graphics_lock() {

}

void graphics_release() {
   /* The closest analog to the actual graphics_flip(): */
   if( (HWND)NULL != g_window ) {
      InvalidateRect( g_window, 0, TRUE );
   }
}

int16_t graphics_get_random( int16_t start, int16_t range ) {
   return start + (rand() % range);
}

uint32_t graphics_get_ms() {
   return g_ms;
}

void graphics_loop_start() {
}

void graphics_loop_end() {
}

int16_t graphics_platform_blit_partial_at(
   const struct GRAPHICS_BITMAP* bmp,
   uint16_t s_x, uint16_t s_y, uint16_t d_x, uint16_t d_y,
   uint16_t w, uint16_t h
) {
   HDC hdc_src = (HDC)NULL;
   HBITMAP old_hbm_src = (HBITMAP)NULL;
#ifdef DEPTH_VGA
   HDC hdc_src_mask = (HDC)NULL;
   HBITMAP old_hbm_src_mask = (HBITMAP)NULL;
#endif /* DEPTH_VGA */

   if( (struct GRAPHICS_BITMAP*)NULL == bmp || (HBITMAP)NULL == bmp->bitmap ) {
      error_printf( "blit bmp is NULL" );
      return 0;
   }

   debug_printf( 0, "blitting resource #%d to %d, %d x %d, %d...",
      bmp->id, d_x, d_y, w, h );

   /* Get the HDCs for the source and screen buffer. */
   bmp_get_hdc(
      bmp->bitmap, hdc_src, old_hbm_src, (HDC)NULL, cleanup ) 

#ifdef DEPTH_VGA

   /* Create HDC for source bitmap mask compatible with the buffer. */
   bmp_get_hdc(
      bmp->mask, hdc_src_mask, old_hbm_src_mask, (HDC)NULL, cleanup ) 

   /* Use mask to blit transparency. */
   BitBlt( g_hdc_buffer, d_x, d_y, w, h, hdc_src_mask, s_x, s_y, SRCAND );
   BitBlt( g_hdc_buffer, d_x, d_y, w, h, hdc_src, s_x, s_y, SRCPAINT );

#else

   /* Simple blit without transparency. */
   BitBlt( g_hdc_buffer, d_x, d_y, w, h, hdc_src, s_x, s_y, SRCCOPY );

#endif /* DEPTH_VGA */

cleanup:

   bmp_cleanup_hdc( hdc_src, old_hbm_src );

#ifdef DEPTH_VGA
   bmp_cleanup_hdc( hdc_src_mask, old_hbm_src_mask );
#endif /* DEPTH_VGA */

   return 1;
}

void graphics_draw_px( uint16_t x, uint16_t y, const GRAPHICS_COLOR color ) {
   SetPixel( g_hdc_buffer, x, y, color );
}

void graphics_draw_block(
   uint16_t x_orig, uint16_t y_orig, uint16_t w, uint16_t h,
   const GRAPHICS_COLOR color
) {
   RECT rect;
   HBRUSH brush = (HBRUSH)NULL;

   rect.left = x_orig;
   rect.top = y_orig;
   rect.right = x_orig + w;
   rect.bottom = y_orig + h;

   brush = CreateSolidBrush( color );
   if( (HBRUSH)NULL == brush ) {
      error_printf( "brush is NULL" );
      /* TODO: Cleanup! */
      return;
   }

   FillRect( g_hdc_buffer, &rect, brush );

   if( (HBRUSH)NULL != brush ) {
      DeleteObject( brush );
   }
}

void graphics_draw_rect(
   uint16_t x_orig, uint16_t y_orig, uint16_t w, uint16_t h,
   uint16_t thickness, const GRAPHICS_COLOR color
) {
   HBITMAP old_hbm_buffer = (HBITMAP)NULL;
   HPEN pen = (HPEN)NULL;
   HPEN old_pen = (HPEN)NULL;
   POINT points[5];

   /* TODO: Standardize this to use GDI Rectangle(). */

#ifndef PLATFORM_WINCE /* TODO */

   bmp_create_pen( pen, thickness, color, g_hdc_buffer, old_pen, cleanup );

   /* MoveTo( hdc_buffer, x1, y1 );
   LineTo( hdc_buffer, x2, y2 );*/
   points[0].x = (x_orig);
   points[0].y = (y_orig);
   
   points[1].x = (x_orig);
   points[1].y = (y_orig + h);

   points[2].x = (x_orig + w);
   points[2].y = (y_orig + h);
   
   points[3].x = (x_orig + w);
   points[3].y = (y_orig);

   points[4].x = (x_orig);
   points[4].y = (y_orig);
   Polyline( g_hdc_buffer, points, 5 );

cleanup:

   bmp_cleanup_pen( pen, g_hdc_buffer, old_pen );

#endif /* PLATFORM_WINCE */
}

void graphics_draw_line(
   uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
   uint16_t thickness, const GRAPHICS_COLOR color
) {
   HPEN pen = (HPEN)NULL;
   HPEN old_pen = (HPEN)NULL;
   POINT points[2];

#ifndef PLATFORM_WINCE /* TODO */

   bmp_create_pen( pen, thickness, color, g_hdc_buffer, old_pen, cleanup );

   /* MoveTo( hdc_buffer, x1, y1 );
   LineTo( hdc_buffer, x2, y2 );*/
   points[0].x = x1;
   points[0].y = y1;
   points[1].x = x2;
   points[1].y = y2;
   Polyline( g_hdc_buffer, points, 2 );

cleanup:

   bmp_cleanup_pen( pen, g_hdc_buffer, old_pen );

#endif /* PLATFORM_WINCE */
}

#ifdef DEPTH_VGA

static int16_t graphics_platform_load_bitmap_trans(
   struct GRAPHICS_BITMAP* b
) {
   HDC hdc_b = (HDC)NULL;
   HBITMAP old_hbm_b = (HBITMAP)NULL;
   BITMAP bm;
   uint32_t txp_color = 0;
   int16_t retval = 1;
#ifdef DEPTH_VGA
   HDC hdc_mask = (HDC)NULL;
   HBITMAP old_hbm_mask = (HBITMAP)NULL;
#endif /* DEPTH_VGA */

   /* Setup transparency. */

   GetObject( b->bitmap, sizeof( BITMAP ), &bm );

   b->mask = CreateBitmap( bm.bmWidth, bm.bmHeight, 1, 1, NULL );

   bmp_get_hdc( b->bitmap, hdc_b, old_hbm_b, (HDC)NULL, cleanup );
   bmp_get_hdc( b->mask, hdc_mask, old_hbm_mask, (HDC)NULL, cleanup );

   /* Convert the color key into bitmap format. */
   txp_color |= (GRAPHICS_TXP_B & 0xff);
   txp_color <<= 8;
   txp_color |= (GRAPHICS_TXP_G & 0xff);
   txp_color <<= 8;
   txp_color |= (GRAPHICS_TXP_R & 0xff);
   SetBkColor( hdc_b, txp_color );

   /* Create the mask from the color key. */
   BitBlt( hdc_mask, 0, 0, bm.bmWidth, bm.bmHeight, hdc_b, 0, 0, SRCCOPY );
   BitBlt( hdc_b, 0, 0, bm.bmWidth, bm.bmHeight, hdc_mask, 0, 0, SRCINVERT );

cleanup:

   bmp_cleanup_hdc( hdc_mask, old_hbm_mask );
   bmp_cleanup_hdc( hdc_b, old_hbm_b );

   return retval;
}

#endif /* DEPTH_VGA */

int16_t graphics_platform_load_bitmap(
   RESOURCE_HANDLE res_handle, struct GRAPHICS_BITMAP* b
) {
   int16_t retval = 1;
#ifdef PLATFORM_WIN16
   HDC hdc = (HDC)NULL;
#ifdef RESOURCE_FILE
   int32_t i, x, y, w, h, bpp, offset;
   char* buf = NULL;
   BITMAPINFO* bmi = NULL;
#endif /* RESOURCE_FILE */
#endif /* PLATFORM_WIN16 */

   /* TODO: Fix this if RESOURCE_FILE is used. */

   /* Load resource into bitmap. */
   if( (RESOURCE_HANDLE)NULL != res_handle ) {
      /* TODO: Handle non-Windows resources. */
#ifdef RESOURCE_FILE

#  ifdef PLATFORM_WIN16
      buf = resource_lock_handle( res_handle );

      bmi = (BITMAPINFO*)&(buf[sizeof( BITMAPFILEHEADER )]);

      bpp = bmp_int( uint16_t, buf, 28 );
      offset = bmp_int( uint32_t, buf, 10 );
      /*
      debug_printf( 2, "bitmap is %d x %d, %d bpp",
         bmih->biWidth, bmih->biHeight, bpp );
      */

      assert( 0 < bmi->bmiHeader.biWidth );
      assert( 0 < bmi->bmiHeader.biHeight );
      assert( 0 == bmi->bmiHeader.biWidth % 8 );
      assert( 0 == bmi->bmiHeader.biHeight % 8 );

      hdc = GetDC( g_window );
      b->bitmap = CreateCompatibleBitmap( hdc,
         bmi->bmiHeader.biWidth, bmi->bmiHeader.biHeight );

      SetDIBits( hdc, b->bitmap, 0,
         bmi->bmiHeader.biHeight, &(buf[offset]), bmi,
         DIB_RGB_COLORS );

      buf = resource_unlock_handle( res_handle );

      ReleaseDC( g_window, hdc );

      resource_free_handle( res_handle );
#  else
      b->bitmap = LoadImage( NULL, b->id, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE );
#  endif /* PLATFORM_WIN16 */

#else
      b->bitmap = res_handle;
#endif /* RESOURCE_FILE */
      /* free( res_handle ); */
   } else {
      error_printf( "NULL handle returned" );
      retval = 0;
      goto cleanup;
   }
   if( !b->bitmap ) {
      error_printf( "unable to load resource" );
      retval = 0;
      goto cleanup;
   }

#ifdef DEPTH_VGA
   graphics_platform_load_bitmap_trans( b );
#endif /* DEPTH_VGA */

cleanup:

   return 1;
}

int16_t graphics_platform_unload_bitmap( struct GRAPHICS_BITMAP* b ) {
   if( NULL == b ) {
      return 0;
   }
   b->ref_count--;
   if( 0 == b->ref_count ) {
      debug_printf( 2, "unloading bitmap resource %d", b->id );
      b->initialized = 0;
      DeleteObject( b->bitmap );
      return 1;
   }
   return 0;
}

#ifndef USE_SOFTWARE_TEXT

void graphics_string_sz(
   const char* str, uint16_t str_sz, uint8_t flags, struct GRAPHICS_RECT* sz_out
) {
   SIZE sz;
   int16_t str_len = 0;

   str_len = memory_strnlen_ptr( str, str_sz );
   GetTextExtentPoint( g_hdc_buffer, str, str_len, &sz );
   sz_out->w = sz.cx;
   sz_out->h = sz.cy;

}

void graphics_char_at(
   char c, uint16_t x_orig, uint16_t y_orig, GRAPHICS_COLOR color,
   uint8_t flags
) {
   /* TODO */
}

void graphics_string_at(
   const char* str, uint16_t str_sz, uint16_t x_orig, uint16_t y_orig,
   GRAPHICS_COLOR color, uint8_t flags
) {
   RECT rect;
   SIZE sz;
   int16_t str_len = 0;

   memory_zero_ptr( &sz, sizeof( SIZE ) );

   str_len = memory_strnlen_ptr( str, str_sz );
   GetTextExtentPoint( g_hdc_buffer, str, str_len, &sz );
   rect.left = x_orig;
   rect.top = y_orig - 4; /* Move text 4px up or it looks weird. */
   rect.right = (x_orig + sz.cx);
   rect.bottom = (y_orig + sz.cy);

   SetTextColor( g_hdc_buffer, color );
   SetBkMode( g_hdc_buffer, TRANSPARENT );

   if( 0 == DrawText( g_hdc_buffer, str, str_len, &rect, 0 ) ) {
      error_printf( "unable to draw string at %u, %u", x_orig, y_orig );
   }

   SetBkMode( g_hdc_buffer, OPAQUE );
   SetTextColor( g_hdc_buffer, GRAPHICS_COLOR_BLACK );

}

#endif /* !USE_SOFTWARE_TEXT */

