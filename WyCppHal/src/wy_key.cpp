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
        KEY_Object();
    };

} // namespace KEY

using namespace KEY;

// >>> for i in range(16):
// ...     print(f'void (*e{i}Callback)(void)=nullptr;')
// >>>
void (*e0Callback)(void) = nullptr;
void (*e1Callback)(void) = nullptr;
void (*e2Callback)(void) = nullptr;
void (*e3Callback)(void) = nullptr;
void (*e4Callback)(void) = nullptr;
void (*e5Callback)(void) = nullptr;
void (*e6Callback)(void) = nullptr;
void (*e7Callback)(void) = nullptr;
void (*e8Callback)(void) = nullptr;
void (*e9Callback)(void) = nullptr;
void (*e10Callback)(void) = nullptr;
void (*e11Callback)(void) = nullptr;
void (*e12Callback)(void) = nullptr;
void (*e13Callback)(void) = nullptr;
void (*e14Callback)(void) = nullptr;
void (*e15Callback)(void) = nullptr;

//>>> def exti_i(i:int):
//...     r=f'''if (EXTI_GetITStatus(EXTI_Line{i}) != RESET)
//... {{
//...     if (e{i}Callback != nullptr)
//...         e{i}Callback();
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
        if (e0Callback != nullptr)
            e0Callback();
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
    if (EXTI_GetITStatus(EXTI_Line1) != RESET)
    {
        if (e1Callback != nullptr)
            e1Callback();
        EXTI_ClearITPendingBit(EXTI_Line1);
    }
}

void EXTI2_3_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line2) != RESET)
    {
        if (e2Callback != nullptr)
            e2Callback();
        EXTI_ClearITPendingBit(EXTI_Line2);
    }
    if (EXTI_GetITStatus(EXTI_Line3) != RESET)
    {
        if (e3Callback != nullptr)
            e3Callback();
        EXTI_ClearITPendingBit(EXTI_Line3);
    }
}
void EXTI4_15_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line4) != RESET)
    {
        if (e4Callback != nullptr)
            e4Callback();
        EXTI_ClearITPendingBit(EXTI_Line4);
    }
    if (EXTI_GetITStatus(EXTI_Line5) != RESET)
    {
        if (e5Callback != nullptr)
            e5Callback();
        EXTI_ClearITPendingBit(EXTI_Line5);
    }
    if (EXTI_GetITStatus(EXTI_Line6) != RESET)
    {
        if (e6Callback != nullptr)
            e6Callback();
        EXTI_ClearITPendingBit(EXTI_Line6);
    }
    if (EXTI_GetITStatus(EXTI_Line7) != RESET)
    {
        if (e7Callback != nullptr)
            e7Callback();
        EXTI_ClearITPendingBit(EXTI_Line7);
    }
    if (EXTI_GetITStatus(EXTI_Line8) != RESET)
    {
        if (e8Callback != nullptr)
            e8Callback();
        EXTI_ClearITPendingBit(EXTI_Line8);
    }
    if (EXTI_GetITStatus(EXTI_Line9) != RESET)
    {
        if (e9Callback != nullptr)
            e9Callback();
        EXTI_ClearITPendingBit(EXTI_Line9);
    }
    if (EXTI_GetITStatus(EXTI_Line10) != RESET)
    {
        if (e10Callback != nullptr)
            e10Callback();
        EXTI_ClearITPendingBit(EXTI_Line10);
    }
    if (EXTI_GetITStatus(EXTI_Line11) != RESET)
    {
        if (e11Callback != nullptr)
            e11Callback();
        EXTI_ClearITPendingBit(EXTI_Line11);
    }
    if (EXTI_GetITStatus(EXTI_Line12) != RESET)
    {
        if (e12Callback != nullptr)
            e12Callback();
        EXTI_ClearITPendingBit(EXTI_Line12);
    }
    if (EXTI_GetITStatus(EXTI_Line13) != RESET)
    {
        if (e13Callback != nullptr)
            e13Callback();
        EXTI_ClearITPendingBit(EXTI_Line13);
    }
    if (EXTI_GetITStatus(EXTI_Line14) != RESET)
    {
        if (e14Callback != nullptr)
            e14Callback();
        EXTI_ClearITPendingBit(EXTI_Line14);
    }
    if (EXTI_GetITStatus(EXTI_Line15) != RESET)
    {
        if (e15Callback != nullptr)
            e15Callback();
        EXTI_ClearITPendingBit(EXTI_Line15);
    }
}