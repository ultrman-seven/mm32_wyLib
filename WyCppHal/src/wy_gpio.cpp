#include "wy_gpio.hpp"
using namespace GPIO;
const uint32_t gpios[] = {GPIOA_BASE, GPIOB_BASE, GPIOC_BASE, GPIOD_BASE};
const uint32_t gpio_rccs[] = {RCC_AHBPeriph_GPIOA, RCC_AHBPeriph_GPIOB, RCC_AHBPeriph_GPIOC, RCC_AHBPeriph_GPIOD};

uint8_t getPinFromStr(const char *s)
{
    uint8_t result = 0;
    while (*s)
    {
        result *= 10;
        result += *s++ - '0';
    }
    return result;
}

void Gpio_Object::familyInit(char port, uint8_t pin, bool rccOn)
{
    uint8_t tmp;
    this->pin_source = pin;
    this->pin = 0x0001 << pin;
    if (port >= 'a' && port <= 'd')
        tmp = port - 'a';
    else if (port >= 'A' && port <= 'D')
        tmp = port - 'A';

    if (rccOn)
        RCC_AHBPeriphClockCmd(gpio_rccs[tmp], ENABLE);

    this->port = (GPIO_TypeDef *)gpios[tmp];
}
void Gpio_Object::familyInit(const char* n, bool rccOn){
        uint8_t tmp;
    this->pin_source = getPinFromStr(n+1);
    this->pin = 0x0001 << (this->pin_source);
    if (*n >= 'a' && *n <= 'd')
        tmp = *n - 'a';
    else if (*n >= 'A' && *n <= 'D')
        tmp = *n - 'A';

    if (rccOn)
        RCC_AHBPeriphClockCmd(gpio_rccs[tmp], ENABLE);
    this->port = (GPIO_TypeDef *)gpios[tmp];
}

void Gpio_Object::modeConfig(GPIOMode_TypeDef m, GPIOSpeed_TypeDef s)
{
    GPIO_InitTypeDef gpio;
    gpio.GPIO_Mode = m;
    gpio.GPIO_Pin = this->pin;
    gpio.GPIO_Speed = s;
    GPIO_Init(this->port, &gpio);
}

void Gpio_Object::setOnOff(bool on)
{
    if (on)
        this->port->BSRR = this->pin;
    else
        this->port->BRR == this->pin;
}

uint8_t Gpio_Object::read(void)
{
    if ((this->port->IDR & this->pin) != (uint32_t)Bit_RESET)
        return (uint8_t)Bit_SET;
    return (uint8_t)Bit_RESET;
}

namespace GPIO
{
    void modeConfig(char port, uint8_t pin, GPIOMode_TypeDef m, GPIOSpeed_TypeDef s)
    {
        GPIO_InitTypeDef gpio;
        uint8_t tmp;
        gpio.GPIO_Mode = m;
        gpio.GPIO_Pin = 0x0001 << pin;
        gpio.GPIO_Speed = s;
        if (port >= 'a' && port <= 'd')
            tmp = port - 'a';
        else if (port >= 'A' && port <= 'D')
            tmp = port - 'A';
        GPIO_Init((GPIO_TypeDef *)gpios[tmp], &gpio);
    }
    void modeConfig(char port, uint8_t pin, GPIOMode_TypeDef m)
    {
        modeConfig(port, pin, m, GPIO_Speed_50MHz);
    }
    void afConfig(char port, uint8_t pin, uint8_t af, GPIOMode_TypeDef m, GPIOSpeed_TypeDef s)
    {
        GPIO_InitTypeDef gpio;
        uint8_t tmp;
        gpio.GPIO_Mode = m;
        gpio.GPIO_Pin = 0x0001 << pin;
        gpio.GPIO_Speed = s;
        if (port >= 'a' && port <= 'd')
            tmp = port - 'a';
        else if (port >= 'A' && port <= 'D')
            tmp = port - 'A';
        GPIO_Init((GPIO_TypeDef *)gpios[tmp], &gpio);
        GPIO_PinAFConfig((GPIO_TypeDef *)gpios[tmp], pin, af);
    }
    void afConfig(const char* g, uint8_t af, GPIOMode_TypeDef m, GPIOSpeed_TypeDef s)
    {
        GPIO_InitTypeDef gpio;
        uint8_t tmp;
        uint8_t pin = getPinFromStr(g + 1);
        gpio.GPIO_Mode = m;
        gpio.GPIO_Pin = 0x0001 << pin;
        gpio.GPIO_Speed = s;
        if (*g >= 'a' && *g <= 'd')
            tmp = *g - 'a';
        else if (*g >= 'A' && *g <= 'D')
            tmp = *g - 'A';
        GPIO_Init((GPIO_TypeDef *)gpios[tmp], &gpio);
        GPIO_PinAFConfig((GPIO_TypeDef *)gpios[tmp], pin, af);
    }
}
