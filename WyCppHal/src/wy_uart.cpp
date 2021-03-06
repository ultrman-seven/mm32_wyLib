#include "common.h"
#include "wy_uart.hpp"
#include "wy_gpio.hpp"
using namespace UART;

void (*uart1RxFux)(void) = nullptr;
void (*uart2RxFux)(void) = nullptr;

UART_Object::UART_Object(InitStruct &s)
{
    UART_InitTypeDef uart;

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
        this->rxFun = &uart1RxFux;
        break;
    case 2:
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART2, ENABLE);
        this->uart = UART2;
        this->nvicChannel = UART2_IRQn;
        this->rxFun = &uart2RxFux;
        break;
    default:
        break;
    }
    UART_Init(this->uart, &uart);
    UART_Cmd(this->uart, ENABLE);
    GPIO::afConfig(s.rx, s.RxAF, GPIO_Mode_IN_FLOATING);
    GPIO::afConfig(s.tx, s.TxAF, GPIO_Mode_AF_PP);
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

void UART_Object::setNVIC(uint8_t priority, bool open, void (*f)(void))
{
    NVIC_InitTypeDef nvic;
    nvic.NVIC_IRQChannel = this->nvicChannel;
    nvic.NVIC_IRQChannelCmd = open ? ENABLE : DISABLE;
    nvic.NVIC_IRQChannelPriority = priority;
    NVIC_Init(&nvic);
    UART_ITConfig(this->uart, UART_IT_RXIEN, ENABLE);
    *(this->rxFun) = f;
}

void (*dmaCh3IntFun)(void) = nullptr;
void (*dmaCh5IntFun)(void) = nullptr;

void UART_Object::setDMA(uint32_t add, uint16_t size, uint8_t priority, char mode, bool interrupt, void (*f)(void))
{
    NVIC_InitTypeDef nvic;
    switch ((uint32_t)(this->uart))
    {
    case UART1_BASE:
        this->dmaChannel = DMA1_Channel3;
        dmaCh3IntFun = f;
        nvic.NVIC_IRQChannel = DMA1_Channel2_3_IRQn;
        break;
    case UART2_BASE:
        this->dmaChannel = DMA1_Channel5;
        dmaCh5IntFun = f;
        nvic.NVIC_IRQChannel = DMA1_Channel4_5_IRQn;
        break;
    default:
        break;
    }
    nvic.NVIC_IRQChannelCmd = ENABLE;
    nvic.NVIC_IRQChannelPriority = priority;
    if (interrupt)
        NVIC_Init(&nvic);

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
    DMA_InitTypeDef dma;
    dma.DMA_Priority = DMA_Priority_High;
    dma.DMA_DIR = mode == 't' ? DMA_DIR_PeripheralDST : DMA_DIR_PeripheralSRC;
    dma.DMA_PeripheralBaseAddr = (uint32_t) & (this->uart->RDR); //(uint32_t)&(mode == 't' ? this->uart->TDR : this->uart->RDR);
    dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;

    dma.DMA_BufferSize = size;
    dma.DMA_M2M = DMA_M2M_Disable;
    dma.DMA_MemoryBaseAddr = add;
    dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    dma.DMA_MemoryInc = DMA_MemoryInc_Enable;

    dma.DMA_Mode = DMA_Mode_Circular;
    UART_DMACmd(this->uart, UART_DMAReq_EN, ENABLE);

    DMA_Init(this->dmaChannel, &dma);
    DMA_ITConfig(this->dmaChannel, DMA_IT_TC, ENABLE);
    DMA_Cmd(this->dmaChannel, ENABLE);
}

void UART_Object::setDMA_InterruptFunction(void (*f)(void))
{
    switch ((uint32_t)(this->uart))
    {
    case UART1_BASE:
        dmaCh3IntFun = f;
        break;
    case UART2_BASE:
        dmaCh5IntFun = f;
        break;
    default:
        break;
    }
}
void DMA1_Channel2_3_IRQHandler(void)
{
    if (DMA_GetITStatus(DMA1_IT_TC3) == SET)
    {
        DMA_ClearITPendingBit(DMA1_IT_TC3);
        // DMA_Cmd(DMA1_Channel3, DISABLE);
        if (dmaCh3IntFun != nullptr)
            dmaCh3IntFun();
    }
}
void DMA1_Channel4_5_IRQHandler(void)
{
    if (DMA_GetITStatus(DMA1_IT_TC5) == SET)
    {
        DMA_ClearITPendingBit(DMA1_IT_TC5);
        // DMA_Cmd(DMA1_Channel5, DISABLE);
        if (dmaCh5IntFun != nullptr)
            dmaCh5IntFun();
    }
}

void UART2_IRQHandler(void)
{
    if (UART_GetITStatus(UART2, UART_IT_RXIEN) == SET)
    {
        if (uart2RxFux != nullptr)
            uart2RxFux();
        UART_ClearITPendingBit(UART2, UART_IT_RXIEN);
    }
}
void UART1_IRQHandler(void)
{
    if (UART_GetITStatus(UART1, UART_IT_RXIEN) == SET)
    {
        if (uart1RxFux != nullptr)
            uart1RxFux();
        UART_ClearITPendingBit(UART1, UART_IT_RXIEN);
    }
}
