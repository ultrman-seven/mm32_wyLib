#ifndef A00A44FC_8422_45AC_8C2D_BA7C3D58383A
#define A00A44FC_8422_45AC_8C2D_BA7C3D58383A
#include "wy_spi.hpp"
#include "generalO.hpp"
#include <vector>
namespace TFT
{
    class Tft : public SPI::SPI_Object, public genO::generalOutputBase
    {
    private:
        void char_display(const uint8_t *ptr_pic, bool contrast);
        void sendByte(uint8_t dat, bool isCMD);
        void Lcd_WriteIndex(uint8_t dat) { sendByte(dat, true); }
        void Lcd_WriteData(uint8_t dat) { sendByte(dat, false); }
        // void LCD_WriteData_16Bit(uint16_t dat) { sendData(dat); }
        void LCD_WriteData_16Bit(uint16_t dat)
        {
            Lcd_WriteData((uint8_t)(dat >> 8));
            Lcd_WriteData((uint8_t)(dat & 0xff));
        }
        void Lcd_SetRegion(uint8_t x_start, uint8_t y_start, uint8_t x_end, uint8_t y_end);

    public:
        void dsp_single_colour(uint16_t color);
        Tft() = default;
        Tft(SPI::SoftInitStruct *s)
        {
            init(s);
            reset();
        }
        Tft(SPI::HardInitStruct *h)
        {
            init(h);
            reset();
        }
        void reset(void);
        void clear(void);
        void showimage(const uint8_t *p);
    };

} // namespace TFT

#endif /* A00A44FC_8422_45AC_8C2D_BA7C3D58383A */
