

#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "pico/cyw43_arch.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lwip/pbuf.h"
#include "lwip/tcp.h"
#include "lwip/netif.h" // Para acessar netif_default e IP

// Configurações de Wi-Fi
#define WIFI_SSID "Multilaser2"
#define WIFI_PASSWORD "123456789"

// Definição dos pinos dos LEDs
#define LED_PIN CYW43_WL_GPIO_LED_PIN
#define LED_BLUE_PIN 12  // GPIO12 - LED azul
#define LED_GREEN_PIN 11 // GPIO11 - LED verde
#define LED_RED_PIN 13   // GPIO13 - LED vermelho

// Função de callback para processar requisições HTTP
static err_t tcp_server_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
    if (!p)
    {
        tcp_close(tpcb);
        tcp_recv(tpcb, NULL);
        return ERR_OK;
    }

    char *request = (char *)malloc(p->len + 1);
    memcpy(request, p->payload, p->len);
    request[p->len] = '\0';

    printf("Request: %s\n", request);

    // Controle dos LEDs
    if (strstr(request, "GET /blue_on") != NULL)
    {
        gpio_put(LED_BLUE_PIN, 1);
    }
    else if (strstr(request, "GET /blue_off") != NULL)
    {
        gpio_put(LED_BLUE_PIN, 0);
    }
    else if (strstr(request, "GET /green_on") != NULL)
    {
        gpio_put(LED_GREEN_PIN, 1);
    }
    else if (strstr(request, "GET /green_off") != NULL)
    {
        gpio_put(LED_GREEN_PIN, 0);
    }
    else if (strstr(request, "GET /red_on") != NULL)
    {
        gpio_put(LED_RED_PIN, 1);
    }
    else if (strstr(request, "GET /red_off") != NULL)
    {
        gpio_put(LED_RED_PIN, 0);
    }
    else if (strstr(request, "GET /on") != NULL)
    {
        cyw43_arch_gpio_put(LED_PIN, 1);
    }
    else if (strstr(request, "GET /off") != NULL)
    {
        cyw43_arch_gpio_put(LED_PIN, 0);
    }

    // Leitura da temperatura interna
    adc_select_input(4);
    uint16_t raw_value = adc_read();
    const float conversion_factor = 3.3f / (1 << 12);
    float temperature = 27.0f - ((raw_value * conversion_factor) - 0.706f) / 0.001721f;

    // Cria a resposta HTML
    char html[1024];

    snprintf(html, sizeof(html),
             "HTTP/1.1 200 OK\r\n"
             "Content-Type: text/html\r\n"
             "\r\n"
             "<!DOCTYPE html>\n"
             "<html>\n"
             "<head>\n"
             "<title>LED Control</title>\n"
             "<style>\n"
             "body { font-family: Arial, sans-serif; text-align: center; margin-top: 50px; }\n"
             "h1 { font-size: 64px; margin-bottom: 30px; }\n"
             "button { font-size: 36px; margin: 10px; padding: 20px 40px; border-radius: 10px; }\n"
             ".temperature { font-size: 48px; margin-top: 30px; color: #333; }\n"
             "</style>\n"
             "</head>\n"
             "<body>\n"
             "<h1>LED Control</h1>\n"
             "<form action=\"./blue_on\"><button>Ligar Azul</button></form>\n"
             "<form action=\"./blue_off\"><button>Desligar Azul</button></form>\n"
             "<form action=\"./green_on\"><button>Ligar Verde</button></form>\n"
             "<form action=\"./green_off\"><button>Desligar Verde</button></form>\n"
             "<form action=\"./red_on\"><button>Ligar Vermelho</button></form>\n"
             "<form action=\"./red_off\"><button>Desligar Vermelho</button></form>\n"
             "<p class=\"temperature\">Temperatura Interna: %.2f &deg;C</p>\n"
             "</body>\n"
             "</html>\n",
             temperature);

    /*       snprintf(html, sizeof(html),
             "HTTP/1.1 200 OK\r\n"
             "Content-Type: text/html\r\n"
             "\r\n"
             "<!DOCTYPE html>\n"
             "<html>\n"
             "<head>\n"
             "<title>LED Control</title>\n"
             "<style>\n"
             "body { font-family: Arial, sans-serif; text-align: center; margin-top: 50px; }\n"
             "h1 { font-size: 64px; margin-bottom: 30px; }\n"
             "button { font-size: 36px; margin: 10px; padding: 20px 40px; border-radius: 10px; }\n"
             ".temperature { font-size: 48px; margin-top: 30px; color: #333; }\n"
             "</style>\n"
             "</head>\n"
             "<body>\n"
             "<h1>LED Control</h1>\n"
             "<form action=\"./blue_on\"><button style=\"background-color: #2196F3; color: white;\">Ligar Azul</button></form>\n"
             "<form action=\"./blue_off\"><button style=\"background-color: #BBDEFB; color: black;\">Desligar Azul</button></form>\n"
             "<form action=\"./green_on\"><button style=\"background-color: #4CAF50; color: white;\">Ligar Verde</button></form>\n"
             "<form action=\"./green_off\"><button style=\"background-color: #C8E6C9; color: black;\">Desligar Verde</button></form>\n"
             "<form action=\"./red_on\"><button style=\"background-color: #F44336; color: white;\">Ligar Vermelho</button></form>\n"
             "<form action=\"./red_off\"><button style=\"background-color: #FFCDD2; color: black;\">Desligar Vermelho</button></form>\n"
             "<p class=\"temperature\">Temperatura Interna: %.2f &deg;C</p>\n"
             "</body>\n"
             "</html>\n",
             temperature);
    */

    tcp_write(tpcb, html, strlen(html), TCP_WRITE_FLAG_COPY);
    tcp_output(tpcb);

    free(request);
    pbuf_free(p);

    return ERR_OK;
}

