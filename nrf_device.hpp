#include "config.hpp"
#include "en_aa.hpp"
#include "setup_aw.hpp"
class nrfdevice
{
public:
    // enaa enaa_register;
    // setupaw setupaw;
    nrfdevice()
    {
    }

    bool powerup()
    {
        config config_register;
        config_register.setPWR_UP(1);
        config_register.UpdateSelf();
        printf("Device powered up!\n");
        return true;
    }

private:
};