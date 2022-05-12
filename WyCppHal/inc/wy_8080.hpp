#ifndef F74A715A_210F_44BD_8FC8_6674C7325634
#define F74A715A_210F_44BD_8FC8_6674C7325634
#include "common.h"
namespace intel8080
{
    class Intel8080_Object
    {
    private:
        /* data */
    public:
        struct
        {
            GPIO_TypeDef *dataPort, *rdPort, *wtPort;
            uint16_t rdPin, wtPin;
            bool dataHigh;
        } pinStr;

        Intel8080_Object() {}
        void init(void);
        void sendByte(uint8_t dat);
    };

} // namespace intel8080
#endif /* F74A715A_210F_44BD_8FC8_6674C7325634 */
