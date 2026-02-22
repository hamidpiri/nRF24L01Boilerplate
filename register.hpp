class Register
{
    uint8_t value;

public:
    Register(uint8_t regValue)
    {
        this->value = regValue;
    };

    bool ReadBit(uint8_t bit)
    {
        return (value >> bit) & 1u;
    };
};
