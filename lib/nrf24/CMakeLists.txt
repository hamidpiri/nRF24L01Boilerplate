add_library(nrf24 STATIC
    src/nrf24.cpp
)

target_include_directories(nrf24
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(nrf24
    PUBLIC
        pico_stdlib
        hardware_spi
)

target_compile_features(nrf24 PUBLIC cxx_std_17)