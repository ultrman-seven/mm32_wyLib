#include "common.h"
#include "wy_spi.hpp"
#include "wy_gpio.hpp"

namespace w25q
{
    class W25Q
    {
    private:
        GPIO::Gpio_Object *cs = nullptr;
        SPI::SPI_Object *spi = nullptr;

    public:
        W25Q() = default;
        W25Q(const char*cs,SPI::SPI_Object *s=nullptr)
        {
            this->cs = new GPIO::Gpio_Object(cs);
            this->spi = s;
        }
    };

} // namespace w25q

using namespace w25q;
