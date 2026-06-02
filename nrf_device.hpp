#include "config.hpp"
#include "en_aa.hpp"
#include "setup_aw.hpp"
#include "en_rxaddr.hpp"
#include "rx_addr_p0.hpp"
#include "rx_pw_p0.hpp"
#include "rf_setup.hpp"
#include "rf_ch.hpp"

class nrfdevice
{
public:
    // enaa enaa_register;
    // setupaw setupaw;
    config config_register;
    en_rxaddr dataPipeAddressRegister;
    rx_addr_p0 rx_addr_p0_register;
    rx_pw_p0 rx_pw_p0_register;
    rf_setup rf_setup_register;
    rf_ch rf_ch_register;
    setupaw setup_aw_register;
    enum deviceMode
    {
        RX = false,
        TX = true
    };

    enum RF_DR
    {
        oneMbps = 0,
        twoMbps = 1
    };
    enum RF_PWR
    {
        _18dbm,
        _12dbm,
        _6dbm,
        _0dbm
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

    bool EnableDataPipe(int pipeNumber)
    {
        dataPipeAddressRegister.setPipeNumber(pipeNumber);
        return true;
    }

    bool SetPipe0Address(uint64_t address)
    {
        rx_addr_p0_register.SetRx_ADDR_P0();
        return true;
    }

    bool SetPipe0Width(int pipe0Width)
    {
        rx_pw_p0_register.SetRX_PW_P0(pipe0Width);
        return true;
    }
    // Sets data rate either 1Mbps or 2Mbps
    bool SetPowerAndDataRate(uint8_t power, RF_DR rf_rate)
    {
        rf_setup_register.RF_DR = rf_rate;
        rf_setup_register.RF_PWR = RF_PWR::_0dbm;

        return true;
    }
    bool SetChannel(int channelNumber)
    {
        rf_ch_register.SetRF_CH(32);
        return true;
    }

    // Set address width (bytes)
    bool SetAddressWidth(setupaw::addressWidth width)
    {
        setup_aw_register.SetAddressWidth(width);
    }

private:
};