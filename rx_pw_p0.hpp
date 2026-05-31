#ifndef REGISTER_HPP
#define REGISTER_HPP
#include "register.hpp"
#endif

// Number of bytes in RX payload in data pipe 0 (1
// to 32 bytes).
// 0 Pipe not used
// 1 to 32 bytes
class rx_pw_p0 : public Register
{
public:
    rx_pw_p0() : Register(0x0A) {}
    bool SetTx_ADDR(uint64_t address = 0x10)
    {
        RX_ADDR_P0 = address;
    }

private:
    uint32_t RX_ADDR_P0;
};