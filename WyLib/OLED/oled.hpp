#ifndef __WY_LIB_CPP_OLED_H__
#define __WY_LIB_CPP_OLED_H__
#include "common.h"
#include "wy_gpio.hpp"
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
        void char_display(const uint8_t *ptr_pic, bool contrast, uint8_t l, uint8_t word,uint8_t h,uint8_t w);
        void char_display(uint8_t *ptr_pic, bool contrast, uint8_t l, uint8_t word,uint8_t h,uint8_t w);
        void (*transFunc)(uint8_t dat) = nullptr;
        GPIO_TypeDef *DC_Port, *CS_Port, *RES_Port;
        uint16_t DC_Pin, CS_Pin, RES_Pin;
        GPIO::Gpio_Object *dc = nullptr, *cs = nullptr, *res = nullptr;

    public:
        char str[64] = {0};
        OLED_Object() = default;
        OLED_Object(const char *dc, const char *cs, const char *res, void (*f)(uint8_t) = nullptr)
        {
            this->cs = new GPIO::Gpio_Object(cs);
            this->dc = new GPIO::Gpio_Object(dc);
            this->res = new GPIO::Gpio_Object(res);
            this->transFunc = f;
            this->reset();
        }
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
