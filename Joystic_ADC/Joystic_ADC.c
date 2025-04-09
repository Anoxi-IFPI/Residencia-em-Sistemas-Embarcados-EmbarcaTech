#include <stdio.h> //Biblioteca padrao de entrada e saida
#include "pico/stdlib.h" //Biblioteca guardachuve padrao
#include "hardware/adc.h"// Biblioteca para manipulação do ADC no RP2040


#define led_AZ 12
#define Led_VD 11
#define led_VM 13


// Definição dos pinos usados para o Joystick e Leds
const int vRx = 26;             //Pino de leitura do eixo X do Joystick (conectado ao ADC)
const int vRy = 27;             //Pino de leitura do eixo Y do Joystick (conectado ao ADC)
const int ADC_canal_0 = 0;      //Canal ADC para o eixo X do Joystick
const int ADC_canal_1 = 1;      //Canal ADC para o eixo Y do Joystick
const int SW_Pin_Booton = 22;              //Pino de leitura do botão do Joystick  




//Função para configurar o Joystick(Pinos de leitura ADC)
void config_Joystick()
{  
    //inicializar o modulo ADC analogico digital
    adc_init();         //Inicializa o modulo ADC
    adc_gpio_init(vRx); //Configura o Pino vRx (eixi X) para entrada ADC
    adc_gpio_init(vRy); //Configura o pino vRy (eixo Y) para entrada ADC


    //inicializa o pino do botao do Joystick
    gpio_init(SW_Pin_Booton);               //inicializa o pino do botao
    gpio_set_dir(SW_Pin_Booton, GPIO_IN);   //configura o pino como entrada
    gpio_pull_up(SW_Pin_Booton);            // ativa o pull - ap para o pino em entado logico alto e evitar ponto flutuante


}


//configura os pinos de entrada e saida
void conf_pin(){
    gpio_init(led_AZ);
    gpio_set_dir(led_AZ, true);
    gpio_init(Led_VD);
    gpio_set_dir(Led_VD,true);
    gpio_init(led_VM);
    gpio_set_dir(led_VM,true);


}


//função para configuração geral
void setup_config()
{
    stdio_init_all();   //inicializa a porta serial para saida de dados
    config_Joystick();   // chama a função de configuração do Joystick
    conf_pin();// chama a função de configuração dos pinos
}


void acender_led(int led_azul, int led_verde, int led_vermelho){
    gpio_put(led_AZ,led_azul);
    gpio_put(Led_VD,led_verde);
    gpio_put(led_VM,led_vermelho);
}


//função para leitura dos eixis (X e Y)
void leitura_eixos_Joystick_canal_0(uint16_t *eixo_x)
{
    // Leytura do valor do eixo X do Joystick
    adc_select_input(ADC_canal_0); //selecionamos o canal ADC para o eixo X
                                    //pequeno delay para estabilidades
    *eixo_x = adc_read();              //Lê o valor do eixo X (0 a 4095)
}
void leitura_eixos_Joystick_canal_1(uint16_t *eixo_y){
     //Leitura do valor do eixo y do Joystick
     adc_select_input(ADC_canal_1);     //seleciona o canal do eyxo Y
     //pequeno delay para estabilidade
    *eixo_y = adc_read();


}


//função principal
int main()
{
    uint16_t valor_x, valor_y; // variaveis para armazenar os valores do oystick (eixo X e Y) e Botao
    setup_config();            // chama a função de configuração
    printf("Joystick\n");      // Exibe uma mensagem inicial via porta serial


    //lop principal
    while(1)
    {
        acender_led(0,0,0);//leds iniciam desligados
         //lê os valores dos eixos do Joystick
        leitura_eixos_Joystick_canal_0(&valor_x);
        leitura_eixos_Joystick_canal_1(&valor_y);
       
       
        //Configuração para exibição dos leds nos eixos X e Y
        if (valor_x < 1920) {
            acender_led(255, 255, 0); // LED verde e azul(azul claro)
        } else if (valor_x > 2085) {
            acender_led(0, 255, 255); // LED verde e vermelho(amarelo)
        }if (valor_y < 1930) {
            acender_led(255, 0, 255); // LED azul e vermelho acesos (lilas)
        } else if (valor_y > 2040) {
            acender_led(255, 255, 255); // LED verde azul e vermelho acesos (branco)
        }


        //leytura e exibição dos valores de X e Y no monitor serial
        printf("X: %d\n", valor_x);
        printf("Y: %d\n", valor_y);
        //printf("Botao: %d\n", gpio_get(SW_Pin_Booton));
        // ;delay antes da proxima leytura
        sleep_ms(200);


       
    }
}


