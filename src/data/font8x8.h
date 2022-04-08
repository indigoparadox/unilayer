
#ifndef FONT_8X8
#define FONT_8X8

#define FONT_IDX_BASIC 0
#define FONT_IDX_SCRIPT 1

#define FONT_GLYPHS_COUNT 95
#define FONT_GLYPH_W_SZ 8

RES_CONST char gc_font8x8[2][FONT_GLYPHS_COUNT][FONT_GLYPH_W_SZ] = {

/** 
 * Credits for gc_font8x8_basic:
 *
 * 8x8 monochrome bitmap font for rendering
 * Author: Daniel Hepper <daniel@hepper.net>
 * 
 * License: Public Domain
 * 
 * Based on:
 * // Summary: font8x8.h
 * // 8x8 monochrome bitmap fonts for rendering
 * //
 * // Author:
 * //     Marcel Sondaar
 * //     International Business Machines (public domain VGA fonts)
 * //
 * // License:
 * //     Public Domain
 * 
 * Fetched from: http://dimensionalrift.homelinux.net/combuster/mos3/?p=viewsource&file=/modules/gfx/font8_8.asm
 **/
{
   { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   /*  U+0020 (space) */
   { 0x18, 0x3C, 0x3C, 0x18, 0x18, 0x00, 0x18, 0x00 },   /*  U+0021 (!) */
   { 0x36, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   /*  U+0022 (") */
   { 0x36, 0x36, 0x7F, 0x36, 0x7F, 0x36, 0x36, 0x00 },   /*  U+0023 (#) */
   { 0x0C, 0x3E, 0x03, 0x1E, 0x30, 0x1F, 0x0C, 0x00 },   /*  U+0024 ($) */
   { 0x00, 0x63, 0x33, 0x18, 0x0C, 0x66, 0x63, 0x00 },   /*  U+0025 (%) */
   { 0x1C, 0x36, 0x1C, 0x6E, 0x3B, 0x33, 0x6E, 0x00 },   /*  U+0026 (&) */
   { 0x06, 0x06, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00 },   /*  U+0027 (') */
   { 0x18, 0x0C, 0x06, 0x06, 0x06, 0x0C, 0x18, 0x00 },   /*  U+0028 (() */
   { 0x06, 0x0C, 0x18, 0x18, 0x18, 0x0C, 0x06, 0x00 },   /*  U+0029 ()) */
   { 0x00, 0x66, 0x3C, 0xFF, 0x3C, 0x66, 0x00, 0x00 },   /*  U+002A (*) */
   { 0x00, 0x0C, 0x0C, 0x3F, 0x0C, 0x0C, 0x00, 0x00 },   /*  U+002B (+) */
   { 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x06 },   /*  U+002C (,) */
   { 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x00 },   /*  U+002D (-) */
   { 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x00 },   /*  U+002E (.) */
   { 0x60, 0x30, 0x18, 0x0C, 0x06, 0x03, 0x01, 0x00 },   /*  U+002F (/) */
   { 0x3E, 0x63, 0x73, 0x7B, 0x6F, 0x67, 0x3E, 0x00 },   /*  U+0030 (0) */
   { 0x0C, 0x0E, 0x0C, 0x0C, 0x0C, 0x0C, 0x3F, 0x00 },   /*  U+0031 (1) */
   { 0x1E, 0x33, 0x30, 0x1C, 0x06, 0x33, 0x3F, 0x00 },   /*  U+0032 (2) */
   { 0x1E, 0x33, 0x30, 0x1C, 0x30, 0x33, 0x1E, 0x00 },   /*  U+0033 (3) */
   { 0x38, 0x3C, 0x36, 0x33, 0x7F, 0x30, 0x78, 0x00 },   /*  U+0034 (4) */
   { 0x3F, 0x03, 0x1F, 0x30, 0x30, 0x33, 0x1E, 0x00 },   /*  U+0035 (5) */
   { 0x1C, 0x06, 0x03, 0x1F, 0x33, 0x33, 0x1E, 0x00 },   /*  U+0036 (6) */
   { 0x3F, 0x33, 0x30, 0x18, 0x0C, 0x0C, 0x0C, 0x00 },   /*  U+0037 (7) */
   { 0x1E, 0x33, 0x33, 0x1E, 0x33, 0x33, 0x1E, 0x00 },   /*  U+0038 (8) */
   { 0x1E, 0x33, 0x33, 0x3E, 0x30, 0x18, 0x0E, 0x00 },   /*  U+0039 (9) */
   { 0x00, 0x0C, 0x0C, 0x00, 0x00, 0x0C, 0x0C, 0x00 },   /*  U+003A (:) */
   { 0x00, 0x0C, 0x0C, 0x00, 0x00, 0x0C, 0x0C, 0x06 },   /*  U+003B (//) */
   { 0x18, 0x0C, 0x06, 0x03, 0x06, 0x0C, 0x18, 0x00 },   /*  U+003C (<) */
   { 0x00, 0x00, 0x3F, 0x00, 0x00, 0x3F, 0x00, 0x00 },   /*  U+003D (=) */
   { 0x06, 0x0C, 0x18, 0x30, 0x18, 0x0C, 0x06, 0x00 },   /*  U+003E (>) */
   { 0x1E, 0x33, 0x30, 0x18, 0x0C, 0x00, 0x0C, 0x00 },   /*  U+003F (?) */
   { 0x3E, 0x63, 0x7B, 0x7B, 0x7B, 0x03, 0x1E, 0x00 },   /*  U+0040 (@) */
   { 0x0C, 0x1E, 0x33, 0x33, 0x3F, 0x33, 0x33, 0x00 },   /*  U+0041 (A) */
   { 0x3F, 0x66, 0x66, 0x3E, 0x66, 0x66, 0x3F, 0x00 },   /*  U+0042 (B) */
   { 0x3C, 0x66, 0x03, 0x03, 0x03, 0x66, 0x3C, 0x00 },   /*  U+0043 (C) */
   { 0x1F, 0x36, 0x66, 0x66, 0x66, 0x36, 0x1F, 0x00 },   /*  U+0044 (D) */
   { 0x7F, 0x46, 0x16, 0x1E, 0x16, 0x46, 0x7F, 0x00 },   /*  U+0045 (E) */
   { 0x7F, 0x46, 0x16, 0x1E, 0x16, 0x06, 0x0F, 0x00 },   /*  U+0046 (F) */
   { 0x3C, 0x66, 0x03, 0x03, 0x73, 0x66, 0x7C, 0x00 },   /*  U+0047 (G) */
   { 0x33, 0x33, 0x33, 0x3F, 0x33, 0x33, 0x33, 0x00 },   /*  U+0048 (H) */
   { 0x1E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00 },   /*  U+0049 (I) */
   { 0x78, 0x30, 0x30, 0x30, 0x33, 0x33, 0x1E, 0x00 },   /*  U+004A (J) */
   { 0x67, 0x66, 0x36, 0x1E, 0x36, 0x66, 0x67, 0x00 },   /*  U+004B (K) */
   { 0x0F, 0x06, 0x06, 0x06, 0x46, 0x66, 0x7F, 0x00 },   /*  U+004C (L) */
   { 0x63, 0x77, 0x7F, 0x7F, 0x6B, 0x63, 0x63, 0x00 },   /*  U+004D (M) */
   { 0x63, 0x67, 0x6F, 0x7B, 0x73, 0x63, 0x63, 0x00 },   /*  U+004E (N) */
   { 0x1C, 0x36, 0x63, 0x63, 0x63, 0x36, 0x1C, 0x00 },   /*  U+004F (O) */
   { 0x3F, 0x66, 0x66, 0x3E, 0x06, 0x06, 0x0F, 0x00 },   /*  U+0050 (P) */
   { 0x1E, 0x33, 0x33, 0x33, 0x3B, 0x1E, 0x38, 0x00 },   /*  U+0051 (Q) */
   { 0x3F, 0x66, 0x66, 0x3E, 0x36, 0x66, 0x67, 0x00 },   /*  U+0052 (R) */
   { 0x1E, 0x33, 0x07, 0x0E, 0x38, 0x33, 0x1E, 0x00 },   /*  U+0053 (S) */
   { 0x3F, 0x2D, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00 },   /*  U+0054 (T) */
   { 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x3F, 0x00 },   /*  U+0055 (U) */
   { 0x33, 0x33, 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x00 },   /*  U+0056 (V) */
   { 0x63, 0x63, 0x63, 0x6B, 0x7F, 0x77, 0x63, 0x00 },   /*  U+0057 (W) */
   { 0x63, 0x63, 0x36, 0x1C, 0x1C, 0x36, 0x63, 0x00 },   /*  U+0058 (X) */
   { 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x0C, 0x1E, 0x00 },   /*  U+0059 (Y) */
   { 0x7F, 0x63, 0x31, 0x18, 0x4C, 0x66, 0x7F, 0x00 },   /*  U+005A (Z) */
   { 0x1E, 0x06, 0x06, 0x06, 0x06, 0x06, 0x1E, 0x00 },   /*  U+005B ([) */
   { 0x03, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x40, 0x00 },   /*  U+005C (\) */
   { 0x1E, 0x18, 0x18, 0x18, 0x18, 0x18, 0x1E, 0x00 },   /*  U+005D (]) */
   { 0x08, 0x1C, 0x36, 0x63, 0x00, 0x00, 0x00, 0x00 },   /*  U+005E (^) */
   { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF },   /*  U+005F (_) */
   { 0x0C, 0x0C, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00 },   /*  U+0060 (`) */
   { 0x00, 0x00, 0x1E, 0x30, 0x3E, 0x33, 0x6E, 0x00 },   /*  U+0061 (a) */
   { 0x07, 0x06, 0x06, 0x3E, 0x66, 0x66, 0x3B, 0x00 },   /*  U+0062 (b) */
   { 0x00, 0x00, 0x1E, 0x33, 0x03, 0x33, 0x1E, 0x00 },   /*  U+0063 (c) */
   { 0x38, 0x30, 0x30, 0x3e, 0x33, 0x33, 0x6E, 0x00 },   /*  U+0064 (d) */
   { 0x00, 0x00, 0x1E, 0x33, 0x3f, 0x03, 0x1E, 0x00 },   /*  U+0065 (e) */
   { 0x1C, 0x36, 0x06, 0x0f, 0x06, 0x06, 0x0F, 0x00 },   /*  U+0066 (f) */
   { 0x00, 0x00, 0x6E, 0x33, 0x33, 0x3E, 0x30, 0x1F },   /*  U+0067 (g) */
   { 0x07, 0x06, 0x36, 0x6E, 0x66, 0x66, 0x67, 0x00 },   /*  U+0068 (h) */
   { 0x0C, 0x00, 0x0E, 0x0C, 0x0C, 0x0C, 0x1E, 0x00 },   /*  U+0069 (i) */
   { 0x30, 0x00, 0x30, 0x30, 0x30, 0x33, 0x33, 0x1E },   /*  U+006A (j) */
   { 0x07, 0x06, 0x66, 0x36, 0x1E, 0x36, 0x67, 0x00 },   /*  U+006B (k) */
   { 0x0E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00 },   /*  U+006C (l) */
   { 0x00, 0x00, 0x33, 0x7F, 0x7F, 0x6B, 0x63, 0x00 },   /*  U+006D (m) */
   { 0x00, 0x00, 0x1F, 0x33, 0x33, 0x33, 0x33, 0x00 },   /*  U+006E (n) */
   { 0x00, 0x00, 0x1E, 0x33, 0x33, 0x33, 0x1E, 0x00 },   /*  U+006F (o) */
   { 0x00, 0x00, 0x3B, 0x66, 0x66, 0x3E, 0x06, 0x0F },   /*  U+0070 (p) */
   { 0x00, 0x00, 0x6E, 0x33, 0x33, 0x3E, 0x30, 0x78 },   /*  U+0071 (q) */
   { 0x00, 0x00, 0x3B, 0x6E, 0x66, 0x06, 0x0F, 0x00 },   /*  U+0072 (r) */
   { 0x00, 0x00, 0x3E, 0x03, 0x1E, 0x30, 0x1F, 0x00 },   /*  U+0073 (s) */
   { 0x08, 0x0C, 0x3E, 0x0C, 0x0C, 0x2C, 0x18, 0x00 },   /*  U+0074 (t) */
   { 0x00, 0x00, 0x33, 0x33, 0x33, 0x33, 0x6E, 0x00 },   /*  U+0075 (u) */
   { 0x00, 0x00, 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x00 },   /*  U+0076 (v) */
   { 0x00, 0x00, 0x63, 0x6B, 0x7F, 0x7F, 0x36, 0x00 },   /*  U+0077 (w) */
   { 0x00, 0x00, 0x63, 0x36, 0x1C, 0x36, 0x63, 0x00 },   /*  U+0078 (x) */
   { 0x00, 0x00, 0x33, 0x33, 0x33, 0x3E, 0x30, 0x1F },   /*  U+0079 (y) */
   { 0x00, 0x00, 0x3F, 0x19, 0x0C, 0x26, 0x3F, 0x00 },   /*  U+007A (z) */
   { 0x38, 0x0C, 0x0C, 0x07, 0x0C, 0x0C, 0x38, 0x00 },   /*  U+007B ({) */
   { 0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x00 },   /*  U+007C (|) */
   { 0x07, 0x0C, 0x0C, 0x38, 0x0C, 0x0C, 0x07, 0x00 },   /*  U+007D (}) */
   { 0x6E, 0x3B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   /*  U+007E (~) */
},

/*
 * Credits for gc_font8x8_script:
 *
 * Based on:
 *
 * Gattars Fork
 *
 * This font is part of the ZX Origins font collection Copyright (c) 1988-2021
 * Damien Guard (https://damieng.com/zx-origins).
 *
 */

{
   { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   /*  U+0020 (space) */
   { 0x60, 0x30, 0x30, 0x18, 0x18, 0x00, 0x0c, 0x00 },   /*  U+0021 (!) */
   { 0x66, 0x22, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00 },   /*  U+0022 (") */
   { 0xc8, 0x6c, 0x7e, 0x24, 0x7e, 0x36, 0x13, 0x00 },   /*  U+0023 (#) */
   { 0x20, 0x78, 0x2c, 0x78, 0x56, 0x3e, 0x08, 0x00 },   /*  U+0024 ($) */
   { 0x46, 0x26, 0x10, 0x18, 0x0c, 0x66, 0x66, 0x00 },   /*  U+0025 (%) */
   { 0x78, 0x6c, 0x0c, 0x5c, 0x36, 0x33, 0x7e, 0x00 },   /*  U+0026 (&) */
   { 0x30, 0x10, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00 },   /*  U+0027 (') */
   { 0x70, 0x1c, 0x0c, 0x06, 0x06, 0x06, 0x1c, 0x00 },   /*  U+0028 (() */
   { 0x38, 0x60, 0x60, 0x60, 0x30, 0x38, 0x0e, 0x00 },   /*  U+0029 ()) */
   { 0x10, 0x10, 0x6c, 0x38, 0x1c, 0x36, 0x08, 0x08 },   /*  U+002A (*) */
   { 0x00, 0x18, 0x18, 0x3e, 0x0c, 0x0c, 0x00, 0x00 },   /*  U+002B (+) */
   { 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x08, 0x04 },   /*  U+002C (,) */
   { 0x00, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x00 },   /*  U+002D (-) */
   { 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00 },   /*  U+002E (.) */
   { 0x40, 0x20, 0x10, 0x18, 0x0c, 0x06, 0x06, 0x00 },   /*  U+002F (/) */
   { 0x30, 0x6c, 0x66, 0x66, 0x63, 0x27, 0x1e, 0x00 },   /*  U+0030 (0) */
   { 0x20, 0x30, 0x1c, 0x18, 0x0c, 0x0c, 0x0e, 0x00 },   /*  U+0031 (1) */
   { 0x38, 0x6c, 0x62, 0x30, 0x0c, 0x06, 0x3f, 0x00 },   /*  U+0032 (2) */
   { 0x38, 0x6c, 0x20, 0x1c, 0x30, 0x33, 0x1f, 0x00 },   /*  U+0033 (3) */
   { 0x30, 0x18, 0x2c, 0x36, 0x7f, 0x18, 0x08, 0x00 },   /*  U+0034 (4) */
   { 0x78, 0x4c, 0x1e, 0x38, 0x30, 0x33, 0x1f, 0x00 },   /*  U+0035 (5) */
   { 0x70, 0x6c, 0x06, 0x3e, 0x63, 0x33, 0x1e, 0x00 },   /*  U+0036 (6) */
   { 0x7c, 0x62, 0x30, 0x3c, 0x18, 0x0c, 0x04, 0x00 },   /*  U+0037 (7) */
   { 0x38, 0x64, 0x2c, 0x1c, 0x32, 0x33, 0x1e, 0x00 },   /*  U+0038 (8) */
   { 0x78, 0xcc, 0x66, 0x6c, 0x30, 0x30, 0x30, 0x00 },   /*  U+0039 (9) */
   { 0x00, 0x18, 0x18, 0x00, 0x0c, 0x0c, 0x00, 0x00 },   /*  U+003A (:) */
   { 0x00, 0x30, 0x30, 0x00, 0x18, 0x08, 0x04, 0x00 },   /*  U+003B (//) */
   { 0x60, 0x30, 0x18, 0x0c, 0x18, 0x30, 0x30, 0x00 },   /*  U+003C (<) */
   { 0x00, 0x00, 0x7c, 0x00, 0x3e, 0x00, 0x00, 0x00 },   /*  U+003D (=) */
   { 0x0c, 0x0c, 0x18, 0x30, 0x18, 0x0c, 0x06, 0x00 },   /*  U+003E (>) */
   { 0x3c, 0x66, 0x30, 0x18, 0x00, 0x0c, 0x0c, 0x00 },   /*  U+003F (?) */
   { 0x38, 0x6c, 0x76, 0x56, 0x3b, 0x07, 0x3e, 0x00 },   /*  U+0040 (@) */
   { 0x60, 0x58, 0x6c, 0x66, 0x3e, 0x33, 0x33, 0x00 },   /*  U+0041 (A) */
   { 0x3c, 0x6c, 0x26, 0x1e, 0x33, 0x33, 0x1f, 0x00 },   /*  U+0042 (B) */
   { 0x70, 0x6c, 0x04, 0x06, 0x03, 0x23, 0x1e, 0x00 },   /*  U+0043 (C) */
   { 0x38, 0x6c, 0x6c, 0x66, 0x66, 0x33, 0x1f, 0x00 },   /*  U+0044 (D) */
   { 0x70, 0x6c, 0x04, 0x1e, 0x03, 0x23, 0x1e, 0x00 },   /*  U+0045 (E) */
   { 0x70, 0x6c, 0x04, 0x1e, 0x03, 0x03, 0x07, 0x00 },   /*  U+0046 (F) */
   { 0x70, 0x6c, 0x04, 0x76, 0x63, 0x33, 0x2e, 0x00 },   /*  U+0047 (G) */
   { 0x88, 0xcc, 0x66, 0x7e, 0x33, 0x33, 0x33, 0x00 },   /*  U+0048 (H) */
   { 0x20, 0x30, 0x18, 0x18, 0x0c, 0x0c, 0x0c, 0x00 },   /*  U+0049 (I) */
   { 0xe0, 0xc0, 0xc0, 0x60, 0x60, 0x36, 0x0e, 0x00 },   /*  U+004A (J) */
   { 0x48, 0x6c, 0x36, 0x1e, 0x1b, 0x33, 0x33, 0x00 },   /*  U+004B (K) */
   { 0x10, 0x18, 0x0c, 0x0c, 0x06, 0x66, 0x7e, 0x00 },   /*  U+004C (L) */
   { 0x88, 0xec, 0x7e, 0x66, 0x33, 0x33, 0x33, 0x00 },   /*  U+004D (M) */
   { 0x88, 0xcc, 0x6e, 0x6e, 0x3b, 0x33, 0x33, 0x00 },   /*  U+004E (N) */
   { 0x38, 0x64, 0x66, 0x66, 0x33, 0x33, 0x1e, 0x00 },   /*  U+004F (O) */
   { 0x38, 0x6c, 0x6c, 0x26, 0x1e, 0x03, 0x01, 0x00 },   /*  U+0050 (P) */
   { 0x30, 0x6c, 0x66, 0x66, 0x3b, 0x33, 0x6e, 0x60 },   /*  U+0051 (Q) */
   { 0x38, 0x6c, 0x6c, 0x66, 0x1e, 0x1b, 0x71, 0x00 },   /*  U+0052 (R) */
   { 0xe0, 0xd8, 0x0c, 0x38, 0x62, 0x63, 0x3e, 0x00 },   /*  U+0053 (S) */
   { 0xfc, 0x30, 0x18, 0x18, 0x0c, 0x0c, 0x0c, 0x00 },   /*  U+0054 (T) */
   { 0xcc, 0x66, 0x66, 0x33, 0x33, 0x1b, 0x0e, 0x00 },   /*  U+0055 (U) */
   { 0x64, 0x66, 0x26, 0x36, 0x16, 0x0e, 0x06, 0x00 },   /*  U+0056 (V) */
   { 0x88, 0xcc, 0xd6, 0xd6, 0x5b, 0x7f, 0x3b, 0x00 },   /*  U+0057 (W) */
   { 0x64, 0x66, 0x36, 0x1c, 0x36, 0x33, 0x13, 0x00 },   /*  U+0058 (X) */
   { 0x44, 0x66, 0x36, 0x1c, 0x18, 0x0c, 0x04, 0x00 },   /*  U+0059 (Y) */
   { 0x7c, 0x64, 0x30, 0x18, 0x04, 0x26, 0x1f, 0x00 },   /*  U+005A (Z) */
   { 0x78, 0x18, 0x0c, 0x0c, 0x06, 0x06, 0x1e, 0x00 },   /*  U+005B ([) */
   { 0x04, 0x0c, 0x18, 0x18, 0x30, 0x30, 0x30, 0x00 },   /*  U+005C (\) */
   { 0x78, 0x60, 0x60, 0x30, 0x30, 0x18, 0x1e, 0x00 },   /*  U+005D (]) */
   { 0x20, 0x38, 0x7e, 0x18, 0x0c, 0x0c, 0x0c, 0x00 },   /*  U+005E (^) */
   { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff },   /*  U+005F (_) */
   { 0xe0, 0xd8, 0x08, 0x3e, 0x06, 0x66, 0x3f, 0x00 },   /*  U+0060 (`) */
   { 0x00, 0x00, 0x70, 0x6c, 0x66, 0x3b, 0x77, 0x00 },   /*  U+0061 (a) */
   { 0x0c, 0x0c, 0x36, 0x6e, 0x66, 0x33, 0x1f, 0x00 },   /*  U+0062 (b) */
   { 0x00, 0x00, 0x70, 0x6c, 0x06, 0x46, 0x3c, 0x00 },   /*  U+0063 (c) */
   { 0xc0, 0xc0, 0x6c, 0x76, 0x33, 0x33, 0x7e, 0x00 },   /*  U+0064 (d) */
   { 0x00, 0x00, 0x30, 0x6c, 0x66, 0x16, 0x7c, 0x00 },   /*  U+0065 (e) */
   { 0x00, 0xe0, 0xd8, 0x0c, 0x3c, 0x06, 0x06, 0x03 },   /*  U+0066 (f) */
   { 0x00, 0x00, 0x7c, 0x66, 0x33, 0x3e, 0x18, 0x0f },   /*  U+0067 (g) */
   { 0x0c, 0x0c, 0x76, 0x6e, 0x66, 0x33, 0x33, 0x00 },   /*  U+0068 (h) */
   { 0x60, 0x00, 0x30, 0x18, 0x18, 0x0c, 0x1c, 0x00 },   /*  U+0069 (i) */
   { 0xc0, 0x00, 0x70, 0x60, 0x30, 0x30, 0x18, 0x0e },   /*  U+006A (j) */
   { 0x08, 0x0c, 0x46, 0x36, 0x1e, 0x1b, 0x33, 0x00 },   /*  U+006B (k) */
   { 0x60, 0x30, 0x18, 0x18, 0x0c, 0x0c, 0x1c, 0x00 },   /*  U+006C (l) */
   { 0x00, 0x00, 0xec, 0xd6, 0xd6, 0x6b, 0x63, 0x00 },   /*  U+006D (m) */
   { 0x00, 0x00, 0x76, 0x6e, 0x66, 0x33, 0x33, 0x00 },   /*  U+006E (n) */
   { 0x00, 0x00, 0x30, 0x6c, 0x66, 0x66, 0x3c, 0x00 },   /*  U+006F (o) */
   { 0x00, 0x00, 0x7c, 0x6c, 0x66, 0x36, 0x0b, 0x03 },   /*  U+0070 (p) */
   { 0x00, 0x00, 0x70, 0x6c, 0x66, 0x3b, 0x37, 0x30 },   /*  U+0071 (q) */
   { 0x00, 0x00, 0x74, 0x6e, 0x06, 0x03, 0x03, 0x00 },   /*  U+0072 (r) */
   { 0x00, 0x00, 0x78, 0x0c, 0x78, 0x66, 0x3e, 0x00 },   /*  U+0073 (s) */
   { 0x60, 0x30, 0xfe, 0x18, 0x0c, 0x0c, 0x3c, 0x00 },   /*  U+0074 (t) */
   { 0x00, 0x00, 0xd8, 0xcc, 0x66, 0x76, 0x6e, 0x00 },   /*  U+0075 (u) */
   { 0x00, 0x00, 0x66, 0x36, 0x16, 0x0e, 0x06, 0x00 },   /*  U+0076 (v) */
   { 0x00, 0x00, 0x84, 0xd6, 0x6b, 0x6b, 0x37, 0x00 },   /*  U+0077 (w) */
   { 0x00, 0x00, 0x66, 0x2c, 0x1c, 0x1a, 0x33, 0x00 },   /*  U+0078 (x) */
   { 0x00, 0x00, 0x64, 0x36, 0x1e, 0x0c, 0x06, 0x06 },   /*  U+0079 (y) */
   { 0x00, 0x00, 0x78, 0x34, 0x18, 0x4c, 0x7e, 0x00 },   /*  U+007A (z) */
   { 0xf0, 0x30, 0x30, 0x0e, 0x18, 0x0c, 0x3c, 0x00 },   /*  U+007B ({) */
   { 0x20, 0x30, 0x18, 0x18, 0x18, 0x0c, 0x0c, 0x00 },   /*  U+007C (|) */
   { 0x78, 0x60, 0x30, 0xe0, 0x18, 0x18, 0x1e, 0x00 },   /*  U+007D (}) */
   { 0x6E, 0x3B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }    /*  U+007E (~) */
}

};

#endif /* FONT_8X8 */

