#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include <string.h>
#include <bitset>

#include "nrf_device.hpp"

// SPI Defines
// We are going to use SPI 0, and allocate it to the following GPIO pins
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define SPI_PORT spi0
#define PIN_MISO 0
#define PIN_CS 17
#define PIN_SCK 18
#define PIN_MOSI 3
#define PIN_CE 15
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

void readFromUser()
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

        printf("You entered: 0x%02X\n", regAddr);
    }
}

int main()
{

    nrfdevice nrfdevice(spi0,
                        PIN_SCK,
                        PIN_MOSI,
                        PIN_MISO,
                        PIN_CE,
                        PIN_CS);
    // 1. Configure RF_CH Default channel 2
    nrfdevice.SetChannel(2);
    // 2. Configure address width
    // nrfdevice.SetAddressWidth(setupaw::addressWidth::_5bytes);
    // 3. Configure Data Rate and Power
    nrfdevice.SetPowerAndDataRate(nrfdevice::RF_PWR::_0dbm, nrfdevice::RF_DR::oneMbps);
    // 4. Program TX Address
    nrfdevice.SetTransmitAddress(0xE8E8E8E8E8);
    // // 5. Program RX pipe 0 address- when auto ack enabled RX_ADDR_p0 = TX_ADDR
    nrfdevice.SetPipe0Address(0xE8E8E8E8E8);
    // 6. Enable Data Pipe 0
    nrfdevice.EnableDataPipe(0);
    // 7. Enable Auto Ack of Data Pipe 0
    nrfdevice.EnableAutoAckOfDataPipe(0);
    // 8. Set Payload Width
    nrfdevice.SetPipe0Width(1);
    // 9. Set Mode TX
    nrfdevice.SetMode(nrfdevice::TX);
    // 10. Power Up
    nrfdevice.powerup();
    // 12. Load Payload
    // nrfdevice.WritePayload(0B01);
    // 13. Poll Status

    uint8_t payload = 0x00;

    stdio_init_all();
    while (1)
    {

        nrfdevice.command.ClearTXPayload();
        nrfdevice.WritePayload(++payload);
        nrfdevice.command.TransmitOverRadio();
        sleep_ms(1000);

        // Status status_reg;
        // uint8_t status_reg_value = status_reg.ReadRegisterValue();

        // printf("STATUS Register Value: 0x%02X\n", nrfdevice.status_register.ReadRegisterValue());
        while (true)
        {
            if (nrfdevice.IsDataTransmitted())
            {
                gpio_put(LED_PIN, 1);
                printf("Success\n");
                nrfdevice.ClearInterrupts();
                break;
            }

            if (nrfdevice.IsTransmitFailed())
            {

                printf("Failed and retrying\n");
                nrfdevice.ClearInterrupts();
                nrfdevice.command.TransmitOverRadio();
                gpio_put(LED_PIN, 0);
                sleep_ms(1000);
            }
        }
        // readFromUser();
    }
}
