#include "register.hpp"

class config : public Register
{

public:
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
    bool PWR_UP;
    //     RX/TX control
    // 1: PRX, 0: PTX
    bool PRIM_RX;

    config() : Register(0x00)
    {
        this->value = 0b00 + MASK_RX_DR + MASK_MAX_RT + MASK_MAX_RT + EN_CRC + CRCO + PWR_UP + PRIM_RX;
    }

private:
    uint8_t value = 0x08;
};