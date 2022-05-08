#include "common.h"
#include "oled.hpp"
#include "wy_spi.hpp"
#include "wy_uart.hpp"
#include "font.hpp"
#include "wy_8080.hpp"
#include <stdio.h>
#include <vector>

// extern const uint8_t qe_gif[][1024];
// extern const uint8_t mao_gif[][1024];
extern const uint8_t a_gif[][1024];
OLED::OLED_Object *s = nullptr;
UART::UART_Object *c = nullptr;
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
        //外部晶振8MHz，12倍频成96MHz
        RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_12);
        RCC_PLLCmd(ENABLE);
        while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
            ;
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
        while (RCC_GetSYSCLKSource() != 0x08)
            ;
    }
    return HSE_StartUpState;
}

uint8_t pic[1024] = {0};

void displayPic(void)
{
    // s->clear();
    s->Picture_display(pic, 0, 0, 64, 128);
    c->DMA_On(true);
}

uint8_t *picPtr = pic;
void cptRxData(void)
{
    *picPtr++ = c->receiveByte();
    if ((picPtr - pic) == 1024)
    {
        s->Picture_display(pic, 0, 0, 64, 128);
        picPtr = pic;
    }
}

void p_re(char ch)
{
    s->putChar(ch);
}

SPI::SPI_Object *spi = new SPI::SPI_Object(GPIOA, GPIO_Pin_6, GPIO_Pin_4);
void spiSend(uint8_t dat)
{
    spi->sendOneByte(dat);
}

intel8080::Intel8080_Object *i_8080 = nullptr;
void send8080(uint8_t dat)
{
    i_8080->sendByte(dat);
}

int main(void)
{
    HSE_SysClock();
    sysConfig::redirect_Printf(p_re);
    delayInit();

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    // std::vector<uint8_t> dat(10, 23);

    // intel8080::Intel8080_Object i8080_4oled;
    // i8080_4oled.pinStr.dataHigh = false;
    // i8080_4oled.pinStr.dataPort = GPIOA;
    // i8080_4oled.pinStr.rdPort = GPIOA;
    // i8080_4oled.pinStr.wtPort = GPIOB;
    // i8080_4oled.pinStr.rdPin = GPIO_Pin_12;
    // i8080_4oled.pinStr.wtPin = GPIO_Pin_10;
    // i8080_4oled.init();
    // i_8080 = &i8080_4oled;

    // OLED::OLED_Object screen(GPIOA, GPIO_Pin_10, GPIOB, GPIO_Pin_11, GPIOA, GPIO_Pin_11, send8080);
    OLED::OLED_Object screen(GPIOB, GPIO_Pin_10, GPIOA, GPIO_Pin_5, GPIOB, GPIO_Pin_11, spiSend);
    s = &screen;
    screen.loadFont(ASCII[0], 16, 8); //装载字体
    // screen.loadFont(ASCII_24_12[0], 24, 12);
    screen.setScreenSize(128, 64); //设置屏幕分辨率

    screen.clear();
    screen.print("hello");
    // UART::InitStruct u;
    // u.bode = 115200;
    // u.uartIdx = 2;
    // u.RxPort = GPIOA;
    // u.RxPin = GPIO_Pin_3;
    // u.RxAF = GPIO_AF_1;
    // u.TxPort = GPIOA;
    // u.TxPin = GPIO_Pin_2;
    // u.TxAF = GPIO_AF_1;
    // UART::UART_Object computer(u);
    // c = &computer;
    //  computer.setNVIC();
    //  computer.setRxFunction(cptRxData);
    // computer.setDMA((uint32_t)pic, 1024, 1, 'r', true, displayPic);

    while (1)
    {
    }

    return 0;
}
