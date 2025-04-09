#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include "semphr.h"
#include "queue.h"

//Definição dos Leds
#define BUTTON_A_PIN  5
#define RED_LED_PIN 13
#define GREEN_LED_PIN 11
#define BLUE_LED_PIN 12


QueueHandle_t xFilaBotaoA;


void vLeituraBotaoA(void *pvParameters) {
    bool estado_botao;
    for (;;) {
        estado_botao = (gpio_get(BUTTON_A_PIN) == 0);
        xQueueSend(xFilaBotaoA, &estado_botao, 0);
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
//Configuração para exibição dos leds
void exibir_led(uint red, uint green, uint blue ){
    gpio_put(RED_LED_PIN, red);
    gpio_put(GREEN_LED_PIN, green);
    gpio_put(BLUE_LED_PIN, blue);    

}
void vProcessaBotaoA(void *pvParameters) {
    bool estado_recebido;
    for (;;) {
        if (xQueueReceive(xFilaBotaoA, &estado_recebido, portMAX_DELAY) == pdTRUE) {
            if (estado_recebido) {
                //Função de exibição dos Leds
                exibir_led(1,0,0); //Acende o led vermelho
                printf("LED Vermelho: Ligado\n");
            } else {
                //Função de exibição dos Leds
                exibir_led(0,0,0); //Apaga todos os Leds
                printf("LED Vermelho: Desligado\n");
            }
           sleep_ms(20); 
        }
    }
}

//função de configuração dos pinos
void config_pin_led(uint gpio){
    gpio_init(gpio);
    gpio_set_dir(gpio,GPIO_OUT);
}

//Configuração de configuração do botão
void config_booton(uint gpio){
    gpio_init(gpio);
    gpio_set_dir(BUTTON_A_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_A_PIN);
}



void setup() {
    //Chama a função de configuração do botão A 
    config_booton(BUTTON_A_PIN);
    
    //Chama a função de configuração dos Leds
    config_pin_led(GREEN_LED_PIN);
    config_pin_led(BLUE_LED_PIN);
    config_pin_led(RED_LED_PIN);

    //Função de exibição dos Leds
    exibir_led(0,0,0);
}


int main() {
    stdio_init_all();

    xFilaBotaoA = xQueueCreate(10, sizeof(bool));

    setup();

    xTaskCreate(vLeituraBotaoA, "Leitura Botao A", 128, NULL, 1, NULL);
    xTaskCreate(vProcessaBotaoA, "Processa Botao A", 128, NULL, 2, NULL);

    vTaskStartScheduler();

    while (1);
    return 0;
}