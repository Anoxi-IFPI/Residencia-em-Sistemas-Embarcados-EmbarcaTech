#include "interrupt.h"
#include "button.h"
#include "led.h"
#include "matrix.h"
#include "pico/bootrom.h"
#include "pico/stdlib.h"
#include "ssd1306.h"
#include <stdio.h>

// Função que lida com eventos de interrupção
void gpio_irq_handler(uint gpio, uint32_t events) {
    // eventos de acionamento de botões
    if(gpio == BUTTON_A || gpio == BUTTON_B || gpio == JOYSTICK_BUTTON) {
        // tratamento de debouncing dos botões
        if(!debouncing(300))
            return;
        // Habilita a entrada do modo BOOTSELL ao pressionar JOYSTICK_BUTTON
        if(gpio == JOYSTICK_BUTTON) {
            // posição da matriz de reset
            actual_number = 10;
            // desliga os leds da matriz
            set_one_led(numbers[actual_number], led_r, led_g, led_b);

            //limpa o display
            ssd1306_fill(&ssd, false);
            ssd1306_send_data(&ssd);
            // Entra no modo BOOTSELL e encerra o programa
            printf("\nFinalizando o programa...\n");
            reset_usb_boot(0,0);
        }

        // Deve alternar o estado do LED verde
        if(gpio == BUTTON_A) {
            if(gpio_get(LED_GREEN)) {
                puts("\nLED verde desligado.");
                // Mensagem no display
                // limpa o display
                ssd1306_fill(&ssd, false);
                ssd1306_send_data(&ssd);
                ssd1306_draw_string(&ssd, "LED VERDE", 10, 20);
                ssd1306_draw_string(&ssd, "DESLIGADO", 10, 30);
                ssd1306_send_data(&ssd);
            }
            else {
                puts("\nLED verde ligado.");
                // Mensagem no display
                // limpa o display
                ssd1306_fill(&ssd, false);
                ssd1306_send_data(&ssd);
                ssd1306_draw_string(&ssd, "LED VERDE", 10, 20);
                ssd1306_draw_string(&ssd, "LIGADO", 10, 30);
                ssd1306_send_data(&ssd);
            }
            // ALTERNA O ESTADO DO LED verde
            gpio_put(LED_GREEN, !gpio_get(LED_GREEN));
        }
        if(gpio == BUTTON_B) {
            if(gpio_get(LED_BLUE)) {
                puts("\nLED azul desligado.");
                // Mensagem no display
                // limpa o display
                ssd1306_fill(&ssd, false);
                ssd1306_send_data(&ssd);
                ssd1306_draw_string(&ssd, "LED AZUL", 10, 20);
                ssd1306_draw_string(&ssd, "DESLIGADO", 10, 30);
                ssd1306_send_data(&ssd);
            }
            else {
                puts("\nLED azul ligado.");
                // Mensagem no display
                // limpa o display
                ssd1306_fill(&ssd, false);
                ssd1306_send_data(&ssd);
                ssd1306_draw_string(&ssd, "LED AZUL", 10, 20);
                ssd1306_draw_string(&ssd, "LIGADO", 10, 30);
                ssd1306_send_data(&ssd);
            }
            // ALTERNA O ESTADO DO LED azul
            gpio_put(LED_BLUE, !gpio_get(LED_BLUE));
        }

    }
}