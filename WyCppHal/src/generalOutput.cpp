#include "common.h"
#include "generalO.hpp"
using namespace genO;

void generalOutputBase::putChar(char ch)
{
    if (wordCount >= maxWord)
    {
        wordCount = 0;
        line += asciiHigh / 8;
    }
    if (line >= maxLine)
        return;
    switch (ch)
    {
    case '\n':
        line += asciiHigh / 8;
        wordCount = 0;
        return;
    case '\b':
        wordCount--;
        break;
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
    while (l <= maxLine/(asciiHigh / 8))
    {
        for (auto w : (this->placeHolder[l]))
            char_display(font + fontUnitSize * (*s++ - 32), chooseLine == line, l*(asciiHigh / 8), w);
        l ++;
    }
}
