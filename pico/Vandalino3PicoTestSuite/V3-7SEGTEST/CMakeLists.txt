cmake_minimum_required(VERSION 3.14)
set(PICO_BOARD adafruit_feather_rp2040)

include(pico_sdk_import.cmake)
project(V3-7SEGTEST VERSION 1.0.0)
add_executable(V3-7SEGTEST
               main.c)





pico_sdk_init()


#add_subdirectory(FreeRTOS)
#add_subdirectory(ProjectFiles)

set(PICO_SDK_FREERTOS_SOURCE /home/tarnished-dan22/RTOS_SP2023/pico/FreeRTOS-Kernel)

add_library(freertos
    ${PICO_SDK_FREERTOS_SOURCE}/event_groups.c
    ${PICO_SDK_FREERTOS_SOURCE}/list.c
    ${PICO_SDK_FREERTOS_SOURCE}/queue.c
    ${PICO_SDK_FREERTOS_SOURCE}/stream_buffer.c
    ${PICO_SDK_FREERTOS_SOURCE}/tasks.c
    ${PICO_SDK_FREERTOS_SOURCE}/timers.c
    ${PICO_SDK_FREERTOS_SOURCE}/portable/MemMang/heap_3.c
    ${PICO_SDK_FREERTOS_SOURCE}/portable/GCC/ARM_CM0/port.c
)


target_include_directories(freertos PUBLIC
    .
    ${PICO_SDK_FREERTOS_SOURCE}/include
    ${PICO_SDK_FREERTOS_SOURCE}/portable/GCC/ARM_CM0
)


pico_enable_stdio_usb(V3-7SEGTEST 1)
pico_enable_stdio_uart(V3-7SEGTEST 1)
pico_add_extra_outputs(V3-7SEGTEST)

target_link_libraries(V3-7SEGTEST
                      pico_stdlib
                      hardware_gpio
                      hardware_i2c
                      hardware_spi
                      hardware_adc
                      hardware_uart
                      freertos)
pico_add_extra_outputs(V3-7SEGTEST)
                   
