#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "pico/binary_info.h"
#include "inc/ssd1306.h"
#include "hardware/i2c.h"

#define VERDE 11
#define AZUL 12
#define VERMELHO 13
#define BTA 5 // Botão A
#define BTB 6 // Botão B

const uint I2C_SDA = 14;
const uint I2C_SCL = 15;

// PWM
const uint16_t periodo = 200; // período do PWM (valor máximo de tensão)
const float divisor_pwm = 16.0; // divisor fracional do clock do PWM 
const uint16_t stemp_pwm = 40; // passo de incremento e decremento do duty cycle do LED
uint8_t nivelLuminosidade = 0; // Nível inicial da luminosidade (0 a 5)
uint8_t contadorA = 0; // Contador para o botão A

ssd1306_t ssd_bm;

void config_pwm(int led) {
    uint slice; // variável para obter o slice do PWM, divisor da frequência
    gpio_set_function(led, GPIO_FUNC_PWM); // configura para saída PWM
    slice = pwm_gpio_to_slice_num(led); // retorna um slice permitindo o controle do sinal gerado 

    // Configuração do período wrap duty cycle      
    pwm_set_clkdiv(slice, divisor_pwm); // define o divisor do clock do PWM
    pwm_set_wrap(slice, periodo); // configura o valor máximo do contador (período PWM)
    pwm_set_gpio_level(led, stemp_pwm); // define o nível de PWM no pino correspondente
    pwm_set_enabled(slice, true); // habilita o PWM no slice correspondente
}

void Incrementar() {
    // Verifica se o botão A foi pressionado
    if (gpio_get(BTA) == 0) { // O botão é pressionado quando o pino é baixo
        if (nivelLuminosidade < 5) {
            contadorA++; // Incrementa o contador do botão A
            nivelLuminosidade++; // Aumenta o nível de luminosidade
            printf("Botão A pressionado: %d - Nível de luminosidade: %d\n", contadorA, nivelLuminosidade);
        }
        sleep_ms(200); // Debounce: espera 200 ms para evitar múltiplas contagens
        while (gpio_get(BTA) == 0); // Aguarda até que o botão seja solto
    }

    // Verifica se o botão B foi pressionado
    if (gpio_get(BTB) == 0) { // O botão é pressionado quando o pino é baixo
        if (nivelLuminosidade > 0) {
            nivelLuminosidade--; // Diminui o nível de luminosidade
            printf("Botão B pressionado - Nível de luminosidade: %d\n", nivelLuminosidade);
        }
        sleep_ms(200); // Debounce
        while (gpio_get(BTB) == 0); // Aguarda até que o botão seja solto
    }
}

void atualizar_display() {
    // Limpa o display
    uint8_t ssd[ssd1306_buffer_length];
    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, NULL); // Limpa o display

    // Mensagens de acordo com o nível de luminosidade
    char *mensagem;
    if (nivelLuminosidade == 0) {
        mensagem = "  Lampada desligada  ";
    } else {
        mensagem = "  Lampada ligada!    ";
    }

    ssd1306_draw_string(ssd, 5, 0, mensagem); // Exibe a mensagem de estado

    // Exibe o nível de luminosidade
    char nivel_msg[20];
    snprintf(nivel_msg, sizeof(nivel_msg), "Luminosidade: %d", nivelLuminosidade);
    ssd1306_draw_string(ssd, 5, 16, nivel_msg); // Exibe o nível de luminosidade

    // Exibe a quantidade de cliques
    char click_msg[30];
    snprintf(click_msg, sizeof(click_msg), "Cliques: %d", contadorA);
    ssd1306_draw_string(ssd, 5, 32, click_msg); // Exibe a quantidade de cliques

    render_on_display(ssd, NULL); // Atualiza o display
}

int main() {
    stdio_init_all(); // Inicializa a porta serial

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

    // Inicializa o I2C e o display
    i2c_init(i2c1, 400000); // Inicializa o I2C
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    ssd1306_init(&ssd_bm, 128, 64, false, 0x3C, i2c1);
    ssd1306_config(&ssd_bm);

    config_pwm(VERDE); // configura o PWM para o LED verde

    // Mensagem inicial
    atualizar_display();

    while (true) {
        // Atualiza o nível atual do PWM (duty cycle) com base na luminosidade
        pwm_set_gpio_level(VERDE, periodo / 5 * nivelLuminosidade); 

        // Incrementa ou decrementa a luminosidade
        Incrementar();

        // Atualiza o display com a nova informação
        atualizar_display();

        sleep_ms(100); // Espera um pouco antes de verificar novamente
    }

    return 0; // Nunca deve chegar aqui
}
