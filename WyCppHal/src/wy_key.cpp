#include "common.h"

namespace KEY
{
    class KEY_Object
    {
    private:
        GPIO_TypeDef *port;
        uint16_t pin;

    public:
        KEY_Object() = default;
        KEY_Object(GPIO_TypeDef *port, uint16_t pin) { KEY_Object(port, pin, true, 0); };
        KEY_Object(GPIO_TypeDef *port, uint16_t pin, bool interrupt, uint8_t p);
    };

} // namespace KEY

using namespace KEY;

void (*extiCallback[16])(void) = {nullptr};

KEY_Object::KEY_Object(GPIO_TypeDef *port, uint16_t pin, bool interrupt, uint8_t p)
{
    this->port = port;
    this->pin = pin;
    uint8_t p_s = pin2pinSource(pin);

    if (interrupt)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
        uint16_t gpio_s = (uint32_t)(this->port) & 0xffff;
        SYSCFG_EXTILineConfig(gpio_s / 0x400, p_s);

        NVIC_InitTypeDef nvic;
        EXTI_InitTypeDef exti;
        switch (p_s)
        {
        case 0:
        case 1:
            nvic.NVIC_IRQChannel = EXTI0_1_IRQn;
            break;
        case 2:
        case 3:
            nvic.NVIC_IRQChannel = EXTI2_3_IRQn;
            break;
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
            nvic.NVIC_IRQChannel = EXTI4_15_IRQn;
            break;

        default:
            break;
        }
        nvic.NVIC_IRQChannelCmd = ENABLE;
        nvic.NVIC_IRQChannelPriority = p;
        NVIC_Init(&nvic);
        exti.EXTI_Line = this->pin;
        exti.EXTI_LineCmd = ENABLE;
        exti.EXTI_Mode = EXTI_Mode_Interrupt;
        exti.EXTI_Trigger = EXTI_Trigger_Falling;
        EXTI_Init(&exti);
    }
}

//>>> def exti_i(i:int):
//...     r=f'''if (EXTI_GetITStatus(EXTI_Line{i}) != RESET)
//... {{
//...     if (extiCallback[{i}] != nullptr)
//...         extiCallback[{i}]();
//...     EXTI_ClearITPendingBit(EXTI_Line{i});
//... }}'''
//...     print(r)

//>>>
//>>> for i in range(16):
//...     exti_i(i)
//>>>

void EXTI0_1_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line0) != RESET)
    {
        if (extiCallback[0] != nullptr)
            extiCallback[0]();
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
    if (EXTI_GetITStatus(EXTI_Line1) != RESET)
    {
        if (extiCallback[1] != nullptr)
            extiCallback[1]();
        EXTI_ClearITPendingBit(EXTI_Line1);
    }
}

void EXTI2_3_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line2) != RESET)
    {
        if (extiCallback[2] != nullptr)
            extiCallback[2]();
        EXTI_ClearITPendingBit(EXTI_Line2);
    }
    if (EXTI_GetITStatus(EXTI_Line3) != RESET)
    {
        if (extiCallback[3] != nullptr)
            extiCallback[3]();
        EXTI_ClearITPendingBit(EXTI_Line3);
    }
}

void EXTI4_15_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line4) != RESET)
    {
        if (extiCallback[4] != nullptr)
            extiCallback[4]();
        EXTI_ClearITPendingBit(EXTI_Line4);
    }
    if (EXTI_GetITStatus(EXTI_Line5) != RESET)
    {
        if (extiCallback[5] != nullptr)
            extiCallback[5]();
        EXTI_ClearITPendingBit(EXTI_Line5);
    }
    if (EXTI_GetITStatus(EXTI_Line6) != RESET)
    {
        if (extiCallback[6] != nullptr)
            extiCallback[6]();
        EXTI_ClearITPendingBit(EXTI_Line6);
    }
    if (EXTI_GetITStatus(EXTI_Line7) != RESET)
    {
        if (extiCallback[7] != nullptr)
            extiCallback[7]();
        EXTI_ClearITPendingBit(EXTI_Line7);
    }
    if (EXTI_GetITStatus(EXTI_Line8) != RESET)
    {
        if (extiCallback[8] != nullptr)
            extiCallback[8]();
        EXTI_ClearITPendingBit(EXTI_Line8);
    }
    if (EXTI_GetITStatus(EXTI_Line9) != RESET)
    {
        if (extiCallback[9] != nullptr)
            extiCallback[9]();
        EXTI_ClearITPendingBit(EXTI_Line9);
    }
    if (EXTI_GetITStatus(EXTI_Line10) != RESET)
    {
        if (extiCallback[10] != nullptr)
            extiCallback[10]();
        EXTI_ClearITPendingBit(EXTI_Line10);
    }
    if (EXTI_GetITStatus(EXTI_Line11) != RESET)
    {
        if (extiCallback[11] != nullptr)
            extiCallback[11]();
        EXTI_ClearITPendingBit(EXTI_Line11);
    }
    if (EXTI_GetITStatus(EXTI_Line12) != RESET)
    {
        if (extiCallback[12] != nullptr)
            extiCallback[12]();
        EXTI_ClearITPendingBit(EXTI_Line12);
    }
    if (EXTI_GetITStatus(EXTI_Line13) != RESET)
    {
        if (extiCallback[13] != nullptr)
            extiCallback[13]();
        EXTI_ClearITPendingBit(EXTI_Line13);
    }
    if (EXTI_GetITStatus(EXTI_Line14) != RESET)
    {
        if (extiCallback[14] != nullptr)
            extiCallback[14]();
        EXTI_ClearITPendingBit(EXTI_Line14);
    }
    if (EXTI_GetITStatus(EXTI_Line15) != RESET)
    {
        if (extiCallback[15] != nullptr)
            extiCallback[15]();
        EXTI_ClearITPendingBit(EXTI_Line15);
    }
}