// Função de callback ao aceitar conexões TCP
static err_t tcp_server_accept(void *arg, struct tcp_pcb *newpcb, err_t err)
{
    tcp_recv(newpcb, tcp_server_recv);
    return ERR_OK;
}

// Função principal
int main()
{
    stdio_init_all();

    // Configuração dos LEDs como saída
    gpio_init(LED_BLUE_PIN);
    gpio_set_dir(LED_BLUE_PIN, GPIO_OUT);
    gpio_put(LED_BLUE_PIN, false);

    gpio_init(LED_GREEN_PIN);
    gpio_set_dir(LED_GREEN_PIN, GPIO_OUT);
    gpio_put(LED_GREEN_PIN, false);

    gpio_init(LED_RED_PIN);
    gpio_set_dir(LED_RED_PIN, GPIO_OUT);
    gpio_put(LED_RED_PIN, false);

    // cyw43_arch_deinit(); // Desativa o Wi-Fi

    while (cyw43_arch_init())
    {
        printf("Falha ao inicializar Wi-Fi\n");
        sleep_ms(100);
        return -1;
    }

    cyw43_arch_gpio_put(LED_PIN, 0);
    cyw43_arch_enable_sta_mode();

    printf("Conectando ao Wi-Fi...\n");
    while (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 20000))
    {
        printf("Falha ao conectar ao Wi-Fi\n");
        sleep_ms(100);
        return -1;
    }

    printf("Conectado ao Wi-Fi\n");

    if (netif_default)
    {
        printf("IP do dispositivo: %s\n", ipaddr_ntoa(&netif_default->ip_addr));
    }

    // Configura o servidor TCP
    struct tcp_pcb *server = tcp_new();
    if (!server)
    {
        printf("Falha ao criar servidor TCP\n");
        return -1;
    }

    if (tcp_bind(server, IP_ADDR_ANY, 80) != ERR_OK)
    {
        printf("Falha ao associar servidor TCP à porta 80\n");
        return -1;
    }

    server = tcp_listen(server);
    tcp_accept(server, tcp_server_accept);

    printf("Servidor ouvindo na porta 80\n");

    // Inicializa o ADC
    adc_init();
    adc_set_temp_sensor_enabled(true);

    while (true)
    {
        cyw43_arch_poll();
    }

    cyw43_arch_deinit();
    return 0;
}
