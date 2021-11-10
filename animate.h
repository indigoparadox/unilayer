
#ifndef ANIMATE_H
#define ANIMATE_H

/*! \file animate.h
 *  \brief Tools for making common animations and effects.
 */

#define ANIMATE_CB_TABLE( f ) f( 0, CIRCLE ) f( 1, RECTANGLE ) f( 2, FIRE )

#define ANIMATE_FLAG_ACTIVE   1
#define ANIMATE_FLAG_LOOP     2
#define ANIMATE_FLAG_INIT     4

/*
struct FRAME {
   GRAPHICS_COLOR color;
   uint8_t shape;
   int16_t radius;
};
*/

struct ANIMATION {
   uint8_t type;
   int16_t x;
   int16_t y;
   int16_t w;
   int16_t h;
   uint8_t flags;
   MEMORY_HANDLE data;
   /* struct FRAME frames[ANIMATE_FRAMES_MAX]; */
};

typedef void (*ANIMATE_CB)( struct ANIMATION* a );

int8_t animate_create_animation(
   uint8_t type, int16_t x, int16_t y, int16_t w, int16_t h );

void animation_frame();

void animation_stop( int8_t idx );

void animation_stop_all();

#define ANIMATE_CB_TABLE_DRAW_PROTOTYPES( idx, name ) void animate_draw_ ## name( struct ANIMATION* );

ANIMATE_CB_TABLE( ANIMATE_CB_TABLE_DRAW_PROTOTYPES );

#ifdef ANIMATE_C

#define ANIMATE_CB_TABLE_LIST( idx, name ) animate_draw_ ## name,

const ANIMATE_CB gc_animate_draw[] = {
   ANIMATE_CB_TABLE( ANIMATE_CB_TABLE_LIST )
};

struct ANIMATION g_animations[ANIMATE_ANIMATIONS_MAX];
#else
extern const ANIMATE_CB gc_animate_draw[];
extern struct ANIMATION g_animations[ANIMATE_ANIMATIONS_MAX];
#endif /* ANIMATE_C */

#endif /* !ANIMATE_H */

