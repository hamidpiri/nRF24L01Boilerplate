#include "nrf24/nrf24.hpp"

#include <cstdint>

#include "pico/stdlib.h"

#include "nrf24_registers.hpp"

namespace nrf24
{
    Radio::Radio(Pins pins) : Pins_(pins) {}

    bool Radio::begin(std::uint32_t spi_baudrate_hz)
    {
        spi_init(Pins_.spi, spi_baudrate_hz);

        gpio_set_function(Pins_.miso_pin, GPIO_FUNC_SPI);
        gpio_set_function(Pins_.mosi_pin, GPIO_FUNC_SPI);
        gpio_set_function(Pins_.sck_pin, GPIO_FUNC_SPI);

        gpio_init(Pins_.csn_pin);
        gpio_set_dir(Pins_.csn_pin, GPIO_OUT);
        csn_high();

        gpio_init(Pins_.ce_pin);
        gpio_set_dir(Pins_.ce_pin, GPIO_OUT);
        ce_low();

        // 5-byte addresses, channel 2, CRC enabled, device powered down.
        write_register(detail::reg_setup_aw, 0x03);
        write_register(detail::reg_rf_ch, 2);
        write_register(detail::reg_config, 0x0C);

        flush_tx();
        flush_rx();
        clear_interrupts();

        return true;
    }

    bool Radio::set_channel(std::uint8_t channel)
    {
        if (channel > 125)
        {
            return false;
        }

        write_register(detail::reg_rf_ch, channel);
        return true;
    }

    bool Radio::set_power_and_data_rate(Power power, DataRate rate)
    {
        std::uint8_t value = read_register(detail::reg_rf_setup);

        value &= static_cast<std::uint8_t>(
            ~((0b11u << detail::rf_setup_power_shift) |
              (1u << detail::rf_setup_data_rate_bit)));

        value |= static_cast<std::uint8_t>(
            static_cast<std::uint8_t>(power) << detail::rf_setup_power_shift);

        if (rate == DataRate::Mbps2)
        {
            value |= static_cast<std::uint8_t>(
                1u << detail::rf_setup_data_rate_bit);
        }

        write_register(detail::reg_rf_setup, value);
        return true;
    }

    bool Radio::set_payload_size(std::uint8_t bytes)
    {
        if (bytes == 0 || bytes > 32)
        {
            return false;
        }

        payload_size_ = bytes;
        write_register(detail::reg_rx_pw_p0, bytes);
        return true;
    }

    bool Radio::set_tx_address(const Address &address)
    {
        write_register(detail::reg_tx_addr, address.data(), address.size());
        return true;
    }

    bool Radio::set_rx_pipe_0_address(const Address &address)
    {
        write_register(detail::reg_rx_addr_p0, address.data(), address.size());
        return true;
    }

    bool Radio::enable_auto_ack(std::uint8_t pipe)
    {
        if (pipe > 5)
        {
            return false;
        }

        std::uint8_t value = read_register(detail::reg_en_aa);
        value |= static_cast<std::uint8_t>(1u << pipe);
        write_register(detail::reg_en_aa, value);
        return true;
    }

    bool Radio::enable_rx_pipe(std::uint8_t pipe)
    {
        if (pipe > 5)
        {
            return false;
        }

        std::uint8_t value = read_register(detail::reg_en_rxaddr);
        value |= static_cast<std::uint8_t>(1u << pipe);
        write_register(detail::reg_en_rxaddr, value);
        return true;
    }

    bool Radio::power_up()
    {
        std::uint8_t value = read_register(detail::reg_config);
        value |= static_cast<std::uint8_t>(1u << detail::config_pwr_up_bit);
        write_register(detail::reg_config, value);

        sleep_ms(2);
        return true;
    }

    bool Radio::start_listening()
    {
        power_up();

        std::uint8_t value = read_register(detail::reg_config);
        value |= static_cast<std::uint8_t>(1u << detail::config_prim_rx_bit);
        write_register(detail::reg_config, value);

        ce_high();
        return true;
    }

    bool Radio::stop_listening()
    {
        ce_low();

        std::uint8_t value = read_register(detail::reg_config);
        value &= static_cast<std::uint8_t>(
            ~(1u << detail::config_prim_rx_bit));
        write_register(detail::reg_config, value);

        return true;
    }

