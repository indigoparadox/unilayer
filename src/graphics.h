
#ifndef GRAPHICS_H
#define GRAPHICS_H

/**
 * \addtogroup unilayer_graphics Unilayer Graphics Layer
 * \brief Simple abstract unified graphics drawing layer designed
 *        for resource-constrained systems.
 *
 * \{
 */

/*! \file graphics.h
 *  \brief Platform-general abstractions for graphics.
 */

#define GRAPHICS_ERROR_NOT_FOUND -1

#define GRAPHICS_INSTANCE_NONE      -1
#define GRAPHICS_INSTANCE_STATIC    -2
#define GRAPHICS_INSTANCE_WINDOW    -3
#define GRAPHICS_INSTANCE_TILEMAP   -4

/**
 * \addtogroup unilayer_graphics_string_flags Unilayer Graphics String Flags
 * \brief Flags for modifying string display options.
 *
 * These flags may be passed to graphics_string_at() or graphics_char_at() to
 * modify how the string or characters are displayed on-screen.
 *
 * \{
 */

/*! \brief Draw string at 1x scale. */
#define GRAPHICS_STRING_FLAG_1X 0x01

/*! \brief Display the string on-screen as ALL CAPITAL LETTERS. */
#define GRAPHICS_STRING_FLAG_ALL_CAPS 0x10

/*! \brief Use a script-style (cursive-style) font to display the string. */
#define GRAPHICS_STRING_FLAG_FONT_SCRIPT  0x20

/*! \brief Draw a black outline around each character in the string. */
#define GRAPHICS_STRING_FLAG_OUTLINE      0x80
 
/*! \} */

#define GRAPHICS_FLAG_SHAKING_MASK 0x03
#define GRAPHICS_FLAG_ZOOM_MASK 0x0c

#ifndef GRAPHICS_TXP_R
/*! \brief Red value for transparency color in VGA DEPTH. */
#define GRAPHICS_TXP_R 0xff
#endif /* !GRAPHICS_TXP_R */

#ifndef GRAPHICS_TXP_G
/*! \brief Green value for transparency color in VGA DEPTH. */
#define GRAPHICS_TXP_G 0x55
#endif /* !GRAPHICS_TXP_G */

#ifndef GRAPHICS_TXP_B
/*! \brief Blue value for transparency color in VGA DEPTH. */
#define GRAPHICS_TXP_B 0xff
#endif /* !GRAPHICS_TXP_B */

#define graphics_char_is_printable( c ) (('a' <= c && 'z' >= c) || ('A' <= c && 'Z' >= c) || ('0' <= c && '9' >= c) || '.' == c || ',' == c || ':' == c || ';' == c || '!' == c || '/' == c || ' ' == c)

#define graphics_draw_char_outline( x, y, flags ) if( GRAPHICS_STRING_FLAG_OUTLINE == (GRAPHICS_STRING_FLAG_OUTLINE & flags) ) { graphics_draw_px( x, y, GRAPHICS_COLOR_BLACK ); }

#define graphics_set_screen_shake( shake ) g_screen_flags &= ~GRAPHICS_FLAG_SHAKING_MASK; g_screen_flags |= (GRAPHICS_FLAG_SHAKING_MASK & shake);

#define graphics_set_screen_zoom( zoom ) g_screen_flags &= ~GRAPHICS_FLAG_ZOOM_MASK; g_screen_flags |= (((zoom) << 2) & GRAPHICS_FLAG_ZOOM_MASK)

/**
 * \brief Struct representing a graphical rectangle.
 */
struct GRAPHICS_RECT {
   /*! \brief Rectangle left-horizontal offset in pixels. */
   int16_t x;
   /*! \brief Rectangle top-vertical offset in pixels. */
   int16_t y;
   /*! \brief Rectangle width in pixels. */
   int16_t w;
   /*! \brief Rectangle height in pixels. */
   int16_t h;
};

/**
 * \addtogroup unilayer_graphics_bmp_flags Unilayer Graphics Bitmap Flags
 * \{
 */

#define GRAPHICS_BMP_FLAG_TYPE_TILE    0x0004
#define GRAPHICS_BMP_FLAG_TYPE_SPRITE  0x0002
#define GRAPHICS_BMP_FLAG_TYPE_MASK    0x0006
#define GRAPHICS_BMP_FLAG_INIT         0x0001

/*! \} */ /* unilayer_graphics_bmp_flags */

/**
 * \brief Struct representing a bitmap. Should be overridden by platform.
 *
 * This struct should be equivalent in size to whatever the bitmap struct for
 * the current platform looks like, so that the abstraction matches.
 */
