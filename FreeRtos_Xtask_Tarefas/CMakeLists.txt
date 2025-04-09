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
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 1.5.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

include($ENV{FreeRtos_Xtask_Tarefas_KERNEL_PATH_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

project(FreeRtos_Xtask_Tarefas_KERNEL_PATH C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(FreeRtos_Xtask_Tarefas_KERNEL_PATH FreeRtos_Xtask_Tarefas_KERNEL_PATH.c)

pico_set_program_name(FreeRtos_Xtask_Tarefas_KERNEL_PATH "FreeRtos_Xtask_Tarefas_KERNEL_PATH")
pico_set_program_version(FreeRtos_Xtask_Tarefas_KERNEL_PATH "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(FreeRtos_Xtask_Tarefas_KERNEL_PATH 0)
pico_enable_stdio_usb(FreeRtos_Xtask_Tarefas_KERNEL_PATH 1)

# Add the standard include files to the build
target_include_directories(FreeRtos_Xtask_Tarefas_KERNEL_PATH PRIVATE ${CMAKE_CURRENT_LIST_DIR})

# Add any user requested libraries
target_link_libraries(FreeRtos_Xtask_Tarefas_KERNEL_PATH pico_stdlib hardware_gpio pico_multicore FreeRtos_Xtask_Tarefas_KERNEL_PATH-Kernel FreeRtos_Xtask_Tarefas_KERNEL_PATH-Kernel-Heap4)

pico_add_extra_outputs(FreeRtos_Xtask_Tarefas_KERNEL_PATH)
