#ifndef FBB603F1_460A_4998_87FD_8B5DFF17B784
#define FBB603F1_460A_4998_87FD_8B5DFF17B784
#include "common.h"
namespace TIM
{
    class TIM_Base_Object
    {
    private:
        TIM_TypeDef *tim;
        void (**updateFunc)(void) = nullptr;
        uint8_t updateNVIC_Channel;

    public:
        TIM_Base_Object() = default;
        TIM_Base_Object(uint8_t num);
        void enable(bool e) { TIM_Cmd(this->tim, e ? ENABLE : DISABLE); }
        void setUsCountTime(uint16_t us) { setCountTime(us, 95, true); };
        void setMsCountTime(uint16_t us) { setCountTime(us * 10, 9600 - 1, true); };
        void setCountTime(uint16_t p, uint16_t s, bool e);
        void setNVIC_Enable(uint8_t p, bool e);
        void setUpdateFunc(void (*f)(void)) { *(this->updateFunc) = f; }
        FlagStatus getFlagStatus(uint16_t flag) { return TIM_GetFlagStatus(this->tim, flag); }
    };
} // TIM

#endif /* FBB603F1_460A_4998_87FD_8B5DFF17B784 */
