#ifndef ADBC878F_1668_46FA_ADFF_19549553C209
#define ADBC878F_1668_46FA_ADFF_19549553C209
#include "common.h"
#include "wy_gpio.hpp"

namespace KEY
{
    class KEY_Object// : private GPIO::Gpio_Object
    {
    private:
        uint8_t pin_source;

    public:
        KEY_Object() = default;
        KEY_Object(const char *n, bool interrupt = true, uint8_t p = 0);
        void setOption(void (*callback)(void));
    };

} // namespace KEY

#endif /* ADBC878F_1668_46FA_ADFF_19549553C209 */
