#include "wy_spi.hpp"

using namespace SPI;

SPI_Object::~SPI_Object() {}

void SPI_Object::init(HardInitStruct *hard)
{
    GPIO_InitTypeDef gpio;
    SPI_InitTypeDef spi;

    switch (hard->Spi_Num)
    {
    case 1:
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
        this->spi = SPI1;
        break;
    case 2:
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
        this->spi = SPI2;
        break;

    default:
        return;
        break;
    }


    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_Mode = GPIO_Mode_AF_PP;
    gpio.GPIO_Pin = hard->MOSI_Pin;
    GPIO_Init(hard->MOSI_Port, &gpio);
    gpio.GPIO_Pin = hard->SCLK_Pin;
    GPIO_Init(hard->SCLK_Port, &gpio);

    uint8_t SCLK_PinSource = pin2pinSource(SCLK_Pin);
    uint8_t MOSI_PinSource = pin2pinSource(MOSI_Pin);

    GPIO_PinAFConfig(hard->SCLK_Port, SCLK_PinSource, hard->SCLK_AF);
    GPIO_PinAFConfig(hard->MOSI_Port, MOSI_PinSource, hard->MOSI_AF);

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
    SPI_BiDirectionalLineConfig(this->spi, SPI_Direction_Rx);}

void SPI_Object::init(SoftInitStruct *soft)
{
    GPIO_InitTypeDef gpio;
    this->spi = nullptr;
    gpio.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;

    gpio.GPIO_Pin = soft->SCLK_Pin;
    GPIO_Init(soft->SCLK_Port, &gpio);
    gpio.GPIO_Pin = soft->MOSI_Pin;
    GPIO_Init(soft->MOSI_Port, &gpio);

    this->SCLK_Pin = soft->SCLK_Pin;
    this->MOSI_Pin = soft->MOSI_Pin;

    this->MOSI_Port = soft->MOSI_Port;
    this->SCLK_Port = soft->SCLK_Port;
}
void SPI_Object::init(GPIO_TypeDef* mosiPort,uint16_t mosiPin,GPIO_TypeDef*sclkPort,uint16_t sclkPin)
{
    GPIO_InitTypeDef gpio;
    this->spi = nullptr;
    gpio.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;

    gpio.GPIO_Pin = sclkPin;
    GPIO_Init(sclkPort, &gpio);
    gpio.GPIO_Pin = mosiPin;
    GPIO_Init(mosiPort, &gpio);

    this->SCLK_Pin = sclkPin;
    this->MOSI_Pin = mosiPin;

    this->SCLK_Port = sclkPort;
    this->MOSI_Port = mosiPort;
}

//之后用模板重写
void SPI_Object::sendOneByte(uint8_t dat)
{
    if (this->spi != nullptr)
    {
        SPI_SendData(this->spi, dat);
        while (!SPI_GetFlagStatus(this->spi, SPI_FLAG_TXEPT))
            ;
    }
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
}

template <typename DataType>
void SPI_Object::sendData(DataType dat)
{
    uint8_t s = sizeof(dat);

    if (this->spi != nullptr)
    {
        uint8_t tmp;
        while (s--)
        {
            /* code */
            tmp = (dat >> (8 * s)) & 0xff;
            SPI_SendData(this->spi, tmp);
            while (!SPI_GetFlagStatus(this->spi, SPI_FLAG_TXEPT))
                ;
        }
    }
    else
    {
        uint8_t cnt;
        s *= 8;
        for (cnt = 0; cnt < s; cnt++)
        {
            GPIO_ResetBits(this->SCLK_Port, this->SCLK_Pin);
            if ((dat & 0x8000) >> s - 1)
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
}
