
#ifndef ANIMATE_H
#define ANIMATE_H

/**
 * \addtogroup unilayer_animate Unilayer Animation Layer
 * \brief Collection of common animation effects and related utilities.
 *
 * \{
 */

/*! \file animate.h
 *  \brief Tools for making common animations and effects.
 */

/**
 * \addtogroup unilayer_animate_effects_sect Unilayer Animation Effects
 * \{
 *
 * \page unilayer_animate_effects Unilayer Animation Effects
 * \tableofcontents
 *
 * \section unilayer_animate_effects_circle CIRCLE
 * Circle starting at center and radiating to w, h.
 * 
 * \section unilayer_animate_effects_rectangle RECTANGLE
 * (TODO) Rectangle starting at center and radiating to w, h.
 *
 * \section unilayer_animate_effects_fire FIRE
 * \image html firecga.png
 * Gradient of fire propagating upwards.
 *
 * \section unilayer_animate_effects_snow SNOW
 * (TODO) Dots moving from top left to bottom right.
 *
 * \section unilayer_animate_effects_frames FRAMES
 * (TODO) Individual bitmap frames from ::RESOURCE_ID.
 *
 * \}
 */
#define ANIMATE_CB_TABLE( f ) f( 0, CIRCLE ) f( 1, RECTANGLE ) f( 2, FIRE ) f( 3, SNOW ) f( 4, CLOUDS ) f( 5, STRING ) f( 6, FRAMES )

/*! \brief Return from animate_create() indicating a problem. */
#define ANIMATE_ERROR -1

#define ANIMATE_COLOR_BLACK      0
#define ANIMATE_COLOR_CYAN       1
#define ANIMATE_COLOR_MAGENTA    2
#define ANIMATE_COLOR_WHITE      3

#ifdef DEPTH_VGA

#define ANIMATE_COLOR_DARKBLUE   4
#define ANIMATE_COLOR_DARKGREEN  5
#define ANIMATE_COLOR_TEAL       6
#define ANIMATE_COLOR_DARKRED    7
#define ANIMATE_COLOR_VIOLET     8
#define ANIMATE_COLOR_BROWN      9
#define ANIMATE_COLOR_GRAY       10
#define ANIMATE_COLOR_DARKGRAY   11
#define ANIMATE_COLOR_BLUE       12
#define ANIMATE_COLOR_GREEN      13
#define ANIMATE_COLOR_RED        15
#define ANIMATE_COLOR_YELLOW     16

#endif /* DEPTH_VGA */

/**
 * \addtogroup unilayer_animate_flags Unilayer Animation Flags
 * \brief Flags to control ::ANIMATION behavior.
 *
 * \{
 */

/**
 * \relates ANIMATION
 * \brief ::ANIMATION::flags indicating animation is active and playing.
 */
#define ANIMATE_FLAG_ACTIVE   0x01
/**
 * \relates ANIMATION
 * \brief ::ANIMATION::flags indicating animation has been initialized.
 */
#define ANIMATE_FLAG_INIT     0x02
/**
 * \relates ANIMATION
 * \brief ::ANIMATION::flags indicating animation should black out previous
 *        frame's non-black pixels.
 */
#define ANIMATE_FLAG_CLEANUP  0x04
/**
 * \relates ANIMATION
 * \brief ::ANIMATION::flags indicating animation has been temporarily paused
 *        and should not update or draw on-screen.
 */
#define ANIMATE_FLAG_PAUSED   0x08

/*! \} */

#define ANIMATE_TILE_W 16
#define ANIMATE_TILE_H 16
#define ANIMATE_TILE_SZ (ANIMATE_TILE_W * ANIMATE_TILE_H)

#define ANIMATE_FIRE_COOLING_MAX 10
#define ANIMATE_FIRE_COOLING_MIN 5
#define ANIMATE_FIRE_WIND 1

#define ANIMATE_TEXT_HEADER_Y_OFFSET   0
#define ANIMATE_TEXT_HEADER_Y_COUNT    1
#define ANIMATE_TEXT_HEADER_STR_SZ     2
#define ANIMATE_TEXT_HEADER_COLOR_IDX  3
#define ANIMATE_TEXT_HEADER_STR        4
#define ANIMATE_TEXT_MAX_SZ            (ANIMATE_TILE_SZ - ANIMATE_TEXT_HEADER_STR)

#if 0
/*! \brief Used with FRAMES ::ANIMATION::type ANIMATION::data to list frames to
 *         play. */
/* TODO: Implementation. */
struct ANIMATION_FRAME {
   RESOURCE_ID frame;
   MEMORY_HANDLE next;
};
#endif

/*! \brief Internal representation of an animation. Do not call directly; use
 *         animate_create() instead.
 */
