
#define GRAPHICS_PLATFORM_C
#define DOSG_C
#include "../unilayer.h"

/* TODO: VGA mode. */

#if defined( DEPTH_VGA )

#  define GRAPHICS_VGA_MODE         0x13
#  define GRAPHICS_VGA_ADDR         (uint8_t far*)0xA0000000L

#elif defined( DEPTH_CGA )

#define GRAPHICS_CGA_MODE           0x05
#define GRAPHICS_CGA_ADDR_1         (uint8_t far*)0xB8000000L
#define GRAPHICS_CGA_ADDR_2         (uint8_t far*)0xB8002000L

/* The offset between the start of the even/odd CGA planes. */
#define GRAPHICS_CGA_PLANE_OFFSET   0x2000

#endif /* DEPTH_VGA || DEPTH_CGA */

#ifndef GRAPHICS_MODE
#define GRAPHICS_MODE      0x05
#endif /* !GRAPHICS_MODE */

#ifndef TEXT_MODE
#define TEXT_MODE      0x03
#endif /* !TEXT_MODE */

#ifndef CGA_COLD
#define CGA_COLD           0x01
#endif /* CGA_COLD */

#if defined( DEPTH_VGA )
#  define GRAPHICS_ADDR     GRAPHICS_VGA_ADDR
#elif defined( DEPTH_CGA )
#  define GRAPHICS_ADDR     GRAPHICS_CGA_ADDR_1
#endif /* GRAPHICS_MODE */

#ifndef DOS_TIMER_DIV
/* #define DOS_TIMER_DIV 1103 */
#define DOS_TIMER_DIV 100
#endif /* !DOS_TIMER_DIV */

#include <stdlib.h>
#include <string.h>
#ifndef NO_I86
#include <i86.h>
#endif /* NO_I86 */
#include <dos.h>
#include <conio.h>
#include <time.h> /* For rand() */

#ifdef USE_LOOKUPS
#include "../data/offsets.h"
#endif /* USE_LOOKUPS */
#include "formats/cga.h"

#ifdef USE_DOUBLEBUF
#  if defined( DEPTH_VGA )
#     define DOUBLEBUF_SZ 76800
#  elif defined( DEPTH_CGA )
#     define DOUBLEBUF_SZ 16000
#  else
#     error "double-buffer unsupported in this depth"
#  endif /* DEPTH_VGA || DEPTH_CGA */

static uint8_t g_buffer[DOUBLEBUF_SZ];

#else

/* Point directly to graphics memory. */
static uint8_t far* g_buffer = GRAPHICS_ADDR;

#endif /* USE_DOUBLEBUF */

#ifdef DEPTH_CGA
/* This is a solid 4 pixels of the color indexed by CGA order for use in
 * filling.
 */
uint8_t g_cga_color_packed[] = {
   0x00, 0x55, 0xaa, 0xff
};
#endif /* DEPTH_CGA */

typedef void (__interrupt __far* INTFUNCPTR)( void );
INTFUNCPTR g_old_timer_interrupt;
volatile uint32_t g_ms;

static uint16_t g_gfx_mem_used = 0;
static uint16_t g_gfx_count = 0;

const uint32_t gc_ms_target = 1000 / FPS;
static uint32_t g_ms_start = 0; 

