#ifndef __WY_LIB_CPP_IIC_HPP__
#define __WY_LIB_CPP_IIC_HPP__
#include "common.h"
#include "vector"
#include "wy_gpio.hpp"

namespace IIC
{
    class IIC_Object
    {
    private:
        GPIO_TypeDef *sda_port, *scl_port;
        uint16_t sda_pin, scl_pin;
        GPIO::Gpio_Object *sda, *scl;
        uint32_t *CR;
        uint32_t CR_clear, mode_in, mode_out;

        void ack(bool a);
        bool wait_ack(void);
        bool start(void);
        void end(void);
        void inputMode(void);
        void outputMode(void);
        void iic_delay(void);
        void sendByte(uint8_t dat);
        uint8_t getByte(bool a);

    public:
        IIC_Object() = default;
        IIC_Object(GPIO_TypeDef *port, uint16_t sda_pin, uint16_t scl_pin)
        {
            IIC_Object(port, sda_pin, port, scl_pin);
        }
        IIC_Object(GPIO_TypeDef *sda_port, uint16_t sda_pin, GPIO_TypeDef *scl_port, uint16_t scl_pin);

        bool sendData(uint8_t slvAdd, uint8_t regAdd, uint8_t len, uint8_t *dat);
        bool sendData(uint8_t slvAdd, uint8_t regAdd, uint8_t dat);

        bool readData(uint8_t slvAdd, uint8_t regAdd, uint8_t len, uint8_t *buf);
        uint8_t readData(uint8_t slvAdd, uint8_t regAdd);
    };

    class IIC_Slaver
    {
    private:
        uint8_t address;

    public:
        IIC_Slaver() = default;
    };
} // namespace IIC

#endif /* __WY_LIB_CPP_IIC_HPP__ */
