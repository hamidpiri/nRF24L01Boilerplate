#ifndef REGISTER_HPP
#include "register.hpp"
#endif

// EN_AA Enable Auto ACk Enhanced Shockburst
class enaa : public Register
{
public:
    enaa() : Register(0x01)
    {
    }

private:
};