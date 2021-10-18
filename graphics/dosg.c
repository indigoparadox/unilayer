
#define GRAPHICS_C
#include "../unilayer.h"

#define GRAPHICS_M_320_200_4_CGA  0x05
#define GRAPHICS_M_320_200_256_V  0x13

#define GRAPHICS_M_320_200_256_VGA_A   0xA0000000L
#define GRAPHICS_M_320_200_4_CGA_A     0xB8000000L

#ifndef GRAPHICS_MODE
#define GRAPHICS_MODE      0x05
#endif /* !GRAPHICS_MODE */

#ifndef CGA_COLD
#define CGA_COLD           0x01
#endif /* CGA_COLD */

#if GRAPHICS_M_320_200_4_CGA == GRAPHICS_MODE
#define GRAPHICS_ADDR     GRAPHICS_M_320_200_4_CGA_A
#elif GRAPHICS_M_320_200_256_VGA == GRAPHICS_MODE
#define GRAPHICS_ADDR     GRAPHICS_M_320_200_256_VGA_A
#endif /* GRAPHICS_MODE */

#include <stdlib.h>
#include <string.h>
#ifndef NO_I86
#include <i86.h>
#endif /* NO_I86 */
#include <dos.h>
#include <conio.h>

#ifdef USE_LOOKUPS
#include "../data/offsets.h"
#endif /* USE_LOOKUPS */
#include "formats/cga.h"

#ifdef USE_DOUBLEBUF
static uint8_t huge g_buffer[76800]; /* Sized for 0x13. */
#else
static uint8_t far* g_buffer = (uint8_t far *)GRAPHICS_ADDR;
#endif /* USE_DOUBLEBUF */

typedef void (__interrupt __far* INTFUNCPTR)( void );
INTFUNCPTR g_old_timer_interrupt;
volatile uint32_t g_ms;

const uint32_t gc_ms_target = 1000 / FPS;
static uint32_t g_ms_start = 0; 

void __interrupt __far graphics_timer_handler() {
   static unsigned long count = 0;

   ++g_ms;
   count += 1103; /* Original DOS timer in parallel. */
   if( 65536 <= count ) {
      /* Call the original handler. */
      count -= 65536;
      _chain_intr( g_old_timer_interrupt );
   } else {
      /* Acknowledge interrupt */
      outp( 0x20, 0x20 );
   }
}

static void graphics_install_timer() {
   union REGS r;
   struct SREGS s;

   _disable();

   /* Backup original handler for later. */
   segread( &s );
   r.h.al = 0x08;
   r.h.ah = 0x35;
   int86x( 0x21, &r, &r, &s );
   g_old_timer_interrupt = (INTFUNCPTR)MK_FP( s.es, r.x.bx );

   /* Install new interrupt handler. */
   g_ms = 0;
   r.h.al = 0x08;
   r.h.ah = 0x25;
   s.ds = FP_SEG( graphics_timer_handler );
   r.x.dx = FP_OFF( graphics_timer_handler );
   int86x( 0x21, &r, &r, &s );

   /* Set resolution of timer chip to 1ms. */
   outp( 0x43, 0x36 );
   outp( 0x40, (uint8_t)(1103 & 0xff) );
   outp( 0x40, (uint8_t)((1103 >> 8) & 0xff) );

   _enable();
}

static void graphics_remove_timer() {
   union REGS r;
   struct SREGS s;

   /* Re-install original interrupt handler. */
   _disable();
   segread( &s );
   r.h.al = 0x08;
   r.h.ah = 0x25;
   s.ds = FP_SEG( g_old_timer_interrupt );
   r.x.dx = FP_OFF( g_old_timer_interrupt );
   int86x( 0x21, &r, &r, &s );

   /* Reset timer chip resolution to 18.2...ms. */
   outp( 0x43,0x36 );
   outp( 0x40,0x00 );
   outp( 0x40,0x00 );

   _enable();
}

/*
 * @return 1 if init was successful and 0 otherwise.
 */
