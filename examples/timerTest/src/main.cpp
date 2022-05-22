#include "common.h"
#include "oled.hpp"
#include "wy_spi.hpp"
#include "font.hpp"
#include "wy_tim.hpp"
#include <stdio.h>

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

void timUpdate(void)
{
    static uint8_t hunMs = 0;
    static uint8_t sec = 0;
    static uint8_t min = 0;
    static uint8_t hour = 0;
    static uint8_t day = 0;

    if (++hunMs == 10)
    {
        hunMs = 0;
        sec++;
    }
    if (sec == 60)
    {
        sec = 0;
        min++;
    }
    if (min == 60)
    {
        min = 0;
        hour++;
    }
    if (hour == 24)
    {
        hour = 0;
        day++;
    }
    // s->clear();
    // printf("day%d time:\n%dh %dm %d.%ds", day, hour, min, sec, hunMs);
    sprintf(s->str, "%2d%2d%2d%2d%d", day, hour, min, sec, hunMs);
    s->placeFill(s->str);
}

SPI::SPI_Object *spi4Oled = nullptr;
void spiSend(uint8_t dat)
{
    spi4Oled->sendOneByte(dat);
}
int main(void)
{
    //HSE_SysClock();
    sys::redirect_Printf(p_re);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    delayInit();

    spi4Oled = new SPI::SPI_Object(GPIOB, GPIO_Pin_13, GPIO_Pin_14);

    // OLED::OLED_Object screen(GPIOB, GPIO_Pin_10, GPIOA, GPIO_Pin_5, GPIOB, GPIO_Pin_11, spiSend);
    OLED::OLED_Object screen(GPIOA, GPIO_Pin_12, GPIOB, GPIO_Pin_12, GPIOA, GPIO_Pin_9, spiSend);
    s = &screen;
    screen.loadFont(ASCII[0], 16, 8); //装载字体
    screen.setScreenSize(128, 64);    //设置屏幕分辨率
    screen.clear();
    screen.print("day\a\a time:\n\a\ah \a\am \a\a.\as");

    TIM::TIM_Base_Object timer(16);
    timer.setMsCountTime(100);
    timer.setUpdateFunc(timUpdate);
    timer.setNVIC_Enable(0, true);

    while (1)
    {
    }

    return 0;
}
