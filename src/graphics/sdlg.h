
#ifndef SDLG_H
#define SDLG_H

#include <SDL.h>

typedef const SDL_Color* GRAPHICS_COLOR;
struct GRAPHICS_BITMAP {
   RESOURCE_ID id;
   uint8_t initialized;
   uint16_t ref_count;

   SDL_Surface* surface;

#ifdef PLATFORM_SDL1
   void* res5;
#else
   SDL_Texture* texture;
#endif /* PLATFORM_SDL1 */

   /* Unused Overrides */
   uint32_t res1;
   uint32_t res2;
   uint8_t res3;
   uint8_t res4;
};

struct GRAPHICS_ARGS {
};

#ifdef SDLG_C

#ifdef DEPTH_VGA

const SDL_Color gc_black =       {0,   0,   0};
const SDL_Color gc_darkblue =    {0, 0, 170};
const SDL_Color gc_darkgreen =   {0, 170, 0};
const SDL_Color gc_teal =        {0, 170, 170};
const SDL_Color gc_darkred =     {170, 0, 0};
const SDL_Color gc_violet =      {170, 0, 170};
const SDL_Color gc_brown =       {170, 85, 0};
const SDL_Color gc_gray =        {170, 170, 170};
const SDL_Color gc_darkgray =    {85, 85, 85};
const SDL_Color gc_blue =        {85, 85, 255};
const SDL_Color gc_green =       {85, 255, 85};
const SDL_Color gc_cyan =        {85, 255, 255};
const SDL_Color gc_red =         {255, 85, 85};
const SDL_Color gc_magenta =     {255, 85, 255};
const SDL_Color gc_yellow =      {255, 255, 85};
const SDL_Color gc_white =       {255, 255, 255};

#else

const SDL_Color gc_black =    {0,   0,   0};
const SDL_Color gc_cyan =     {0,   255, 255};
const SDL_Color gc_magenta =  {255, 0,   255};
const SDL_Color gc_white =    {255, 255, 255};

#endif /* DEPTH_VGA */

#else

extern const SDL_Color gc_black;
extern const SDL_Color gc_cyan;
extern const SDL_Color gc_magenta;
extern const SDL_Color gc_white;

#ifdef DEPTH_VGA

extern const SDL_Color gc_darkblue;
extern const SDL_Color gc_darkgreen;
extern const SDL_Color gc_teal;
extern const SDL_Color gc_darkred;
extern const SDL_Color gc_violet; 
extern const SDL_Color gc_brown;
extern const SDL_Color gc_gray;
extern const SDL_Color gc_darkgray;
extern const SDL_Color gc_blue;
extern const SDL_Color gc_green;
extern const SDL_Color gc_red;
extern const SDL_Color gc_yellow;

#endif /* DEPTH_VGA */

#endif /* SDLG_C */

#define GRAPHICS_COLOR_BLACK     &gc_black
#define GRAPHICS_COLOR_CYAN      &gc_cyan
#define GRAPHICS_COLOR_MAGENTA   &gc_magenta
#define GRAPHICS_COLOR_WHITE     &gc_white

#ifdef DEPTH_VGA

#define GRAPHICS_COLOR_DARKBLUE  &gc_darkblue
#define GRAPHICS_COLOR_DARKGREEN &gc_darkgreen
#define GRAPHICS_COLOR_TEAL      &gc_teal
#define GRAPHICS_COLOR_DARKRED   &gc_darkred
#define GRAPHICS_COLOR_VIOLET    &gc_violet
#define GRAPHICS_COLOR_BROWN     &gc_brown
#define GRAPHICS_COLOR_GRAY      &gc_gray
#define GRAPHICS_COLOR_DARKGRAY  &gc_darkgray
#define GRAPHICS_COLOR_BLUE      &gc_blue
#define GRAPHICS_COLOR_GREEN     &gc_green
#define GRAPHICS_COLOR_RED       &gc_red
#define GRAPHICS_COLOR_YELLOW    &gc_yellow

#endif /* DEPTH_VGA */

#endif /* SDLG_H */

