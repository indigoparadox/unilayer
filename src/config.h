
#ifndef CONFIG_H
#define CONFIG_H

/* Do platform tests first, so they can force options below if they need to. */

#ifdef PLATFORM_WEB
#define PLATFORM_GL
#endif /* PLATFORM_WEB */

#ifndef NO_RESEXT
#include <resext.h>
#endif

/* ------ */
#ifdef PLATFORM_DOS
/* ------ */

/* ------ */
#elif defined( PLATFORM_SDL )
/* ------ */

/* ------ */
#elif defined( PLATFORM_XLIB )
/* ------ */

/* ------ */
#elif defined( PLATFORM_PALM )
/* ------ */

/*
#include <PalmOS.h>

#define stringify_line( line ) #line

#define assert( test ) ErrFatalDisplayIf( !(test), __FILE__ ": " stringify_line( __LINE__ ) ": assert failure" ) */

/* ------ */
#elif defined( PLATFORM_WIN16 )
/* ------ */

#define PLATFORM_WIN

/* ------ */
#elif defined( PLATFORM_WINCE )
/* ------ */

#define PLATFORM_WIN

/* ------ */
#elif defined( PLATFORM_WIN32 )
/* ------ */

#define PLATFORM_WIN

/* ------ */
#elif defined( PLATFORM_MAC6 )
/* ------ */

/* ------ */
#elif defined( PLATFORM_NDS )
/* ------ */

/* ------ */
#elif defined( PLATFORM_GL )
/* ------ */

/* ------ */
#endif /* PLATFORM_DOS, PLATFORM_SDL, PLATFORM_PALM, PLATFORM_WIN16 */
/* ------ */

#ifndef UNILAYER_WINDOW_CLASS
#define UNILAYER_WINDOW_CLASS UNILAYER_PROJECT_NAME
#endif /* !UNILAYER_WINDOW_CLASS */

#ifndef UNILAYER_WINDOW_TITLE
#define UNILAYER_WINDOW_TITLE UNILAYER_PROJECT_NAME
#endif /* !UNILAYER_WINDOW_TITLE */

#ifndef JSON_BUFFER_SZ
#define JSON_BUFFER_SZ 8192
#endif /* !JSON_BUFFER_SZ */

#ifndef SCREEN_SCALE
#define SCREEN_SCALE 1
#endif /* !SCREEN_SCALE */

#ifndef ANIMATE_ANIMATIONS_MAX
#define ANIMATE_ANIMATIONS_MAX 5
#endif /* !ANIMATE_ANIMATIONS_MAX */

#ifndef ANIMATE_FRAMES_MAX
#define ANIMATE_FRAMES_MAX 5
#endif /* !ANIMATE_FRAMES_MAX */

#ifndef FPS
#define FPS 15
#endif /* !FPS */

#ifndef MEMORY_FAKE_HEAP_SZ
#define MEMORY_FAKE_HEAP_SZ 112500
#endif /* !MEMORY_FAKE_HEAP_S */

#ifndef DIO_READ_FILE_BLOCK_SZ
#define DIO_READ_FILE_BLOCK_SZ 4096
#endif /* !DIO_READ_FILE_BLOCK_SZ */

#ifndef NAMEBUF_MAX
#define NAMEBUF_MAX 255
#endif /* !NAMEBUF_MAX */

#ifndef DEBUG_THRESHOLD
#define DEBUG_THRESHOLD 2
#endif /* !DEBUG_THRESHOLD */

#ifndef DIRTY_THRESHOLD
#define DIRTY_THRESHOLD 3
#endif /* !DIRTY_THRESHOLD */

#ifndef FAKE_HEAP_SIZE
#define FAKE_HEAP_SIZE 524288
#endif /* !FAKE_HEAP_SIZE */

#ifndef DIO_PATH_MAX
#define DIO_PATH_MAX 254
#endif /* !DIO_PATH_MAX */

#ifndef INPUT_BLOCK_DELAY
#define INPUT_BLOCK_DELAY 5
#endif /* !INPUT_BLOCK_DELAY */

#ifndef GRAPHICS_CACHE_INITIAL_SZ
#define GRAPHICS_CACHE_INITIAL_SZ 50
#endif /* !GRAPHICS_CACHE_INITIAL_SZ */

/* Graphics Parameters */

#define FONT_W 8
#define FONT_H 8
#define FONT_SPACE 0

/* Derived Parameters */

#if defined( SCREEN_W ) && defined( SCREEN_SCALE )
#define SCREEN_REAL_W (SCREEN_W * SCREEN_SCALE)
#endif /* SCREEN_W, SCREEN_SCALE */

#if defined( SCREEN_H ) && defined( SCREEN_SCALE )
#define SCREEN_REAL_H (SCREEN_H * SCREEN_SCALE)
#endif /* SCREEN_H, SCREEN_SCALE */

#endif /* CONFIG_H */

