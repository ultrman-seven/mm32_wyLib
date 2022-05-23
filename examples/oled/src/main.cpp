#include "common.h"
#include "oled.hpp"
#include "wy_spi.hpp"
#include "wy_uart.hpp"
#include "font.hpp"
#include "wy_8080.hpp"
#include "wy_gpio.hpp"
#include <stdio.h>
#include <vector>
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
        RCC_PCLK1Config(RCC_HCLK_Div1);
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

void p_re(char ch)
{
    s->putChar(ch);
}

SPI::SPI_Object *spi4Oled = nullptr;
void spiSend(uint8_t dat)
{
    spi4Oled->sendOneByte(dat);
}

intel8080::Intel8080_Object *i8080_4oled = nullptr;
void i8080Send(uint8_t dat)
{
    i8080_4oled->sendByte(dat);
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

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

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

    /*i8080_4oled = new intel8080::Intel8080_Object;
    i8080_4oled->pinStr.dataHigh = false;
    i8080_4oled->pinStr.dataPort = GPIOA;
    i8080_4oled->pinStr.rdPort = GPIOC;
    i8080_4oled->pinStr.wtPort = GPIOC;
    i8080_4oled->pinStr.rdPin = GPIO_Pin_0;
    i8080_4oled->pinStr.wtPin = GPIO_Pin_1;
    i8080_4oled->init();*/

    OLED::OLED_Object screen("a12", "b12", "a9", spiSend);
    s = &screen;
    screen.loadFont(ASCII[0], 16, 8); //装载字体
    screen.setScreenSize(128, 64);    //设置屏幕分辨率

    screen.clear();
    UART::InitStruct u;
    u.bode = 115200;
    u.uartIdx = 1;
    u.RxPort = GPIOB;
    u.RxPin = GPIO_Pin_8;
    u.RxAF = GPIO_AF_0;
    u.TxPort = GPIOB;
    u.TxPin = GPIO_Pin_9;
    u.TxAF = GPIO_AF_0;
    UART::UART_Object computer(u);
    computer.setDMA((uint32_t)pic, 32, 1, 'r', true, pis_s);
    // screen.clear();
    screen.print(e == SUCCESS ? "HSE ok\n" : "HES not ok\n");
    screen.print("abc");

    GPIO::Gpio_Object led("c9");
    while (1)
    {
        led = 0;
        delayMs(500);
        led = 1;
        delayMs(500);
    }
}
