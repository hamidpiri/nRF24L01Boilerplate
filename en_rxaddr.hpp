#ifndef REGISTER_HPP
#define REGISTER_HPP
#include "register.hpp"
#endif

class en_rxaddr : public Register
{
public:
    bool ERX_P5 = false;
    bool ERX_P4 = false;
    bool ERX_P3 = false;
    bool ERX_P2 = false;
    bool ERX_P1 = false;
    bool ERX_P0 = false;
    en_rxaddr() : Register(0x02)
    {
    }

    void UpdateSelf()
    {
        printf("en_rx reg value = %d\n", value);
        WriteRegisterValue(this->value);
    }

    bool setPipeNumber(int pipeNumber)
    {
        this->value = std::bitset<8>(this->value).set(pipeNumber, 1).to_ulong();
        UpdateSelf();
        return true;
    }

private:
};
