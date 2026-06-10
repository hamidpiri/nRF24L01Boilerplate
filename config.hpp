#ifndef REGISTER_HPP
#define REGISTER_HPP
#include "register.hpp"
#endif

class config : public Register
{

public:
    config() : Register(0x00)
    {
    }

    void UpdateSelf()
    {
        this->value = std::bitset<8>(value).set(7, false).set(6, MASK_RX_DR).set(5, MASK_TX_DS).set(4, MASK_MAX_RT).set(3, EN_CRC).set(2, CRCO).set(1, PWR_UP).set(0, PRIM_RX).to_ulong();
        printf("value = %d\n", value);
        WriteRegisterValue(this->value);
    }

    void setMASK_TX_DS(bool value)
    {
        this->MASK_TX_DS = value;
    }
    void setMASK_RX_DR(bool value)
    {
        this->MASK_RX_DR = value;
    }
    void setMASK_MAX_RT(bool value)
    {
        this->MASK_MAX_RT = value;
    }
    void setEN_CRC(bool value)
    {
        this->EN_CRC = value;
    }
    void setCRCO(bool value)
    {
        this->CRCO = value;
    }
    void setPWR_UP(bool value)
    {
        this->PWR_UP = value;
    }
    void setPRIM_RX(bool value)
    {
        this->PRIM_RX = value;
    }

private:
    uint8_t value = 0x08;
    // Mask interrupt caused by RX_DR
    // 1: Interrupt not reflected on the IRQ pin
    // 0: Reflect RX_DR as active low interrupt on the
    // IRQ pin
    bool MASK_RX_DR = 0;

    //     Mask interrupt caused by TX_DS
    // 1: Interrupt not reflected on the IRQ pin
    // 0: Reflect TX_DS as active low interrupt on the
    // IRQ pin
    bool MASK_TX_DS = 0;
    //     Mask interrupt caused by MAX_RT
    // 1: Interrupt not reflected on the IRQ pin
    // 0: Reflect MAX_RT as active low interrupt on the
    // IRQ pin
    bool MASK_MAX_RT = 0;
    //     Enable CRC. Forced high if one of the bits in
    // the EN_AA is high
    bool EN_CRC = 1;
    //     CRC encoding scheme
    // '0' - 1 byte
    // '1' – 2 bytes
    bool CRCO = 0;
    // 1: POWER UP, 0:POWER DOWN
    bool PWR_UP = 0;
    //     RX/TX control
    // 1: PRX, 0: PTX
    bool PRIM_RX = 0;
};