#ifndef __WY_LIB_COMMON_H__
#define __WY_LIB_COMMON_H__

#include "HAL_conf.h"
#include "MM32SPIN2xx_p.h"
#define __weak __attribute__((weak))
// #include "system_MM32SPIN2xx_p.h"

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
