#include "config.hpp"
#include "en_aa.hpp"
#include "setup_aw.hpp"
class nrfdevice
{
public:
    // enaa enaa_register;
    // setupaw setupaw;
    config config_register;
    enum deviceMode
    {
        RX = false,
        TX = true
    };
    nrfdevice()
    {
    }

    bool powerup()
    {
        config_register.setPWR_UP(1);
        config_register.UpdateSelf();
        printf("Device powered up!\n");
        return true;
    }

    // Set Device Mode as Receiver (0) or Sender (1)
    bool SetMode(bool mode)
    {
        if (mode)
        {
            config_register.setPRIM_RX(true);
            config_register.UpdateSelf();
            return true;
        }
        config_register.setPRIM_RX(false);
        config_register.UpdateSelf();
        return false;
    }

private:
};