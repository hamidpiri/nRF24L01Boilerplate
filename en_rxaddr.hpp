#ifndef REGISTER_HPP
#define REGISTER_HPP
#include "register.hpp"
#endif

class en_rxaddr : public Register
{
public:
    en_rxaddr() : Register(0x02)
    {
    }

    void UpdateSelf()
    {
        this->value = std::bitset<8>(value).set(7, false).set(6, false).set(5, ERX_P5).set(4, ERX_P4).set(3, ERX_P3).set(2, ERX_P2).set(1, ERX_P1).set(0, ERX_P0).to_ulong();
        printf("value = %d\n", value);
        WriteRegisterValue(this->value);
    }

    // Enable data pipe 5.
    void setERX_P5(bool value)
    {
        this->ERX_P5 = value;
    }
    // Enable data pipe 4
    void setERX_P4(bool value)
    {
        this->ERX_P4 = value;
    }
    // Enable data pipe 3
    void setERX_P3(bool value)
    {
        this->ERX_P3 = value;
    }
    // Enable data pipe 2
    void setERX_P2(bool value)
    {
        this->ERX_P2 = value;
    }
    // Enable data pipe 1
    void setERX_P1(bool value)
    {
        this->ERX_P1 = value;
    }
    // Enable data pipe 0
    void setERX_P0(bool value)
    {
        this->ERX_P0 = value;
    }

private:
    bool ERX_P5 = false;
    bool ERX_P4 = false;
    bool ERX_P3 = false;
    bool ERX_P2 = false;
    bool ERX_P1 = false;
    bool ERX_P0 = false;
};
