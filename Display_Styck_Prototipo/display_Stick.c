#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "inc/ssd1306.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"

const uint I2C_SDA = 14;
const uint I2C_SCL = 15;

#define led_AZ 12
#define Led_VD 11
#define led_VM 13

const int vRx = 26;             // Pino de leitura do eixo X do Joystick (conectado ao ADC)
const int vRy = 27;             // Pino de leitura do eixo Y do Joystick (conectado ao ADC)
const int ADC_canal_0 = 0;      // Canal ADC para o eixo X do Joystick
const int ADC_canal_1 = 1;      // Canal ADC para o eixo Y do Joystick
const int SW_Pin_Booton = 22;   // Pino de leitura do botão do Joystick  

// Função para configurar o Joystick (Pinos de leitura ADC)
void config_Joystick() {  
    adc_init();         // Inicializa o módulo ADC
    adc_gpio_init(vRx); // Configura o Pino vRx (eixo X) para entrada ADC
    adc_gpio_init(vRy); // Configura o pino vRy (eixo Y) para entrada ADC
    gpio_init(SW_Pin_Booton);               // Inicializa o pino do botão
    gpio_set_dir(SW_Pin_Booton, GPIO_IN);   // Configura o pino como entrada
    gpio_pull_up(SW_Pin_Booton);            // Ativa o pull-up
}

// Configura os pinos de entrada e saída
void conf_pin() {
    gpio_init(led_AZ);
    gpio_set_dir(led_AZ, true);
    gpio_init(Led_VD);
    gpio_set_dir(Led_VD, true);
    gpio_init(led_VM);
    gpio_set_dir(led_VM, true);
}

// Função de configuração geral
void setup_config() {
    stdio_init_all();   // Inicializa a porta serial
    config_Joystick();   // Chama a função de configuração do Joystick
    conf_pin();          // Chama a função de configuração dos pinos
}

// Função para acender LEDs
void acender_led(int led_azul, int led_verde, int led_vermelho) {
    gpio_put(led_AZ, led_azul);
    gpio_put(Led_VD, led_verde);
    gpio_put(led_VM, led_vermelho);
}

// Função para leitura dos eixos (X e Y)
void leitura_eixos_Joystick_canal_0(uint16_t *eixo_x) {
    adc_select_input(ADC_canal_0); // Seleciona o canal ADC para o eixo X
    *eixo_x = adc_read();           // Lê o valor do eixo X (0 a 4095)
}

void leitura_eixos_Joystick_canal_1(uint16_t *eixo_y) {
    adc_select_input(ADC_canal_1); // Seleciona o canal do eixo Y
    *eixo_y = adc_read();           // Lê o valor do eixo Y
}

// Função para atualizar a mensagem no display OLED
void atualizar_display(ssd1306_t *ssd, const char *mensagem) {
    uint8_t ssd_buffer[ssd1306_buffer_length];
    memset(ssd_buffer, 0, ssd1306_buffer_length); // Limpa o buffer
    ssd1306_draw_string(ssd_buffer, 0, 0, mensagem); // Desenha a mensagem
    struct render_area frame_area = {
        .start_column = 0,
        .end_column = ssd1306_width - 1,
        .start_page = 0,
        .end_page = ssd1306_n_pages - 1
    };
    render_on_display(ssd_buffer, &frame_area); // Atualiza o display
}

// Função principal
int main() {
    uint16_t valor_x, valor_y; // Variáveis para armazenar os valores do joystick
    setup_config();            // Chama a função de configuração

    // Inicializa o I2C e o display OLED
    i2c_init(i2c1, 400000); // Inicializa o I2C com 400kHz
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    
    ssd1306_t ssd;
    ssd1306_init_bm(&ssd, 128, 64, false, 0x3C, i2c1);
    ssd1306_config(&ssd);
    
    printf("Joystick\n"); // Exibe uma mensagem inicial via porta serial

    // Loop principal
    while (1) {
        acender_led(0, 0, 0); // LEDs iniciam desligados

        // Lê os valores dos eixos do joystick
        leitura_eixos_Joystick_canal_0(&valor_x);
        leitura_eixos_Joystick_canal_1(&valor_y);

        // Configuração para exibição dos LEDs nos eixos X e Y
        if (valor_x < 1920) {
            acender_led(255, 255, 0); // LED verde e azul (azul claro)
            atualizar_display(&ssd, "LED: Azul Claro");
        } else if (valor_x > 2085) {
            acender_led(0, 255, 255); // LED verde e vermelho (amarelo)
            atualizar_display(&ssd, "LED: Amarelo");
        }

        if (valor_y < 1930) {
            acender_led(255, 0, 255); // LED azul e vermelho acesos (lilas)
            atualizar_display(&ssd, "LED: Lilas");
        } else if (valor_y > 2040) {
            acender_led(255, 255, 255); // LED verde, azul e vermelho acesos (branco)
            atualizar_display(&ssd, "LED: Branco");
        }

        // Leitura e exibição dos valores de X e Y no monitor serial
        printf("X: %d\n", valor_x);
        printf("Y: %d\n", valor_y);

        // Delay antes da próxima leitura
        sleep_ms(200);
    }

    return 0;
}
