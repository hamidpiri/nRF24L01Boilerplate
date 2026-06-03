#ifndef REGISTER_HPP
#define REGISTER_HPP
#include "register.hpp"
#endif
// EN_AA Enable Auto ACk Enhanced Shockburst
class enaa : public Register
{
public:
    enaa() : Register(0x01)
    {
    }
    void UpdateSelf()
    {
        printf("en_aa reg value = %d\n", value);
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