#include "wy_8080.hpp"

using namespace intel8080;

void Intel8080_Object::init(void)
{
    GPIO_InitTypeDef gpio;

    gpio.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_Pin = this->pinStr.rdPin;
    GPIO_Init(this->pinStr.rdPort, &gpio);

    gpio.GPIO_Pin = this->pinStr.wtPin;
    GPIO_Init(this->pinStr.wtPort, &gpio);

    gpio.GPIO_Pin = this->pinStr.dataHigh ? 0xff00 : 0x00ff;
    GPIO_Init(this->pinStr.dataPort, &gpio);
}

void Intel8080_Object::sendByte(uint8_t dat)
{
    GPIO_SetBits(this->pinStr.wtPort, this->pinStr.wtPin);

    if (this->pinStr.dataHigh)
    {
        this->pinStr.dataPort->ODR &= 0x00ff;
        this->pinStr.dataPort->ODR |= (uint16_t)dat << 8;
    }
    else
    {
        this->pinStr.dataPort->ODR &= 0xff00;
        this->pinStr.dataPort->ODR |= dat;
    }

    // uint16_t i = this->pinStr.dataHigh ? GPIO_Pin_8 : GPIO_Pin_0;
    // uint8_t cnt = 8;
    // while (cnt--)
    // {
    //     if (dat & 0x01)
    //         GPIO_SetBits(this->pinStr.dataPort, i);
    //     else
    //         GPIO_ResetBits(this->pinStr.dataPort, i);
    //     dat >>= 1;
    //     i <<= 1;
    // }

    GPIO_ResetBits(this->pinStr.wtPort, this->pinStr.wtPin);
    // delayMs(10);
    delay(3);
    GPIO_SetBits(this->pinStr.wtPort, this->pinStr.wtPin);
}
