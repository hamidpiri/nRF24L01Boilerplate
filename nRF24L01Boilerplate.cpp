#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
// SPI Defines
// We are going to use SPI 0, and allocate it to the following GPIO pins
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define SPI_PORT spi0
#define PIN_MISO 0
#define PIN_CS 17
#define PIN_SCK 18
#define PIN_MOSI 3
#define LED_PIN 25

/***NRF24L01 Commands***/
#define R_REGISTER 0B000AAAAA
#define W_REGISTER 0B001AAAAA
#define R_RX_PAYLOAD 0B01100001
#define W_TX_PAYLOAD 0B10100000
#define FLUSH_TX 0B11100001
#define FLUSH_RX 0B11100010
#define REUSE_TX_PL 0B11100011
#define ACTIVATE 0B01010000
#define R_RX_PL_WID 0B01100000
#define W_ACK_PAYLOAD 0B10101000
#define W_TX_PAYLOAD_NO_ACK 0B10110000
#define NOP 0B11111111
/***NRF24L01 Commands***/

int main()
{
    stdio_init_all();

    // SPI initialisation. This example will use SPI at 1MHz.
    spi_init(SPI_PORT, 1000 * 1000);
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(PIN_CS, GPIO_FUNC_SIO);
    gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);

    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 1);

    // Chip select is active-low, so we'll initialise it to a driven-high state
    gpio_set_dir(PIN_CS, GPIO_OUT);
    gpio_put(PIN_CS, 1);
    // For more examples of SPI use see https://github.com/raspberrypi/pico-examples/tree/master/spi

    while (true)
    {
        // char key_input = getchar();
        // printf("Key pressed: %c\n", key_input);
        // key_input == 'A' ? cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1) : cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
    }
}
