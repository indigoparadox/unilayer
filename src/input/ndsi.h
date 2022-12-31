
#ifndef NDSI_H
#define NDSI_H

typedef int8_t INPUT_VAL;

/* These are pretty much arbitrary, and ndsi.c is written to feed these back
 * according to gamepad input.
 */
#define INPUT_KEYS( f ) f( INPUT_KEY_A, 1 ) f( INPUT_KEY_D, 2 ) f( INPUT_KEY_ESCAPE, 3 ) f( INPUT_KEY_S, 4 ) f( INPUT_KEY_W, 5 ) f( INPUT_KEY_Z, 6 ) f( INPUT_KEY_E, 7 ) 

#define INPUT_CLICK 127

#endif /* NDSI_H */

