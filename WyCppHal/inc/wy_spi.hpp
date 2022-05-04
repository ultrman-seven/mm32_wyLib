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
        uint16_t DC_Pin, RES_Pin, CS_Pin, SCLK_Pin, MOSI_Pin;

        GPIO_TypeDef *DC_Port, *RES_Port, *CS_Port, *SCLK_Port, *MOSI_Port;

        // uint32_t DC_RCC, CS_RCC, RES_RCC, SCLK_RCC, MOSI_RCC;
        // vector<uint32_t>* GPIO_RCCs;
        uint8_t Spi_Num;
        // uint8_t SCLK_PinSource, MOSI_PinSource;
        uint8_t SCLK_AF, MOSI_AF;
    } HardInitStruct;

    class SPI_Object
    {
    private:
    protected:
        uint16_t DC_Pin, RES_Pin, CS_Pin, SCLK_Pin, MOSI_Pin;
        GPIO_TypeDef *DC_Port, *RES_Port, *CS_Port, *SCLK_Port, *MOSI_Port;
        SPI_TypeDef *spi;
        void init(HardInitStruct *h);
        void init(SoftInitStruct *s);
        void sendOneByte(uint8_t dat);
        template <typename DataType>
        void sendData(DataType dat);
        // void sendData(uint8_t dat) {sendOneByte(dat);}
        // void sendData(uint16_t dat);
        // void sendData(uint32_t dat);

    public:
        SPI_Object(/* args */) = default;
        SPI_Object(HardInitStruct *h) { init(h); };
        SPI_Object(SoftInitStruct *s) { init(s); };

        ~SPI_Object();
    };

} // namespace SPI

#endif /* __WY_LIB_CPP_SPI_HPP__ */
