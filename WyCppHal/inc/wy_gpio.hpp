#ifndef FD25E189_CD54_4550_BCB3_29427BBA5968
#define FD25E189_CD54_4550_BCB3_29427BBA5968
#include "common.h"

namespace GPIO
{
    class Gpio_Object
    {
    private:
        uint16_t pin;

    protected:
        GPIO_TypeDef *port;
        uint8_t pin_source;
        void familyInit(char port, uint8_t pin, bool rccOn);
        void familyInit(const char *n, bool rccOn);

    public:
        Gpio_Object() = default;
        Gpio_Object(char port, uint8_t pin, bool rccOn = true,
                    GPIOMode_TypeDef m = GPIO_Mode_Out_PP, GPIOSpeed_TypeDef s = GPIO_Speed_50MHz)
        {
            familyInit(port, pin, rccOn);
            this->modeConfig(m, s);
        }
        Gpio_Object(const char *g, bool rccOn = true,
                    GPIOMode_TypeDef m = GPIO_Mode_Out_PP, GPIOSpeed_TypeDef s = GPIO_Speed_50MHz)
        {
            familyInit(g, rccOn);
            this->modeConfig(m, s);
        }

        void modeConfig(GPIOMode_TypeDef m, GPIOSpeed_TypeDef s = GPIO_Speed_50MHz);
        void setOnOff(bool on);
        void set(void) { this->port->BSRR = this->pin; }
        void reset(void) { this->port->BRR = this->pin; }
        void operator=(uint8_t s)
        {
            this->setOnOff(s);
        }
        uint8_t read(void);
    };

    uint8_t getPinFromStr(const char *s);
    uint8_t getGPIO_Index(const char n);
    void turnOnRcc(uint8_t index);
    void turnOnRcc(const char GPIOx);
    void modeConfig(char port, uint8_t pin, GPIOMode_TypeDef m,
                    GPIOSpeed_TypeDef s = GPIO_Speed_50MHz, bool rcc = true);
    void modeConfig(const char *p, GPIOMode_TypeDef m,
                    GPIOSpeed_TypeDef s = GPIO_Speed_50MHz, bool rcc = true);
    void afConfig(char port, uint8_t pin, uint8_t af, GPIOMode_TypeDef m,
                  GPIOSpeed_TypeDef s = GPIO_Speed_50MHz, bool rcc = true);
    void afConfig(const char *g, uint8_t af, GPIOMode_TypeDef m,
                  GPIOSpeed_TypeDef s = GPIO_Speed_50MHz, bool rcc = true);
} // namespace GPIO

#endif /* FD25E189_CD54_4550_BCB3_29427BBA5968 */
