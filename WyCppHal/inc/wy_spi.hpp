#ifndef __WY_LIB_CPP_SPI_HPP__
#define __WY_LIB_CPP_SPI_HPP__
#include "common.h"
namespace SPI
{

    typedef struct
    {
        uint16_t DC_Pin, RES_Pin, CS_Pin, SCLK_Pin, MOSI_Pin;

        GPIO_TypeDef *DC_Port, *RES_Port, *CS_Port, *SCLK_Port, *MOSI_Port;

        // uint32_t DC_RCC, CS_RCC, RES_RCC, SCLK_RCC, MOSI_RCC;
        // vector<uint32_t>* GPIO_RCCs;

    } SoftInitStruct;

    typedef struct
    {
        uint16_t SCLK_Pin, MOSI_Pin;

        GPIO_TypeDef *SCLK_Port, *MOSI_Port;

        // uint32_t DC_RCC, CS_RCC, RES_RCC, SCLK_RCC, MOSI_RCC;
        // vector<uint32_t>* GPIO_RCCs;
        uint8_t Spi_Num;
        // uint8_t SCLK_PinSource, MOSI_PinSource;
        uint8_t SCLK_AF, MOSI_AF;
    } HardInitStruct;

    class SPI_Object
    {
    protected:
    private:
        uint16_t SCLK_Pin, MOSI_Pin;
        GPIO_TypeDef *SCLK_Port, *MOSI_Port;
        SPI_TypeDef *spi;
        void init(HardInitStruct *h);
        void init(SoftInitStruct *s);
        void init(GPIO_TypeDef *mosiPort, uint16_t mosiPin, GPIO_TypeDef *sclkPort, uint16_t sclkPin);
        // void sendData(uint8_t dat) {sendOneByte(dat);}
        // void sendData(uint16_t dat);
        // void sendData(uint32_t dat);

    public:
        // template <typename DataType>
        // void sendData(DataType dat);
        SPI_Object(/* args */) = default;
        SPI_Object(HardInitStruct *h) { init(h); };
        SPI_Object(SoftInitStruct *s) { init(s); };
        SPI_Object(GPIO_TypeDef *mosiPort, uint16_t mosiPin,
                   GPIO_TypeDef *sclkPort, uint16_t sclkPin)
        {
            init(mosiPort, mosiPin, sclkPort, sclkPin);
        };
        SPI_Object(GPIO_TypeDef *port, uint16_t mosiPin, uint16_t sclkPin)
        {
            init(port, mosiPin, port, sclkPin);
        };

        void sendOneByte(uint8_t dat);
    };

} // namespace SPI

#endif /* __WY_LIB_CPP_SPI_HPP__ */
