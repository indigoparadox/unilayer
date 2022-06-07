
#ifndef BMP_H
#define BMP_H

struct CONVERT_GRID;
struct CONVERT_OPTIONS;

#define BMP_COLOR_TABLE( f ) f( black, 0x00000000 ) f( blue, 0x000000aa ) f( green, 0x0000aa00 ) f( cyan, 0x0000aaaa ) f( red, 0x00aa0000 ) f( magenta, 0x00aa00aa ) f( brown, 0x00aa5500 ) f( light_gray, 0x00aaaaaa ) f( dark_gray, 0x00555555 ) f( light_blue, 0x005555ff ) f( light_green, 0x0055ff55 ) f( light_cyan, 0x0055ffff ) f( light_red, 0x00ff5555 ) f( light_magenta, 0x00ff55ff ) f( yellow, 0x00ffff55 ) f( white, 0x00ffffff )

struct
#ifdef __GNUC__
__attribute__( (__packed__) )
#endif /* __GNUC__ */
BITMAP_FILE_HEADER {
   char id[2];
   uint32_t file_sz;
   uint16_t reserved1;
   uint16_t reserved2;
   uint32_t bmp_offset;
};

struct
#ifdef __GNUC__
__attribute__( (__packed__) )
#endif /* __GNUC__ */
BITMAP_DATA_HEADER {
   uint32_t header_sz;
   int32_t bitmap_w;
   int32_t bitmap_h;
   uint16_t planes;
   uint16_t bpp;
   uint32_t compression;
   uint32_t image_sz;
   int32_t hres;
   int32_t vres;
   uint32_t colors;
   uint32_t colors_important;
};

#define BMP_COMPRESSION_NONE (0)

/**
 * \return The byte size of the given ::CONVERT_GRID if written as a bitmap.
 */
int32_t bmp_grid_sz(
   const struct CONVERT_GRID* grid, struct CONVERT_OPTIONS* o );
uint8_t bmp_colors_count( uint8_t );
int32_t bmp_write_file(
   const char*, const struct CONVERT_GRID*, struct CONVERT_OPTIONS* );
int32_t bmp_write(
   uint8_t*, uint32_t, const struct CONVERT_GRID*, struct CONVERT_OPTIONS* );
struct CONVERT_GRID* bmp_read_file( const char*, struct CONVERT_OPTIONS* );
struct CONVERT_GRID* bmp_read(
   const uint8_t*, uint32_t, struct CONVERT_OPTIONS* );

#define bmp_int( type, buf, offset ) *((type*)&(buf[offset]))

#ifdef BMP_C

#  define BMP_COLOR_TABLE_CONSTS( color, value ) const uint32_t gc_bmp_color_ ## color = value;

BMP_COLOR_TABLE( BMP_COLOR_TABLE_CONSTS )

#else

#  define BMP_COLOR_TABLE_CONSTS( color, value ) extern const uint32_t gc_bmp_color_ ## color;

BMP_COLOR_TABLE( BMP_COLOR_TABLE_CONSTS )

#endif /* BMP_C */

#endif /* BMP_H */