int16_t graphics_platform_init( struct GRAPHICS_ARGS* args ) {
   union REGS r;

   memory_zero_ptr( &r, sizeof( union REGS ) );

	r.h.ah = 0;
	r.h.al = GRAPHICS_MODE;
	int86( 0x10, &r, &r );

   memory_zero_ptr( &r, sizeof( union REGS ) );

   r.h.ah = 0x0b;
   r.h.bh = 0x01;
   r.h.bl = CGA_COLD;
	int86( 0x10, &r, &r );

   graphics_install_timer();

   return 1;
}

void graphics_platform_shutdown( struct GRAPHICS_ARGS* args ) {
   graphics_remove_timer();
}

void graphics_flip( struct GRAPHICS_ARGS* args ) {
#ifdef USE_DOUBLEBUF
#if GRAPHICS_M_320_200_256_VGA == GRAPHICS_MODE
      _fmemcpy( GRAPHICS_M_320_200_256_VGA_A,
         g_buffer, SCREEN_W * SCREEN_H );
#elif GRAPHICS_M_320_200_4_CGA == GRAPHICS_MODE
      /* memory_copy_ptr both planes. */
      _fmemcpy( (char far *)0xB8000000, g_buffer, 16000 );
#endif /* GRAPHICS_MODE */
#endif /* USE_DOUBLEBUF */
}

uint32_t graphics_get_ms() {
   return g_ms;
}

void graphics_loop_start() {
   g_ms_start = g_ms;
}

void graphics_loop_end() {
   int32_t delta = 0;
   
   do {
      delta = gc_ms_target - (g_ms - g_ms_start);
   } while( 0 < delta );  
}

void graphics_draw_px( uint16_t x, uint16_t y, GRAPHICS_COLOR color ) {
	int screen_byte_offset = 0,
      bit_offset = 0,
      bit_mask = 0;
   uint16_t scaled_x = x,
      scaled_y = y;

#if GRAPHICS_M_320_200_256_VGA == GRAPHICS_MODE
      screen_byte_offset = ((y * SCREEN_W) + x);
      g_buffer[screen_byte_offset] = color;
#elif GRAPHICS_M_320_200_4_CGA == GRAPHICS_MODE
#ifdef USE_LOOKUPS
      /* Use pre-generated lookup tables for offsets to improve performance. */
      screen_byte_offset = gc_offsets_cga_bytes_p1[scaled_y][scaled_x];
      bit_offset = gc_offsets_cga_bits_p1[scaled_y][scaled_x];
#else
      /* Divide y by 2 since both planes are SCREEN_H / 2 high. */
      /* Divide result by 4 since it's 2 bits per pixel. */
      screen_byte_offset = (((scaled_y / 2) * SCREEN_W) + scaled_x) / 4;
      /* Shift the bits over by the remainder. */
      bit_offset = 
         6 - (((((scaled_y / 2) * SCREEN_W) + scaled_x) % 4) * 2);
#endif /* USE_LOOKUPS */

      /* Clear the existing pixel. */
      if( 1 == scaled_y % 2 ) {
         g_buffer[0x2000 + screen_byte_offset] &= ~(0x03 << bit_offset);
         g_buffer[0x2000 + screen_byte_offset] |= (color << bit_offset);
      } else {
         g_buffer[screen_byte_offset] &= ~(0x03 << bit_offset);
         g_buffer[screen_byte_offset] |= (color << bit_offset);
      }
#endif /* GRAPHICS_MODE */
}

/*
 * @return 1 if blit was successful and 0 otherwise.
 */
int16_t graphics_platform_blit_at(
   const struct GRAPHICS_BITMAP* bmp,
   uint16_t x, uint16_t y, uint16_t w, uint16_t h
) {
	int y_offset = 0;
   uint16_t screen_byte_offset = 0;
   /* Still not sure why copy seems to start w/2px in? */
   const uint8_t* plane_1 = bmp->plane_1 - bmp->w / 8;
   const uint8_t* plane_2 = bmp->plane_2 - bmp->w / 8;

#if GRAPHICS_M_320_200_256_VGA == GRAPHICS_MODE
#error "not implemented"
#elif GRAPHICS_M_320_200_4_CGA == GRAPHICS_MODE

   if( NULL == plane_1 || NULL == plane_2 ) {
      return 0;
   }

	for( y_offset = 0 ; h > y_offset ; y_offset++ ) {
#ifdef USE_LOOKUPS
      screen_byte_offset = gc_offsets_cga_bytes_p1[y + y_offset][x];
#else
      /* Divide y by 2 since both planes are SCREEN_H / 2 high. */
      /* Divide result by 4 since it's 2 bits per pixel. */
      screen_byte_offset = ((((y + y_offset) / 2) * SCREEN_W) + x) / 4;
#endif /* USE_LOOKUPS */

      /* 4px per byte * 4 bytes = 16 px. */
      _fmemcpy( &(g_buffer[screen_byte_offset]), plane_1, 4 );
      _fmemcpy( &(g_buffer[0x2000 + screen_byte_offset]), plane_2, 4 );

      /* Advance source address by bytes per copy. */
      plane_1 += bmp->w / 8;
      plane_2 += bmp->w / 8;
	}
#endif /* GRAPHICS_MODE */

   return 1;
}

