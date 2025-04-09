#include <stdio.h>
#include "pico/stdlib.h"

#define bot_A 5   // Pino do botão
#define azul 12 // Led azul
#define verm 13 // Led vermelho
#define verd 11  // Pino do LED
#define bot_B 6   // Pino do botão


void config_led(uint gpio, bool stado){
    gpio_init(gpio);
    gpio_set_dir(gpio, stado);
}

void config_butoon(uint gpio, bool stado){
    gpio_init(gpio);
    gpio_set_dir(gpio, stado);
    gpio_pull_up(gpio);
}

void misturarCorres(uint green, uint blue, uint red){
    gpio_put(verd, green);
    gpio_put(azul,blue);
    gpio_put(verm,red);
}

int main() {
    stdio_init_all();

    config_butoon(bot_A, false);
    config_butoon(bot_B,false);
    config_led(verd,true);
    config_led(azul,true);
    config_led(verm,true);


    // Inicializa o pino do botão
    // gpio_init(bot_A);
    // gpio_set_dir(bot_A, GPIO_IN); // Define o pino como entrada
    // gpio_pull_up(bot_A);           // Ativa o resistor de pull-up

    // gpio_init(bot_B);
    // gpio_set_dir(bot_B, GPIO_IN);
    // gpio_pull_up(bot_B);

    // // Inicializa o pino do LED
    // gpio_init(verd);
    // gpio_set_dir(verd, GPIO_OUT); // Define o pino como saída

    while (true) {
        printf("Hello, world!\n");
        
        // Lógica para acender o LED
        if (gpio_get(bot_A) == 0) {  // Verifica se o botão está pressionado
            misturarCorres(1,1,0);
        }else if(gpio_get(bot_B) == 0){
            misturarCorres(0,1,1);
        }else{
            misturarCorres(0,0,0);
        }
        
        sleep_ms(20); // Aguarda 1 segundo
    }
}
