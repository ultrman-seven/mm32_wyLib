#include "common.h"
#include "generalO.hpp"
#include "stdlib.h"
using namespace genO;

uint16_t utf8ToUnicode(uint8_t *c)
{
    uint16_t result = 0;
    uint8_t tmp;
    result += c[2] & 0x3f;
    tmp = c[1] & 0x3f;
    result += tmp << 6;
    tmp = c[0] & 0x0f;
    result += tmp << 12;
    return result;
}
#define ZhStart 0x3400
#define MidLen 0x19b5
#define Zh2Start 0x4e00
void generalOutputBase::putChar(char ch)
{
    if (wordCount >= maxWord)
    {
        wordCount = 0;
        line += asciiHigh / 8;
    }
    if (line >= maxLine)
        return;
    if (this->zhCnt)
    {
        this->zhBuf[this->zhCnt++] = (uint8_t)ch;
        if (this->zhCnt == 3)
        {
            this->zhCnt = 0;
            uint16_t zh = utf8ToUnicode(this->zhBuf);
            uint8_t zhSize = zh_High * zh_Wide / 8;
            uint8_t *pic = (uint8_t *)calloc(zhSize, sizeof(uint8_t));
            if (zh - ZhStart > MidLen)
                this->loadZH((MidLen + zh - Zh2Start + 1) * zhSize, pic, zhSize);
            else
                this->loadZH((zh - ZhStart) * zhSize, pic, zhSize);
            char_display(pic, chooseLine == line, this->line, this->wordCount, zh_High, zh_Wide);
            this->wordCount+=2;
            free(pic);
        }
        return;
    }
    if (((uint8_t)ch & 0xf0) == 0xe0)
    {
        if (this->loadZH != nullptr)
            this->zhBuf[this->zhCnt++] = (uint8_t)ch;
        return;
    }
    switch (ch)
    {
    case '\n':
        line += asciiHigh / 8;
        wordCount = 0;
        return;
    case '\b':
        wordCount--;
        char_display(font, chooseLine == line);
        return;
    case '\a':
        char_display(font, chooseLine == line);
        (this->placeHolder)[line / (asciiHigh / 8)].push_back(wordCount);
        break;
    case 127:
        break;
    default:
        if (ch >= 32)
            char_display(font + fontUnitSize * (ch - 32), chooseLine == line);
        break;
    }
    wordCount++;
}

void generalOutputBase::putNum(__IO int num)
{
    if (num < 0)
    {
        putChar('-');
        num = -num;
    }
    if (num >= 10)
        putNum(num / 10);
    putChar(num % 10 + '0');
}

void generalOutputBase::putNum(__IO int num, uint8_t decimalPlaces)
{
    uint32_t count;
    if (num < 0)
    {
        putChar('-');
        num = -num;
    }
    while (decimalPlaces--)
        count *= 10;
    if (num > count)
    {
        putNum(num / count);
        putChar('.');
        putNum(num % count);
    }
    else
        putNum(num);
}

void generalOutputBase::print(char *s)
{
    while (*s)
        putChar(*s++);
}

void generalOutputBase::loadFont(const uint8_t *font, uint8_t asciiHigh, uint8_t asciiWide)
{
    this->font = font;
    this->asciiHigh = asciiHigh;
    this->asciiWide = asciiWide;
    this->fontUnitSize = asciiWide * asciiHigh / 8;
    this->chooseLine.setStep(asciiHigh / 8);
}

void generalOutputBase::loadZH_Font(void (*f)(uint32_t add, uint8_t *buf, uint16_t len), uint8_t h, uint8_t w)
{
    this->loadZH = f;
    this->zh_High = h;
    this->zh_Wide = w;
}

void generalOutputBase::setScreenSize(uint8_t width, uint8_t height)
{
    maxWord = width / asciiWide;
    // maxLine = height / asciiHigh;
    maxLine = height / 8;
    this->placeHolder.resize(maxLine);
    // this->placeHolder = new std::vector<std::vector<uint8_t> >(maxLine);
    for (auto &i : this->placeHolder)
        i = std::vector<uint8_t>(0);
};

void generalOutputBase::clearPlaceHolder(void)
{
    for (auto i : this->placeHolder)
        i.clear();
}

void generalOutputBase::placeFill(char *s)
{
    uint8_t l = 0;
    while (l <= maxLine / (asciiHigh / 8))
    {
        for (auto w : (this->placeHolder[l]))
            char_display(font + fontUnitSize * (*s++ - 32), chooseLine == line, l * (asciiHigh / 8), w);
        l++;
    }
}