struct GRAPHICS_BITMAP_BASE {
   /*! \brief Identifier for asset this bitmap represents/should load. */
   RESOURCE_ID id;
   uint16_t flags;
   /*! \brief Flags indicating. */
   uint16_t ref_count;

   /*! \brief Platform-specific graphics structure; should be overridden. */
   MEMORY_PTR ptr1;
   /*! \brief Platform-specific graphics structure; should be overridden. */
   MEMORY_PTR ptr2;
   /*! \brief Platform-specific uint32_t, should be overridden. */
   uint32_t res1;
   /*! \brief Platform-specific uint32_t, should be overridden. */
   uint32_t res2;
   /*! \brief Platform-specific byte, should be overridden. */
   uint8_t res3;
   /*! \brief Platform-specific byte, should be overridden. */
   uint8_t res4;
};

int16_t graphics_init();

void graphics_shutdown();

void graphics_lock();

/**
 * \brief Draw the current graphics buffer on-screen.
 */
void graphics_release();

/**
 * \brief Get the number of ticks since the program started.
 * \return A constantly increasing number of ticks since program start.
 */
uint32_t graphics_get_ms();

/**
 * \brief Mark the beginning of rendering the current graphical frame.
 *
 * This should be called from the main loop, and is critical to maintaining
 * timing.
 */
void graphics_loop_start();

/**
 * \brief Mark the end of rendering the current graphical frame.
 *
 * This should be called from the main loop, and is critical to maintaining
 * timing.
 */
void graphics_loop_end();

/**
 * \brief Plot a discrete point on screen.
 * \param x Horizontal coordinate of point to plot.
 * \param y Vertical coordinate of point to plot.
 * \param color \ref unilayer_graphics_colors of the point to plot.
 *
 * On some platforms, namely those using USE_SOFTWARE_TEXT and
 * USE_SOFTWARE_LINES, this is the only primitive that must be defined
 * in the implementation-specific module (e.g. nullg.h). Other primitives are
 * then implemented using this (e.g. graphics_draw_line()).
 */
void graphics_draw_px( uint16_t x, uint16_t y, const GRAPHICS_COLOR color );

/**
 * \brief Draw a filled rectangle on screen.
 * \param x_orig Horizontal coordinate of left side in pixels.
 * \param y_orig Vertical coordinate of top side in pixels.
 * \param w Width of rectangle in pixels.
 * \param h Height of rectangle in pixels.
 * \param color \ref unilayer_graphics_colors of the rectangle to draw.
 *
 * This uses the platform's native methods to draw a line if available, or
 * a general software emulation that relies on graphics_draw_px() if not.
 */
void graphics_draw_block(
   uint16_t x_orig, uint16_t y_orig, uint16_t w, uint16_t h,
   const GRAPHICS_COLOR color );

/**
 * \brief Draw a hollowed rectangle on screen.
 * \param x_orig Horizontal coordinate of left side in pixels.
 * \param y_orig Vertical coordinate of top side in pixels.
 * \param w Width of rectangle in pixels.
 * \param h Height of rectangle in pixels.
 * \param thickness Thickness of the rectangle border in pixels.
 * \param color \ref unilayer_graphics_colors of the borders of the rectangle
 *              to draw.
 *
 * This uses the platform's native methods to draw a line if available, or
 * a general software emulation that relies on graphics_draw_px() if not.
 */
void graphics_draw_rect(
   uint16_t x_orig, uint16_t y_orig, uint16_t w, uint16_t h,
   uint16_t thickness, const GRAPHICS_COLOR color );

/**
 * \brief Draw a line on screen.
 * \param x1 Horizontal coordinate of the starting point in pixels.
 * \param y1 Vertical coordinate of the starting point in pixels.
 * \param x2 Horizontal coordinate of the ending point in pixels.
 * \param y2 Vertical coordinate of the ending point in pixels.
 * \param thickness Thickness of the line in pixels.
 * \param color \ref unilayer_graphics_colors of the borders of the line
 *              to draw.
 * 
 * This uses the platform's native methods to draw a line if available, or
 * a general software emulation that relies on graphics_draw_px() if not.
 */
void graphics_draw_line(
   uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t thickness,
   const GRAPHICS_COLOR color );

/**
 * \brief Draw a text character at the specified coordinates.
 * \param c Text character to draw.
 * \param x_orig X coordinate to draw at, in pixels.
 * \param y_orig Y coordinate to draw at, in pixels.
 * \param color \ref unilayer_graphics_colors in which to draw the character.
 * \param flags \ref unilayer_graphics_string_flags to modify how the string is
 *              displayed.
 */
void graphics_char_at(
   char c, uint16_t x_orig, uint16_t y_orig, GRAPHICS_COLOR color,
   uint8_t flags );

