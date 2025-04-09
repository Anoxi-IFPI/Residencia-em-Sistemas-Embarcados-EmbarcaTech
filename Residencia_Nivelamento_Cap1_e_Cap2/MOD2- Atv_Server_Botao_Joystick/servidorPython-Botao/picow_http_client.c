#include <stdio.h>
#include "pico/stdio.h"
#include "pico/cyw43_arch.h"
#include "pico/async_context.h"
#include "lwip/altcp_tls.h"
#include "example_http_client_util.h"

// ======= CONFIGURAÇÕES ======= //
#define HOST "192.168.39.102"  // Substitua pelo IP do servidor
#define PORT 5000
#define INTERVALO_MS 1000    // Intervalo entre mensagens (1 segundo)
#define BTA 5
#define verde 11
#define DEBOUNCE_MS 50       // Tempo de debounce em milissegundos
// ============================= //

void config_pin(uint gpio){
    gpio_init(gpio);
    gpio_set_dir(gpio, GPIO_OUT);
}

void config_button(uint gpio){
    gpio_init(gpio);
    gpio_set_dir(gpio, GPIO_IN);
    gpio_pull_up(gpio);
}

int main() {
    config_button(BTA);
    config_pin(verde);

    stdio_init_all();
    printf("\nIniciando cliente HTTP...\n");

    if (cyw43_arch_init()) {
        printf("Erro ao inicializar Wi-Fi\n");
        return 1;
    }
    cyw43_arch_enable_sta_mode();

    printf("Conectando a %s...\n", WIFI_SSID);
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, 
                                          CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        printf("Falha na conexão Wi-Fi\n");
        return 1;
    }
    printf("Conectado! IP: %s\n", ip4addr_ntoa(netif_ip4_addr(netif_list)));

    int counter = 0;
    char url[128];
    bool last_button_state = true; // Estado anterior do botão
    absolute_time_t last_debounce_time = get_absolute_time(); // Último tempo de debounce

    while(1) {
        bool current_button_state = gpio_get(BTA);

        // Verifica se o estado do botão mudou e se o tempo de debounce passou
        if (current_button_state != last_button_state) {
            last_debounce_time = get_absolute_time(); // Atualiza o tempo de debounce
        }

        // Verifica se o tempo de debounce passou
        if (absolute_time_diff_us(last_debounce_time, get_absolute_time()) > DEBOUNCE_MS * 1000) {
            // Apenas mude o estado se o botão estiver realmente pressionado
            if (current_button_state == 0) { // Botão pressionado
                sprintf(url, "/mensagem?msg=Botao_A_Clicado_%d", counter++);
                gpio_put(verde,1);
            } else { // Botão solto
                sprintf(url, "/mensagem?msg=Botao_A_Offline_%d", counter++);
                gpio_put(verde,0);
            }

            // Configura requisição
            EXAMPLE_HTTP_REQUEST_T req = {0};
            req.hostname = HOST;
            req.url = url;
            req.port = PORT;
            req.headers_fn = http_client_header_print_fn;
            req.recv_fn = http_client_receive_print_fn;

            // Envia requisição
            printf("[%d] Enviando: %s\n", counter, url);
            int result = http_client_request_sync(cyw43_arch_async_context(), &req);

            // Verifica resultado
            if (result == 0) {
                printf("Sucesso!\n");
            } else {
                printf("Erro %d - Verifique conexão\n", result);
                cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, 
                                                 CYW43_AUTH_WPA2_AES_PSK, 10000);
            }
        }

        last_button_state = current_button_state; // Atualiza o estado anterior do botão
        sleep_ms(INTERVALO_MS);
    }

    return 0; // Nunca chegará aqui devido ao while(1)
}
