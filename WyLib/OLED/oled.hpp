#ifndef __WY_LIB_CPP_OLED_H__
#define __WY_LIB_CPP_OLED_H__
#include "common.h"
#include "wy_spi.hpp"
#include "generalO.hpp"
#include <stdio.h>

// #include<vector>
// using std::vector;
namespace OLED
{
    class OLED_Object : public genO::generalOutputBase //,public SPI::SPI_Object
    {
    private:
        void sendByte(uint8_t dat, bool isCMD);
        void setCol(uint8_t col);
        void setPage(uint8_t page);
        void char_display(const uint8_t *ptr_pic, bool contrast, uint8_t l, uint8_t word);
        void (*transFunc)(uint8_t dat) = nullptr;
        GPIO_TypeDef *DC_Port, *CS_Port, *RES_Port;
        uint16_t DC_Pin, CS_Pin, RES_Pin;

    public:
        char str[64] = {0};
        OLED_Object(/* args */) = default;
        OLED_Object(GPIO_TypeDef *port, uint16_t dc, uint16_t cs, uint16_t res)
        {
            this->reInit(port, dc, port, cs, port, res, nullptr);
        }
        OLED_Object(GPIO_TypeDef *dcPort, uint16_t dcPin,
                    GPIO_TypeDef *csPort, uint16_t csPin,
                    GPIO_TypeDef *resPort, uint16_t resPin, void (*f)(uint8_t))
        {
            this->reInit(dcPort, dcPin, csPort, csPin, resPort, resPin, f);
        }
        void reInit(GPIO_TypeDef *dcPort, uint16_t dcPin,
                    GPIO_TypeDef *csPort, uint16_t csPin,
                    GPIO_TypeDef *resPort, uint16_t resPin, void (*f)(uint8_t));
        void loadTransFun(void (*f)(uint8_t)) { this->transFunc = f; }
        void printStr(void) { print(str); }
        void reset(void);
        void clear(void);
        void Picture_display(uint8_t *ptr_pic, uint8_t colStart, uint8_t pageStart, uint8_t line, uint8_t col);
        void Picture_display(const uint8_t *ptr_pic, uint8_t colStart, uint8_t pageStart, uint8_t line, uint8_t col);
        void Screen_FillClear(uint8_t filler);
        void reverse(bool on);
        void setBrightness(uint8_t val);
    };
}
#endif /* __WY_LIB_CPP_OLED_H__ */
