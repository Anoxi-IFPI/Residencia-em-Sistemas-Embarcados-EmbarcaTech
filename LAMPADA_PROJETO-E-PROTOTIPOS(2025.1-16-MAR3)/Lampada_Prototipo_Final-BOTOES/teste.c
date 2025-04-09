#include <stdio.h>
#include "pico/stdlib.h"
#include  "hardware/pwm.h"

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

void Incrementar(int led, int botao) {
       // Verifica se o botão foi pressionado
    if (gpio_get(botao) == 0) { // O botão é pressionado quando o pino é baixo
        if (botao == BTA && nivelLuminosidade < 5) {
            contadorA++; // Incrementa o contador do botão A
            nivelLuminosidade++; // Aumenta o nível de luminosidade
            printf("Botão A pressionado: %d - Nível de luminosidade da lampada: %d\n", contadorA, nivelLuminosidade);
        } else if (botao == BTB && nivelLuminosidade > 1) {
            nivelLuminosidade--; // Diminui o nível de luminosidade
            printf("Botão B pressionado - Nível de luminosidade da lampada: %d\n", nivelLuminosidade);
        }
        else if(BTB && nivelLuminosidade == 1){//quando o nivel de luminosidade for 1
            nivelLuminosidade--; //ele decrementa 1 para que 0 a contagem
            if(nivelLuminosidade == 0){//apos zerar eleverifica se a contagem e igual a 0 para que a contagem n fique negativa
                gpio_put(led, 0); //quando chegar em 0 ele desliga a lampada 
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
/*Explicação das Alterações
Nível de Luminosidade:

A variável nivelLuminosidade foi introduzida para controlar o nível de luminosidade, variando de 1 a 5.
Contagem de Cliques:

O contador contadorA conta quantas vezes o botão A foi pressionado, mas a luminosidade só aumenta até 5.
Se o botão A for pressionado e o nível de luminosidade for menor que 5, ele aumenta.
Decremento com Botão B:

O botão B apenas diminui o nível de luminosidade se ele for maior que 1.
O botão A não pode aumentar a luminosidade se já estiver no nível máximo.
Saída Serial:

O código imprime no console quantas vezes o botão A foi pressionado e o nível atual de luminosidade.
Considerações Finais
Limites: O nível de luminosidade é limitado entre 1 e 5, conforme solicitado.
Debounce: O debounce foi implementado com um simples sleep_ms, mas considere usar uma abordagem mais robusta se necessário.
Testes: Certifique-se de testar o código em seu ambiente para garantir que tudo funcione conforme o esperado.
*/