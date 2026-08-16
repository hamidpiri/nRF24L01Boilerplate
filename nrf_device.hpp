#include "config.hpp"
#include "en_aa.hpp"
#include "setup_aw.hpp"
#include "en_rxaddr.hpp"
#include "rx_addr_p0.hpp"
#include "rx_pw_p0.hpp"
#include "rf_setup.hpp"
#include "rf_ch.hpp"
#include "tx_addr.hpp"
#include "commands.hpp"
#include "status.hpp"

class nrfdevice
{
public:
    // enaa enaa_register;
    // setupaw setupaw;
    config config_register;
    Status status_register;
    en_rxaddr dataPipeAddressRegister;
    rx_addr_p0 rx_addr_p0_register;
    rx_pw_p0 rx_pw_p0_register;
    rf_setup rf_setup_register;
    rf_ch rf_ch_register;
    setupaw setup_aw_register;
    tx_addr tx_addr_register;
    enaa enaa_register;
    commands command;

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
    nrfdevice(
        spi_inst_t *spi,
        uint8_t sck_pin_number,
        uint8_t mosi_pin_number,
        uint8_t miso_pin_number,
        uint8_t ce_pin_number,
        uint8_t csn_pin_number)
    {
        this->spi_ = spi;
        this->sck_pin_ = sck_pin_number;
        this->mosi_pin_ = mosi_pin_number;
        this->miso_pin_ = miso_pin_number;
        this->ce_pin_ = ce_pin_number;
        this->csn_pin_ = csn_pin_number;

        gpio_init(ce_pin_);
        gpio_set_function(ce_pin_, GPIO_FUNC_SIO);
        gpio_set_dir(ce_pin_, GPIO_OUT);

        // SPI initialisation. This example will use SPI at 1MHz.
        gpio_set_function(miso_pin_, GPIO_FUNC_SPI);
        gpio_set_function(csn_pin_, GPIO_FUNC_SIO);
        gpio_set_function(sck_pin_, GPIO_FUNC_SPI);
        gpio_set_function(mosi_pin_, GPIO_FUNC_SPI);
        spi_init(this->spi_, 1000 * 1000);
        // Chip select is active-low, so we'll initialise it to a driven-high state
        gpio_set_dir(csn_pin_, GPIO_OUT);
        gpio_put(csn_pin_, 1);

        gpio_init(LED_PIN);
        gpio_set_dir(LED_PIN, GPIO_OUT);
        gpio_put(LED_PIN, 1);
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
        if (mode == deviceMode::RX)
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
    bool EnableAutoAckOfDataPipe(int pipeNumber)
    {
        enaa_register.setPipeNumber(pipeNumber);
        return true;
    }

    // Sets data rate either 1Mbps or 2Mbps
    bool SetPowerAndDataRate(uint8_t power, RF_DR rf_rate)
    {
        rf_setup_register.RF_DR = rf_rate;
        rf_setup_register.RF_PWR = RF_PWR::_0dbm;
        rf_setup_register.SetRF_Setup();
        return true;
    }
    bool SetChannel(int channelNumber)
    {
        rf_ch_register.SetRF_CH(channelNumber);
        return true;
    }

    // Set address width (bytes)
    bool SetAddressWidth(setupaw::addressWidth width)
    {
        setup_aw_register.SetAddressWidth(width);
        return true;
    }

    bool SetPipe0Address(uint64_t address)
    {
        rx_addr_p0_register.SetRx_ADDR_P0(address);
        return true;
    }

    bool SetTransmitAddress(uint64_t address)
    {
        tx_addr_register.SetTx_ADDR(address);
        return true;
    }

    bool SetPipe0Width(uint64_t pipe0Width)
    {
        rx_pw_p0_register.SetRX_PW_P0(pipe0Width);
        return true;
    }

    bool WritePayload(uint8_t payload)
    {
        command.WriteTXPayload(payload);
        return true;
    }
    bool IsDataReceived(uint8_t pipeNo)
    {
        return status_register.ReadBit(6);
    }
    // Check IsDataReceived() first!
    uint8_t ReadRxPayload()
    {
        return command.ReadRXPayload();
    }

    bool IsDataTransmitted()
    {
        return this->status_register.ReadBit(5);
    }
    bool IsTransmitFailed()
    {
        return this->status_register.ReadBit(4);
    }
    // Clears RX_DR, TX_DS, MAX_RT interrupt flags by setting them to 1
    bool ClearInterrupts()
    {

        status_register.WriteRegisterValue(0x70);
        return true;
    }

private:
    spi_inst_t *spi_;

    uint8_t sck_pin_;
    uint8_t mosi_pin_;
    uint8_t miso_pin_;
    uint8_t ce_pin_;
    uint8_t csn_pin_;
};