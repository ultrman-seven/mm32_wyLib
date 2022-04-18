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
    class OLED_Object : public SPI::SPI_Object, public genO::generalOutputBase
    {
    private:
        void sendByte(uint8_t dat, bool isCMD);
        void setCol(uint8_t col);
        void setPage(uint8_t page);
        void char_display(const uint8_t *ptr_pic, bool contrast);

    public:
        char str[64] = {0};
        OLED_Object(/* args */) = default;
        OLED_Object(SPI::HardInitStruct *hard)
        {
            init(hard);
            reset();
        };
        OLED_Object(SPI::SoftInitStruct *soft)
        {
            init(soft);
            reset();
        };
        void reInit(SPI::HardInitStruct *hard) { init(hard); };
        void reInit(SPI::SoftInitStruct *soft) { init(soft); };
        void printStr(void) {print(str);}
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
