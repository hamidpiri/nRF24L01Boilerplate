#include "register.hpp"

// EN_AA Enable Auto ACk Enhanced Shockburst
class enaa : public Register
{
public:
    enaa() : Register(0x01)
    {
    }

private:
};