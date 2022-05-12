#include "common.h"
#include "oled.hpp"
#include "wy_spi.hpp"
#include "wy_uart.hpp"
#include "font.hpp"
#include "wy_8080.hpp"
#include <stdio.h>
#include <vector>

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
int main(void)
{
    HSE_SysClock();
    sysConfig::redirect_Printf(p_re);
    delayInit();

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

    spi4Oled = new SPI::SPI_Object(GPIOA, GPIO_Pin_6, GPIO_Pin_4);
    i8080_4oled = new intel8080::Intel8080_Object;
    i8080_4oled->pinStr.dataHigh = false;
    i8080_4oled->pinStr.dataPort = GPIOA;
    i8080_4oled->pinStr.rdPort = GPIOA;
    i8080_4oled->pinStr.wtPort = GPIOB;
    i8080_4oled->pinStr.rdPin = GPIO_Pin_12;
    i8080_4oled->pinStr.wtPin = GPIO_Pin_10;
    i8080_4oled->init();
    // i_8080 = &i8080_4oled;

    OLED::OLED_Object screen(GPIOA, GPIO_Pin_10, GPIOB, GPIO_Pin_11, GPIOA, GPIO_Pin_11, i8080Send);
    // OLED::OLED_Object screen(GPIOB, GPIO_Pin_10, GPIOA, GPIO_Pin_5, GPIOB, GPIO_Pin_11, spiSend);
    s = &screen;
    screen.loadFont(ASCII[0], 16, 8); //装载字体
    screen.setScreenSize(128, 64);    //设置屏幕分辨率

    screen.clear();
    screen.print("he\al\alo\a");
    screen.placeFill("666");

    GPIO_InitTypeDef gpio;
    gpio.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_Init(GPIOB, &gpio);
    GPIO_ResetBits(GPIOB, GPIO_Pin_0);
    GPIO_ResetBits(GPIOB, GPIO_Pin_1);

    while (1)
    {
    }

    return 0;
}
