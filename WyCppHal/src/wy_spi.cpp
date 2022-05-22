#include "wy_spi.hpp"

using namespace SPI;

void SPI_Object::init(HardInitStruct *hard)
{
    SPI_InitTypeDef spi;

    GPIO::afConfig(hard->miso, hard->MISO_AF, GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz);
    GPIO::afConfig(hard->mosi, hard->MOSI_AF, GPIO_Mode_AF_PP, GPIO_Speed_50MHz);
    GPIO::afConfig(hard->sclk, hard->SCLK_AF, GPIO_Mode_AF_PP, GPIO_Speed_50MHz);

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
    }

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
}

void SPI_Object::init(char mosiPort, uint8_t mosiPin, char misoPort, uint8_t misoPin, char sclkPort, uint8_t sclkPin)
{
    this->mosi = new GPIO::Gpio_Object(mosiPort, mosiPin, true);
    this->miso = new GPIO::Gpio_Object(misoPort, misoPin, true, GPIO_Mode_IN_FLOATING);
    this->sclk = new GPIO::Gpio_Object(sclkPort, sclkPin, true);
    this->spi = nullptr;
}

void SPI_Object::init(const char *mosi, const char *miso, const char *sclk)
{
    if (mosi != nullptr)
        this->mosi = new GPIO::Gpio_Object(mosi);
    if (miso != nullptr)
        this->miso = new GPIO::Gpio_Object(miso, true, GPIO_Mode_IN_FLOATING);
    this->sclk = new GPIO::Gpio_Object(sclk);
    this->spi = nullptr;
}

//之后用模板重写
void SPI_Object::sendOneByte(uint8_t dat)
{
    if (this->spi != nullptr)
    {
        SPI_SendData(this->spi, (uint32_t)dat);
        // while (!SPI_GetFlagStatus(this->spi, SPI_FLAG_TXEPT))
        //     ;
    }
    else
    {
        uint8_t cnt;
        for (cnt = 0; cnt < 8; cnt++)
        {
            this->sclk->reset();
            *(this->mosi) = ((dat & 0x80) >> 7);
            if ((dat & 0x80) >> 7)
                this->mosi->set();
            else
                this->mosi->reset();
            dat = dat << 1;
            delay(3);
            this->sclk->set();
            delay(3);
            this->sclk->reset();
        }
    }

    // delay(2);
}

#if 0
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
#endif
