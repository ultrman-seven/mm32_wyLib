#include "common.h"
#include "oled.hpp"
#include "wy_spi.hpp"
#include "font.hpp"
#include <stdio.h>
// extern const uint8_t qe_gif[][1024];
// extern const uint8_t mao_gif[][1024];
extern const uint8_t a_gif[][1024];

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

int main(void)
{
    HSE_SysClock();
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

    SPI::SoftInitStruct oledSoft;

    oledSoft.CS_Pin = GPIO_Pin_5;
    oledSoft.CS_Port = GPIOA;
    oledSoft.DC_Pin = GPIO_Pin_10;
    oledSoft.DC_Port = GPIOB;
    oledSoft.MOSI_Pin = GPIO_Pin_6;
    oledSoft.MOSI_Port = GPIOA;
    oledSoft.RES_Pin = GPIO_Pin_11;
    oledSoft.RES_Port = GPIOB;
    oledSoft.SCLK_Pin = GPIO_Pin_4;
    oledSoft.SCLK_Port = GPIOA;

    delayInit();
    OLED::OLED_Object screen(&oledSoft);
    screen.loadFont(ASCII[0], 16, 8); //装载字体
    // screen.loadFont(ASCII_24_12[0], 24, 12);
    screen.setScreenSize(128, 64); //设置屏幕分辨率
    screen.clear();
    screen.print("hello world!\n");
    // screen.reverse(true);
    //screen.setBrightness(0);
    while (1)
    {
        int i = 0;
        while (i < 20)
        {
            screen.Picture_display(a_gif[i++], 0, 0, 64, 128);
            delayMs(100);
        }
        // i = 0;
        // while (i < 45)
        // {
        //     screen.Picture_display(mao_gif[i++], 0, 0, 64, 128);
        //     delayMs(40);
        // }
    }

    return 0;
}