/**
 * \brief Draw a text string on screen.
 * \param str String to draw on-screen.
 * \param str_sz Maximum length of the string in characters.
 * \param x_orig Left origin of the string drawn in pixels.
 * \param y_orig Top origin of the string drawn in pixels.
 * \param color \ref unilayer_graphics_colors in which to draw the string.
 * \param flags \ref unilayer_graphics_string_flags to modify how the string is
 *              displayed.
 */
void graphics_string_at(
   const char* str, uint16_t str_sz, uint16_t x_orig, uint16_t y_orig,
   GRAPHICS_COLOR color, uint8_t flags );

/**
 * \brief Get width and height of a text string in pixels if drawn on screen.
 * \param str String to get the dimensions for.
 * \param str_sz Maximum length of the string in characters.
 * \param flags String options.
 * \param sz_out ::MEMORY_PTR to the GRAPHICS_RECT to hold the output size.
 */
void graphics_string_sz(
   const char* str, uint16_t str_sz, uint8_t flags, struct GRAPHICS_RECT* sz_out
);

/**
 * \addtogroup unilayer_graphics_cache Unilayer Graphics Cache
 * \brief Interface to cache that holds/deduplicates sprites and tiles.
 * \{
 */

/**
 * \brief Copy part or all of a ::GRAPHICS_BITMAP image to the screen.
 * \param bitmap_idx Index of the ::GRAPHICS_BITMAP in the graphics cache.
 * \param instance_id Unique ID for the specific mobile being blitted, or -1.
 * \param s_x Horizontal location of upper-left corner of copy source rectangle.
 * \param s_y Vertical location of upper-left corner of copy source rectangle.
 * \param d_x Horizontal location of upper-left corner of destination rectangle.
 * \param d_y Vertical location of upper-left corner of destination rectangle.
 * \param w Pixel width of copy rectangle.
 * \param h Pixel height of copy rectangle.
 * \return 1 if blit was successful or 0 otherwise.
 *
 * This tries to load the bitmap into the internal bitmap cache the first time
 * that bitmap's resource ID is given.
 */
int16_t graphics_cache_blit_at(
   uint16_t bitmap_idx, uint16_t instance_id,
   uint16_t s_x, uint16_t s_y, uint16_t d_x, uint16_t d_y,
   uint16_t w, uint16_t h );

/**
 * \brief Free up graphics and resources used by loaded assets. Should only be
 *        called as part of cleanup (i.e. between maps).
 */
void graphics_clear_cache();

int16_t graphics_cache_load_bitmap( RESOURCE_ID id, uint8_t type_flag );

/*! \} */ /* unilayer_graphics_cache */

/**
 * \brief Get a random integer.
 * \param start Inclusive start of range.
 * \param range
 * \return Random integer higher than or equal to start and within range.
 */
int16_t graphics_get_random( int16_t start, int16_t range );

void graphics_on_resize( int16_t new_w, int16_t new_h );

#if defined( GRAPHICS_C ) || defined( GRAPHICS_PLATFORM_C )

/* Internal prototypes for abstract routines to access platform-specific
 * implementation features.
 */

/** 
 * \brief Load the graphics asset into a handle and pass that into the
 *        platform-specific loader.
 * \param res_handle Opened resource to load into the GRAPHICS_BITMAP.
 * \param b ::MEMORY_PTR to empty GRAPHICS_BITMAP to load asset into.
 * \return 1 if load was successful or 0 otherwise.
 */
int16_t graphics_platform_load_bitmap(
   RESOURCE_HANDLE res_handle, struct GRAPHICS_BITMAP* b );

/**
 * \brief Free up the platform-specific resources associated with a loaded
 *        graphical asset.
 * \param b ::MEMORY_PTR to the ::GRAPHICS_BITMAP to unload. Will not be
 *          deallocated if dynamic.
 * \return 1 if unload was successful or 0 otherwise.
 */
int16_t graphics_platform_unload_bitmap( struct GRAPHICS_BITMAP* b );

/**
 * \brief Load the given ::RESOURCE_ID into the graphics cache and return its
 *        index or ::GRAPHICS_ERROR_NOT_FOUND if not found.
 */
int16_t graphics_platform_blit_partial_at(
   const struct GRAPHICS_BITMAP*, int16_t,
   uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t );

int16_t graphics_platform_init();
void graphics_platform_shutdown();

#endif /* GRAPHICS_C || GRAPHICS_PLATFORM_C */

#ifdef GRAPHICS_C

uint16_t g_screen_real_w = 0;
uint16_t g_screen_real_h = 0;
uint8_t g_screen_flags = 0;

#else

extern uint16_t g_screen_real_w;
extern uint16_t g_screen_real_h;
extern uint8_t g_screen_flags;

#endif /* GRAPHICS_C */

/*! \} */

#endif /* GRAPHICS_H */