struct ANIMATION {
   /*! \brief Type of animation this is (See ::ANIMATE_CB_TABLE). */
   uint8_t type;
   /*! \brief Left origin of animation on screen in pixels. */
   int16_t x;
   /*! \brief Top origin of animation on screen in pixels. */
   int16_t y;
   /*! \brief Width of animation on screen in pixels. */
   int16_t w;
   /*! \brief Height of animation on screen in pixels. */
   int16_t h;
   /**
    * \brief Bitfield indicating animation's \ref unilayer_animate_flags.
    *
    * Lower 8 bits are Unilayer-specified flags. Upper 8 bits are application-
    * specific and will be ignored by Unilayer.
    */
   uint16_t flags;
   /*! \brief Data specific to particular animation playing. */
   int8_t tile[ANIMATE_TILE_SZ];
};

/*! \brief Callback to call on active animations for every frame. */
typedef void (*ANIMATE_CB)( struct ANIMATION* a );

/**
 * \brief Setup string animation.
 */
void animate_set_string(
   int8_t a_idx, char* str_in, uint8_t str_sz_in, uint8_t color_idx_in );

/**
 * \brief Create a new animation in the global animations list.
 * \param type See ::ANIMATE_CB_TABLE for animation types.
 * \param x Left origin of animation on screen in pixels.
 * \param y Top origin of animation on screen in pixels.
 * \param w Width of animation on screen in pixels.
 * \param h Height of animation on screen in pixels.
 * \return Internal index of newly created animation or ::ANIMATE_ERROR.
 */
int8_t animate_create(
   uint8_t type, uint16_t flags, int16_t x, int16_t y, int16_t w, int16_t h );

/**
 * \brief Draw the animation tile to the screen, tiled to fill its area.
 */
void animate_tesselate( struct ANIMATION* a, int16_t y_orig );

/**
 * \brief Should be called during every frame to overlay animations on screen.
 * \param flags Bitfield indicating which animations to animate/draw.
 */
void animate_frame( uint16_t flags );

/**
 * \brief Pause all animations with the given flags without deleting them.
 * \param flags Bitfield indicating which animations to pause.
 */
void animate_pause( uint16_t flags );

/**
 * \brief Resume all animations with the given flags that have been paused with
 *        animate_pause().
 * \param flags Bitfield indicating which animations to resume.
 */
void animate_resume( uint16_t flags );

/**
 * \brief Stop the animation with the given internal index.
 * \param idx Index to stop as returned from animate_create().
 */
void animate_stop( int8_t idx );

/**
 * \brief Stop all currently running animations on screen.
 */
void animate_stop_all();

#define ANIMATE_CB_TABLE_DRAW_PROTOTYPES( idx, name ) void animate_draw_ ## name( struct ANIMATION* );

ANIMATE_CB_TABLE( ANIMATE_CB_TABLE_DRAW_PROTOTYPES )

#ifdef ANIMATE_C

#define ANIMATE_CB_TABLE_LIST( idx, name ) animate_draw_ ## name,

const ANIMATE_CB gc_animate_draw[] = {
   ANIMATE_CB_TABLE( ANIMATE_CB_TABLE_LIST )
};

#define ANIMATE_CB_TABLE_TYPES( idx, name ) const uint8_t ANIMATE_TYPE_ ## name = idx;

ANIMATE_CB_TABLE( ANIMATE_CB_TABLE_TYPES );

struct ANIMATION g_animations[ANIMATE_ANIMATIONS_MAX];

static const GRAPHICS_COLOR gc_animation_colors[] = {
   GRAPHICS_COLOR_BLACK,
   GRAPHICS_COLOR_CYAN,
   GRAPHICS_COLOR_MAGENTA,
   GRAPHICS_COLOR_WHITE,
#ifdef DEPTH_VGA
   GRAPHICS_COLOR_DARKBLUE,
   GRAPHICS_COLOR_DARKGREEN,
   GRAPHICS_COLOR_TEAL,
   GRAPHICS_COLOR_DARKRED,
   GRAPHICS_COLOR_VIOLET,
   GRAPHICS_COLOR_BROWN,
   GRAPHICS_COLOR_GRAY,
   GRAPHICS_COLOR_DARKGRAY,
   GRAPHICS_COLOR_BLUE,
   GRAPHICS_COLOR_GREEN,
   GRAPHICS_COLOR_RED,
   GRAPHICS_COLOR_YELLOW
#endif /* DEPTH_VGA */
};

#else

/*! \brief Internal animation callback table. */
extern const ANIMATE_CB gc_animate_draw[];
/*! \brief Internal running animation list. */
extern struct ANIMATION g_animations[ANIMATE_ANIMATIONS_MAX];

#define ANIMATE_CB_TABLE_TYPES( idx, name ) extern const uint8_t ANIMATE_TYPE_ ## name;

ANIMATE_CB_TABLE( ANIMATE_CB_TABLE_TYPES )

#endif /* ANIMATE_C */

/*! \} */

#endif /* !ANIMATE_H */

