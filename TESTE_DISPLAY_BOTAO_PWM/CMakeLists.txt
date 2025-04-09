# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 1.5.1)
set(toolchainVersion 13_2_Rel1)
set(picotoolVersion 2.0.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(TESTE_DISPLAY_BOTAO_PWM C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Adiciona os arquivos da biblioteca SSD1306
set(SSD1306_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/inc/ssd1306_i2c.c
    ${CMAKE_CURRENT_LIST_DIR}/inc/ssd1306_font.c  # Certifique-se de que o arquivo existe
)

# Add executable. Default name is the project name, version 0.1
add_executable(TESTE_DISPLAY_BOTAO_PWM
    TESTE_DISPLAY_BOTAO_PWM.c
    ${SSD1306_SOURCES}  # Adiciona os arquivos da biblioteca SSD1306
)

pico_set_program_name(TESTE_DISPLAY_BOTAO_PWM "TESTE_DISPLAY_BOTAO_PWM")
pico_set_program_version(TESTE_DISPLAY_BOTAO_PWM "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(TESTE_DISPLAY_BOTAO_PWM 0)
pico_enable_stdio_usb(TESTE_DISPLAY_BOTAO_PWM 1)

# Add the standard library to the build
target_link_libraries(TESTE_DISPLAY_BOTAO_PWM
        pico_stdlib
        hardware_adc
        hardware_pwm
        hardware_i2c  # Adiciona a biblioteca I2C
)

# Add the standard include files to the build
target_include_directories(TESTE_DISPLAY_BOTAO_PWM PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/inc  # Adiciona o diretório "inc" para incluir os cabeçalhos da SSD1306
)

pico_add_extra_outputs(TESTE_DISPLAY_BOTAO_PWM)s