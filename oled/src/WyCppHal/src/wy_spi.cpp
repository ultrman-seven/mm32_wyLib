#include "wy_spi.hpp"

using namespace SPI;

// SPI_Object::SPI_Object(/* args */) {}

SPI_Object::SPI_Object(HardInitStruct *h) { init(h); }
SPI_Object::SPI_Object(SoftInitStruct *s) { init(s); }

SPI_Object::~SPI_Object() {}

void SPI_Object::init(HardInitStruct *hard)
{
    GPIO_InitTypeDef gpio;
    SPI_InitTypeDef spi;
    if (hard->Spi_Num == 1)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
        this->spi = SPI1;
    }
    if (hard->Spi_Num == 2)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
        this->spi = SPI2;
    }

    gpio.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;

    gpio.GPIO_Pin = hard->CS_Pin;
    GPIO_Init(hard->CS_Port, &gpio);
    gpio.GPIO_Pin = hard->DC_Pin;
    GPIO_Init(hard->DC_Port, &gpio);
    gpio.GPIO_Pin = hard->RES_Pin;
    GPIO_Init(hard->RES_Port, &gpio);

    gpio.GPIO_Mode = GPIO_Mode_AF_PP;
    gpio.GPIO_Pin = hard->MOSI_Pin;
    GPIO_Init(hard->MOSI_Port, &gpio);
    gpio.GPIO_Pin = hard->SCLK_Pin;
    GPIO_Init(hard->SCLK_Port, &gpio);

    GPIO_PinAFConfig(hard->SCLK_Port, hard->SCLK_PinSource, hard->SCLK_AF);
    GPIO_PinAFConfig(hard->MOSI_Port, hard->MOSI_PinSource, hard->MOSI_AF);

    spi.SPI_NSS = SPI_NSS_Soft;
    spi.SPI_Mode = SPI_Mode_Master;
    spi.SPI_DataSize = SPI_DataSize_8b;
    spi.SPI_DataWidth = SPI_DataWidth_8b;
    spi.SPI_CPOL = SPI_CPOL_Low;
    spi.SPI_CPHA = SPI_CPHA_1Edge;
    spi.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
    spi.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_Init(this->spi, &spi);
    SPI_Cmd(this->spi, ENABLE);
    SPI_BiDirectionalLineConfig(this->spi, SPI_Direction_Tx);
    SPI_BiDirectionalLineConfig(this->spi, SPI_Direction_Rx);
    this->CS_Pin = hard->CS_Pin;
    this->DC_Pin = hard->DC_Pin;
    this->RES_Pin = hard->RES_Pin;

    this->CS_Port = hard->CS_Port;
    this->DC_Port = hard->DC_Port;
    this->RES_Port = hard->RES_Port;
}

void SPI_Object::init(SoftInitStruct *soft)
{
    GPIO_InitTypeDef gpio;
    this->spi = nullptr;
    gpio.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;

    gpio.GPIO_Pin = soft->CS_Pin;
    GPIO_Init(soft->CS_Port, &gpio);
    gpio.GPIO_Pin = soft->DC_Pin;
    GPIO_Init(soft->DC_Port, &gpio);
    gpio.GPIO_Pin = soft->RES_Pin;
    GPIO_Init(soft->RES_Port, &gpio);
    gpio.GPIO_Pin = soft->SCLK_Pin;
    GPIO_Init(soft->SCLK_Port, &gpio);
    gpio.GPIO_Pin = soft->MOSI_Pin;
    GPIO_Init(soft->MOSI_Port, &gpio);

    this->CS_Pin = soft->CS_Pin;
    this->DC_Pin = soft->DC_Pin;
    this->RES_Pin = soft->RES_Pin;
    this->SCLK_Pin = soft->SCLK_Pin;
    this->MOSI_Pin = soft->MOSI_Pin;

    this->CS_Port = soft->CS_Port;
    this->DC_Port = soft->DC_Port;
    this->RES_Port = soft->RES_Port;
    this->MOSI_Port = soft->MOSI_Port;
    this->SCLK_Port = soft->SCLK_Port;
}

void SPI_Object::sendByte(uint8_t dat)
{
    GPIO_ResetBits(this->CS_Port, this->CS_Pin);

    if (this->spi != nullptr)
        SPI_SendData(this->spi, dat);
    else
    {
        uint8_t cnt;
        for (cnt = 0; cnt < 8; cnt++)
        {
            GPIO_ResetBits(this->SCLK_Port, this->SCLK_Pin);
            if ((dat & 0x80) >> 7)
                GPIO_SetBits(this->MOSI_Port, this->MOSI_Pin);
            else
                GPIO_ResetBits(this->MOSI_Port, this->MOSI_Pin);
            dat = dat << 1;
            delay(3);
            GPIO_SetBits(this->SCLK_Port, this->SCLK_Pin);
            delay(3);
            GPIO_ResetBits(this->SCLK_Port, this->SCLK_Pin);
        }
    }
    delay(2);
    GPIO_SetBits(this->CS_Port, this->CS_Pin);
}
