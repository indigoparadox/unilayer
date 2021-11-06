
#ifndef GRAPHICS_H
#define GRAPHICS_H

/*! \file graphics.h
 *  \brief Platform-general abstractions for graphics.
 */

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

#ifndef graphics_blit_sprite_at
#define graphics_blit_sprite_at( res_id, s_x, s_y, d_x, d_y, w, h ) graphics_blit_at( res_id, s_x, s_y, d_x, d_y, w, h )
#endif /* !graphics_blit_sprite_at */

#ifndef graphics_blit_tile_at
#define graphics_blit_tile_at( res_id, s_x, s_y, d_x, d_y, w, h ) graphics_blit_at( res_id, s_x, s_y, d_x, d_y, w, h )
#endif /* !graphics_blit_tile_at */

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

/**
 * \brief Copy part or all of a ::GRAPHICS_BITMAP image to the screen.
 * \param res_id ::RESOURCE_ID of the image to copy.
 * \param s_x Horizontal location of upper-left corner of copy source rectangle.
 * \param s_y Vertical location of upper-left corner of copy source rectangle.
 * \param d_x Horizontal location of upper-left corner of destination rectangle.
 * \param d_y Vertical location of upper-left corner of destination rectangle.
 * \param w Pixel width of copy rectangle.
 * \param h Pixel height of copy rectangle.
 * \return 1 if blit was successful or 0 otherwise.
 */
int16_t graphics_blit_at(
   RESOURCE_ID res_id,
   uint16_t s_x, uint16_t s_y, uint16_t d_x, uint16_t d_y,
   uint16_t w, uint16_t h );

/** 
 * \brief Load the graphics asset into a handle and pass that into the
 *        platform-specific loader.
 * \param id Identifier specifying the asset to load into the GRAPHICS_BITMAP.
 * \param b ::MEMORY_PTR to empty GRAPHICS_BITMAP to load asset into.
 */
int16_t graphics_load_bitmap( RESOURCE_ID id, struct GRAPHICS_BITMAP* b );

/**
 * \brief Free up the platform-specific resources associated with a loaded
 *        graphical asset.
 * \param b ::MEMORY_PTR to the ::GRAPHICS_BITMAP to unload. Will not be
 *          deallocated if dynamic.
 * \return 1 if unload was successful or 0 otherwise.
 */
int16_t graphics_unload_bitmap( struct GRAPHICS_BITMAP* b );

/**
 * \brief Free up graphics and resources used by loaded assets. Should only be
 *        called as part of cleanup (i.e. between maps).
 */
void graphics_clear_cache();

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