void __interrupt __far graphics_timer_handler() {
   static unsigned long count = 0;

   ++g_ms;
   count += DOS_TIMER_DIV; /* Original DOS timer in parallel. */
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
   outp( 0x40, (uint8_t)(DOS_TIMER_DIV & 0xff) );
   outp( 0x40, (uint8_t)((DOS_TIMER_DIV >> 8) & 0xff) );

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
int16_t graphics_platform_init() {
   union REGS r;

   memory_zero_ptr( &r, sizeof( union REGS ) );

   /* Use the graphics mode defined above. */
	r.h.ah = 0;
	r.h.al = GRAPHICS_MODE;
	int86( 0x10, &r, &r );

   memory_zero_ptr( &r, sizeof( union REGS ) );

#ifdef DEPTH_CGA
   /* Set the CGA palette. */
   r.h.ah = 0x0b;
   r.h.bh = 0x01;
   r.h.bl = CGA_COLD;
	int86( 0x10, &r, &r );
#endif /* DEPTH_CGA */

   graphics_install_timer();

   srand( (unsigned int)time( NULL ) );

   return 1;
}

void graphics_platform_shutdown() {
   union REGS r;

   graphics_remove_timer();

   memory_zero_ptr( &r, sizeof( union REGS ) );

   /* Reset the display back to text mode. */
	r.h.ah = 0;
	r.h.al = TEXT_MODE;
	int86( 0x10, &r, &r );
}

void graphics_lock() {
}

void graphics_release() {
#ifdef USE_DOUBLEBUF
      _fmemcpy( GRAPHICS_ADDR, g_buffer, DOUBLEBUF_SZ );
#endif /* USE_DOUBLEBUF */
}

int16_t graphics_get_random( int16_t start, int16_t range ) {
   return start + (rand() % range);
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
	uint16_t screen_byte_offset = 0,
      bit_offset = 0,
      bit_mask = 0,
      scaled_x = x,
      scaled_y = y;

#if defined( DEPTH_VGA )

      /* Just write directly to the address. */
      screen_byte_offset = ((y * SCREEN_W) + x);
      g_buffer[screen_byte_offset] = color;

#elif defined( DEPTH_CGA )

#  ifdef USE_LOOKUPS
      /* Use pre-generated lookup tables for offsets to improve performance. */
      screen_byte_offset = gc_offsets_cga_bytes_p1[scaled_y][scaled_x];
      bit_offset = gc_offsets_cga_bits_p1[scaled_y][scaled_x];
#  else
      /* Divide y by 2 since both planes are SCREEN_H / 2 high. */
      /* Divide result by 4 since it's 2 bits per pixel. */
      screen_byte_offset = (((scaled_y / 2) * SCREEN_W) + scaled_x) / 4;
      /* Shift the bits over by the remainder. */
      bit_offset = 
         6 - (((((scaled_y / 2) * SCREEN_W) + scaled_x) % 4) * 2);
#  endif /* USE_LOOKUPS */

      /* Clear the existing pixel. */
      if( 1 == scaled_y % 2 ) {
         g_buffer[GRAPHICS_CGA_PLANE_OFFSET + screen_byte_offset] &= ~(0x03 << bit_offset);
         g_buffer[GRAPHICS_CGA_PLANE_OFFSET + screen_byte_offset] |= (color << bit_offset);
      } else {
         g_buffer[screen_byte_offset] &= ~(0x03 << bit_offset);
         g_buffer[screen_byte_offset] |= (color << bit_offset);
      }
#else
#  error "graphics mode unsupported"
#endif /* DEPTH_VGA || DEPTH_CGA */
}

int16_t graphics_platform_blit_partial_at(
   const struct GRAPHICS_BITMAP* bmp, int16_t instance_id,
   uint16_t s_x, uint16_t s_y,
   uint16_t d_x, uint16_t d_y, uint16_t w, uint16_t h
) {
   uint16_t screen_byte_offset = 0,
	   y_offset = 0;
   /* Still not sure why copy seems to start w/2px in? */
   int16_t retval = 1,
      bmp_line_step = 0; /* Advance this many bytes between source lines. */

#if defined( DEPTH_VGA )

   /* TODO */

#elif defined( DEPTH_CGA )
   uint8_t* plane_1 = NULL;
   uint8_t* plane_2 = NULL;

   plane_1 = memory_lock( bmp->plane_1 );
   plane_1 -= bmp->w / 8;
   plane_2 = memory_lock( bmp->plane_2 );
   plane_2 -= bmp->w / 8;

   if( NULL == plane_1 || NULL == plane_2 ) {
      retval = 0;
      goto cleanup;
   }

   if( 0 != s_x % 4 ) {
      error_printf( "s_x must be divisible by 4" );
      retval = 0;
      goto cleanup;
   }

   /* d_y must be even because of dual-plane setup but we can fudge this. */
   if( 0 != d_y % 2 ) {
      d_y++;
   }

   /* 8 bytes per line blit (for 16px-wide source). */
   bmp_line_step = bmp->w / 8;

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
      _fmemcpy(
         &(g_buffer[GRAPHICS_CGA_PLANE_OFFSET + screen_byte_offset]),
         plane_2, 4 );

      /* Advance source address by bytes per copy. */
      plane_1 += bmp_line_step;
      plane_2 += bmp_line_step;
	}
#endif /* GRAPHICS_MODE */

#ifdef DEPTH_CGA
cleanup:

   if( NULL != plane_1 ) {
      plane_1 = memory_unlock( bmp->plane_1 );
   }

   if( NULL != plane_2 ) {
      plane_2 = memory_unlock( bmp->plane_2 );
   }
#endif /* DEPTH_CGA */

   return retval;
}

