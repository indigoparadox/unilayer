
#ifndef ANIMATE_H
#define ANIMATE_H

/**
 * \addtogroup unilayer_animation Unilayer Animation Layer
 * \brief Collection of common animation effects and related utilities.
 *
 * \{
 */

/*! \file animate.h
 *  \brief Tools for making common animations and effects.
 */

/**
 * \addtogroup unilayer_animation_effects_sect Unilayer Animation Effects
 * \{
 *
 * \page unilayer_animation_effects Unilayer Animation Effects
 * \tableofcontents
 *
 * \section unilayer_animation_effects_circle CIRCLE
 * Circle starting at center and radiating to w, h.
 * 
 * \section unilayer_animation_effects_rectangle RECTANGLE
 * (TODO) Rectangle starting at center and radiating to w, h.
 *
 * \section unilayer_animation_effects_fire FIRE
 * \image html firecga.png
 * Gradient of fire propagating upwards.
 *
 * \section unilayer_animation_effects_snow SNOW
 * (TODO) Dots moving from top left to bottom right.
 *
 * \section unilayer_animation_effects_frames FRAMES
 * (TODO) Individual bitmap frames from ::RESOURCE_ID.
 *
 * \}
 */
#define ANIMATE_CB_TABLE( f ) f( 0, CIRCLE ) f( 1, RECTANGLE ) f( 2, FIRE ) f( 3, SNOW ) f( 4, FRAMES )

/*! \brief Return from animate_create_animation() indicating a problem. */
#define ANIMATE_ERROR -1

/*! \brief ::ANIMATION::flags indicating animation is active and playing. */
#define ANIMATE_FLAG_ACTIVE   1
/*! \brief ::ANIMATION::flags indicating animation has been initialized. */
#define ANIMATE_FLAG_INIT     2

#define ANIMATE_TILE_W 16
#define ANIMATE_TILE_H 16
#define ANIMATE_TILE_SZ (ANIMATE_TILE_W * ANIMATE_TILE_H)

#define ANIMATE_FIRE_COOLING_MAX 10
#define ANIMATE_FIRE_COOLING_MIN 5

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
 *         animate_create_animation() instead.
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
   /*! \brief Bitfield indicating properties of animation. */
   uint8_t flags;
   /*! \brief Data specific to particular animation playing. */
   uint8_t tile[ANIMATE_TILE_SZ];
};

/*! \brief Callback to call on active animations for every frame. */
typedef void (*ANIMATE_CB)( struct ANIMATION* a );

/**
 * \brief Create a new animation in the global animations list.
 * \param type See ::ANIMATE_CB_TABLE for animation types.
 * \param x Left origin of animation on screen in pixels.
 * \param y Top origin of animation on screen in pixels.
 * \param w Width of animation on screen in pixels.
 * \param h Height of animation on screen in pixels.
 * \return Internal index of newly created animation or ::ANIMATE_ERROR.
 */
int8_t animate_create_animation(
   uint8_t type, int16_t x, int16_t y, int16_t w, int16_t h );

/**
 * \brief Draw the animation tile to the screen, tiled to fill its area.
 */
void animate_tesselate( struct ANIMATION* a, int16_t y_orig );

/**
 * \brief Should be called during every frame to overlay animations on screen.
 */
void animation_frame();

/**
 * \brief Stop the animation with the given internal index.
 * \param idx Index to stop as returned from animate_create_animation().
 */
void animation_stop( int8_t idx );

/**
 * \brief Stop all currently running animations on screen.
 */
void animation_stop_all();

#define ANIMATE_CB_TABLE_DRAW_PROTOTYPES( idx, name ) void animate_draw_ ## name( struct ANIMATION* );

ANIMATE_CB_TABLE( ANIMATE_CB_TABLE_DRAW_PROTOTYPES );

#ifdef ANIMATE_C

#define ANIMATE_CB_TABLE_LIST( idx, name ) animate_draw_ ## name,

const ANIMATE_CB gc_animate_draw[] = {
   ANIMATE_CB_TABLE( ANIMATE_CB_TABLE_LIST )
};

#define ANIMATE_CB_TABLE_TYPES( idx, name ) const uint8_t ANIMATE_TYPE_ ## name = idx;

ANIMATE_CB_TABLE( ANIMATE_CB_TABLE_TYPES );

struct ANIMATION g_animations[ANIMATE_ANIMATIONS_MAX];

#else

/*! \brief Internal animation callback table. */
extern const ANIMATE_CB gc_animate_draw[];
/*! \brief Internal running animation list. */
extern struct ANIMATION g_animations[ANIMATE_ANIMATIONS_MAX];

#define ANIMATE_CB_TABLE_TYPES( idx, name ) extern const uint8_t ANIMATE_TYPE_ ## name;

ANIMATE_CB_TABLE( ANIMATE_CB_TABLE_TYPES );

#endif /* ANIMATE_C */

/*! \} */

#endif /* !ANIMATE_H */

