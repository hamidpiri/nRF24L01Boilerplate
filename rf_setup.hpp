#ifndef REGISTER_HPP
#define REGISTER_HPP
#include "register.hpp"
#endif

class rf_setup : public Register
{
public:
    uint8_t RF_SETUP = 0x0F;
    bool RF_DR;
    uint8_t RF_PWR : 2;
    rf_setup() : Register(0x05) {}
    bool SetRF_Setup()
    {
        std::bitset<8>(value).set(7, false).set(6, false).set(5, 0).set(4, 0).set(3, RF_DR).set(2, (RF_PWR >> 1) & 1).set(1, (RF_PWR >> 0) & 1).set(0, 1);
        return true;
    }

private:
};