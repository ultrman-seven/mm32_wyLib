#ifndef A95005C1_D83D_4052_97C0_473EC7C3A091
#define A95005C1_D83D_4052_97C0_473EC7C3A091
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
        void erase();
        void erase(uint16_t position, uint8_t mode = 0);
        void disable(void);
        void enable(void);
        bool isBusy(void);
        void writePage(uint16_t pageNum,uint8_t*dat,uint16_t datLen);

    public:
        W25Q() = default;
        W25Q(const char *cs, SPI::SPI_Object *&s)
        {
            this->cs = new GPIO::Gpio_Object(cs);
            this->cs->set();
            this->spi = s;
        }
        uint32_t getID(void);
        void write(uint16_t pageNum, uint8_t *dat, uint16_t len = 256);
        void writeSector(uint16_t sectorNum, uint8_t *dat, uint16_t datLen);
        void read(uint32_t add, uint8_t *buf, uint16_t len);
    };
} // namespace w25q

#endif /* A95005C1_D83D_4052_97C0_473EC7C3A091 */
