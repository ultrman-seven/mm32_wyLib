#include "common.h"
#include "oled.hpp"
#include "wy_spi.hpp"
#include "wy_uart.hpp"
#include "font.hpp"
#include <stdio.h>
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
    s->putChar(ch);
    // uart->sendByte(ch);
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
    // uart->setDMA((uint32_t)rxBuf, 4096, 1, 'r', true, dataTrans);
    uart->setDMA((uint32_t)pic, 32, 1, 'r', true, pis_s);
    

    screen.loadFont(ASCII[0], 16, 8); //装载字体
    screen.setScreenSize(128, 64);    //设置屏幕分辨率
    screen.print(e == SUCCESS ? "HSE ok\n" : "HES not ok\n");
    screen.print("abc");

    while (1)
    {
    }
}
