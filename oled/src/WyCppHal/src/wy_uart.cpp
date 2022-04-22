#include "common.h"
#include "wy_uart.hpp"
using namespace UART;

uint8_t pin2pinSource(uint16_t pin);
// {
//     uint8_t result = 0;
//     while (pin != 1)
//     {
//         pin /= 2;
//         result++;
//     }
//     return result;
// }

UART_Object::UART_Object(InitStruct &s)
{
    UART_InitTypeDef uart;
    GPIO_InitTypeDef gpio;

    uart.UART_BaudRate = s.bode;
    uart.UART_Mode = UART_Mode_Rx | UART_Mode_Tx;
    uart.UART_Parity = UART_Parity_No;
    uart.UART_StopBits = UART_StopBits_1;
    uart.UART_WordLength = UART_WordLength_8b;
    uart.UART_HardwareFlowControl = UART_HardwareFlowControl_None;
    switch (s.uartIdx)
    {
    case 1:
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART1, ENABLE);
        this->nvicChannel = UART1_IRQn;
        this->uart = UART1;
        break;
    case 2:
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART2, ENABLE);
        this->uart = UART2;
        this->nvicChannel = UART2_IRQn;
        break;
    default:
        break;
    }
    UART_Init(this->uart, &uart);
    UART_Cmd(this->uart, ENABLE);

    uint8_t rxS = pin2pinSource(s.RxPin);
    uint8_t txS = pin2pinSource(s.TxPin);

    GPIO_PinAFConfig(s.RxPort, rxS, s.RxAF);
    GPIO_PinAFConfig(s.TxPort, txS, s.TxAF);

    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_Mode = GPIO_Mode_AF_PP;
    gpio.GPIO_Pin = s.TxPin;
    GPIO_Init(s.TxPort, &gpio);

    gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    gpio.GPIO_Pin = s.RxPin;
    GPIO_Init(s.RxPort, &gpio);
}

void UART_Object::sendByte(uint8_t dat)
{
    UART_SendData(this->uart, dat);
    while (!UART_GetFlagStatus(this->uart, UART_FLAG_TXEPT))
        ;
}
void UART_Object::sendByte(uint8_t *dat, uint8_t len)
{
    while (len--)
    {
        UART_SendData(this->uart, *dat++);
        while (!UART_GetFlagStatus(this->uart, UART_FLAG_TXEPT))
            ;
    }
}

void UART_Object::setNVIC(uint8_t priority, bool open)
{
    NVIC_InitTypeDef nvic;
    nvic.NVIC_IRQChannel = this->nvicChannel;
    nvic.NVIC_IRQChannelCmd = open ? ENABLE : DISABLE;
    nvic.NVIC_IRQChannelPriority = priority;
    NVIC_Init(&nvic);
    UART_ITConfig(this->uart, UART_IT_RXIEN, ENABLE);
}
