
#ifndef RESOURCE_NULL_H
#define RESOURCE_NULL_H

/*! \file nullr.h
 *  \brief Platform-overridden typedefs and macros for loading assets.
 */

/*! String or numerical identifier specifying an asset on disk. */
typedef uint32_t RESOURCE_ID;
/*! Resource handle for physical bitmap asset. */
typedef MEMORY_HANDLE RESOURCE_BITMAP_HANDLE;
/*! Resource handle for physical JSON asset. */
typedef MEMORY_HANDLE RESOURCE_JSON_HANDLE;

/**
 * \brief Compare two RESOURCE_ID instances for equivalency.
 */
#define resource_compare_id( a, b ) (a == b)
/**
 * \brief Set the content of RESOURCE_ID dest to the same as source.
 */
#define resource_assign_id( dest, source ) dest = source

#endif /* RESOURCE_NULL_H */

