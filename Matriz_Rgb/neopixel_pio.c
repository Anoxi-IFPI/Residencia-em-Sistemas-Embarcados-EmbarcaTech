#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/pwm.h"

// Biblioteca gerada pelo arquivo .pio durante compilação.
#include "ws2818b.pio.h"

// Definição do número de LEDs e pino.
#define LED_COUNT 25
#define LED_PIN 7

// Definição de pixel GRB
struct pixel_t {
    uint8_t G, R, B; // Três valores de 8-bits compõem um pixel.
};
typedef struct pixel_t pixel_t;
typedef pixel_t npLED_t; // Mudança de nome de "struct pixel_t" para "npLED_t" por clareza.

// Declaração do buffer de pixels que formam a matriz.
npLED_t leds[LED_COUNT];

// Variáveis para uso da máquina PIO.
PIO np_pio;
uint sm;

// Configurações de PWM
const uint16_t periodo = 200; // período do PWM (valor máximo de tensão)
const float divisor_pwm = 16.0; // divisor fracional do clock do PWM 
uint8_t nivelLuminosidade = 0; // Nível inicial da luminosidade (0 a 5)

// Botões
#define BTA 5 // Botão A
#define BTB 6 // Botão B

// Função para inicializar a matriz de LEDs
void npInit(uint pin) {
    uint offset = pio_add_program(pio0, &ws2818b_program);
    np_pio = pio0;
    sm = pio_claim_unused_sm(np_pio, false);
    ws2818b_program_init(np_pio, sm, offset, pin, 800000.f);
    
    // Limpa o buffer de pixels
    for (uint i = 0; i < LED_COUNT; ++i) {
        leds[i].R = 0;
        leds[i].G = 0;
        leds[i].B = 0;
    }
}

// Função para escrever os dados do buffer nos LEDs
void npWrite() {
    for (uint i = 0; i < LED_COUNT; ++i) {
        pio_sm_put_blocking(np_pio, sm, leds[i].G);
        pio_sm_put_blocking(np_pio, sm, leds[i].R);
        pio_sm_put_blocking(np_pio, sm, leds[i].B);
    }
    sleep_us(100); // Espera 100us, sinal de RESET do datasheet.
}

// Função para configurar os botões
void setupButtons() {
    gpio_init(BTA);
    gpio_set_dir(BTA, GPIO_IN);
    gpio_pull_up(BTA); // Resistor pull-up para o botão A

    gpio_init(BTB);
    gpio_set_dir(BTB, GPIO_IN);
    gpio_pull_up(BTB); // Resistor pull-up para o botão B
}

// Função para ajustar a luminosidade
void ajustarLuminosidade() {
    if (gpio_get(BTA) == 0) { // O botão A é pressionado quando o pino é baixo
        if (nivelLuminosidade < 5) {
            nivelLuminosidade++; // Aumenta o nível de luminosidade
            printf("Luminosidade aumentada para: %d\n", nivelLuminosidade);
        }
        sleep_ms(200); // Debounce
    }

    if (gpio_get(BTB) == 0) { // O botão B é pressionado quando o pino é baixo
        if (nivelLuminosidade > 0) {
            nivelLuminosidade--; // Diminui o nível de luminosidade
            printf("Luminosidade diminuída para: %d\n", nivelLuminosidade);
        }
        sleep_ms(200); // Debounce
    }
}

// Função para desenhar na matriz de LEDs com base no nível de luminosidade
void drawLamp() {
    for (int i = 0; i < LED_COUNT; ++i) {
        // Ajuste de cor baseado no nível de luminosidade
        leds[i].R = (uint8_t)(255 * nivelLuminosidade / 5); // Vermelho
        leds[i].G = (uint8_t)(255 * nivelLuminosidade / 5); // Verde
        leds[i].B = (uint8_t)(0); // Azul

        // Se quiser cores diferentes, ajuste aqui
    }
}

int main() {
    stdio_init_all();
    npInit(LED_PIN);
    setupButtons();

    while (true) {
        ajustarLuminosidade(); // Ajusta a luminosidade com base nos botões
        drawLamp(); // Desenha a matriz de LEDs com o nível atual de luminosidade
        npWrite(); // Escreve os dados nos LEDs
        sleep_ms(100); // Espera um pouco antes de verificar novamente
    }

    return 0; // Nunca deve chegar aqui
}
