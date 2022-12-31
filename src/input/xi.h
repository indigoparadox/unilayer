
#ifndef XI_H
#define XI_H

typedef int8_t INPUT_VAL;

/* Sniffed most of these using xev or printf() on the keycode directly and
 * guessed the rest. They appear to be in QWERTY order.
 */
#define INPUT_KEYS( f ) f( INPUT_KEY_Q, 24 ) f( INPUT_KEY_W, 25 ) f( INPUT_KEY_E, 26 ) f( INPUT_KEY_R, 27 ) f( INPUT_KEY_T, 28 ) f( INPUT_KEY_Y, 29 ) f( INPUT_KEY_U, 30 ) f( INPUT_KEY_I, 31 ) f( INPUT_KEY_O, 32 ) f( INPUT_KEY_P, 33 ) f( INPUT_KEY_A, 38 ) f( INPUT_KEY_S, 39 ) f( INPUT_KEY_D, 40 ) f( INPUT_KEY_F, 41 ) f( INPUT_KEY_G, 42 ) f( INPUT_KEY_H, 43 ) f( INPUT_KEY_J, 44 ) f( INPUT_KEY_K, 45 ) f( INPUT_KEY_L, 46 ) f( INPUT_KEY_Z, 52 ) f( INPUT_KEY_X, 53 ) f( INPUT_KEY_C, 54 ) f( INPUT_KEY_V, 55 ) f( INPUT_KEY_B, 56 ) f( INPUT_KEY_N, 57 ) f( INPUT_KEY_M, 58 ) f( INPUT_KEY_ESCAPE, 9 ) f( INPUT_KEY_TAB, 23 ) f( INPUT_KEY_RETURN, 36 ) f( INPUT_KEY_SPACE, 65 ) f( INPUT_KEY_COMMA, 59 ) f( INPUT_KEY_PERIOD, 60 )

#define INPUT_CLICK 127

#endif /* XI_H */

