#include "tft.hpp"
using namespace SPI;
using namespace genO;
using namespace TFT;

void Tft::char_display(const uint8_t *ptr_pic, bool contrast) {}

void Tft::sendByte(uint8_t dat, bool isCMD)
{
    if (isCMD)
        GPIO_ResetBits(this->DC_Port, this->DC_Pin);
    else
        GPIO_SetBits(this->DC_Port, this->DC_Pin);
    this->sendOneByte(dat);
}

void Tft::reset(void)
{

    GPIO_ResetBits(this->RES_Port, this->RES_Pin);
    delayMs(100);
    GPIO_SetBits(this->RES_Port, this->RES_Pin);
    delayMs(100);
    sendByte(0x11, true);
    delayMs(100);

    // ST7735R Frame Rate
    Lcd_WriteIndex(0xB1);
    Lcd_WriteData(0x01);
    Lcd_WriteData(0x2C);
    Lcd_WriteData(0x2D);

    Lcd_WriteIndex(0xB2);
    Lcd_WriteData(0x01);
    Lcd_WriteData(0x2C);
    Lcd_WriteData(0x2D);

    Lcd_WriteIndex(0xB3);
    Lcd_WriteData(0x01);
    Lcd_WriteData(0x2C);
    Lcd_WriteData(0x2D);
    Lcd_WriteData(0x01);
    Lcd_WriteData(0x2C);
    Lcd_WriteData(0x2D);

    Lcd_WriteIndex(0xB4); // Column inversion
    Lcd_WriteData(0x07);

    // ST7735R Power Sequence
    Lcd_WriteIndex(0xC0);
    Lcd_WriteData(0xA2);
    Lcd_WriteData(0x02);
    Lcd_WriteData(0x84);
    Lcd_WriteIndex(0xC1);
    Lcd_WriteData(0xC5);

    Lcd_WriteIndex(0xC2);
    Lcd_WriteData(0x0A);
    Lcd_WriteData(0x00);

    Lcd_WriteIndex(0xC3);
    Lcd_WriteData(0x8A);
    Lcd_WriteData(0x2A);
    Lcd_WriteIndex(0xC4);
    Lcd_WriteData(0x8A);
    Lcd_WriteData(0xEE);

    Lcd_WriteIndex(0xC5); // VCOM
    Lcd_WriteData(0x0E);

    Lcd_WriteIndex(0x36); // MX, MY, RGB mode
    Lcd_WriteData(0xC0);

    Lcd_WriteData(0xC8); //竖屏C8 横屏08 A8

    // ST7735R Gamma Sequence
    Lcd_WriteIndex(0xe0);
    Lcd_WriteData(0x0f);
    Lcd_WriteData(0x1a);
    Lcd_WriteData(0x0f);
    Lcd_WriteData(0x18);
    Lcd_WriteData(0x2f);
    Lcd_WriteData(0x28);
    Lcd_WriteData(0x20);
    Lcd_WriteData(0x22);
    Lcd_WriteData(0x1f);
    Lcd_WriteData(0x1b);
    Lcd_WriteData(0x23);
    Lcd_WriteData(0x37);
    Lcd_WriteData(0x00);
    Lcd_WriteData(0x07);
    Lcd_WriteData(0x02);
    Lcd_WriteData(0x10);

    Lcd_WriteIndex(0xe1);
    Lcd_WriteData(0x0f);
    Lcd_WriteData(0x1b);
    Lcd_WriteData(0x0f);
    Lcd_WriteData(0x17);
    Lcd_WriteData(0x33);
    Lcd_WriteData(0x2c);
    Lcd_WriteData(0x29);
    Lcd_WriteData(0x2e);
    Lcd_WriteData(0x30);
    Lcd_WriteData(0x30);
    Lcd_WriteData(0x39);
    Lcd_WriteData(0x3f);
    Lcd_WriteData(0x00);
    Lcd_WriteData(0x07);
    Lcd_WriteData(0x03);
    Lcd_WriteData(0x10);

    Lcd_WriteIndex(0x2a);
    Lcd_WriteData(0x00);
    Lcd_WriteData(0x00 + 2);
    Lcd_WriteData(0x00);
    Lcd_WriteData(0x80 + 2);

    Lcd_WriteIndex(0x2b);
    Lcd_WriteData(0x00);
    Lcd_WriteData(0x00 + 3);
    Lcd_WriteData(0x00);
    Lcd_WriteData(0x80 + 3);

    Lcd_WriteIndex(0xF0); // Enable test command
    Lcd_WriteData(0x01);
    Lcd_WriteIndex(0xF6); // Disable ram power save mode
    Lcd_WriteData(0x00);

    Lcd_WriteIndex(0x3A); // 65k mode
    Lcd_WriteData(0x05);

    Lcd_WriteIndex(0x29); // Display on
}

void Tft::Lcd_SetRegion(uint8_t x_start, uint8_t y_start, uint8_t x_end, uint8_t y_end)
{
#ifdef USE_LANDSCAPE //使用横屏模式
    Lcd_WriteIndex(0x2a);
    Lcd_WriteData(0x02);
    Lcd_WriteData(x_start + 2);
    Lcd_WriteData(0x02);
    Lcd_WriteData(x_end + 2);

    Lcd_WriteIndex(0x2b);
    Lcd_WriteData(0x01);
    Lcd_WriteData(y_start + 1);
    Lcd_WriteData(0x01);
    Lcd_WriteData(y_end + 1);

#else //竖屏模式
    Lcd_WriteIndex(0x2a);
    Lcd_WriteData(0x02);
    Lcd_WriteData(x_start + 2);
    Lcd_WriteData(0x02);
    Lcd_WriteData(x_end + 2);

    Lcd_WriteIndex(0x2b);
    Lcd_WriteData(0x01);
    Lcd_WriteData(y_start + 1);
    Lcd_WriteData(0x01);
    Lcd_WriteData(y_end + 1);
#endif
    Lcd_WriteIndex(0x2c);
}

void Tft::dsp_single_colour(uint16_t color)
{
    uint8_t i, j;
    Lcd_SetRegion(0, 0, 128 - 1, 160 - 1);
    for (i = 0; i < 160; i++)
        for (j = 0; j < 128; j++)
            LCD_WriteData_16Bit(color);
}
//定义常用颜色
#define RED 0xf800
#define GREEN 0x07e0
#define BLUE 0x001f
#define WHITE 0xffff
#define BLACK 0x0000
#define YELLOW 0xFFE0
#define GRAY0 0xEF7D
#define GRAY1 0x8410
#define GRAY2 0x4208
void Tft::showimage(const uint8_t *p) //显示40*40 QQ图片
{
    int i=0, j=0, k=0;
    uint8_t picH, picL;
    dsp_single_colour(WHITE); //清屏
    // for (k = 0; k < 4; k++)
    //     for (j = 0; j < 3; j++)
    //     {
            // Lcd_SetRegion(40 * j, 40 * k, 40 * j + 39, 40 * k + 39); //坐标设置
            for (i = 0; i < 40 * 40; i++)
            {
                picL = *(p + i * 2); //数据低位在前
                picH = *(p + i * 2 + 1);
                LCD_WriteData_16Bit(picH << 8 | picL);
            }
        // }
}
void Tft::clear(void)
{
    dsp_single_colour(WHITE);
}