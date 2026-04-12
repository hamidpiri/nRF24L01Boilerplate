#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include <string.h>
#include "./register.hpp"
#include <bitset>

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
#define R_REGISTER 0B00000000
#define W_REGISTER 0B00100000
#define R_RX_PAYLOAD 0B01100001
#define W_TX_PAYLOAD 0B10100000
#define FLUSH_TX 0B11100001
#define FLUSH_RX 0B11100010
#define REUSE_TX_PL 0B11100011
#define ACTIVATE 0B01010000
#define R_RX_PL_WID 0B01100000
#define W_ACK_PAYLOAD 0B10101000
#define W_TX_PAYLOAD_NO_ACK 0B10110000
// #define NOP 0B11111111
/***NRF24L01 Commands***/

/***Register Addresses***/

#define STATUS 0x07
#define CONFIG 0x00
#define EN_AA 0x01
#define EN_RXADDR 0x02
#define SETUP_AW 0x03
#define SETUP_RETR 0x04
#define RF_CH 0x05
#define RF_SETUP 0x06
#define OBSERVE_TX 0x08
#define RPD 0x09
#define RX_ADDR_P0 0x0A
#define RX_ADDR_P1 0x0B
#define RX_ADDR_P2 0x0C
#define RX_ADDR_P3 0x0D
#define RX_ADDR_P4 0x0E
#define RX_ADDR_P5 0x0F
#define TX_ADDR 0x10
#define RX_PW_P0 0x11
#define RX_PW_P1 0x12
#define RX_PW_P2 0x13
#define RX_PW_P3 0x14
#define RX_PW_P4 0x15
#define RX_PW_P5 0x16
#define NOP 0xFF
#define FIFO_STATUS (1 << 5)
/***Register Addresses***/

int main()
{
    stdio_init_all();

    // SPI initialisation. This example will use SPI at 1MHz.
    spi_init(SPI_PORT, 1000 * 1000);
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(PIN_CS, GPIO_FUNC_SIO);
    gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 1);

    // Chip select is active-low, so we'll initialise it to a driven-high state
    gpio_set_dir(PIN_CS, GPIO_OUT);
    gpio_put(PIN_CS, 1);
    // For more examples of SPI use see https : // github.com/raspberrypi/pico-examples/tree/master/spi

    while (true)
    {

        printf("Write 0 for read or 1 for write");
        char selection = getchar();
        if (selection == '0')
        {

            printf("Enter register address (hex): ");
            char input[3];

            int i = 0;
            int ch;
            // Read characters until newline or buffer full
            while (i < sizeof(input) - 1)
            {
                ch = getchar();
                if (ch == '\n' || ch == '\r')
                {
                    break;
                }
                input[i++] = (char)ch;
            }
            input[i] = '\0'; // Null-terminate string

            // Manual hex string to uint8_t conversion (no strtol)
            uint8_t regAddr = 0;
            for (int j = 0; j < i; j++)
            {
                regAddr <<= 4;
                if (input[j] >= '0' && input[j] <= '9')
                    regAddr |= (input[j] - '0');
                else if (input[j] >= 'A' && input[j] <= 'F')
                    regAddr |= (input[j] - 'A' + 10);
                else if (input[j] >= 'a' && input[j] <= 'f')
                    regAddr |= (input[j] - 'a' + 10);
                else
                    break; // Invalid character
            }

            printf("You entered: %s\n", input);
            printf("Hex value: 0x%02X\n", regAddr);
            printf("Binary value: 0b%s\n", std::bitset<8>(regAddr).to_string().c_str());

            Register reg(regAddr);
            uint8_t regValue = reg.ReadRegisterValue();
            uint8_t regValueBinary = std::bitset<8>(regValue).to_ulong();
            printf("Register 0x%02X Value: 0x%02X\n", regAddr, regValue);
            printf("Register 0x%02X Binary value: 0b%s\n", regAddr, std::bitset<8>(regValue).to_string().c_str());
        }
        else if (selection == '1')
        {

            printf("Enter register address (hex): ");
            char input[3];

            int i = 0;
            int ch;
            // Read characters until newline or buffer full
            while (i < sizeof(input) - 1)
            {
                ch = getchar();
                if (ch == '\n' || ch == '\r')
                {
                    break;
                }
                input[i++] = (char)ch;
            }
            input[i] = '\0'; // Null-terminate string

            // Manual hex string to uint8_t conversion (no strtol)
            uint8_t regAddr = 0;
            for (int j = 0; j < i; j++)
            {
                regAddr <<= 4;
                if (input[j] >= '0' && input[j] <= '9')
                    regAddr |= (input[j] - '0');
                else if (input[j] >= 'A' && input[j] <= 'F')
                    regAddr |= (input[j] - 'A' + 10);
                else if (input[j] >= 'a' && input[j] <= 'f')
                    regAddr |= (input[j] - 'a' + 10);
                else
                    break; // Invalid character
            }

            printf("You entered: %s\n", input);
            printf("Hex value: 0x%02X\n", regAddr);
            Register regToWrite(regAddr);
        }
    }
}