    bool Radio::transmit(const std::uint8_t *data, std::size_t length)
    {
        if (data == nullptr || length == 0 || length > payload_size_)
        {
            return false;
        }

        stop_listening();
        power_up();
        clear_interrupts();
        flush_tx();

        std::uint8_t command = detail::command_write_tx_payload;
        std::uint8_t payload[32] = {};

        for (std::size_t i = 0; i < length; ++i)
        {
            payload[i] = data[i];
        }

        csn_low();
        spi_write_blocking(Pins_.spi, &command, 1);
        spi_write_blocking(Pins_.spi, payload, payload_size_);
        csn_high();

        ce_high();
        sleep_us(15);
        ce_low();

        // Wait until the radio reports success or maximum retries reached.
        for (int i = 0; i < 100; ++i)
        {
            const std::uint8_t status = read_register(detail::reg_status);

            if (status & (1u << detail::status_tx_ds_bit))
            {
                clear_interrupts();
                return true;
            }

            if (status & (1u << detail::status_max_rt_bit))
            {
                clear_interrupts();
                flush_tx();
                return false;
            }

            sleep_ms(1);
        }

        return false;
    }

    bool Radio::data_available()
    {
        const std::uint8_t status = read_register(detail::reg_status);
        return (status & (1u << detail::status_rx_dr_bit)) != 0;
    }

    bool Radio::receive(std::uint8_t *data, std::size_t capacity,
                        std::size_t &received)
    {
        received = 0;

        if (data == nullptr || capacity < payload_size_ || !data_available())
        {
            return false;
        }

        std::uint8_t command = detail::command_read_rx_payload;
        std::uint8_t nop = detail::command_nop;
        std::uint8_t ignored = 0;

        csn_low();
        spi_write_read_blocking(Pins_.spi, &command, &ignored, 1);

        for (std::uint8_t i = 0; i < payload_size_; ++i)
        {
            spi_write_read_blocking(Pins_.spi, &nop, &data[i], 1);
        }

        csn_high();

        write_register(
            detail::reg_status,
            static_cast<std::uint8_t>(1u << detail::status_rx_dr_bit));

        received = payload_size_;
        return true;
    }

    void Radio::clear_interrupts()
    {
        write_register(
            detail::reg_status,
            static_cast<std::uint8_t>(
                (1u << detail::status_rx_dr_bit) |
                (1u << detail::status_tx_ds_bit) |
                (1u << detail::status_max_rt_bit)));
    }

    void Radio::flush_tx()
    {
        std::uint8_t command = detail::command_flush_tx;
        csn_low();
        spi_write_blocking(Pins_.spi, &command, 1);
        csn_high();
    }

    void Radio::flush_rx()
    {
        std::uint8_t command = detail::command_flush_rx;
        csn_low();
        spi_write_blocking(Pins_.spi, &command, 1);
        csn_high();
    }

    void Radio::csn_low() const
    {
        gpio_put(Pins_.csn_pin, 0);
    }

    void Radio::csn_high() const
    {
        gpio_put(Pins_.csn_pin, 1);
    }

    void Radio::ce_low() const
    {
        gpio_put(Pins_.ce_pin, 0);
    }

    void Radio::ce_high() const
    {
        gpio_put(Pins_.ce_pin, 1);
    }

    std::uint8_t Radio::read_register(std::uint8_t address)
    {
        const std::uint8_t command =
            detail::command_read_register | (address & 0x1F);

        const std::uint8_t nop = detail::command_nop;
        std::uint8_t ignored = 0;
        std::uint8_t value = 0;

        csn_low();
        spi_write_read_blocking(Pins_.spi, &command, &ignored, 1);
        spi_write_read_blocking(Pins_.spi, &nop, &value, 1);
        csn_high();

        return value;
    }

    void Radio::write_register(std::uint8_t address, std::uint8_t value)
    {
        write_register(address, &value, 1);
    }

    void Radio::write_register(std::uint8_t address, const std::uint8_t *data,
                               std::size_t length)
    {
        const std::uint8_t command =
            detail::command_write_register | (address & 0x1F);

        csn_low();
        spi_write_blocking(Pins_.spi, &command, 1);
        spi_write_blocking(Pins_.spi, data, length);
        csn_high();
    }

}
