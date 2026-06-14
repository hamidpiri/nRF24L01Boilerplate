#ifndef REGISTER_HPP
#define REGISTER_HPP
#include "register.hpp"
#endif

// Number of bytes in RX payload in data pipe 0 (1
// to 32 bytes).
// 0 Pipe not used
// 1 to 32 bytes
class rf_ch : public Register
{
public:
    rf_ch() : Register(0x05) {}
    bool SetRF_CH(uint8_t channel)
    {
        // TODO : Set the Frequency
        this->value = channel;
        this->WriteRegisterValue(channel);
        return true;
    }

private:
    uint32_t RX_PW_P0;
};