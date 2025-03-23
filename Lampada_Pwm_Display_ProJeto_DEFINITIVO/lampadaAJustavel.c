#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "inc/ssd1306.h"
#include "hardware/i2c.h"
#include "hardware/pwm.h"

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
uint8_t nivelLuminosidade = 0; // Nível inicial da luminosidade (1 a 5)
uint8_t contadorA = 0; // Contador para o botão A

void config_pwm(int led) {
    uint slice; // variável para obter o slice do PWM, divisor da frequência
    gpio_set_function(led, GPIO_FUNC_PWM); // configura para saída PWM
    slice = pwm_gpio_to_slice_num(led); // retorna um slice permitindo o controle do sinal gerado 

    // Configuração do período wrap duty cycle      
    pwm_set_clkdiv(slice, divisor_pwm); // define o divisor do clock do PWM
    pwm_set_wrap(slice, periodo); // configura o valor máximo do contador(periodo pwm)
    pwm_set_gpio_level(led, stemp_pwm); // define o nível de pwm no pino correspondente
    pwm_set_enabled(slice, true); // habilita o pwm no slice correspondente
}

void render_on_display(uint8_t *ssd, struct render_area *area);
extern void ssd1306_draw_string(uint8_t *ssd, int16_t x, int16_t y, char *string);
void calculate_render_area_buffer_length(struct render_area *area);

// Função para exibir mensagens
void exibir_mensagem(uint8_t *ssd, struct render_area *frame_area, const char *mensagem) {
    memset(ssd, 0, sizeof(ssd)); // Zera o buffer do display
    ssd1306_draw_string(ssd, 5, 0, mensagem); // Desenha a mensagem no display
    render_on_display(ssd, frame_area); // Atualiza o display
}

// Funções específicas para cada situação
void exibir_nivel_luminosidade_a(uint8_t *ssd, struct render_area *frame_area) {
    char mensagem[20];
    snprintf(mensagem, sizeof(mensagem), "Luminosidade: %d", nivelLuminosidade);
    exibir_mensagem(ssd, frame_area, mensagem);
}

void exibir_nivel_luminosidade_b(uint8_t *ssd, struct render_area *frame_area) {
    char mensagem[20];
    snprintf(mensagem, sizeof(mensagem), "Luminosidade: %d", nivelLuminosidade);
    exibir_mensagem(ssd, frame_area, mensagem);
}

void exibir_lampada_desligada(uint8_t *ssd, struct render_area *frame_area) {
    char mensagem[30];
    snprintf(mensagem, sizeof(mensagem), "Lampada desligada");
    exibir_mensagem(ssd, frame_area, mensagem);
}

void Incrementar(int led, int botao, uint8_t *ssd, struct render_area *frame_area) {
    // Verifica se o botão foi pressionado
    if (gpio_get(botao) == 0) { // O botão é pressionado quando o pino é baixo
        if (botao == BTA && nivelLuminosidade < 5) {
            contadorA++; // Incrementa o contador do botão A
            nivelLuminosidade++; // Aumenta o nível de luminosidade
            printf("Botão A pressionado: %d - Nível de luminosidade da lampada: %d\n", contadorA, nivelLuminosidade);
            exibir_nivel_luminosidade_a(ssd, frame_area); // Exibe o nível de luminosidade
        } else if (botao == BTB && nivelLuminosidade > 1) {
            nivelLuminosidade--; // Diminui o nível de luminosidade
            printf("Botão B pressionado - Nível de luminosidade da lampada: %d\n", nivelLuminosidade);
            exibir_nivel_luminosidade_b(ssd, frame_area); // Exibe o nível de luminosidade
        } else if (botao == BTB && nivelLuminosidade == 1) { // quando o nível de luminosidade for 1
            nivelLuminosidade--; // ele decrementa 1 para que 0 a contagem
            if (nivelLuminosidade == 0) { // após zerar, verifica se a contagem é igual a 0
                gpio_put(led, 0); // quando chegar em 0 ele desliga a lâmpada 
                printf("Botão B pressionado - Lampada desligada - Nível de luminosidade: %d\n", nivelLuminosidade);
                exibir_lampada_desligada(ssd, frame_area); // Exibe que a lâmpada está desligada
            }
        }

        sleep_ms(200); // Debounce: espera 200 ms para evitar múltiplas contagens

        // Aguarda até que o botão seja solto
        while (gpio_get(botao) == 0);
    }
}

int main() {
    stdio_init_all(); // Inicializa os tipos stdio padrão presentes ligados ao binário

    // Inicialização do i2c
    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // Inicializa os LEDs
    gpio_init(VERDE);
    gpio_set_dir(VERDE, GPIO_OUT);

     // Inicializa os botões
    gpio_init(BTA);
    gpio_set_dir(BTA, GPIO_IN);
    gpio_pull_up(BTA); // Resistor pull-up para o botão A
 
    gpio_init(BTB);
    gpio_set_dir(BTB, GPIO_IN);
    gpio_pull_up(BTB); // Resistor pull-up para o botão B
 
     config_pwm(VERDE); // configura o PWM para o LED verde
 

    // Processo de inicialização completo do OLED SSD1306
    ssd1306_init();

    // Preparar área de renderização para o display (ssd1306_width pixels por ssd1306_n_pages páginas)
    struct render_area frame_area = {
        .start_column = 0,
        .end_column = ssd1306_width - 1,
        .start_page = 0,
        .end_page = ssd1306_n_pages - 1
    };

    calculate_render_area_buffer_length(&frame_area);

    // Zera o display inteiro
    uint8_t ssd[ssd1306_buffer_length];
    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, &frame_area);

    // Parte do código para exibir a mensagem no display
    
    render_on_display(ssd, &frame_area);

    while (true) {
        // Ajusta o nível de PWM baseado no nível de luminosidade
        pwm_set_gpio_level(VERDE, periodo / 5 * nivelLuminosidade); 
    
        // Chama a função Incrementar com os parâmetros apropriados
        Incrementar(VERDE, BTA, ssd, &frame_area); // Exemplo de chamada com LED Verde e Botão A
        Incrementar(VERDE, BTB, ssd, &frame_area); // Exemplo de chamada com LED Verde e Botão B
        
        sleep_ms(1000); // Delay de 1 segundo
    }
    return 0;
}
