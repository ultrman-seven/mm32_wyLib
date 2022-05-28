#include "wy_spi.hpp"

using namespace SPI;
void SPI_Object::init(HardInitStruct *hard)
{
    GPIO::afConfig(hard->miso, hard->MISO_AF, GPIO_Mode_IPU, GPIO_Speed_50MHz);
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

    SPI_InitTypeDef SPI_InitStructure;

    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_DataWidth = SPI_DataWidth_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    //	SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_Init(this->spi, &SPI_InitStructure);

    SPI_Cmd(this->spi, ENABLE);
    SPI_BiDirectionalLineConfig(this->spi, SPI_Direction_Tx);
    SPI_BiDirectionalLineConfig(this->spi, SPI_Direction_Rx);
}

void SPI_Object::init(char mosiPort, uint8_t mosiPin, char misoPort, uint8_t misoPin, char sclkPort, uint8_t sclkPin)
{
    this->mosi = new GPIO::Gpio_Object(mosiPort, mosiPin, true);
    this->miso = new GPIO::Gpio_Object(misoPort, misoPin, true, GPIO_Mode_IPU);
    this->sclk = new GPIO::Gpio_Object(sclkPort, sclkPin, true);
    this->spi = nullptr;
}

void SPI_Object::init(const char *mosi, const char *miso, const char *sclk)
{
    if (mosi != nullptr)
        this->mosi = new GPIO::Gpio_Object(mosi);
    if (miso != nullptr)
        this->miso = new GPIO::Gpio_Object(miso, true, GPIO_Mode_IPU);
    this->sclk = new GPIO::Gpio_Object(sclk);
    this->spi = nullptr;
}

//之后用模板重写
void SPI_Object::sendOneByte(uint8_t dat)
{
    if (this->spi != nullptr)
    {
        // SPI_SendData(this->spi, (uint32_t)dat);
        this->spi->TXREG = dat;
        while (!SPI_GetFlagStatus(this->spi, SPI_FLAG_TXEPT))
            ;
        dat = this->spi->RXREG;
    }
    else
    {
        uint8_t cnt;
        for (cnt = 0; cnt < 8; cnt++)
        {
            this->sclk->reset();
            *this->mosi = ((dat & 0x80) >> 7);
            dat = dat << 1;
            delay(3);
            this->sclk->set();
            delay(3);
            this->sclk->reset();
        }
    }

    // delay(2);
}

uint8_t SPI_Object::read(void)
{
    if (this->spi != nullptr)
        return SPI_ReceiveData(this->spi);
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
            this->sclk->reset();
            this->mosi = (dat & 0x8000) >> (s - 1);
            dat = dat << 1;
            delay(3);
            this->sclk->set();
            delay(3);
            this->sclk->reset();
        }
    }
    delay(2);
}
#endif
uint32_t SPI_Object::writeRead(uint8_t dat)
{

    // SPI_SendData(this->spi, dat);
    this->spi->TXREG = dat;
    while (!(this->spi->CSTAT & SPI_FLAG_TXEPT))
        ;
    while (!(this->spi->CSTAT & SPI_CSTAT_RXAVL))
        ;
    return (u32)this->spi->RXREG;
}
