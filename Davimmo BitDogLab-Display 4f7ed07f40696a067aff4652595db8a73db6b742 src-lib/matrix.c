#include "matrix.h"
#include "pico/stdlib.h"
#include <stdio.h>


// **************** NUMEROS NA MATRIZ *************************
bool number_0[] = {
    0, 1, 1, 1, 0, 
    0, 1, 0, 1, 0, 
    0, 1, 0, 1, 0, 
    0, 1, 0, 1, 0, 
    0, 1, 1, 1, 0
};
bool number_1[] = {
    0, 1, 1, 1, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 1, 1, 0, 0,
    0, 0, 1, 0, 0
};
bool number_2[] = {
    0, 1, 1, 1, 0,
    0, 1, 0, 0, 0,
    0, 1, 1, 1, 0,
    0, 0, 0, 1, 0,
    0, 1, 1, 1, 0
};
bool number_3[] = {
    0, 1, 1, 1, 0,
    0, 0, 0, 1, 0,
    0, 1, 1, 1, 0,
    0, 0, 0, 1, 0,
    0, 1, 1, 1, 0
};
bool number_4[] = {
    0, 1, 0, 0, 0, 
    0, 0, 0, 1, 0, 
    0, 1, 1, 1, 0, 
    0, 1, 0, 1, 0, 
    0, 1, 0, 1, 0
};
bool number_5[] = {
    0, 1, 1, 1, 0,
    0, 0, 0, 1, 0,
    0, 1, 1, 1, 0,
    0, 1, 0, 0, 0,
    0, 1, 1, 1, 0
};
bool number_6[] = {
    0, 1, 1, 1, 0, 
    0, 1, 0, 1, 0, 
    0, 1, 1, 1, 0, 
    0, 1, 0, 0, 0, 
    0, 1, 1, 1, 0
};
bool number_7[] = {
    0, 0, 0, 1, 0, 
    0, 1, 0, 0, 0, 
    0, 0, 1, 0, 0, 
    0, 0, 0, 1, 0, 
    0, 1, 1, 1, 0
};
bool number_8[] = {
    0, 1, 1, 1, 0, 
    0, 1, 0, 1, 0, 
    0, 1, 1, 1, 0, 
    0, 1, 0, 1, 0, 
    0, 1, 1, 1, 0
};
bool number_9[] = {
    0, 1, 1, 1, 0, 
    0, 0, 0, 1, 0, 
    0, 1, 1, 1, 0, 
    0, 1, 0, 1, 0, 
    0, 1, 1, 1, 0
};
bool reset[] = {
    0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0
};
// **************************************************************
// ************************* MACROS *****************************

// ponteiro para a matriz que está atualmente sendo exibida
bool *numbers[] = {number_0, number_1, number_2, 
                    number_3, number_4, number_5,
                    number_6, number_7, number_8,
                    number_9, reset};
// variável global que controla o numero a ser exibido
volatile uint actual_number = 0;

// controla a intensidade da cor dos leds
uint8_t led_r = 100;
uint8_t led_g = 0;
uint8_t led_b = 0;

// ************ FUNÇÕES PARA CONTROLE DA MATRIZ *****************
void put_pixel(uint32_t pixel_grb) {
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b);
}

void set_one_led(bool *ledbuffer, uint8_t r, uint8_t g, uint8_t b) {
    // Define a cor com base nos parâmetros fornecidos
    uint32_t color = urgb_u32(r, g, b);

    // Define todos os LEDs com a cor especificada
    for (int i = 0; i < NUM_PIXELS; i++) {
        if (ledbuffer[i])
            put_pixel(color); // Liga o LED com um no buffer
        else
            put_pixel(0);  // Desliga os LEDs com zero no buffer
    }
}

