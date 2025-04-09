#include <stdio.h> // Inclui a biblioteca padrão de
// entrada/saída em C para usar funções como printf e scanf.
#include "pico/stdlib.h" // Inclui a biblioteca padrão da Raspberry
// Pi Pico, que fornece funções para GPIO, timers e comunicação serial.
#include "pico/cyw43_arch.h" // Inclui a biblioteca para gerenciar o
// módulo de Wi-Fi (CYW43439) integrado à Raspberry Pi Pico W.
#include "hardware/vreg.h" // Inclui a biblioteca para configurar e
// controlar o regulador de tensão (Voltage Regulator) da Rasp Pi Pico.
#include "hardware/clocks.h" // Inclui a biblioteca para controlar os
// clocks internos da Rasp Pi Pico, como frequência da CPU e periféricos.

const uint led_pin_red = 13;
// Declara e inicializa uma constante `led_pin_red` com o valor `13`,
// que será usada para referenciar o pino GPIO 13. Esse pino é
// conectado ao LED vermelho da BitDogLab.

// Callback para resultados de varredura. Esta função será chamada
// automaticamente sempre que um resultado de varredura Wi-Fi for encontrado.

static int scan_result(void *env, const cyw43_ev_scan_result_t *result)
// Declara uma função `scan_result` que recebe:
// - `env`: um ponteiro genérico para um ambiente (não usado aqui).
// - `result`: um ponteiro para uma estrutura contendo os dados do
//             resultado da varredura Wi-Fi.
// A função retorna um inteiro (usado para controle da varredura).
{ // Verifica se o ponteiro `result` é válido (não NULL).
    if (result)
    { // Imprime as informações do resultado da varredura.
        // Os dados exibidos incluem:
        printf("SSID: %-32s RSSI: %4d CHAN: %3d MAC: %02x:%02x:%02x:%02x:%02x:%02x SEC: %u\n",
               result->ssid,     // Nome da rede Wi-Fi (SSID).
               result->rssi,     // Intensidade do sinal (RSSI).
               result->channel,  // Canal Wi-Fi onde a rede foi encontrada.
               result->bssid[0], // Endereço MAC (primeiro byte).
               result->bssid[1], // Endereço MAC (segundo byte).
               result->bssid[2], // Endereço MAC (terceiro byte).
               result->bssid[3], // Endereço MAC (quarto byte).
               result->bssid[4], // Endereço MAC (quinto byte).
               result->bssid[5], // Endereço MAC (sexto byte).
               result->auth_mode);
    }         // Modo de autenticação (ex.: WPA2, WPA3).
    return 0; // Retorna 0 para continuar a varredura.
}

int main()
{
    stdio_init_all();
    sleep_ms(1000);
    printf("Wi-FI Scan Embarcatech 2025\n");
    gpio_init(led_pin_red);
    gpio_set_dir(led_pin_red, GPIO_OUT);

    if (cyw43_arch_init())
    { // Inicializa o Wi-Fi
        // Tenta inicializar o módulo Wi-Fi integrado da Rasp Pi Pico W.
        // Se falhar, exibe uma mensagem de erro e encerra o programa.
        printf("Falha ao inicializar Wi-Fi\n");
        return 1; // Sai do programa com código de erro.
    }
    printf("Wi-Fi inicializado com sucesso\n");
    cyw43_arch_enable_sta_mode(); // Ativa o modo Station (STA),
    // permitindo que o dispositivo se conecte a redes Wi-Fi.
    absolute_time_t scan_time = make_timeout_time_ms(0); // Define a variável
    // scan_time para iniciar uma varredura imediatamente (tempo de espera = 0 ms).
    bool scan_in_progress = false;
    // Variável que indica se uma varredura Wi-Fi está em andamento.

    while (true)
    {
        if (absolute_time_diff_us(get_absolute_time(), scan_time) < 0)
        {   // Verifica se
            // o tempo atual ultrapassou o tempo definido para iniciar a próxima varredura.
            if (!scan_in_progress)
            {
                // Se nenhuma varredura estiver em andamento, inicia uma nova varredura.
                cyw43_wifi_scan_options_t scan_options = {0};
                // Cria uma estrutura para configurar as opções de varredura.
                int err = cyw43_wifi_scan(&cyw43_state, &scan_options, NULL, scan_result);
                // Inicia a varredura Wi-Fi usando as opções configuradas
                // e define a função callback `scan_result`.
                if (err == 0)
                { // Se a varredura foi iniciada com sucesso.
                    printf("\nIniciando varredura Wi-Fi....\n");
                    scan_in_progress = true; // Marca que uma varredura está em andamento.
                }
                else
                {                                                   // Caso ocorra um erro ao tentar iniciar a varredura.
                    printf("Erro ao iniciar varredura: %d\n", err); // Exibe o código de erro.
                    // Define o tempo para tentar novamente em 10 segundos.
                    scan_time = make_timeout_time_ms(10000);
                }
            }
            else if (!cyw43_wifi_scan_active(&cyw43_state))
            {
                // Verifica se a varredura em andamento foi concluída.
                printf("Varredura concluída\n");
                // Define o tempo para iniciar a próxima varredura em 10 segundos.
                scan_time = make_timeout_time_ms(10000);
                // Marca que não há mais varredura em andamento.
                scan_in_progress = false;
            }

            gpio_put(led_pin_red, true);
            sleep_ms(200);
            gpio_put(led_pin_red, false);
            sleep_ms(200);
        }
#if PICO_CYW43_ARCH_POLL
        // Se o modo de polling estiver habilitado, executa as seguintes funções.
        cyw43_arch_poll();
        // Processa eventos pendentes para o módulo Wi-Fi.
        cyw43_arch_wait_for_work_until(scan_time);
        // Aguarda até o tempo especificado para a próxima ação.
#else
        sleep_ms(1000);
        // Se o modo de polling não estiver habilitado, simplesmente aguarda 1 segundo.
#endif
    }
    cyw43_arch_deinit();
    // Libera os recursos do Wi-Fi antes de encerrar o programa.
    return 0;
    // Retorna 0, indicando que o programa foi encerrado com sucesso.
}