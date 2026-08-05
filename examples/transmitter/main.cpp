#include <cstdint>
#include <cstdio>

#include "pico/stdlib.h"

#include "nrf24/nrf24.hpp"

int main()
{
    stdio_init_all();

    // تنظیمات مخصوص برد شما؛ این‌ها بخشی از خود کتابخانه نیستند.
    const nrf24::Pins pins{
        spi0, // SPI peripheral
        0,    // MISO
        3,    // MOSI
        18,   // SCK
        17,   // CSN
        15,   // CE
    };

    nrf24::Radio radio{pins};

    if (!radio.begin())
    {
        printf("nRF24L01 initialization failed\n");
        while (true)
        {
            sleep_ms(1000);
        }
    }

    // ترتیب بایت‌ها در Address از کم‌ارزش‌ترین بایت به پرارزش‌ترین بایت است.
    const nrf24::Address address{
        0xE7, 0xE7, 0xE7, 0xE7, 0xE7};

    radio.set_channel(2);
    radio.set_power_and_data_rate(
        nrf24::Power::Zerodbm,
        nrf24::DataRate::Mbps1);

    radio.set_payload_size(1);
    radio.set_tx_address(address);

    // برای دریافت ACK در حالت ارسال، RX pipe 0 باید همان آدرس TX باشد.
    radio.set_rx_pipe_0_address(address);
    radio.enable_rx_pipe(0);
    radio.enable_auto_ack(0);

    std::uint8_t payload = 0;

    while (true)
    {
        ++payload;

        if (radio.transmit(&payload, sizeof(payload)))
        {
            printf("Sent: %u\n", payload);
        }
        else
        {
            printf("Transmission failed\n");
        }

        sleep_ms(1000);
    }
}