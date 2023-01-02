
#ifndef WINI_H
#define WINI_H

typedef int8_t INPUT_VAL;

#define INPUT_KEYS( f ) f( INPUT_KEY_UP, VK_UP ) f( INPUT_KEY_DOWN, VK_DOWN ) f( INPUT_KEY_RIGHT, VK_RIGHT ) f( INPUT_KEY_LEFT, VK_LEFT ) f( INPUT_KEY_A, 0x41 ) f( INPUT_KEY_B, 0x42 ) f( INPUT_KEY_C, 0x43 ) f( INPUT_KEY_D, 0x44 ) f( INPUT_KEY_E, 0x45 ) f( INPUT_KEY_F, 0x46 ) f( INPUT_KEY_G, 0x47 ) f( INPUT_KEY_H, 0x48 ) f( INPUT_KEY_I, 0x49 ) f( INPUT_KEY_J, 0x4a ) f( INPUT_KEY_K, 0x4b ) f( INPUT_KEY_L, 0x4c ) f( INPUT_KEY_M, 0x4d ) f( INPUT_KEY_N, 0x4e ) f( INPUT_KEY_O, 0x4f ) f( INPUT_KEY_P, 0x50 ) f( INPUT_KEY_Q, 0x51 ) f( INPUT_KEY_R, 0x52 ) f( INPUT_KEY_S, 0x53 ) f( INPUT_KEY_T, 0x54 ) f( INPUT_KEY_U, 0x55 ) f( INPUT_KEY_V, 0x56 ) f( INPUT_KEY_W, 0x57 ) f( INPUT_KEY_X, 0x58 ) f( INPUT_KEY_Y, 0x59 ) f( INPUT_KEY_Z, 0x5a ) f( INPUT_KEY_0, 0x30 ) f( INPUT_KEY_1, 0x31 ) f( INPUT_KEY_2, 0x32 ) f( INPUT_KEY_3, 0x33 ) f( INPUT_KEY_4, 0x34 ) f( INPUT_KEY_5, 0x35 ) f( INPUT_KEY_6, 0x36 ) f( INPUT_KEY_7, 0x37 ) f( INPUT_KEY_8, 0x38 ) f( INPUT_KEY_9, 0x39 ) f( INPUT_KEY_TAB, VK_TAB ) f( INPUT_KEY_SPACE, VK_SPACE ) f( INPUT_KEY_ESCAPE, VK_ESCAPE ) f( INPUT_KEY_RETURN, VK_RETURN ) f( INPUT_KEY_HOME, VK_HOME ) f( INPUT_KEY_END, VK_END ) 

#define INPUT_CLICK 127

#endif /* WINI_H */

