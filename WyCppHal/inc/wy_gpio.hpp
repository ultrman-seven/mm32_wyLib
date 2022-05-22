#ifndef FD25E189_CD54_4550_BCB3_29427BBA5968
#define FD25E189_CD54_4550_BCB3_29427BBA5968
#include "common.h"

namespace GPIO
{
    class Gpio_Object
    {
    private:
        uint8_t pin_source;
        uint16_t pin;
        GPIO_TypeDef *port;

    protected:
        void familyInit(char port, uint8_t pin, bool rccOn);
        void familyInit(const char* n, bool rccOn);

    public:
        Gpio_Object() = default;
        Gpio_Object(const char* n)
        {
            familyInit(n,true);
            this->modeConfig(GPIO_Mode_Out_PP);
        }
        Gpio_Object(const char* n,bool rccOn)
        {
            familyInit(n,rccOn);
            this->modeConfig(GPIO_Mode_Out_PP);
        }
        Gpio_Object(char port, uint8_t pin, bool rccOn)
        {
            familyInit(port, pin, rccOn);
            this->modeConfig(GPIO_Mode_Out_PP);
        }
        Gpio_Object(char port, uint8_t pin)
        {
            familyInit(port, pin, true);
            this->modeConfig(GPIO_Mode_Out_PP);
        }
        Gpio_Object(char port, uint8_t pin, bool rccOn, GPIOMode_TypeDef m)
        {
            familyInit(port, pin, rccOn);
            this->modeConfig(m);
        }
        Gpio_Object(char port, uint8_t pin, bool rccOn, GPIOMode_TypeDef m, GPIOSpeed_TypeDef s)
        {
            familyInit(port, pin, rccOn);
            this->modeConfig(m, s);
        }
        Gpio_Object(const char *g, bool rccOn, GPIOMode_TypeDef m)
        {
            familyInit(g, rccOn);
            this->modeConfig(m, GPIO_Speed_50MHz);
        }
        Gpio_Object(const char *g, bool rccOn, GPIOMode_TypeDef m, GPIOSpeed_TypeDef s)
        {
            familyInit(g, rccOn);
            this->modeConfig(m, s);
        }

        void modeConfig(GPIOMode_TypeDef m) { this->modeConfig(m, GPIO_Speed_50MHz); }
        void modeConfig(GPIOMode_TypeDef m, GPIOSpeed_TypeDef s);
        void setOnOff(bool on);
        void set(void) { this->port->BSRR = this->pin; }
        void reset(void) { this->port->BRR = this->pin; }
        void operator=(uint8_t s) { this->setOnOff(s); }
        uint8_t read(void);
    };

    void modeConfig(char port, uint8_t pin, GPIOMode_TypeDef m, GPIOSpeed_TypeDef s);
    void modeConfig(char port, uint8_t pin, GPIOMode_TypeDef m);
    void afConfig(char port, uint8_t pin, uint8_t af, GPIOMode_TypeDef m, GPIOSpeed_TypeDef s);
    void afConfig(const char *g, uint8_t af, GPIOMode_TypeDef m, GPIOSpeed_TypeDef s);
} // namespace GPIO

#endif /* FD25E189_CD54_4550_BCB3_29427BBA5968 */
