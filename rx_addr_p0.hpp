#ifndef REGISTER_HPP
#define REGISTER_HPP
#include "register.hpp"
#endif

// Receive address data pipe 0. 5 Bytes maximum
// length. (LSByte is written first. Write the number
// of bytes defined by SETUP_AW)
class rx_addr_p0 : public Register
{
public:
    rx_addr_p0() : Register(0x0A) {}
    bool SetRx_ADDR_P0(uint64_t address = 0xE7E7E7E7E7)
    {
        RX_ADDR_P0 = address;
        uint8_t data[5];
        for (int i = 0; i < 5; i++)
        {
            data[i] = (RX_ADDR_P0 >> (i * 8)) & 0xFF;
        }
        WriteRegisterBytes(data, 5);
        return true;
    }

private:
    uint64_t RX_ADDR_P0;
};