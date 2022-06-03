#include "oled.hpp"
#include "common.h"
using namespace OLED;

void OLED_Object::sendByte(uint8_t dat, bool isCMD)
{
    cs->reset();
    dc->setOnOff(!isCMD);
    if (this->transFunc != nullptr)
        this->transFunc(dat);
    cs->set();
}

#define Brightness
void OLED_Object::reset(void)
{
    // const uint8_t rstCmd[] = {0xAE, 0x02, 0x10, 0x40,
    //                           0xB0, 0x81, 0xFF, 0xA1, 0xA4, 0xA6, 0xC8, 0xA8,
    //                           0x3F, 0xD5, 0x80, 0xD3, 0x00, 0xAD, 0x8B, 0xDA,
    //                           0x12, 0xDB, 0x40, 0xD9, 0xF1, 0xAF};
    const uint8_t rstCmd[] = {0xae, 0x00, 0x10, 0x40, 0x81,
                              0x7f, 0xa1, 0xc8, 0xa6, 0xa8, 0x3f, 0xd3,
                              0x00, 0xd5, 0x80, 0xd9, 0xf1, 0xda, 0x12, 0xdb,
                              0x40, 0x20, 0x02, 0x8d, 0x14, 0xa4, 0xa6, 0xaf};
    this->cs->set();
    this->res->reset();
    delayMs(100);
    this->res->set();
    for (auto cmd : rstCmd)
        this->sendByte(cmd, true);
    this->clear();
}

void OLED_Object::setCol(uint8_t col)
{
    this->sendByte(0x10 | (col >> 4), true);
    this->sendByte(0x00 | (col & 0x0f), true);
}

void OLED_Object::setPage(uint8_t page)
{
    this->sendByte(0xb0 + page, true);
}

void OLED_Object::Screen_FillClear(uint8_t filler)
{
    uint8_t page, col;
    for (page = 0; page < 8; page++)
    {
        this->setPage(page);
        this->setCol(0);
        for (col = 0; col < 128; col++)
            this->sendByte(filler, false);
    }
}

void OLED_Object::Picture_display(uint8_t *ptr_pic, uint8_t colStart, uint8_t pageStart, uint8_t line, uint8_t col)
{
    uint8_t page, column;

    for (page = pageStart; page < pageStart + (line / 8); page++) // page loop
    {
        this->setCol(colStart);
        setPage(page);
        for (column = 0; column < col; column++) // column loop
            this->sendByte(*ptr_pic++, false);
    }
}
void OLED_Object::Picture_display(const uint8_t *ptr_pic, uint8_t colStart, uint8_t pageStart, uint8_t line, uint8_t col)
{
    uint8_t page, column;

    for (page = pageStart; page < pageStart + (line / 8); page++) // page loop
    {
        this->setCol(colStart);
        setPage(page);
        for (column = 0; column < col; column++) // column loop
            this->sendByte(*ptr_pic++, false);
    }
}

void OLED_Object::char_display(const uint8_t *ptr_pic, bool contrast, uint8_t l, uint8_t word, uint8_t h, uint8_t w)
{
    uint8_t page, column;
    for (page = l; page < l + (h / 8); page++) // page loop
    {
        this->setCol(word * this->asciiWide);
        setPage(page);
        for (column = 0; column < w; column++) // column loop
            if (contrast)
                this->sendByte(~*ptr_pic++, false);
            else
                this->sendByte(*ptr_pic++, false);
    }
}
void OLED_Object::char_display(uint8_t *ptr_pic, bool contrast, uint8_t l, uint8_t word, uint8_t h, uint8_t w)
{
    uint8_t page, column;
    for (page = l; page < l + (h / 8); page++) // page loop
    {
        this->setCol(word * this->asciiWide);
        setPage(page);
        for (column = 0; column < w; column++) // column loop
            if (contrast)
                this->sendByte(~*ptr_pic++, false);
            else
                this->sendByte(*ptr_pic++, false);
    }
}

void OLED_Object::clear(void)
{
    Screen_FillClear(0x00);
    wordCount = line = 0;
}

void OLED_Object::reverse(bool on)
{
    sendByte(0xa6 + on, true);
}

void OLED_Object::setBrightness(uint8_t val)
{
    sendByte(0x81, true);
    sendByte(val, true);
}
