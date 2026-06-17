#ifndef REGISTER_HPP
#define REGISTER_HPP
#include "register.hpp"
#endif

// Number of bytes in RX payload in data pipe 0 (1
// to 32 bytes).
// 0 Pipe not used
// 1 to 32 bytes
class Status : public Register
{
public:
    Status() : Register(0x07) {}

private:
    uint8_t status_reg_value;
};