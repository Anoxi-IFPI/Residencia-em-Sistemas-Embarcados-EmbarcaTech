#include <stdio.h>
#include "pico/stdlib.h"

#define red 13 //Definição do pino
#define blue 12 //Definição do pino led azul
#define green 11 //Definição pino led verde

void configPin(uint gpio, int function){
    gpio_init(gpio);
    gpio_set_dir(gpio,function);
}


void PiscarLed(uint GPIO, int tempo){
    gpio_put(GPIO,1);
    sleep_ms(tempo);
    gpio_put(GPIO,0);
    sleep_ms(tempo);
}


int main()
{

    stdio_init_all(); //inicializa a entrada e saida padrao

    /*configuração vermelho
    gpio_init(red); // Inicialização do pino
    gpio_set_dir(red, GPIO_OUT); // definição do pino 13 como saida

    configuração azul
    gpio_init(blue);
    gpio_set_dir(blue, GPIO_OUT);

    configuração verde
    gpio_init(green);
    gpio_set_dir(green, GPIO_OUT);*/

    config_pin(red,1);
    config_pin(blue,1);
    config_pin(green,1);

    while (true) {
        printf("Hello, world!\n");
        /*gpio_put(red,1);
        sleep_ms(1000);
        gpio_put(red,0);
        sleep_ms(1000);

        gpio_put(green,1);
        sleep_ms(3000);
        gpio_put(green,0);
        sleep_ms(3000);

        gpio_put(blue,1);
        sleep_ms(5000);
        gpio_put(blue,0);
        sleep_ms(5000);*/
        PiscarLed(red,1000);
        piscarLed(blue,1000);
        piscarLed(green,1000);

    }
    sleep_ms(20);
}
