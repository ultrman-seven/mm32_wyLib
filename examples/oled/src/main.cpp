#include "common.h"
#include "oled.hpp"
#include "wy_spi.hpp"
#include "wy_uart.hpp"
#include "font.hpp"
#include "wy_gpio.hpp"
#include <stdio.h>
#include <vector>
#include "wy_key.hpp"
#include "w25q.hpp"
//#include "Dense"

OLED::OLED_Object *s = nullptr;
ErrorStatus HSE_SysClock(void)
{
    ErrorStatus HSE_StartUpState = ERROR;
    RCC_DeInit();
    RCC_HSEConfig(RCC_HSE_ON);

    HSE_StartUpState = RCC_WaitForHSEStartUp();
    if (HSE_StartUpState == SUCCESS)
    {
        RCC_HCLKConfig(RCC_SYSCLK_Div1);
        RCC_PCLK1Config(RCC_HCLK_Div2);
        RCC_PCLK2Config(RCC_HCLK_Div1);
        //外部晶振16MHz，6倍频成96MHz
        RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_6);
        RCC_PLLCmd(ENABLE);
        while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
            ;
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
        while (RCC_GetSYSCLKSource() != 0x08)
            ;
    }
    return HSE_StartUpState;
}
UART::UART_Object *uart = nullptr;
void p_re(char ch)
{
    // s->putChar(ch);
    uart->sendByte(ch);
}

SPI::SPI_Object *spi4Oled = nullptr;
void spiSend(uint8_t dat)
{
    // spi4Oled->sendOneByte(dat);
    spi4Oled->writeRead(dat);
}

uint8_t pic[32] = {0};
uint8_t word = 0;
uint8_t page = 0;
void pis_s(void)
{
    s->Picture_display(pic, 16 * word++, page, 16, 16);
    if (word == 8)
    {
        word = 0;
        page += 2;
    }
    if (page == 8)
    {
        page = word = 0;
        s->clear();
    }
    // s->print("\nok ok");
}

w25q::W25Q *q64 = nullptr;
uint8_t rxBuf[4096];
uint16_t secCnt = 0;
void datTrans(void)
{
    // q64->writeSector(secCnt++, rxBuf, 4096);
    // s->clear();
    // s->print("ok");
}

uint32_t chAdd = 0;
void showChar(void)
{
    q64->read(chAdd, pic, 32);
    chAdd += 32;
    pis_s();
}

void getZH(uint32_t add,uint8_t*buf,uint16_t len)
{
    q64->read(add, buf, len);
}

GPIO::Gpio_Object beep("a8");
void beepFlip(void)
{
    beep = !(beep.read());
}

// u4e00-u9fa5 (中文)
// (0x3400, 0x4DB5)
// #define ZhStart 0x3400
// #define MidLen 0x19b5
// #define Zh2Start 0x4e00
// uint16_t utf8ToUnicode(const char *c)
// {
//     uint16_t result = 0;
//     uint8_t tmp;
//     result += (uint8_t)c[2] & 0x3f;
//     tmp = (uint8_t)c[1] & 0x3f;
//     result += tmp << 6;
//     tmp = (uint8_t)c[0] & 0x0f;
//     result += tmp << 12;
//     return result;
// }
// void printZh(const char *c)
// {
//     // u16_split zh;
//     uint16_t zh;
//     while (*c != 0)
//     {
//         // zh.unit[1] = *c++;
//         // zh.unit[0] = *c++;
//         zh = utf8ToUnicode(c);
//         printf("0x%x\r\n", zh);
//         c += 3;
//         if (zh - ZhStart > MidLen)
//             q64->read((MidLen + zh - Zh2Start + 1) * 32, pic, 32);
//         else
//             q64->read((zh - ZhStart) * 32, pic, 32);
//         pis_s();
//         // printf("0x%x\r\n", *c++);
//     }
// }

void main(void)
{
    ErrorStatus e = ERROR;
    e = HSE_SysClock();
    sys::redirect_Printf(p_re);
    delayInit();

    SPI::HardInitStruct h;
    h.miso = "b15";
    h.mosi = "b13";
    h.sclk = "b14";
    h.MOSI_AF = GPIO_AF_4;
    h.SCLK_AF = GPIO_AF_3;
    h.MISO_AF = GPIO_AF_3;
    h.Spi_Num = 2;

    spi4Oled = new SPI::SPI_Object(&h);
    // spi4Oled = new SPI::SPI_Object("b13", "b15", "b14");
    OLED::OLED_Object screen("a12", "b12", "a9", spiSend);
    s = &screen;

    UART::InitStruct u;
    u.bode = 115200;
    u.uartIdx = 1;
    u.rx = "b8";
    u.RxAF = GPIO_AF_0;
    u.tx = "b9";
    u.TxAF = GPIO_AF_0;
    uart = new UART::UART_Object(u);
    uart->setDMA((uint32_t)rxBuf, 4096, 1, 'r', true, datTrans);
    // uart->setDMA((uint32_t)pic, 32, 1, 'r', true, pis_s);
    // uart->setNVIC(1, true, beepFlip);

    screen.loadFont(ASCII[0], 16, 8); //装载字体
    screen.setScreenSize(128, 64);    //设置屏幕分辨率
    // screen.print(e == SUCCESS ? "HSE ok\n" : "HES not ok\n");
    // screen.print("abc");
    spi4Oled = new SPI::SPI_Object(&h);
    q64 = new w25q::W25Q("c15", spi4Oled);
    screen.loadZH_Font(getZH, 16, 16);
    screen.print("我1是2你3爹4\n润");
    // printZh("大家好我是你爹");
    printf("\n0x%x", q64->getID());
    // uint8_t tmp[1024]={0};
    // for (size_t i = 0; i < 256; i++)
    // {
    //     tmp[i] = i;
    //     tmp[i + 256] = 255 - i;
    // }
    // // q64->write(1, tmp, 521);
    // q64->writeSector(0, tmp, 1024);
    KEY::KEY_Object k("c10");
    k.setOption(showChar);
    KEY::KEY_Object down("a15");
    // down.setOption(printData);
    while (1)
    {
    }
}
