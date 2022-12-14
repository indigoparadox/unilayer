
#ifndef NULLG_H
#define NULLG_H

/**
 * \addtogroup unilayer_graphics
 *
 * \{
 */

/*! \file nullg.h
 *  \brief Platform-overridden typedefs and macros for drawing graphics.
 */

/*! \brief Representation for specific colors on the current platform. */
typedef int GRAPHICS_COLOR;

/**
 * \brief Struct representing a bitmap on the current platform.
 * \extends GRAPHICS_BITMAP_BASE
 *
 * This should match the size of the GRAPHICS_BITMAP_BASE.
 */
struct GRAPHICS_BITMAP {
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

/**
 * \brief Platform-overridden struct supporting graphics engine.
 *
 * Contains globals needed to perform graphical operations on the current
 * platform.
 */
struct GRAPHICS_ARGS {
};

/**
 * \addtogroup unilayer_graphics_colors Unilayer Graphics Colors
 * \brief Platform-defined colors in which to draw graphical elements.
 *
 * This is an abstraction to handle how different platforms define colors.
 * These colors may appear slightly different on each platform.
 *
 * \{
 */

/*! \brief GRAPHICS_COLOR value for black on the current platform. */
#define GRAPHICS_COLOR_BLACK     0
/*! \brief GRAPHICS_COLOR value for cyan on the current platform. */
#define GRAPHICS_COLOR_CYAN      1
/*! \brief GRAPHICS_COLOR value for magenta on the current platform. */
#define GRAPHICS_COLOR_MAGENTA   2
/*! \brief GRAPHICS_COLOR value for white on the current platform. */
#define GRAPHICS_COLOR_WHITE     3

/*! \} */ /* unilayer_graphics_colors */

/*! \} */

#endif /* !NULLG_H */

