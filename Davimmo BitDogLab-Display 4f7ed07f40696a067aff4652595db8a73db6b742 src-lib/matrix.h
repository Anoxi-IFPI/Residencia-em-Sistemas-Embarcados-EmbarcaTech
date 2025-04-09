#ifndef MATRIX_H
#define MATRIX_H

#include "ws2812.pio.h"

#define NUM_PIXELS 25
#define WS2812_PIN 7
#define IS_RGBW false


extern uint8_t led_r; // Intensidade do vermelho
extern uint8_t led_g; // Intensidade do verde
extern uint8_t led_b; // Intensidade do azul
extern bool *numbers[];
extern volatile uint actual_number;
void set_one_led(bool *ledbuffer, uint8_t r, uint8_t g, uint8_t b);


#endif