
#ifndef XI_H
#define XI_H

typedef int8_t INPUT_VAL;

/* Sniffed these using the printf() on the keycode directly. They appear to be
 * in QWERTY order.
 */

#define XKEY_q          24
#define XKEY_w          25
#define XKEY_e          26
#define XKEY_a          38
#define XKEY_s          39
#define XKEY_d          40
#define XKEY_z          52

#define INPUT_KEY_LEFT  XKEY_a
#define INPUT_KEY_RIGHT XKEY_d
#define INPUT_KEY_QUIT  XKEY_q
#define INPUT_KEY_DOWN  XKEY_s
#define INPUT_KEY_UP    XKEY_w
#define INPUT_KEY_OK    XKEY_z
#define INPUT_KEY_MENU  XKEY_e

#define INPUT_CLICK 127

#endif /* XI_H */