void graphics_draw_block(
   uint16_t x_orig, uint16_t y_orig, uint16_t w, uint16_t h,
   GRAPHICS_COLOR color
) {
   uint16_t y = 0;

#if defined( DEPTH_VGA )
   for( y = y_orig ; y < y_orig + h ; y++ ) {
      _fmemset( GRAPHICS_ADDR + (y * SCREEN_W) + x_orig, color, h );
   }

#elif defined( DEPTH_CGA )
   int16_t screen_byte_offset = 0;
   uint16_t x = 0,
      x_end = x_orig + w,
      w_bytes = w / 4;

   for( y = y_orig ; y < y_orig + h ; y++ ) {

      if( 0 == x_orig % 4 && 0 == w % 4 ) {
#  ifdef USE_LOOKUPS
         screen_byte_offset = gc_offsets_cga_bytes_p1[y][x_orig];
#  else
         screen_byte_offset = (((y / 2) * SCREEN_W) + x_orig) / 4;
#  endif /* USE_LOOKUPS */
         /* Apply to correct even/odd CGA plane. */
         if( 0 == y % 2 ) {
            _fmemset( GRAPHICS_CGA_ADDR_1 + screen_byte_offset,
               g_cga_color_packed[color], w_bytes );
         } else {
            _fmemset( GRAPHICS_CGA_ADDR_2 + screen_byte_offset,
               g_cga_color_packed[color], w_bytes );
         }
      } else {
         /* Make up any odd pixels we couldn't draw (not divisible by 4). */
         for( x = x_orig ; x_end > x ; x++ ) {
            graphics_draw_px( x, y, color );
         }
      }
   }
#else
#  error "graphics mode unsupported"
#endif /* DEPTH_VGA || DEPTH_CGA */

}

/*
 * @return 1 if bitmap is loaded and 0 otherwise.
 */
int16_t graphics_platform_load_bitmap(
   RESOURCE_HANDLE res_handle, struct GRAPHICS_BITMAP* b
) {
   uint8_t* buffer = NULL,
      * plane_ptr = NULL;
   int32_t buffer_sz = 0;
   uint16_t plane_sz = 0,
      plane_offset = 0;
   int32_t retval = 1;
   struct CGA_HEADER* header = NULL;

   buffer_sz = resource_sz_handle( res_handle );
   buffer = resource_lock_handle( res_handle );
   header = (struct CGA_HEADER*)buffer;

   debug_printf( 1, "found CGA header version %u", header->version );

   /* Parse the resource into a usable struct. */
   b->w = header->width;
   b->h = header->height;
   b->palette = header->palette;

   debug_printf( 1, "%u x %x px, %u colors", b->w, b->h, b->palette );

   /* Allocate and load first plane. */
   plane_sz = header->plane1_sz;
   plane_offset = header->plane1_offset;
   b->plane_1 = memory_alloc( plane_sz, 1 );
   if( NULL == b->plane_1 ) {
      error_printf( "unable to allocate plane 1" );
      retval = 0;
      goto cleanup;
   }
   plane_ptr = memory_lock( b->plane_1 );
   if( NULL == plane_ptr ) {
      error_printf( "unable to lock plane 1" );
      retval = 0;
      goto cleanup;
   }
   debug_printf( 1, "copying buffer into plane 1" );
   memory_copy_ptr( plane_ptr, &(buffer[plane_offset]), plane_sz );
   plane_ptr = memory_unlock( b->plane_1 );

   /* Allocate and load second plane. */
   plane_sz = header->plane2_sz;
   plane_offset = header->plane2_offset;
   b->plane_2 = memory_alloc( plane_sz, 1 );
   if( NULL == b->plane_2 ) {
      error_printf( "unable to allocate plane 2" );
      retval = 0;
      goto cleanup;
   }
   plane_ptr = memory_lock( b->plane_2 );
   if( NULL == plane_ptr ) {
      error_printf( "unable to lock plane 2" );
      retval = 0;
      goto cleanup;
   }
   debug_printf( 1, "copying buffer into plane 2" );
   memory_copy_ptr( plane_ptr, &(buffer[plane_offset]), plane_sz );
   plane_ptr = memory_unlock( b->plane_2 );

   /* Update system statistics. */
   g_gfx_mem_used += memory_sz( b->plane_1 ) + memory_sz( b->plane_2 );
   g_gfx_count++;

   debug_printf( 1, "graphic successfully loaded (%d bytes in use for %d gfx)",
      g_gfx_mem_used, g_gfx_count );

cleanup:

   if( 0 >= retval && b->plane_1 ) {
      memory_free( b->plane_1 );
   }

   if( 0 >= retval && b->plane_2 ) {
      memory_free( b->plane_2 );
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
int16_t graphics_platform_unload_bitmap( struct GRAPHICS_BITMAP* b ) {
   assert( NULL != b );
   b->ref_count--;
   if( 0 == b->ref_count ) {
      g_gfx_mem_used -= (memory_sz( b->plane_1 ) + memory_sz( b->plane_2 ));
      g_gfx_count--;
      memory_free( b->plane_1 );
      memory_free( b->plane_2 );
      b->flags &= ~GRAPHICS_BMP_FLAG_INIT;

      debug_printf( 1,
         "graphic successfully unloaded (%d bytes in use for %d gfx)",
         g_gfx_mem_used, g_gfx_count );

      return 1;
   }
   return 0;
}

#ifndef USE_SOFTWARE_TEXT
#error "platform dos does not support hardware text"
#endif /* !USE_SOFTWARE_TEXT */

