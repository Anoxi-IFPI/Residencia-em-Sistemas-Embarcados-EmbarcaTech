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

project(picow_http_client C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
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


# Add executable. Default name is the project name, version 0.1

add_executable(picow_http_client picow_http_client.c )
set(WIFI_SSID "Moto g20")
set(WIFI_PASSWORD "@dm1n202!")
target_compile_definitions(picow_http_client PRIVATE
        WIFI_SSID=\"${WIFI_SSID}\"
        WIFI_PASSWORD=\"${WIFI_PASSWORD}\"
        )

pico_set_program_name(picow_http_client "picow_http_client")
pico_set_program_version(picow_http_client "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(picow_http_client 0)
pico_enable_stdio_usb(picow_http_client 1)

# Add the standard library to the build
target_link_libraries(picow_http_client
        pico_stdlib
        pico_cyw43_arch_lwip_threadsafe_background
        example_lwip_http_util
        )

# Add the standard include files to the build
target_include_directories(picow_http_client PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/..
)

# Add any user requested libraries
target_link_libraries(picow_http_client 
        
        )

pico_add_extra_outputs(picow_http_client)

add_executable(picow_http_client_verify
        picow_http_verify.c
        )
target_compile_definitions(picow_http_client_verify PRIVATE
        WIFI_SSID=\"${WIFI_SSID}\"
        WIFI_PASSWORD=\"${WIFI_PASSWORD}\"
        # By default verification is optional (MBEDTLS_SSL_VERIFY_OPTIONAL)
        # Make it required for this test
        ALTCP_MBEDTLS_AUTHMODE=MBEDTLS_SSL_VERIFY_REQUIRED
        )
target_include_directories(picow_http_client_verify PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts and mbedtls_config
        )
target_link_libraries(picow_http_client_verify
        pico_cyw43_arch_lwip_threadsafe_background
        example_lwip_http_util
        pico_stdlib
        )
pico_add_extra_outputs(picow_http_client_verify)

# Ignore warnings from lwip code
set_source_files_properties(
        ${PICO_LWIP_PATH}/src/apps/altcp_tls/altcp_tls_mbedtls.c
        PROPERTIES
        COMPILE_OPTIONS "-Wno-unused-result"
        )