/*
 * @return 1 if blit was successful and 0 otherwise.
 */
int16_t graphics_platform_blit_partial_at(
   const struct GRAPHICS_BITMAP* bmp,
   uint16_t s_x, uint16_t s_y,
   uint16_t d_x, uint16_t d_y, uint16_t w, uint16_t h
) {
	int y_offset = 0;
   uint16_t screen_byte_offset = 0;
   /* Still not sure why copy seems to start w/2px in? */
   const uint8_t* plane_1 = bmp->plane_1 - bmp->w / 8;
   const uint8_t* plane_2 = bmp->plane_2 - bmp->w / 8;

#if GRAPHICS_M_320_200_256_VGA == GRAPHICS_MODE
#error "not implemented"
#elif GRAPHICS_M_320_200_4_CGA == GRAPHICS_MODE

   if( NULL == plane_1 || NULL == plane_2 ) {
      return 0;
   }

   if( 0 != s_x % 4 ) {
      error_printf( "s_x must be divisible by 4" );
      return 0;
   }

   /* Set starting X/Y from source planes. */
   /* s_y / 2 because each plane is 1/2 height. */
   plane_1 += (((s_y / 2) * bmp->w) / 4) + (s_x / 4);
   plane_2 += (((s_y / 2) * bmp->w) / 4) + (s_x / 4);

	for( y_offset = 0 ; h > y_offset ; y_offset++ ) {
#ifdef USE_LOOKUPS
      screen_byte_offset = gc_offsets_cga_bytes_p1[d_y + y_offset][d_x];
#else
      /* Divide y by 2 since both planes are SCREEN_H / 2 high. */
      /* Divide result by 4 since it's 2 bits per pixel. */
      screen_byte_offset = ((((d_y + y_offset) / 2) * SCREEN_W) + d_x) / 4;
#endif /* USE_LOOKUPS */

      /* 4px per byte * 4 bytes = 16 px. */
      _fmemcpy( &(g_buffer[screen_byte_offset]), plane_1, 4 );
      _fmemcpy( &(g_buffer[0x2000 + screen_byte_offset]), plane_2, 4 );

      /* Advance source address by bytes per copy. */
      plane_1 += bmp->w / 8;
      plane_2 += bmp->w / 8;
	}
#endif /* GRAPHICS_MODE */

   return 1;
}

void graphics_draw_block(
   uint16_t x_orig, uint16_t y_orig, uint16_t w, uint16_t h,
   GRAPHICS_COLOR color
) {
	int x = 0;
	int y = 0;
   uint16_t screen_byte_offset = 0;

#if GRAPHICS_M_320_200_256_VGA == GRAPHICS_MODE
#error "not implemented"
#elif GRAPHICS_M_320_200_4_CGA == GRAPHICS_MODE
   for( y = y_orig ; y < y + h ; y++ ) {
#ifdef USE_LOOKUPS
      screen_byte_offset = gc_offsets_cga_bytes_p1[y][x_orig];
#else
/* #error "not implemented" */
#endif /* USE_LOOKUPS */
      _fmemset( (char far *)0xB8000000 + screen_byte_offset, color, 2 );
      _fmemset( (char far *)0xB8002000 + screen_byte_offset, color, 2 );
   }
#endif /* GRAPHICS_MODE */

}

