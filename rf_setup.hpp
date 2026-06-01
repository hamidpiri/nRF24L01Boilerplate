#ifndef REGISTER_HPP
#define REGISTER_HPP
#include "register.hpp"
#endif

class rf_setup : public Register
{
public:
    uint8_t RF_SETUP = 0x0F;

    typedef struct
    {
        uint8_t RF_PWR : 2;
        bool RF_DR;

    } rf_setup_t;
    rf_setup() : Register(0x05) {}
    bool SetRF_Setup()
    {
        std::bitset<8>(value).set(7, false).set(6, false).set(5, 0).set(4, 0).set(3, RF_DR)..set(2, 0).set(1, 0).set(0, 0);
        return true;
    }

private:
};