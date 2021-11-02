
#ifndef GRAPHICS_H
#define GRAPHICS_H

/*! \file graphics.h
 *  \brief Platform-general abstractions for graphics.
 */

#ifndef GRAPHICS_TXP_R
#define GRAPHICS_TXP_R 0xff
#endif /* !GRAPHICS_TXP_R */

#ifndef GRAPHICS_TXP_G
#define GRAPHICS_TXP_G 0x55
#endif /* !GRAPHICS_TXP_G */

#ifndef GRAPHICS_TXP_B
#define GRAPHICS_TXP_B 0xff
#endif /* !GRAPHICS_TXP_B */

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
 * \brief Struct representing a bitmap. Should be overridden by platform.
 *
 * This struct should be equivalent in size to whatever the bitmap struct for
 * the current platform looks like, so that the abstraction matches.
 */
struct GRAPHICS_BITMAP_BASE {
   /*! \brief Identifier for asset this bitmap represents/should load. */
   RESOURCE_ID id;
   /*! \brief Whether the asset GRAPHICS_BITMAP_BASE::id has been loaded. */
   uint8_t initialized;
   /*! \brief (UNUSED) Reference counter. */
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

int16_t graphics_init( struct GRAPHICS_ARGS* args );

void graphics_shutdown( struct GRAPHICS_ARGS* args );

/**
 * \brief Draw the current graphics buffer on-screen.
 */
void graphics_flip( struct GRAPHICS_ARGS* args );

/**
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

void graphics_draw_px( uint16_t, uint16_t, const GRAPHICS_COLOR );
void graphics_draw_block(
   uint16_t, uint16_t, uint16_t, uint16_t, const GRAPHICS_COLOR );
void graphics_draw_rect(
   uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, const GRAPHICS_COLOR );
void graphics_draw_line(
   uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, const GRAPHICS_COLOR );

/**
 * \brief Draw a text character at the specified coordinates.
 * \param c Text character to draw.
 * \param x_orig X coordinate to draw at, in pixels.
 * \param y_orig Y coordinate to draw at, in pixels.
 * \param color Color in which to draw the character.
 * \param scale Zoom/scale at which to draw the character.
 */
void graphics_char_at(
   const char c, uint16_t x_orig, uint16_t y_orig, GRAPHICS_COLOR color,
   uint8_t scale );
void graphics_string_at(
   const char*, uint16_t, uint16_t, uint16_t, const GRAPHICS_COLOR, uint8_t );
void graphics_string_sz(
   const char*, uint16_t, uint8_t, struct GRAPHICS_RECT* );
int16_t graphics_blit_at(
   RESOURCE_ID, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t );

/** 
 * \brief Load the graphics asset into a handle and pass that into the
 *        platform-specific loader.
 * \param id Identifier specifying the asset to load into the GRAPHICS_BITMAP.
 * \param b ::MEMORY_PTR to empty GRAPHICS_BITMAP to load asset into.
 */
int16_t graphics_load_bitmap( RESOURCE_ID id, struct GRAPHICS_BITMAP* b );

int16_t graphics_unload_bitmap( struct GRAPHICS_BITMAP* );
void graphics_blit_masked_at(
   const struct GRAPHICS_BITMAP*, const uint8_t*, uint16_t,
   uint16_t, uint16_t, uint16_t, uint16_t );
#ifdef GRAPHICS_C
int16_t graphics_platform_blit_partial_at(
   const struct GRAPHICS_BITMAP*,
   uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t );
int16_t graphics_platform_init( struct GRAPHICS_ARGS* );
void graphics_platform_shutdown( struct GRAPHICS_ARGS* );
int16_t graphics_platform_load_bitmap(
   RESOURCE_BITMAP_HANDLE, struct GRAPHICS_BITMAP* );
#endif /* GRAPHICS_C */

#endif /* GRAPHICS_H */

