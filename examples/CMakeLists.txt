cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)

project(fhdm-mtof171000c0-pico-examples)

pico_sdk_init()

add_subdirectory(../lib build)

set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS
    $<$<CONFIG:Debug>:PARAM_ASSERTIONS_ENABLE_ALL=1>
  )

######################################
#
# example-simple
#
# Get distance measurements and write to stdout
#

add_executable(mtof171000c0-example-simple
    example-simple.c
)

target_link_libraries(mtof171000c0-example-simple 
    fhdm-mtof171000c0-pico
    pico_stdlib
)

# enable usb output, disable uart output
pico_enable_stdio_usb(mtof171000c0-example-simple 0)
pico_enable_stdio_uart(mtof171000c0-example-simple 1)

target_compile_options(fhdm-mtof171000c0-pico INTERFACE -Wall -Werror -Wno-error=unused-variable -Wno-error=unused-function)
target_compile_options(mtof171000c0-example-simple PRIVATE -Wall -Werror -Wno-error=unused-variable -Wno-error=unused-function)

pico_set_binary_type(mtof171000c0-example-simple no_flash)
pico_add_extra_outputs(mtof171000c0-example-simple)
