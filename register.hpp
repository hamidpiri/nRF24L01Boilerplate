// #ifndef REGISTER_H
// #define REGISTER_H

/***NRF24L01 Commands***/
#define R_REGISTER 0B00000000
#define W_REGISTER 0B00100000
#define R_RX_PAYLOAD 0B01100001
#define W_TX_PAYLOAD 0B10100000
#define FLUSH_TX 0B11100001
#define FLUSH_RX 0B11100010
#define REUSE_TX_PL 0B11100011
#define ACTIVATE 0B01010000
#define R_RX_PL_WID 0B01100000
#define W_ACK_PAYLOAD 0B10101000
#define W_TX_PAYLOAD_NO_ACK 0B10110000
#define NOP 0xFF
#define SPI_PORT spi0

#define PIN_MISO 0
#define PIN_CS 17
#define PIN_SCK 18
#define PIN_MOSI 3
#define LED_PIN 25
class Register
{

public:
    uint8_t address;
    uint8_t value;
    // Give me the address of desired register
    Register(uint8_t address)
    {
        this->address = address;
    };

    bool ReadBit(uint8_t bit)
    {
        this->ReadRegisterValue();
        return (value >> bit) & 1u;
    };
    bool WriteBit(uint8_t bit, uint8_t bitValue)
    {
        ReadRegisterValue();
        if (bitValue)
            this->value |= (1 << bit);
        else
            this->value &= ~(1 << bit);
        WriteRegisterValue(this->value);
        return true;
    };

    static inline void csn_low() { gpio_put(PIN_CS, 0); }
    static inline void csn_high() { gpio_put(PIN_CS, 1); }

    uint8_t ReadRegisterValue()
    {
        static uint8_t buffer_Read[8];
        uint8_t cmd = R_REGISTER | (address & 0x1F);
        uint8_t nop = NOP;
        memset(buffer_Read, 0, sizeof(buffer_Read));
        csn_low();
        // printf("Reading Register 0x%02X...\n", address);
        spi_write_read_blocking(SPI_PORT, &cmd, &buffer_Read[0], 1);
        spi_write_read_blocking(SPI_PORT, &nop, &buffer_Read[1], 1);
        csn_high();
        this->value = buffer_Read[1];
        return buffer_Read[1];
    }

    uint8_t WriteRegisterValue(uint8_t value)
    {
        uint8_t nop = NOP;
        static uint8_t buffer_Read[8];
        uint8_t cmd = W_REGISTER | (address & 0x1F); // 0B00100000; 0x20
        printf("CMD is: 0x%02X \n", cmd);
        printf("Writing to Register 0x%02X Value: 0x%02X...\n", address, value);

        csn_low();
        spi_write_read_blocking(SPI_PORT, &cmd, &buffer_Read[0], 1);
        spi_write_read_blocking(SPI_PORT, &value, &buffer_Read[1], 1);

        csn_high();

        printf("Writing Success!\n");
        return 0;
    }
};
