#ifndef REGISTER_HPP
#define REGISTER_HPP
#include "register.hpp"
#endif
class setupaw : public Register
{

public:
    enum addressWidth
    {
        _3bytes = 0b01,
        _4bytes = 0b10,
        _5bytes = 0b11
    };
    uint8_t SETUP_AW;
    setupaw() : Register(0x03)
    {
    }
    bool SetAddressWidth(uint8_t addressWidth)
    {
        WriteRegisterValue(addressWidth);
    }

private:
};