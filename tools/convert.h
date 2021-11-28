
#ifndef CONVERT_H
#define CONVERT_H

#include "../src/unilayer.h"

#define CONVERT_BUFFER_SZ 1024

#define CONVERT_ERROR_FILE_WRITE    -1

#define FORMAT_TABLE( f ) f( 0, bmp ) f( 1, cga ) f( 2, icns )

struct CONVERT_GRID {
   int32_t sz_x;
   int32_t sz_y;
   uint16_t bpp;
   uint32_t data_sz;
   uint8_t *data;
};

struct CONVERT_OPTIONS {
   uint8_t reverse;
   uint16_t bpp;
   int32_t w;
   int32_t h;
   uint32_t line_padding;
   uint32_t plane_padding;
   uint32_t bmp_data_sz;
   uint8_t cga_use_header;
   uint8_t little_endian;
   uint8_t bmp_no_file_header;
   uint32_t bmp_data_offset_out;
};

typedef int32_t (*FMT_GRID_SZ)(
   const struct CONVERT_GRID* grid, struct CONVERT_OPTIONS* o );

typedef int32_t (*FMT_VERIFY_OPTS)( struct CONVERT_OPTIONS* o );

typedef struct CONVERT_GRID* (*FMT_READ)(
   const uint8_t* buf, uint32_t buf_sz, struct CONVERT_OPTIONS* o );

typedef struct CONVERT_GRID* (*FMT_READ_FILE)(
   const char* path, struct CONVERT_OPTIONS* o );

typedef int (*FMT_WRITE)(
   uint8_t* buf_ptr, uint32_t buf_sz,
   const struct CONVERT_GRID* grid, struct CONVERT_OPTIONS* o );

typedef int (*FMT_WRITE_FILE)(
   const char* path, const struct CONVERT_GRID* grid, struct CONVERT_OPTIONS* o
);

#define FORMAT_TABLE_SZ_CBS_PROTO( idx, fmt ) int32_t fmt ## _grid_sz( const struct CONVERT_GRID* grid, struct CONVERT_OPTIONS* o );

FORMAT_TABLE( FORMAT_TABLE_SZ_CBS_PROTO )

#define FORMAT_TABLE_OPT_CBS_PROTO( idx, fmt ) int32_t fmt ## _verify_opts( struct CONVERT_OPTIONS* o );

FORMAT_TABLE( FORMAT_TABLE_OPT_CBS_PROTO )

#define FORMAT_TABLE_READ_CBS_PROTO( idx, fmt ) struct CONVERT_GRID* fmt ## _read( const uint8_t* buf, uint32_t buf_sz, struct CONVERT_OPTIONS* o );

FORMAT_TABLE( FORMAT_TABLE_READ_CBS_PROTO )

#define FORMAT_TABLE_READ_FILE_CBS_PROTO( idx, fmt ) struct CONVERT_GRID* fmt ## _read_file( const char* path, struct CONVERT_OPTIONS* o );

FORMAT_TABLE( FORMAT_TABLE_READ_FILE_CBS_PROTO )

#define FORMAT_TABLE_WRITE_CBS_PROTO( idx, fmt ) int fmt ## _write( uint8_t* buf_ptr, uint32_t buf_sz, const struct CONVERT_GRID* grid, struct CONVERT_OPTIONS* o );

FORMAT_TABLE( FORMAT_TABLE_WRITE_CBS_PROTO )

#define FORMAT_TABLE_WRITE_FILE_CBS_PROTO( idx, fmt ) int fmt ## _write_file( const char* path, const struct CONVERT_GRID* grid, struct CONVERT_OPTIONS* o );

FORMAT_TABLE( FORMAT_TABLE_WRITE_FILE_CBS_PROTO )

#ifdef CONVERT_C

#define FORMAT_TABLE_SZ_CBS( idx, fmt ) fmt ## _grid_sz,

FMT_GRID_SZ gc_fmt_sz_cbs[] = {
   FORMAT_TABLE( FORMAT_TABLE_SZ_CBS )
};

#define FORMAT_TABLE_OPT_CBS( idx, fmt ) fmt ## _verify_opts,

FMT_VERIFY_OPTS gc_fmt_opt_cbs[] = {
   FORMAT_TABLE( FORMAT_TABLE_OPT_CBS )
};

#define FORMAT_TABLE_READ_CBS( idx, fmt ) fmt ## _read,

FMT_READ gc_fmt_read_cbs[] = {
   FORMAT_TABLE( FORMAT_TABLE_READ_CBS )
};

#define FORMAT_TABLE_READ_FILE_CBS( idx, fmt ) fmt ## _read_file,

FMT_READ_FILE gc_fmt_read_file_cbs[] = {
   FORMAT_TABLE( FORMAT_TABLE_READ_FILE_CBS )
};

#define FORMAT_TABLE_WRITE_CBS( idx, fmt ) fmt ## _write,

FMT_WRITE gc_fmt_write_cbs[] = {
   FORMAT_TABLE( FORMAT_TABLE_WRITE_CBS )
};

#define FORMAT_TABLE_WRITE_FILE_CBS( idx, fmt ) fmt ## _write_file,

FMT_WRITE_FILE gc_fmt_write_file_cbs[] = {
   FORMAT_TABLE( FORMAT_TABLE_WRITE_FILE_CBS )
};

#define FORMAT_TABLE_TOKENS( idx, fmt ) #fmt,

const char* gc_fmt_tokens[] = {
   FORMAT_TABLE( FORMAT_TABLE_TOKENS )
   ""
};

#define FORMAT_TABLE_CONSTS( idx, fmt ) const int gc_fmt_ ## fmt = idx;

FORMAT_TABLE( FORMAT_TABLE_CONSTS )

#else

#endif /* CONVERT_C */

#endif /* CONVERT_H */

