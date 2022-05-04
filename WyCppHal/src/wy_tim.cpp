#include "wy_tim.hpp"

using namespace TIM;

void TIM_Base_Object::setCountTime(uint16_t p, uint16_t s, bool e)
{
    this->tim->PSC = s;
    this->tim->ARR = p;
    if (e)
        this->tim->CR1 |= TIM_CR1_CEN;
    else
        this->tim->CR1 &= (uint16_t)(~((uint16_t)TIM_CR1_CEN));
}

void TIM_Base_Object::setNVIC_Enable(uint8_t p, bool e)
{
    NVIC_InitTypeDef nvic;
    nvic.NVIC_IRQChannel = this->updateNVIC_Channel;
    nvic.NVIC_IRQChannelCmd = e ? ENABLE : DISABLE;
    nvic.NVIC_IRQChannelPriority = p;
    NVIC_Init(&nvic);
    if (e)
        this->tim->DIER |= TIM_IT_Update;
    else
        this->tim->DIER &= (uint32_t)~TIM_IT_Update;
}

void (*tim1UpdateFunction)(void) = nullptr;
void (*tim2UpdateFunction)(void) = nullptr;
void (*tim3UpdateFunction)(void) = nullptr;
void (*tim14UpdateFunction)(void) = nullptr;
void (*tim16UpdateFunction)(void) = nullptr;
void (*tim17UpdateFunction)(void) = nullptr;
void (*tim8UpdateFunction)(void) = nullptr;

TIM_Base_Object::TIM_Base_Object(uint8_t num)
{
    switch (num)
    {
    case 1:
        this->tim = TIM1;
        this->updateFunc = &tim1UpdateFunction;
        this->updateNVIC_Channel = TIM1_BRK_UP_TRG_COM_IRQn;
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
        break;
    case 2:
        this->tim = TIM2;
        this->updateFunc = &tim2UpdateFunction;
        this->updateNVIC_Channel = TIM2_IRQn;
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
        break;
    case 3:
        this->tim = TIM3;
        this->updateFunc = &tim3UpdateFunction;
        this->updateNVIC_Channel = TIM3_IRQn;
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
        break;
    case 14:
        this->tim = TIM14;
        this->updateFunc = &tim14UpdateFunction;
        this->updateNVIC_Channel = TIM14_IRQn;
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM14, ENABLE);
        break;
    case 16:
        this->tim = TIM16;
        this->updateFunc = &tim16UpdateFunction;
        this->updateNVIC_Channel = TIM16_IRQn;
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16, ENABLE);
        break;
    case 17:
        this->tim = TIM17;
        this->updateFunc = &tim17UpdateFunction;
        this->updateNVIC_Channel = TIM17_IRQn;
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM17, ENABLE);
        break;
    case 8:
        this->tim = TIM8;
        this->updateFunc = &tim8UpdateFunction;
        this->updateNVIC_Channel = TIM8_BRK_UP_TRG_COM_IRQn;
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
        break;
    default:
        break;
    }

    TIM_TimeBaseInitTypeDef tim;
    tim.TIM_ClockDivision = TIM_CKD_DIV1;
    tim.TIM_CounterMode = TIM_CounterMode_Up;
    tim.TIM_Period = 0xffff;
    tim.TIM_Prescaler = 95;
    tim.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(this->tim, &tim);
}

void TIM1_BRK_UP_TRG_COM_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
    {
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
        if (tim1UpdateFunction != nullptr)
            tim1UpdateFunction();
    }
}

void TIM8_BRK_UP_TRG_COM_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM8, TIM_IT_Update) == SET)
    {
        TIM_ClearITPendingBit(TIM8, TIM_IT_Update);
        if (tim8UpdateFunction != nullptr)
            tim8UpdateFunction();
    }
}

void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        if (tim2UpdateFunction != nullptr)
            tim2UpdateFunction();
    }
}

void TIM3_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
        if (tim3UpdateFunction != nullptr)
            tim3UpdateFunction();
    }
}

void TIM14_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM14, TIM_IT_Update) == SET)
    {
        TIM_ClearITPendingBit(TIM14, TIM_IT_Update);
        if (tim14UpdateFunction != nullptr)
            tim14UpdateFunction();
    }
}

void TIM16_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM16, TIM_IT_Update) == SET)
    {
        TIM_ClearITPendingBit(TIM16, TIM_IT_Update);
        if (tim16UpdateFunction != nullptr)
            tim16UpdateFunction();
    }
}

void TIM17_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM17, TIM_IT_Update) == SET)
    {
        TIM_ClearITPendingBit(TIM17, TIM_IT_Update);
        if (tim17UpdateFunction != nullptr)
            tim17UpdateFunction();
    }
}
