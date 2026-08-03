add_executable(nrf24_transmitter    main.cpp)   

target_link_libraries(nrf24_transmitter
PRIVATE
nrf24)

pico_enable_stdio_uart(nrf24_transmitter 0)
pico_enable_stdio_usb(nrf24_transmitter 1)

pico_add_extra_outputs(nrf24_transmitter)