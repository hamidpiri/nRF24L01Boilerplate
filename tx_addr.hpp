#ifndef REGISTER_HPP
#define REGISTER_HPP
#include "register.hpp"
#endif

// Transmit address. Used for a PTX device only.
// (LSByte is written first)
// Set RX_ADDR_P0 equal to this address to han-
// dle automatic acknowledge if this is a PTX
// device with Enhanced ShockBurst™ enabled.
class tx_addr : public Register
{
public:
    tx_addr() : Register(0x10) {}
    bool SetTx_ADDR(uint64_t address = 0xE7E7E7E7E7)
    {
        TX_ADDR = address;
        return true;
        // Todo
        //  WriteRegisterValue()
    }

private:
    uint64_t TX_ADDR;
};