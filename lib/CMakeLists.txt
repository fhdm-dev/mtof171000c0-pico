add_library(fhdm-mtof171000c0-pico INTERFACE)

target_sources(fhdm-mtof171000c0-pico INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/fhdm-mtof171000c0.c
  ${CMAKE_CURRENT_LIST_DIR}/include/fhdm-mtof171000c0.h
  ${CMAKE_CURRENT_LIST_DIR}/fhdm-mtof171000c0-pico.c
  ${CMAKE_CURRENT_LIST_DIR}/include/fhdm-mtof171000c0-pico.h
)

target_include_directories(fhdm-mtof171000c0-pico INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(fhdm-mtof171000c0-pico INTERFACE hardware_uart hardware_gpio)
