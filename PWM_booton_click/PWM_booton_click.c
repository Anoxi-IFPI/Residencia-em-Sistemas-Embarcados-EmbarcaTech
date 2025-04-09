#include <stdio.h>
#include "pico/stdlib.h"

#define VERDE 11
#define AZUL 12
#define VERMELHO 13
#define BTA 5
#define BTB 6


//pwm
const uint16_t periodo = 200; //periodo do pwm(valor maximo de tenção)
const float divisor_pwm = 16.0; //divisor fracional do clock do pwm 
const uint16_t stemp_pwm = 100; //passo de incremento e decremento do duty cicly do LED
uint16_t led_level = 100; //nivel inicial do pwm

void config_pwm(int led){
    uint slice; //varavel para obter o slice do pwm, divisor da frequencia
    gpio_set_function(led, GPIO_FUNC_PWM);//configura para saida pwm
    slice = pwm_gpio_to_slice_num(led);//retorna um slice permitindo o controle do o sinal gerado 
    //configuração do pino para operar como pwm

    //configuração do periodo wrap dutycicle
    pwm_set_clcdiv(slice, divisor_pwm); // define o divisor pwm
    pwm_set_wrap(slice, periodo);//configura o valor maximo do contador(periodo pwm)
    pwm_set_level(led,led_level); //define o nivel de pwm no slice correspondente
    pwm_set_enable(slice, true); //habilita o pwm no slice correspondente
}



uint contador = 0; // Contador global

void Incrementar(int led, int botao) {
    // Verifica se o botão foi pressionado
    if (gpio_get(botao) == 0) { // O botão é pressionado quando o pino é baixo
        contador++; // Incrementa 1 no contador
        sleep_ms(200); // Debounce: espera 200 ms para evitar múltiplas contagens

        // Aguarda até que o botão seja solto
        while (gpio_get(botao) == 0);

        // Se o botão foi pressionado 5 vezes
        if (contador >= 5) {
            // Aciona o LED por 2 segundos
            gpio_put(led, 1); // Liga o LED
            sleep_ms(2000);   // Espera 2 segundos
            gpio_put(led, 0); // Desliga o LED

            // Reinicia a contagem
            contador = 0;

            // Aciona o LED intermitentemente
            for (int i = 0; i < 10; i++) { // 10 ciclos de 500 ms
                gpio_put(led, 1); // Liga o LED
                sleep_ms(500);    // Espera 500 ms
                gpio_put(led, 0); // Desliga o LED
                sleep_ms(500);    // Espera 500 ms
            }
        }
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

    uint controle = 1;//variavel para controlar o nivel de pwm
        stdio_init_all(); //inicializa a entrada I/O
        config_pwm(VERDE); //configura o Pwm para o led verde

    while (true) 
    {
        pwm_set_gpio_level(VERDE,led_level); //define o nivel atual do pwm (duty cycle)
        sleep_ms(1000); // atrazo de 1 segundo
        if(controle)
        {
            led_level += stemp_pwm; //incrementa o nivel do led
            if(led_level >= periodo)
                controle = 0; //muda a direção para diminuir o quando atingir p periodo maximo
        }
        else
        {
            led_level -= stemp_pwm; // decrementa o nivel do led
            if(led_level <= stemp_pwm)
                controle = 1; //muda a direção para aumentar quando atingir o minimo

        }

        Incrementar(VERDE, BTA); // Incrementa usando o botão A
        Incrementar(AZUL, BTB);   // Incrementa usando o botão B
        printf("Contador: %d\n", contador); // Exibe o contador
        sleep_ms(100); // Espera um pouco antes de verificar novamente
    }

    return 0; // Nunca deve chegar aqui
}
