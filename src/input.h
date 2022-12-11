
#ifndef INPUT_H
#define INPUT_H

uint8_t input_init();
uint8_t input_poll( int16_t* x, int16_t* y );
void input_shutdown();

#define INPUT_CLICK 0xff

#endif /* INPUT_H */

