#include <stdio.h> // Biblioteca padrão de entrada e saída
#include "pico/stdlib.h" // Biblioteca guardachuve padrão
#include "hardware/adc.h" // Biblioteca para manipulação do ADC no RP2040
#include "hardware/pwm.h" // Biblioteca para manipulação do PWM

#define led_AZ 12
#define Led_VD 11
#define led_VM 13

// Definição dos pinos usados para o Joystick e LEDs
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

    // Inicializa o pino do botão do Joystick
    gpio_init(SW_Pin_Booton);               
    gpio_set_dir(SW_Pin_Booton, GPIO_IN);   
    gpio_pull_up(SW_Pin_Booton);            
}

// Configura os pinos de entrada e saída
void conf_pin() {
    gpio_init(led_AZ);
    gpio_set_dir(led_AZ, true);
    gpio_init(Led_VD);
    gpio_set_dir(Led_VD, true);
    gpio_init(led_VM);
    gpio_set_dir(led_VM, true);

    // Configura PWM para os LEDs
    pwm_set_wrap(pwm_gpio_to_slice_num(led_AZ), 255);
    pwm_set_wrap(pwm_gpio_to_slice_num(Led_VD), 255);
    pwm_set_wrap(pwm_gpio_to_slice_num(led_VM), 255);
    pwm_set_enabled(pwm_gpio_to_slice_num(led_AZ), true);
    pwm_set_enabled(pwm_gpio_to_slice_num(Led_VD), true);
    pwm_set_enabled(pwm_gpio_to_slice_num(led_VM), true);
}

// Função para configuração geral
void setup_config() {
    stdio_init_all();   // Inicializa a porta serial para saída de dados
    config_Joystick();   // Chama a função de configuração do Joystick
    conf_pin();          // Chama a função de configuração dos pinos
}

// Função para acender os LEDs com PWM
void acender_led(int led_azul, int led_verde, int led_vermelho) {
    pwm_set_gpio_level(led_AZ, led_azul);
    pwm_set_gpio_level(Led_VD, led_verde);
    pwm_set_gpio_level(led_VM, led_vermelho);
}

// Função para leitura dos eixos (X e Y)
void leitura_eixos_Joystick_canal_0(uint16_t *eixo_x) {
    adc_select_input(ADC_canal_0); // Selecionamos o canal ADC para o eixo X
    *eixo_x = adc_read();           // Lê o valor do eixo X (0 a 4095)
}

void leitura_eixos_Joystick_canal_1(uint16_t *eixo_y) {
    adc_select_input(ADC_canal_1); // Seleciona o canal do eixo Y
    *eixo_y = adc_read();           // Lê o valor do eixo Y (0 a 4095)
}

// Função principal
int main() {
    uint16_t valor_x, valor_y; // Variáveis para armazenar os valores do joystick (eixo X e Y)
    setup_config();            // Chama a função de configuração
    printf("Joystick\n");      // Exibe uma mensagem inicial via porta serial

    // Loop principal
    while (1) {
        acender_led(0, 0, 0); // LEDs iniciam desligados
        // Lê os valores dos eixos do Joystick
        leitura_eixos_Joystick_canal_0(&valor_x);
        leitura_eixos_Joystick_canal_1(&valor_y);

        // Configuração para exibição dos LEDs nos eixos X e Y
        int led_azul = 0;
        int led_verde = 0;
        int led_vermelho = 0;

        if (valor_x < 1920) {
            led_azul = 255; // LED azul se X < 1920
        } else if (valor_x > 2085) {
            led_verde = 255; // LED verde se X > 2085
        }

        if (valor_y < 1930) {
            led_vermelho = 255; // LED vermelho se Y < 1930
        } else if (valor_y > 2040) {
            led_verde = 255; // LED verde se Y > 2040
            led_azul = 255; // LED azul se Y > 2040
            led_vermelho = 255; // LED vermelho se Y > 2040
        }

        // Ajusta a intensidade dos LEDs com base nos valores do joystick
        acender_led(led_azul * (valor_y / 16), led_verde * (valor_y / 16), led_vermelho * (valor_y / 16));

        // Leitura e exibição dos valores de X e Y no monitor serial
        printf("X: %d\n", valor_x);
        printf("Y: %d\n", valor_y);
        sleep_ms(200);
    }
}
