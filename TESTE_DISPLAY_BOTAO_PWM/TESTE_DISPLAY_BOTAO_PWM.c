#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "pico/binary_info.h"
#include "inc/ssd1306.h" // Inclui o cabeçalho da biblioteca SSD1306
#include "hardware/i2c.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define VERDE 11
#define AZUL 12
#define VERMELHO 13
#define BTA 5 // Botão A
#define BTB 6 // Botão B

// PWM
const uint16_t periodo = 200; // período do PWM (valor máximo de tensão)
const float divisor_pwm = 16.0; // divisor fracional do clock do PWM 
const uint16_t stemp_pwm = 40; // passo de incremento e decremento do duty cycle do LED
uint8_t nivelLuminosidade = 0; // Nível inicial da luminosidade (1 a 5)
uint8_t contadorA = 0; // Contador para o botão A

// Display OLED
#define I2C_PORT i2c0
#define I2C_SDA 14 // Pino SDA na Bit Dog Lab
#define I2C_SCL 15 // Pino SCL na Bit Dog Lab
#define OLED_ADDRESS 0x3C // Endereço I2C do display OLED

// Variável global para o display
ssd1306_t display;

void config_pwm(int led) {
    uint slice; // variável para obter o slice do PWM, divisor da frequência
    gpio_set_function(led, GPIO_FUNC_PWM); // configura para saída PWM
    slice = pwm_gpio_to_slice_num(led); // retorna um slice permitindo o controle do sinal gerado 

    // Configuração do período wrap duty cycle      
    pwm_set_clkdiv(slice, divisor_pwm); // define o divisor do clock do PWM
    pwm_set_wrap(slice, periodo);//configura o valor maximo do contador(periodo pwm)
    pwm_set_gpio_level(led, stemp_pwm); //define o nivel de pwm no pino correspondente
    pwm_set_enabled(slice, true); //habilita o pwm no slice correspondente
}

void exibirMensagemOLED(const char* mensagem) {
    ssd1306_clear(&display); // Limpa o display
    ssd1306_draw_string(&display, 0, 0, mensagem); // Exibe a mensagem na posição (0, 0)
    ssd1306_show(&display); // Atualiza o display
}

void Incrementar(int led, int botao) {
    // Verifica se o botão foi pressionado
    if (gpio_get(botao) == 0) { // O botão é pressionado quando o pino é baixo
        if (botao == BTA && nivelLuminosidade < 5) {
            contadorA++; // Incrementa o contador do botão A
            nivelLuminosidade++; // Aumenta o nível de luminosidade
            char mensagem[50];
            if (nivelLuminosidade == 1) {
                snprintf(mensagem, sizeof(mensagem), "Lampada ligada nivel 1");
            } else if (nivelLuminosidade == 5) {
                snprintf(mensagem, sizeof(mensagem), "Lampada maximo nivel 5");
            } else {
                snprintf(mensagem, sizeof(mensagem), "Lampada nivel %d", nivelLuminosidade);
            }
            exibirMensagemOLED(mensagem);
            printf("Botão A pressionado: %d - Lampada ligada Nível de luminosidade: %d\n", contadorA, nivelLuminosidade);
        } else if (botao == BTB && nivelLuminosidade > 1) {
            nivelLuminosidade--; // Diminui o nível de luminosidade
            char mensagem[50];
            if (nivelLuminosidade == 0) {
                snprintf(mensagem, sizeof(mensagem), "Lampada apagada");
            } else {
                snprintf(mensagem, sizeof(mensagem), "Lampada nivel %d", nivelLuminosidade);
            }
            exibirMensagemOLED(mensagem);
            printf("Botão B pressionado - Nível de luminosidade da lampada: %d\n", nivelLuminosidade);
        }
        else if (BTB && nivelLuminosidade == 1) { // Quando o nível de luminosidade for 1
            nivelLuminosidade--; // Decrementa 1 para que a contagem vá a 0
            if (nivelLuminosidade == 0) { // Após zerar, verifica se a contagem é igual a 0
                gpio_put(led, 0); // Quando chegar em 0, desliga a lampada
                exibirMensagemOLED("Lampada apagada");
                printf("Botão B pressionado - Lampada desligada - Nível de luminosidade: %d\n", nivelLuminosidade);
            }
        }

        sleep_ms(200); // Debounce: espera 200 ms para evitar múltiplas contagens

        // Aguarda até que o botão seja solto
        while (gpio_get(botao) == 0);
    }
}

int main() {
    stdio_init_all();

    // Inicializa os LEDs
    gpio_init(VERDE);
    gpio_set_dir(VERDE, GPIO_OUT);

    gpio_init(AZUL);
    gpio_set_dir(AZUL, GPIO_OUT);

    gpio_init(VERMELHO);
    gpio_set_dir(VERMELHO, GPIO_OUT);

    // Inicializa os botões
    gpio_init(BTA);
    gpio_set_dir(BTA, GPIO_IN);
    gpio_pull_up(BTA); // Resistor pull-up para o botão A

    gpio_init(BTB);
    gpio_set_dir(BTB, GPIO_IN);
    gpio_pull_up(BTB); // Resistor pull-up para o botão B

    // Inicializa o display OLED
    i2c_init(I2C_PORT, 400000); // Inicializa o I2C com frequência de 400 kHz
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // Inicializa o display SSD1306
    ssd1306_init_bm(&display, 128, 64, false, OLED_ADDRESS, I2C_PORT);
    ssd1306_config(&display);

    config_pwm(VERDE); // configura o PWM para o LED verde

    while (true) {
        // Define o nível atual do PWM (duty cycle) com base na luminosidade
        pwm_set_gpio_level(VERDE, periodo / 5 * nivelLuminosidade); 
        Incrementar(VERDE, BTA); // Incrementa usando o botão A
        Incrementar(VERDE, BTB); // Decrementa usando o botão B
        sleep_ms(100); // Espera um pouco antes de verificar novamente
    }

    return 0; // Nunca deve chegar aqui
}