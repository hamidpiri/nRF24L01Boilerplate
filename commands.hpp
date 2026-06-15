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
#define PIN_CE 15
#define LED_PIN 25

class commands
{
public:
    static bool ClearTXPayload()
    {
        uint8_t cmd = FLUSH_TX;

        csn_low();
        spi_write_blocking(SPI_PORT, &cmd, 1);
        csn_high();
    }
    static bool WriteTXPayload(uint8_t payload)
    {
        static uint8_t buffer_Read[8];
        uint8_t cmd = W_TX_PAYLOAD;
        uint8_t nop = NOP;
        uint8_t _payload = payload;
        memset(buffer_Read, 0, sizeof(buffer_Read));
        csn_low();
        spi_write_read_blocking(SPI_PORT, &cmd, &buffer_Read[0], 1);
        printf("Status after writing payload: %d \n", buffer_Read[0]);
        spi_write_read_blocking(SPI_PORT, &_payload, &buffer_Read[2], 1);
        csn_high();
        return true;
    }

    static bool TransmitOverRadio()
    {
        // 11. Wait
        gpio_init(PIN_CE);
        sleep_ms(500);
        gpio_set_dir(PIN_CE, GPIO_OUT);
        // Pulse CE High
        gpio_put(PIN_CE, 1);
        sleep_ms(10);
        gpio_put(PIN_CE, 0);
        return true;
    };

    static bool ReceiveOverRadio()
    {
        // 11. Wait
        gpio_init(PIN_CE);
        sleep_ms(500);
        gpio_set_dir(PIN_CE, GPIO_OUT);
        // Pulse CE High
        gpio_put(PIN_CE, 1);
        gpio_put(LED_PIN, 1);
        return true;
    }

private:
    static inline void csn_low() { gpio_put(PIN_CS, 0); }
    static inline void csn_high() { gpio_put(PIN_CS, 1); }
};