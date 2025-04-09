/*------------------------------------------------------------------------------------------#
#                                   INTERRUPÇÕES – PROJ1                                    # 
#                                 ========================                                  #
#              O objetivo deste projeto é mostrar o uso da API de hardware das interrupções #
#                          Autor: Antonio josé #File: PROJ_01_INT #Date : março, 2025       #
# ------------------------------------------------------------------------------------- ---*/  
/*                 ============== Diretivas de Processamento =====================          #
#                                                                                           #
#                  =============== Interrupção com Botôes =======================           #
#                                                                                           #
# -----------------------------------------------------------------------------------------*/


/* ==== Inclusao das bibliotecas =====*/
#include <stdio.h>
#include "pico/stdlib.h"

/* ==== Definição dos pinos do botao e led rgb ===*/
#define Azul 12
#define BTA 5

/* === criacao de uma variavel para mudar o estado do led === */
bool acesa = true;

/* ========= função de CALLBACK para tratamento de interrupcao no pino do botao A*/
void minhaInterrupcao(uint gpio, uint32_t events){
    if(gpio_get(BTA) == 0){ /*quando o boto e precionado */
        acesa =!acesa; /* verifica e auterna o estado da cariavel de (true) para (false) para ligar e desligar o led */
        gpio_put(Azul,acesa); /*acende a apaga o led*/
        printf("BOTAO A precionado| estado = %d\n ", acesa );
    }      
}

int main()
{
    /*--- Inicia a comunicação serial ---*/
    stdio_init_all();

    /*======================================== CONFIGURAÇÂO DO BOTAO ================================================
      ===utiliza a função ( --- gpio_set_function --- ) para definir o pino do botao tanto como entrada como saìda===*/
    gpio_set_function(BTA, GPIO_FUNC_SIO);
    /* --- a funcao ( --- gpio_set_dir --- para definir o pino do botao como entrada)*/
    gpio_set_dir(BTA, GPIO_IN);
    /* --- ativa o resistor de pull_up com a funcao (--- gpio_pull_up ---)*/
    gpio_pull_up(BTA);


    /*============================================== CONFIGURAÇÂO DO LED ============================================== 
      ======== utiliza a funcao ( --- gpio_set_function --- ) para definir o led tanto como entrada ou saida =========*/
    gpio_set_function(Azul, GPIO_FUNC_SIO);
    /* --- a função (gpio_set_dir) define o pino 12 referente ao LED_RGB AZUL armazenado na variavel AZUl como entrada*/
    gpio_set_dir(Azul, GPIO_OUT);

    /*=========================== FUNCAO DE CALLBACK ==============================*/
    gpio_set_irq_enabled_with_callback(BTA, GPIO_IRQ_EDGE_RISE,true,&minhaInterrupcao);

    while (true) {
        printf("Hello, world!\n");
        sleep_ms(1000);
    }
}
