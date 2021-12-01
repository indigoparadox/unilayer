
#define GRAPHICS_C
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
volatile uint32_t g_ms;

struct GRAPHICS_ARGS g_graphics_args;

const uint32_t gc_ms_target = 1000 / FPS;
static uint32_t g_ms_start = 0; 

#define bmp_int( type, buf, offset ) *((type*)&(buf[offset]))

static LRESULT CALLBACK WndProc(
   HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam
) {
   PAINTSTRUCT ps;
   HDC hdcScreen;
   HDC hdcBuffer;
   BITMAP srcBitmap;
   HBITMAP oldHbm;

   switch( message ) {
      case WM_CREATE:
         break;

      case WM_PAINT:

         /* Create HDC for screen. */
         hdcScreen = BeginPaint( hWnd, &ps );
         if( (HDC)NULL == hdcScreen ) {
            error_printf( "screen HDC is NULL" );
            break;
         }

         if( 0 == g_screen.initialized ) {
            debug_printf( 2, "creating screen buffer..." );
            g_screen.bitmap = CreateCompatibleBitmap( hdcScreen,
               SCREEN_W, SCREEN_H );
            g_screen.initialized = 1;
         }
         if( (HBITMAP)NULL == g_screen.bitmap ) {
            error_printf( "screen buffer bitmap is NULL" );
            break;
         }

         /* Create a new HDC for buffer and select buffer into it. */
         hdcBuffer = CreateCompatibleDC( hdcScreen );
         if( (HDC)NULL == hdcBuffer ) {
            error_printf( "screen buffer HDC is NULL" );
            break;
         }
         oldHbm = SelectObject( hdcBuffer, g_screen.bitmap );

         /* Load parameters of the buffer into info object (srcBitmap). */
         GetObject( g_screen.bitmap, sizeof( BITMAP ), &srcBitmap );

         debug_printf( 0, "blitting buffer bitmap (%d x %d)",
            srcBitmap.bmWidth, srcBitmap.bmHeight );

         StretchBlt(
            hdcScreen,
            0, 0,
            SCREEN_REAL_W, SCREEN_REAL_H,
            hdcBuffer,
            0, 0,
            srcBitmap.bmWidth,
            srcBitmap.bmHeight,
            SRCCOPY
         );

         /* Return the default object into the HDC. */
         SelectObject( hdcScreen, oldHbm );

         DeleteDC( hdcBuffer );
         DeleteDC( hdcScreen );

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
   MSG msg;
   WNDCLASS wc = { 0 };

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

   g_window = CreateWindowEx(
      0, UNILAYER_WINDOW_CLASS "WindowClass", UNILAYER_WINDOW_TITLE,
      WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
      CW_USEDEFAULT, CW_USEDEFAULT,
      SCREEN_REAL_W, SCREEN_REAL_H, 0, 0, g_instance, 0
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
}

void graphics_flip() {
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

int16_t graphics_platform_blit_at(
   const struct GRAPHICS_BITMAP* bmp,
   uint16_t x, uint16_t y, uint16_t w, uint16_t h
) {
   /* TODO: Delete. */
#if 0
   HDC hdcBuffer = (HDC)NULL;
   HDC hdcSrc = (HDC)NULL;
   BITMAP srcBitmap;
   HBITMAP oldHbmSrc = (HBITMAP)NULL;
   HBITMAP oldHbmBuffer = (HBITMAP)NULL;
   HBITMAP oldHbmSrcMask = (HBITMAP)NULL;

   if( (struct GRAPHICS_BITMAP*)NULL == bmp || (HBITMAP)NULL == bmp->bitmap ) {
      error_printf( "blit bmp is NULL" );
      return 0;
   }

   debug_printf( 0, "blitting resource #%d to %d, %d x %d, %d...",
      bmp->id, x, y, w, h );

   /* Create HDC for the off-screen buffer to blit to. */
   hdcBuffer = CreateCompatibleDC( (HDC)NULL );
   oldHbmBuffer = SelectObject( hdcBuffer, g_screen.bitmap );

   /* Create HDC for source bitmap compatible with the buffer. */
   hdcSrc = CreateCompatibleDC( (HDC)NULL );
   oldHbmSrc = SelectObject( hdcSrc, bmp->bitmap );

   GetObject( bmp->bitmap, sizeof( BITMAP ), &srcBitmap );

   /*
   BitBlt(
      hdcBuffer,
      x, y, w, h,
      hdcSrc,
      0, 0,
      SRCCOPY
   );
   */

   StretchBlt(
      hdcBuffer,
      x * SCREEN_SCALE, y * SCREEN_SCALE,
      w * SCREEN_SCALE,
      h * SCREEN_SCALE,
      hdcSrc,
      0, 0,
      w,
      h,
      SRCCOPY
   );

   /* Reselect the initial objects into the provided DCs. */
   SelectObject( hdcSrc, oldHbmSrc );
   SelectObject( hdcBuffer, oldHbmBuffer );

   DeleteDC( hdcSrc );
   DeleteDC( hdcBuffer );
#endif
   return 0;
}

int16_t graphics_platform_blit_partial_at(
   const struct GRAPHICS_BITMAP* bmp,
   uint16_t s_x, uint16_t s_y, uint16_t d_x, uint16_t d_y,
   uint16_t w, uint16_t h
) {
   HDC hdcBuffer = (HDC)NULL;
   HDC hdcSrc = (HDC)NULL;
   /* BITMAP srcBitmap;
   BITMAP srcBitmapMask; */
   HBITMAP oldHbmSrc = (HBITMAP)NULL;
   HBITMAP oldHbmBuffer = (HBITMAP)NULL;
#ifdef DEPTH_VGA
   HDC hdcSrcMask = (HDC)NULL;
   HBITMAP oldHbmSrcMask = (HBITMAP)NULL;
#endif /* DEPTH_VGA */

   if( (struct GRAPHICS_BITMAP*)NULL == bmp || (HBITMAP)NULL == bmp->bitmap ) {
      error_printf( "blit bmp is NULL" );
      return 0;
   }

   debug_printf( 0, "blitting resource #%d to %d, %d x %d, %d...",
      bmp->id, d_x, d_y, w, h );

   /* Create HDC for the off-screen buffer to blit to. */
   hdcBuffer = CreateCompatibleDC( (HDC)NULL );
   oldHbmBuffer = SelectObject( hdcBuffer, g_screen.bitmap );

   /* Create HDC for source bitmap compatible with the buffer. */
   hdcSrc = CreateCompatibleDC( (HDC)NULL );
   oldHbmSrc = SelectObject( hdcSrc, bmp->bitmap );

   /* GetObject( bmp->bitmap, sizeof( BITMAP ), &srcBitmap );
   GetObject( bmp->mask, sizeof( BITMAP ), &srcBitmapMask ); */

#ifdef DEPTH_VGA

   /* Create HDC for source bitmap mask compatible with the buffer. */
   hdcSrcMask = CreateCompatibleDC( (HDC)NULL );
   oldHbmSrcMask = SelectObject( hdcSrcMask, bmp->mask );

   /* Use mask to blit transparency. */

   BitBlt(
      hdcBuffer,
      d_x, d_y, w, h,
      hdcSrcMask,
      s_x, s_y,
      SRCAND
   );

   BitBlt(
      hdcBuffer,
      d_x, d_y, w, h,
      hdcSrc,
      s_x, s_y,
      SRCPAINT
   );

   /* Reselect the initial objects into the provided DC. */
   SelectObject( hdcSrcMask, oldHbmSrcMask );
   DeleteDC( hdcSrcMask );

#else

   /* Simple blit without transparency. */
   BitBlt(
      hdcBuffer,
      d_x, d_y, w, h,
      hdcSrc,
      s_x, s_y,
      SRCCOPY
   );

#endif /* DEPTH_VGA */

   /* Reselect the initial objects into the provided DCs. */
   SelectObject( hdcSrc, oldHbmSrc );
   SelectObject( hdcBuffer, oldHbmBuffer );

   DeleteDC( hdcSrc );
   DeleteDC( hdcBuffer );

   return 1;
}

void graphics_draw_px( uint16_t x, uint16_t y, const GRAPHICS_COLOR color ) {
   HDC hdcBuffer = (HDC)NULL;
   HBITMAP oldHbmBuffer = (HBITMAP)NULL;

   /* Create HDC for the off-screen buffer to blit to. */
   hdcBuffer = CreateCompatibleDC( (HDC)NULL );
   if( (HDC)NULL == hdcBuffer ) {
      error_printf( "screen buffer HDC is NULL" );
      return;
   }
   oldHbmBuffer = SelectObject( hdcBuffer, g_screen.bitmap );

   SetPixel( hdcBuffer, x, y, color );

   /* Reselect the initial objects into the provided DCs. */
   SelectObject( hdcBuffer, oldHbmBuffer );
   DeleteDC( hdcBuffer );
}

void graphics_draw_block(
   uint16_t x_orig, uint16_t y_orig, uint16_t w, uint16_t h,
   const GRAPHICS_COLOR color
) {
   HDC hdcBuffer = (HDC)NULL;
   HBITMAP oldHbmBuffer = (HBITMAP)NULL;
   RECT rect;
   HBRUSH brush = (HBRUSH)NULL;

   rect.left = x_orig;
   rect.top = y_orig;
   rect.right = x_orig + w;
   rect.bottom = y_orig + h;

   /* Create HDC for the off-screen buffer to blit to. */
   hdcBuffer = CreateCompatibleDC( (HDC)NULL );
   if( (HDC)NULL == hdcBuffer ) {
      error_printf( "screen buffer HDC is NULL" );
      return;
   }
   oldHbmBuffer = SelectObject( hdcBuffer, g_screen.bitmap );

   brush = CreateSolidBrush( color );
   if( (HBRUSH)NULL == brush ) {
      error_printf( "brush is NULL" );
      return;
   }

   FillRect( hdcBuffer, &rect, brush );

   DeleteObject( brush );

   /* Reselect the initial objects into the provided DCs. */
   SelectObject( hdcBuffer, oldHbmBuffer );
   DeleteDC( hdcBuffer );
}

void graphics_draw_rect(
   uint16_t x_orig, uint16_t y_orig, uint16_t w, uint16_t h,
   uint16_t thickness, const GRAPHICS_COLOR color
) {
   HDC hdcBuffer = (HDC)NULL;
   HBITMAP oldHbmBuffer = (HBITMAP)NULL;
   HPEN pen = (HPEN)NULL;
   HPEN oldPen = (HPEN)NULL;
   POINT points[5];

#ifndef PLATFORM_WINCE /* TODO */
   /* Create HDC for the off-screen buffer to blit to. */
   hdcBuffer = CreateCompatibleDC( (HDC)NULL );
   if( (HDC)NULL == hdcBuffer ) {
      error_printf( "screen buffer HDC is NULL" );
      return;
   }
   oldHbmBuffer = SelectObject( hdcBuffer, g_screen.bitmap );

   pen = CreatePen( PS_SOLID, thickness, color );
   if( (HPEN)NULL == pen ) {
      error_printf( "pen is NULL" );
      return;
   }
   oldPen = SelectObject( hdcBuffer, pen );
   /* MoveTo( hdcBuffer, x1, y1 );
   LineTo( hdcBuffer, x2, y2 );*/
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
   Polyline( hdcBuffer, points, 5 );
   SelectObject( hdcBuffer, oldPen );
   DeleteObject( pen );

   /* Reselect the initial objects into the provided DCs. */
   SelectObject( hdcBuffer, oldHbmBuffer );
   DeleteDC( hdcBuffer );
#endif /* PLATFORM_WINCE */
}

void graphics_draw_line(
   uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
   uint16_t thickness, const GRAPHICS_COLOR color
) {
   HDC hdcBuffer = (HDC)NULL;
   HBITMAP oldHbmBuffer = (HBITMAP)NULL;
   HPEN pen = (HPEN)NULL;
   HPEN oldPen = (HPEN)NULL;
   POINT points[2];

#ifndef PLATFORM_WINCE /* TODO */
   /* Create HDC for the off-screen buffer to blit to. */
   hdcBuffer = CreateCompatibleDC( (HDC)NULL );
   oldHbmBuffer = SelectObject( hdcBuffer, g_screen.bitmap );

   pen = CreatePen( PS_SOLID, thickness, color );
   if( (HPEN)NULL == pen ) {
      error_printf( "pen is NULL" );
      return;
   }
   oldPen = SelectObject( hdcBuffer, pen );
   /* MoveTo( hdcBuffer, x1, y1 );
   LineTo( hdcBuffer, x2, y2 );*/
   points[0].x = x1;
   points[0].y = y1;
   points[1].x = x2;
   points[1].y = y2;
   Polyline( hdcBuffer, points, 2 );
   SelectObject( hdcBuffer, oldPen );
   DeleteObject( pen );

   /* Reselect the initial objects into the provided DCs. */
   SelectObject( hdcBuffer, oldHbmBuffer );
   DeleteDC( hdcBuffer );
#endif /* PLATFORM_WINCE */
}

int16_t graphics_platform_load_bitmap(
   RESOURCE_HANDLE res_handle, struct GRAPHICS_BITMAP* b
) {
   uint8_t* buffer = NULL;
   int16_t retval = 1;
   int32_t buffer_sz = 0;
   uint32_t txp_color = 0;
   HDC hdc;
   HDC hdc_mask;
   BITMAP bm;
#ifdef RESOURCE_FILE
   int i, x, y, w, h, bpp, offset;
   char* buf = NULL;
   BITMAPINFO* bmi = NULL;
#endif /* RESOURCE_FILE */

   /* TODO: Fix this if RESOURCE_FILE is used. */

   /* Load resource into bitmap. */
   if( (RESOURCE_HANDLE)NULL != res_handle ) {
      /* TODO: Handle non-Windows resources. */
#ifdef RESOURCE_FILE

#ifdef PLATFORM_WIN16
      buf = memory_lock( res_handle );

      bmi = (BITMAPINFO*)&(buf[sizeof( BITMAPFILEHEADER )]);

      /*
      for( i = 0 ; sizeof( BITMAPFILEHEADER ) > i ; i++ ) {
         printf( "0x%02x ", buf[sizeof( BITMAPFILEHEADER ) + i] );
      }
      */
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

      /*
      hdc = CreateCompatibleDC( NULL );
      */
      hdc = GetDC( g_window );
      b->bitmap = CreateCompatibleBitmap( hdc,
         bmi->bmiHeader.biWidth, bmi->bmiHeader.biHeight );

      SetDIBits( hdc, b->bitmap, 0,
         bmi->bmiHeader.biHeight, &(buf[offset]), bmi,
         DIB_RGB_COLORS );
      /*
      SetBitmapBits( b->bitmap, bmih->biSizeImage, &(buf[offset]) );
      */

      buf = memory_unlock( res_handle );

      ReleaseDC( g_window, hdc );

      resource_free_handle( res_handle );
#else
      b->bitmap = LoadImage( NULL, b->id, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE );
#endif /* PLATFORM_WIN16 */

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

   /* Setup transparency. */

   GetObject( b->bitmap, sizeof( BITMAP ), &bm );

   b->mask = CreateBitmap( bm.bmWidth, bm.bmHeight, 1, 1, NULL );

   hdc = CreateCompatibleDC( 0 );
   hdc_mask = CreateCompatibleDC( 0 );

   SelectObject( hdc, b->bitmap );
   SelectObject( hdc_mask, b->mask );

   /* Convert the color key into bitmap format. */
   txp_color |= (GRAPHICS_TXP_B & 0xff);
   txp_color <<= 8;
   txp_color |= (GRAPHICS_TXP_G & 0xff);
   txp_color <<= 8;
   txp_color |= (GRAPHICS_TXP_R & 0xff);
   SetBkColor( hdc, txp_color );

   /* Create the mask from the color key. */
   BitBlt( hdc_mask, 0, 0, bm.bmWidth, bm.bmHeight, hdc, 0, 0, SRCCOPY );
   BitBlt( hdc, 0, 0, bm.bmWidth, bm.bmHeight, hdc_mask, 0, 0, SRCINVERT );

   DeleteDC( hdc );
   DeleteDC( hdc_mask );

   /* TODO: Restore old HDC contents. */

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
   HDC hdcBuffer = NULL;
   HBITMAP oldHbmBuffer = NULL;
   SIZE sz;
   int16_t str_len = 0;

   hdcBuffer = CreateCompatibleDC( NULL );
   if( NULL == hdcBuffer ) {
      error_printf( "screen buffer HDC is NULL" );
      return;
   }
   oldHbmBuffer = SelectObject( hdcBuffer, g_screen.bitmap );

   str_len = memory_strnlen_ptr( str, str_sz );
   GetTextExtentPoint( hdcBuffer, str, str_len, &sz );
   sz_out->w = sz.cx;
   sz_out->h = sz.cy;

   /* Reselect the initial objects into the provided DCs. */
   SelectObject( hdcBuffer, oldHbmBuffer );
   DeleteDC( hdcBuffer );
}

void graphics_string_at(
   const char* str, uint16_t str_sz, uint16_t x_orig, uint16_t y_orig,
   GRAPHICS_COLOR color, uint8_t flags
) {
   HDC hdcBuffer = NULL;
   HBITMAP oldHbmBuffer = NULL;
   RECT rect;
   SIZE sz;
   int16_t str_len = 0;

   assert( x_orig < SCREEN_W );
   assert( y_orig < SCREEN_H );

   memory_zero_ptr( &sz, sizeof( SIZE ) );

   /* Create HDC for the off-screen buffer to blit to. */
   hdcBuffer = CreateCompatibleDC( NULL );
   if( NULL == hdcBuffer ) {
      error_printf( "screen buffer HDC is NULL" );
      return;
   }
   oldHbmBuffer = SelectObject( hdcBuffer, g_screen.bitmap );

   str_len = memory_strnlen_ptr( str, str_sz );
   GetTextExtentPoint( hdcBuffer, str, str_len, &sz );
   rect.left = x_orig;
   rect.top = y_orig;
   rect.right = (x_orig + sz.cx);
   rect.bottom = (y_orig + sz.cy);

   if( 0 == DrawText( hdcBuffer, str, str_len, &rect, 0 ) ) {
      error_printf( "unable to draw string at %u, %u", x_orig, y_orig );
   }

   /* Reselect the initial objects into the provided DCs. */
   SelectObject( hdcBuffer, oldHbmBuffer );
   DeleteDC( hdcBuffer );
}

#endif /* !USE_SOFTWARE_TEXT */

