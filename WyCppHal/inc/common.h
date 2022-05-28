#ifndef __WY_LIB_COMMON_H__
#define __WY_LIB_COMMON_H__

#include "HAL_conf.h"
#include "MM32SPIN2xx_p.h"
#define __weak __attribute__((weak))
// #include "system_MM32SPIN2xx_p.h"

union u64_split
{
    uint64_t val;
    uint8_t unit[8];
};
union u32_split
{
    uint32_t val;
    uint8_t unit[4];
};

union u16_split
{
    uint16_t val;
    uint8_t unit[2];
};
union float_split
{
    float val;
    uint8_t unit[4];
};
union double_split
{
    double val;
    uint8_t unit[8];
};

void delay(__IO uint32_t time);
void delayInit(void);
void delayMs(uint32_t time);
int getTimeStamp(uint32_t *t);
uint8_t pin2pinSource(uint16_t pin);
namespace sys
{
    void redirect_Printf(void (*f)(char));
    void runFunList(void);
    void throwFun2Main(void (*f)(void));
    void delFun(void (*f)(void));
} // namespace sys

#endif /* __WY_LIB_COMMON_H__ */