void graphics_draw_rect(
   uint16_t x_orig, uint16_t y_orig, uint16_t w, uint16_t h,
   uint16_t thickness, const GRAPHICS_COLOR color
) {
   
   /* TODO: Handle thickness. */

   /* Left Wall */
   graphics_draw_line( x_orig, y_orig, x_orig, y_orig + h, thickness, color );
   /* Bottom Wall */
   graphics_draw_line(
      x_orig, y_orig + h, x_orig + w, y_orig + h, thickness, color );
   /* Right Wall */
   graphics_draw_line(
      x_orig + w, y_orig + h, x_orig + w, y_orig, thickness, color );
   /* Top Wall */
   graphics_draw_line( x_orig, y_orig, x_orig + w, y_orig, thickness, color );
}

void graphics_draw_line(
   uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t thickness,
   const GRAPHICS_COLOR color
) {
   uint16_t delta_x = 0,
      delta_y = 0,
      delta = 0,
      x_start = 0,
      y_start = 0,
      x_end = 0,
      y_end = 0,
      x = 0,
      y = 0;

   /* TODO: Handle thickness. */

   /* Keep coordinates positive. */
   if( x1 > x2 ) {
      x_start = x2;
      x_end = x1;
   } else {
      x_start = x1;
      x_end = x2;
   }
   if( y1 > y2 ) {
      y_start = y2;
      y_end = y1;
   } else {
      y_start = y1;
      y_end = y2;
   }

   /* Bresenham's line algorithm. */
   delta_x = x_end - x_start;
   delta_y = y_end - y_start;
   delta = (2 * delta_y) - delta_x;
   for( x = x_start ; x_end > x ; x++ ) {
      graphics_draw_px( x, y, color );
      if( 0 < delta ) {
         y++;
         delta -= (2 * delta_x);
      }
      delta += (2 * delta_y);
   }
}

/*
 * @return 1 if bitmap is loaded and 0 otherwise.
 */
int16_t graphics_platform_load_bitmap(
   RESOURCE_BITMAP_HANDLE res_handle, struct GRAPHICS_BITMAP* b
) {
   uint8_t* buffer = NULL;
   int32_t buffer_sz = 0;
   uint16_t plane_sz = 0,
      plane_offset = 0;
   int32_t retval = 1;
   struct CGA_HEADER* header = NULL;

   buffer_sz = memory_sz( res_handle );
   buffer = resource_lock_handle( res_handle );
   header = (struct CGA_HEADER*)buffer;

   debug_printf( 1, "found CGA header version %u", header->version );

   /* Parse the resource into a usable struct. */
   b->w = header->width;
   b->h = header->height;
   b->palette = header->palette;

   debug_printf( 1, "%u x %x px, %u colors", b->w, b->h, b->palette );

   plane_sz = header->plane1_sz;
   plane_offset = header->plane1_offset;
   /* TODO: Memory architecture? */
   b->plane_1 = (uint8_t*)calloc( plane_sz, 1 );
   if( NULL == b->plane_1 ) {
      retval = 0;
      goto cleanup;
   }
   memory_copy_ptr( b->plane_1, &(buffer[plane_offset]), plane_sz );

   plane_sz = header->plane2_sz;
   plane_offset = header->plane2_offset;
   b->plane_2 = (uint8_t*)calloc( plane_sz, 1 );
   if( NULL == b->plane_2 ) {
      retval = 0;
      goto cleanup;
   }
   memory_copy_ptr( b->plane_2, &(buffer[plane_offset]), plane_sz );

cleanup:

   if( 0 >= retval && b->plane_1 ) {
      free( b->plane_1 );
   }

   if( 0 >= retval && b->plane_2 ) {
      free( b->plane_1 );
   }

   if( NULL != buffer ) {
      buffer = resource_unlock_handle( res_handle );
   }

   if( NULL != res_handle ) {
      resource_free_handle( res_handle );
   }

   return retval;
}

/*
 * @return 1 if bitmap is unloaded and 0 otherwise.
 */
int16_t graphics_unload_bitmap( struct GRAPHICS_BITMAP* b ) {
   assert( NULL != b );
   b->ref_count--;
   if( 0 == b->ref_count ) {
      free( b->plane_1 );
      free( b->plane_2 );
      b->initialized = 0;
      return 1;
   }
   return 0;
}

#ifndef USE_SOFTWARE_TEXT
#error "platform dos does not support hardware text"
#endif /* !USE_SOFTWARE_TEXT */

