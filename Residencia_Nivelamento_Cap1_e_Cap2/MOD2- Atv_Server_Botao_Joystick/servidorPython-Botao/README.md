# Configurando a Raspberry Pi Pico W para Enviar Mensagens a um Servidor Flask

Este projeto configura uma Raspberry Pi Pico W para enviar mensagens HTTP GET a um servidor Flask local.

## Requisitos

### Hardware:
- Raspberry Pi Pico W
- Conexão Wi-Fi

### Software:
- Raspberry Pi Pico SDK
- CMake
- Toolchain ARM (GCC)
- Python 3
- Flask

## Configuração do Ambiente

### 1. Iniciar um novo projeto com a extensão Raspberry Pico

Você irá iniciar um novo projeto C/C++ e selecionar a Pico W.
Escolha a configuração do SDK de sua preferência, mas se for usar este repositório, utilize a versão 1.5.1.
Selecione a opção "console over USB" e então crie o projeto.

### 2. Instalar Dependências Python (CMD ou PowerShell)

Para o servidor Flask:
```sh
pip install flask
```

## Configuração do Projeto

### 1. Estrutura dos Arquivos
Lembre-se de que o arquivo principal é o `picow_http_client.c`, então ajuste seu CMake conforme necessário.
Certifique-se de ter os seguintes arquivos no diretório do projeto:
```
- CMakeLists.txt
- example_http_client_util.c
- example_http_client_util.h
- lwipopts.h
- lwipopts_examples_common.h
- mbedtls_config.h
- mbedtls_config_examples_common.h
- picow_http_client.c
- picow_http_verify.c
```

### 2. Configuração do CMake

Edite o `CMakeLists.txt` conforme abaixo:
```cmake
# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Inicializar pico_sdk

set(PICO_BOARD pico_w CACHE STRING "Board type")

# Incluir o Raspberry Pi Pico SDK
include(pico_sdk_import.cmake)

project(picow_http_client C CXX ASM)

pico_sdk_init()

pico_add_library(example_lwip_http_util NOFLAG)
target_sources(example_lwip_http_util INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/example_http_client_util.c
        )
pico_mirrored_target_link_libraries(example_lwip_http_util INTERFACE
        pico_lwip_http
        pico_lwip_mbedtls
        pico_mbedtls
        )
target_include_directories(example_lwip_http_util INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}
        )

# Adicionar executável
add_executable(picow_http_client picow_http_client.c )
set(WIFI_SSID "SuaRedeWiFi")
set(WIFI_PASSWORD "SuaSenhaWiFi")
target_compile_definitions(picow_http_client PRIVATE
        WIFI_SSID="${WIFI_SSID}"
        WIFI_PASSWORD="${WIFI_PASSWORD}"
        )

pico_set_program_name(picow_http_client "picow_http_client")
pico_set_program_version(picow_http_client "0.1")

# Configuração de saída
pico_enable_stdio_uart(picow_http_client 0)
pico_enable_stdio_usb(picow_http_client 1)

target_link_libraries(picow_http_client
        pico_stdlib
        pico_cyw43_arch_lwip_threadsafe_background
        example_lwip_http_util
        )

target_include_directories(picow_http_client PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/..
)

pico_add_extra_outputs(picow_http_client)
```

### 3. Configuração do Wi-Fi

No `CMakeLists.txt`, edite as definições para sua rede Wi-Fi:
```cmake
set(WIFI_SSID "SuaRedeWiFi")
set(WIFI_PASSWORD "SuaSenhaWiFi")
```

## Configuração do Servidor Flask

Crie um arquivo `server.py` e adicione o seguinte código:
```python
from flask import Flask, request

app = Flask(__name__)

@app.route("/mensagem", methods=["GET"])
def receber_mensagem():
    mensagem = request.args.get("msg", "Nenhuma mensagem recebida")
    print(f"Mensagem recebida: {mensagem}")
    return "Mensagem recebida", 200

app.run(host="0.0.0.0", port=5000)
```

Execute o servidor Flask:
```sh
python server.py
```

Agora sua Raspberry Pi Pico W poderá enviar mensagens para o servidor Flask local!

