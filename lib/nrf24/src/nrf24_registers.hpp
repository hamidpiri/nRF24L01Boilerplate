#pragma once

#include <cstdint>

namespace nrf24::detail
{

    // SPI commands
    inline constexpr std::uint8_t command_read_register = 0x00;
    inline constexpr std::uint8_t command_write_register = 0x20;
    inline constexpr std::uint8_t command_read_rx_payload = 0x61;
    inline constexpr std::uint8_t command_write_tx_payload = 0xA0;
    inline constexpr std::uint8_t command_flush_tx = 0xE1;
    inline constexpr std::uint8_t command_flush_rx = 0xE2;
    inline constexpr std::uint8_t command_nop = 0xFF;

    // Register addresses
    inline constexpr std::uint8_t reg_config = 0x00;
    inline constexpr std::uint8_t reg_en_aa = 0x01;
    inline constexpr std::uint8_t reg_en_rxaddr = 0x02;
    inline constexpr std::uint8_t reg_setup_aw = 0x03;
    inline constexpr std::uint8_t reg_rf_ch = 0x05;
    inline constexpr std::uint8_t reg_rf_setup = 0x06;
    inline constexpr std::uint8_t reg_status = 0x07;
    inline constexpr std::uint8_t reg_rx_addr_p0 = 0x0A;
    inline constexpr std::uint8_t reg_tx_addr = 0x10;
    inline constexpr std::uint8_t reg_rx_pw_p0 = 0x11;

    // Bit positions
    inline constexpr std::uint8_t config_prim_rx_bit = 0;
    inline constexpr std::uint8_t config_pwr_up_bit = 1;

    inline constexpr std::uint8_t status_rx_dr_bit = 6;
    inline constexpr std::uint8_t status_tx_ds_bit = 5;
    inline constexpr std::uint8_t status_max_rt_bit = 4;

    inline constexpr std::uint8_t rf_setup_data_rate_bit = 3;
    inline constexpr std::uint8_t rf_setup_power_shift = 1;

} // namespace nrf24::detail