#ifndef __WY_LIB_CPP_SPI_HPP__
#define __WY_LIB_CPP_SPI_HPP__
#include "common.h"
#include "wy_gpio.hpp"
namespace SPI
{
    typedef struct
    {
        char *miso, *mosi, *sclk;
        uint8_t Spi_Num;
        uint8_t SCLK_AF, MOSI_AF, MISO_AF;
    } HardInitStruct;

    class SPI_Object
    {
    protected:
    private:
        uint16_t SCLK_Pin, MOSI_Pin, MISO_Pin;
        GPIO_TypeDef *SCLK_Port, *MOSI_Port, *MISO_Port;
        SPI_TypeDef *spi;

        GPIO::Gpio_Object *sclk = nullptr, *mosi = nullptr, *miso = nullptr;
        void init(HardInitStruct *h);
        void init(const char *mosi, const char *miso, const char *sclk);
        void init(char mosiPort, uint8_t mosiPin,
                  char misoPort, uint8_t misoPin,
                  char sclkPort, uint8_t sclkPin);

    public:
        // template <typename DataType>
        // void sendData(DataType dat);
        SPI_Object(/* args */) = default;
        SPI_Object(HardInitStruct *h) { init(h); };
        SPI_Object(const char *mosi, const char *miso, const char *sclk)
        {
            init(mosi, miso, sclk);
        }
        SPI_Object(char mosiPort, uint8_t mosiPin,
                   char misoPort, uint8_t misoPin,
                   char sclkPort, uint8_t sclkPin)
        {
            init(mosiPort, mosiPin, misoPort, misoPin, sclkPort, sclkPin);
        }
        void sendOneByte(uint8_t dat);
    };

} // namespace SPI

#endif /* __WY_LIB_CPP_SPI_HPP__ */
