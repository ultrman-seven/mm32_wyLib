#include "common.h"
#include "stdio.h"
// #include "iostream"
#include "oled/oled.hpp"
#pragma import(__use_no_semihosting)

extern "C"
{
    void _sys_exit(int returncode)
    {
        printf("Exited! returncode=%d\n", returncode);
        while (1)
            ;
    }

    void _ttywrch(int ch)
    {
    }
}
void (*printFunction)(char c) = nullptr;

void sysConfig::redirect_Printf(void (*f)(char))
{
    printFunction = f;
}

namespace std
{
    struct __FILE
    {
        int handle;
        /* Whatever you require here. If the only file you are using is */
        /* standard output using printf() for debugging, no file handling */
        /* is required. */
    };

    FILE __stdin = {0};
    FILE __stdout = {1};
    FILE __stderr = {2};

    int fputc(int ch, FILE *f)
    {
        printFunction(ch);
        return ch;
    }
    int fclose(FILE *stream)
    {
        return 0;
    }

    int fseek(FILE *stream, long int offset, int whence)
    {
        return -1;
    }

    int fflush(FILE *stream)
    {
        return 0;
    }

} // namespace std

uint32_t sysTicDecTime = 0;
__IO uint32_t msTimeStamp = 0;
void delayInit(void)
{
    SysTick_Config(96000);
    NVIC_SetPriority(SysTick_IRQn, 0x00);
}

void delay(__IO uint32_t time)
{
    while (time--)
        ; //__nop();
}

void delayMs(uint32_t time)
{
    sysTicDecTime = time;
    while (sysTicDecTime)
        ;
}

int getTimeStamp(uint32_t *t)
{
    *t = msTimeStamp;
    return 0;
}

// extern "C" {
void SysTick_Handler(void)
{
    msTimeStamp++;
    if (sysTicDecTime)
        sysTicDecTime--;
}

//}
