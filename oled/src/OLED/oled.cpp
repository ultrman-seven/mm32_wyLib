#include "oled.hpp"
#include "common.h"
using namespace OLED;

void OLED_Object::sendByte(uint8_t dat, bool isCMD)
{

    GPIO_ResetBits(this->CS_Port, this->CS_Pin);

    if (isCMD)
        GPIO_ResetBits(this->DC_Port, this->DC_Pin);
    else
        GPIO_SetBits(this->DC_Port, this->DC_Pin);

    if (this->spi != nullptr)
        SPI_SendData(this->spi, dat);
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
    GPIO_SetBits(this->CS_Port, this->CS_Pin);
}

void OLED_Object::reset(void)
{
    uint8_t rstCmd[] = {0xAE, 0x02, 0x10, 0x40,
                        0xB0, 0x81, 0xFF, 0xA1, 0xA4, 0xA6, 0xC8, 0xA8,
                        0x3F, 0xD5, 0x80, 0xD3, 0x00, 0xAD, 0x8B, 0xDA,
                        0x12, 0xDB, 0x40, 0xD9, 0xF1, 0xAF};
    uint8_t cnt;
    GPIO_ResetBits(this->RES_Port, this->RES_Pin);
    delayMs(100);
    GPIO_SetBits(this->RES_Port, this->RES_Pin);
    for (cnt = 0; cnt < 26; cnt++)
        this->sendByte(rstCmd[cnt], true);
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

void OLED_Object::char_display(const uint8_t *ptr_pic, bool contrast)
{
    uint8_t page, column;
    for (page = line; page < line + (asciiHigh / 8); page++) // page loop
    {
        this->setCol(wordCount * asciiWide);
        setPage(page);
        for (column = 0; column < asciiWide; column++) // column loop
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
