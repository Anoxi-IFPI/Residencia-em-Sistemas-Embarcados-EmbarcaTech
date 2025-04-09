#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/uart.h"
#include "hardware/i2c.h"
#include "pico/bootrom.h"


// ************************* LIBS ******************************

// biblioteca para manipulação da matriz de leds
#include "matrix.h"
// biblioteca para manipulação de botões
#include "lib/button.h"
// biblioteca para manipulação de LEDs
#include "lib/led.h"
// biblioteca para manipulação de eventos de interrupção
#include "lib/interrupt.h"
// menu interativo
#include "menu.h"
//arquivo gerado pelo Cmakelist.txt para manipulação de PIO
#include "lib/ws2812.pio.h"
// biblioteca para manipulação do display SSD1306
#include "lib/ssd1306.h"

// *********************** MACROS *******************************
// Configurações da UART
#define UART_ID uart0
#define UART_TX 0
#define UART_RX 1
#define BAUD_RATE 115200
// Configurações da I2C
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C

int main() {
    // habilita o uso da comunicação UART
    stdio_init_all();

    // Configuração da UART
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX, GPIO_FUNC_UART);
    gpio_set_function(UART_RX, GPIO_FUNC_UART);

    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
    gpio_pull_up(I2C_SDA); // Pull up the data line
    gpio_pull_up(I2C_SCL); // Pull up the clock line
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
    ssd1306_config(&ssd); // Configura o display
    ssd1306_send_data(&ssd); // Envia os dados para o display
  
    // Limpa o display. O display inicia com todos os pixels apagados.
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);

    
    // Inicializa os LEDS
    init_led(LED_GREEN);
    init_led(LED_BLUE);
    // Inicialização do botões e configuração de eventos de interrupção
    init_button_with_interrupt(BUTTON_A, GPIO_IRQ_EDGE_FALL, true);
    init_button_with_interrupt(BUTTON_B, GPIO_IRQ_EDGE_FALL, true);
    init_button_with_interrupt(JOYSTICK_BUTTON, GPIO_IRQ_EDGE_FALL, true);

    // Configuração da matriz de LEDS
    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);


    // Aguarda pressionar Enter para exibir o menu pela primeira vez
    getchar();
    char input;
    // rotina principal
    while(true) {

        // so vai entrar nessa rotina se a USB estiver conectada
        if(stdio_usb_connected()) {
            main_menu();
            puts("");
            while(true) {
                printf("Escolha uma opcao: ");
                scanf("%c", &input);
                // encerra o loop se pressionar
                    // um caractere entre 0 e 9
                    // um caractere entre A-Z
                    // um caractere entre a-z
                    // /: para finalizar o programa
                if((input>= '0' && input <= '9')
                    || (input >= 'A' && input <= 'Z')
                    || (input >= 'a' && input <= 'z')
                    || input == '/')
                    break;
                printf("Caractere inválido!\n");
            }
            // Exibe o caractere digitado no display (se for alfanumérico)
            if((input >= 'A' && input <= 'Z') || 
            (input >= 'a' && input <= 'z') || 
            (input>= '0' && input <= '9')) {
                char input_string[20];
                // limpa o display
                ssd1306_fill(&ssd, false);
                ssd1306_send_data(&ssd);
                ssd1306_draw_string(&ssd, "caractere", 10, 20);
                snprintf(input_string, sizeof(input_string), "digitado %c", input);
                ssd1306_draw_string(&ssd, input_string, 10, 30);
                ssd1306_send_data(&ssd);
            }
            if(!(input>= '0' && input <= '9'))
                printf("\nCaractere digitado: %c\n", input);
            switch (input) {
                case '0':
                    if(actual_number != 0) {
                        actual_number = 0;
                        set_one_led(numbers[actual_number], led_r, led_g, led_b);
                    }
                    printf("\nExibindo o numero %d na matriz\n", actual_number);
                    break;
                case '1':
                    if(actual_number != 1) {
                        actual_number = 1;
                        set_one_led(numbers[actual_number], led_r, led_g, led_b);
                    }
                    printf("\nExibindo o numero %d na matriz\n", actual_number);
                    break;
                case '2':
                    if(actual_number != 2) {
                        actual_number = 2;
                        set_one_led(numbers[actual_number], led_r, led_g, led_b);
                    }
                    printf("\nExibindo o numero %d na matriz\n", actual_number);
                    break;
                case '3':
                    if(actual_number != 3) {
                        actual_number = 3;
                        set_one_led(numbers[actual_number], led_r, led_g, led_b);
                    }
                    printf("\nExibindo o numero %d na matriz\n", actual_number);
                    break;
                case '4':
                    if(actual_number != 4) {
                        actual_number = 4;
                        set_one_led(numbers[actual_number], led_r, led_g, led_b);
                    }
                    printf("\nExibindo o numero %d na matriz\n", actual_number);
                    break;
                case '5':
                    if(actual_number != 5) {
                        actual_number = 5;
                        set_one_led(numbers[actual_number], led_r, led_g, led_b);
                    }
                    printf("\nExibindo o numero %d na matriz\n", actual_number);
                    break;   
                case '6':
                    if(actual_number != 6) {
                        actual_number = 6;
                        set_one_led(numbers[actual_number], led_r, led_g, led_b);
                    }
                    printf("\nExibindo o numero %d na matriz\n", actual_number);
                    break;  
                case '7':
                    if(actual_number != 7) {
                        actual_number = 7;
                        set_one_led(numbers[actual_number], led_r, led_g, led_b);
                    }
                    printf("\nExibindo o numero %d na matriz\n", actual_number);
                    break; 
                case '8':
                    if(actual_number != 8) {
                        actual_number = 8;
                        set_one_led(numbers[actual_number], led_r, led_g, led_b);
                    }
                    printf("\nExibindo o numero %d na matriz\n", actual_number);
                    break; 
                case '9':
                    if(actual_number != 9) {
                        actual_number = 9;
                        set_one_led(numbers[actual_number], led_r, led_g, led_b);
                    }
                    printf("\nExibindo o numero %d na matriz\n", actual_number);
                    break;   
                // ESCAPE para finalizar o programa
                case '/':
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
                    break;
                default:
                    break;
            }
        }
        // para evitar que o loop seja executado muito rapidamente
        sleep_ms(100);
    }
}