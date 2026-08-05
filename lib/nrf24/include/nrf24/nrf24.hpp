#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

#include "hardware/spi.h"

namespace nrf24
{
    using Address = std::array<std::uint8_t, 5>;

    struct Pins
    {
        spi_inst_t *spi;
        uint8_t miso_pin;
        uint8_t mosi_pin;
        uint8_t sck_pin;
        uint8_t csn_pin;
        uint8_t ce_pin;
    };

    enum class DataRate : uint8_t
    {
        Mbps1,
        Mbps2
    };

    enum class Power : uint8_t
    {
        Minus18dbm = 0,
        Minus12dbm = 1,
        Minus6dbm = 2,
        Zerodbm = 3,
    };

    class Radio
    {
    public:
        explicit Radio(Pins pins);
        // Configures SPI, GPIO, and safe initial nRF24L01 settings.
        bool begin(uint32_t spi_baudrate_hz = 1'000'000);
        bool set_channel(uint8_t channel);
        bool set_power_and_data_rate(Power power, DataRate rate);
        bool set_payload_size(uint8_t bytes);

        bool set_tx_address(const Address &address);
        bool set_rx_pipe_0_address(const Address &address);

        bool enable_auto_ack(uint8_t pipe);
        bool enable_rx_pipe(uint8_t pipe);

        bool power_up();
        bool start_listening();
        bool stop_listening();

        bool transmit(const uint8_t *data, size_t length);
        bool data_available();
        bool receive(uint8_t *data, size_t capacity,
                     size_t &received);

        void clear_interrupts();
        void flush_tx();
        void flush_rx();

    private:
        Pins Pins_;
        uint8_t payload_size_{1};
        void csn_low() const;
        void csn_high() const;
        void ce_low() const;
        void ce_high() const;

        uint8_t read_register(uint8_t address);
        void write_register(uint8_t address, uint8_t value);
        void write_register(std::uint8_t address, const std::uint8_t *data,
                            std::size_t length);
    };

}