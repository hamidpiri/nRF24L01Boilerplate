class Register
{
    volatile uint32_t *reg;

public:
    Register(uint32_t address)
    {
        reg = (volatile uint32_t *)address;
    };

    bool ReadBit(uint8_t bit)
    {
        return (*reg >> bit) & 1u;
    }
}