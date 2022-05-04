#include "common.h"

namespace IIC
{
    class IIC_Object
    {
    private:
        uint8_t slaveAdd;
        GPIO_TypeDef *sda_port, *scl_port;
        uint16_t sda_pin, scl_pin;
        uint32_t *CR;
        uint32_t CR_clear, mode_in, mode_out;

        void ack(bool a);
        void start(void);
        void end(void);
        void inputMode(void);
        void outputMode(void);
        /* data */
    public:
        IIC_Object() = default;
        IIC_Object(GPIO_TypeDef *port, uint16_t sda_pin, uint16_t scl_pin)
        {
            IIC_Object(port, sda_pin, port, scl_pin);
        }
        IIC_Object(GPIO_TypeDef *sda_port, uint16_t sda_pin, GPIO_TypeDef *scl_port, uint16_t scl_pin);

        void sendByte(uint8_t dat);
        void getByte(void);

        template <typename dataType>
        void sendData(dataType dat);
    };

} // namespace IIC

using namespace IIC;

IIC_Object::IIC_Object(GPIO_TypeDef *sda_port, uint16_t sda_pin, GPIO_TypeDef *scl_port, uint16_t scl_pin)
{
    this->scl_port = scl_port;
    this->sda_port = sda_port;
    this->scl_pin = scl_pin;
    this->sda_pin = sda_pin;
    GPIO_InitTypeDef gpio;
    gpio.GPIO_Speed = GPIO_Speed_20MHz;
    gpio.GPIO_Mode = GPIO_Mode_Out_OD;
    gpio.GPIO_Pin = sda_pin;
    GPIO_Init(sda_port, &gpio);
    
    gpio.GPIO_Pin = scl_pin;
    gpio.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_Init(scl_port, &gpio);

    uint8_t sda_pin_num = pin2pinSource(sda_pin);
    if (sda_pin_num >= 8)
    {
        this->CR = (uint32_t *)&(this->sda_port->CRH);
        sda_pin_num -= 8;
    }
    else
        this->CR = (uint32_t *)&(this->sda_port->CRL);
    this->CR_clear = 0xfffffff0 << sda_pin_num;
    this->mode_in = 4 << (4 * sda_pin_num);
    this->mode_out = 5 << (4 * sda_pin_num);
}

void IIC_Object::inputMode(void)
{
    *(this->CR) &= this->CR_clear;
    *(this->CR) |= this->mode_in;
}

void IIC_Object::outputMode(void)
{
    *(this->CR) &= this->CR_clear;
    *(this->CR) |= this->mode_out;
}

void IIC_Object::start(void)
{
}
