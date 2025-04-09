#include "pico/stdlib.h"    //Biblioteca Guarda_Chuva 
#include "FreeRTOS.h"       //Biblioteca FreeRTOS que permite o uso de multitarefas
#include "task.h"           //Definições relaçionadas ao FreeRTOS
#include <stdio.h>          //Biblioteca padrao do C
#include "semphr.h"         //Definição para utilização de semafaros
#include "queue.h"          //Definição para utilização de filas

//Definição dos Pinos dos Leds 
#define led_pin_Blue 12
#define led_pin_Green 11  
const uint led_pin_red = 12;

//Definição dos Pinos dos Botões
#define pin_booton_A 5
#define pin_booton_B 6

QueueHandle_t Fila_Botao_A;

void xMinhaTarefa_1(void * xParametros){

}

void vBlinkTask(){
    for(;;)
    {
        gpio_put(led_pin_red, 1);
        vTaskDelay(250);
        gpio_put(led_pin_red, 0);
        vTaskDelay(250);
        printf("Blinking\n");
    }
}

int main(){
    stdio_init_all();
    gpio_init(led_pin_red);
    gpio_set_dir(led_pin_red, GPIO_OUT);
    xTaskCreate(vBlinkTask, "Blink Task", 128, NULL, 1, NULL);
    vTaskStartScheduler();
}
