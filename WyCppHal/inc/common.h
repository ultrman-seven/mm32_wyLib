#ifndef __WY_LIB_COMMON_H__
#define __WY_LIB_COMMON_H__

#include "HAL_conf.h"
#include "MM32SPIN2xx_p.h"
#define __weak __attribute__((weak))
// #include "system_MM32SPIN2xx_p.h"
//extern "C" {

void delay(__IO uint32_t time);
void delayInit(void);
void delayMs(uint32_t time);
int getTimeStamp(uint32_t *t);
namespace sysConfig
{
    void redirect_Printf(void (*f)(char));
} // namespace sysConfig

//}
#endif /* __WY_LIB_COMMON_H__ */
