
#ifndef WING_H
#define WING_H

typedef COLORREF GRAPHICS_COLOR;
struct GRAPHICS_BITMAP {
   RESOURCE_ID id;
   uint16_t flags;
   uint16_t ref_count;
   HBITMAP bitmap;
   HBITMAP mask;

   /* Unused Overrides */
   uint32_t res1;
   uint32_t res2;
   uint8_t res3;
   uint8_t res4;
};

struct GRAPHICS_ARGS {
   int cmd_show;
   int icon_res;
   uint8_t scale;
};

#ifdef MAIN_C
extern struct GRAPHICS_ARGS g_graphics_args;
#endif /* MAIN_C */

#define WIN_GFX_TIMER_ID 6001

#ifdef DEPTH_VGA

#define GRAPHICS_COLOR_BLACK        RGB(0,   0,   0)
#define GRAPHICS_COLOR_DARKBLUE     RGB(0, 0, 170)
#define GRAPHICS_COLOR_DARKGREEN    RGB(0, 170, 0)
#define GRAPHICS_COLOR_TEAL         RGB(0, 170, 170)
#define GRAPHICS_COLOR_DARKRED      RGB(170, 0, 0)
#define GRAPHICS_COLOR_VIOLET       RGB(170, 0, 170)
#define GRAPHICS_COLOR_BROWN        RGB(170, 85, 0)
#define GRAPHICS_COLOR_GRAY         RGB(170, 170, 170)
#define GRAPHICS_COLOR_DARKGRAY     RGB(85, 85, 85)
#define GRAPHICS_COLOR_BLUE         RGB(85, 85, 255)
#define GRAPHICS_COLOR_GREEN        RGB(85, 255, 85)
#define GRAPHICS_COLOR_CYAN         RGB(85, 255, 255)
#define GRAPHICS_COLOR_RED          RGB(255, 85, 85)
#define GRAPHICS_COLOR_MAGENTA      RGB(255, 85, 255)
#define GRAPHICS_COLOR_YELLOW       RGB(255, 255, 85)
#define GRAPHICS_COLOR_WHITE        RGB(255, 255, 255)

#else
                                         
#define GRAPHICS_COLOR_BLACK        RGB(0, 0, 0)
#define GRAPHICS_COLOR_CYAN         RGB(0, 255, 255)
#define GRAPHICS_COLOR_MAGENTA      RGB(255, 0, 255)
#define GRAPHICS_COLOR_WHITE        RGB(255, 255, 255)

#endif /* DEPTH_VGA */

#ifdef MAIN_C
static void win_process_messages() {
   MSG msg;
   int msg_retval = 0;

   /* Process messages here, which triggers the message handler above. */
   msg_retval = GetMessage( &msg, 0, 0, 0 );
   if( 0 >= msg_retval ) {
      g_running = 0;
      return;
   }

   TranslateMessage( &msg );
   DispatchMessage( &msg );
}
#endif /* MAIN_C */

#endif /* WING_H */

