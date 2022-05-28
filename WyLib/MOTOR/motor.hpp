#ifndef A9E838A7_C918_4A4A_80FA_7E486A459882
#define A9E838A7_C918_4A4A_80FA_7E486A459882
#include "common.h"
#include "wy_gpio.hpp"

namespace motor
{
    class StepMotor
    {
    private:
        GPIO::Gpio_Object *a, *b, *c, *d;

    public:
        StepMotor() = default;
        StepMotor(const char *a, const char *b, const char *c, const char *d);
        void run(uint8_t speed = 10, uint8_t circles = 1);
    };

} // namespace motor

#endif /* A9E838A7_C918_4A4A_80FA_7E486A459882 */
