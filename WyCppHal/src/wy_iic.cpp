#include "wy_iic.hpp"

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
    // gpio.GPIO_Mode = GPIO_Mode_Out_PP;
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

void IIC_Object::iic_delay(void)
{
    __IO uint8_t i = 5;
    while (i--)
        ;//__nop();
}

bool IIC_Object::start(void)
{
    this->outputMode();
    GPIO_SetBits(this->sda_port, this->sda_pin);
    if (!GPIO_ReadInputDataBit(this->sda_port, this->sda_pin))
        return false;
    GPIO_SetBits(this->scl_port, this->scl_pin);
    iic_delay();
    GPIO_ResetBits(this->sda_port, this->sda_pin); // START:when CLK is high,DATA change form high to low
    if (GPIO_ReadInputDataBit(this->sda_port, this->sda_pin))
        return false;
    iic_delay();
    GPIO_ResetBits(this->scl_port, this->scl_pin); //钳住I2C总线，准备发送或接收数据
    return true;
}

void IIC_Object::end(void)
{
    outputMode(); // sda线输出
    GPIO_ResetBits(this->scl_port, this->scl_pin);
    GPIO_ResetBits(this->sda_port, this->sda_pin); // STOP:when CLK is high DATA change form low to high
    iic_delay();
    GPIO_SetBits(this->scl_port, this->scl_pin);
    GPIO_SetBits(this->sda_port, this->sda_pin); //发送I2C总线结束信号
    iic_delay();
}

void IIC_Object::ack(bool val)
{
    GPIO_ResetBits(this->scl_port, this->scl_pin);
    outputMode();
    if (val)
        GPIO_ResetBits(this->sda_port, this->sda_pin);
    else
        GPIO_SetBits(this->sda_port, this->sda_pin);
    iic_delay();
    GPIO_SetBits(this->scl_port, this->scl_pin);
    iic_delay();
    GPIO_ResetBits(this->scl_port, this->scl_pin);
}

bool IIC_Object::wait_ack(void)
{
    uint8_t ucErrTime = 0;
    inputMode(); // SDA设置为输入
    GPIO_SetBits(this->sda_port, this->sda_pin);
    iic_delay();
    GPIO_SetBits(this->scl_port, this->scl_pin);
    iic_delay();
    while (GPIO_ReadInputDataBit(this->sda_port, this->sda_pin))
    {
        ucErrTime++;
        if (ucErrTime > 100)
        {
            end(); //
            return false;
        }
        iic_delay();
    }
    GPIO_ResetBits(this->scl_port, this->scl_pin); //时钟输出0
    return true;
}

void IIC_Object::sendByte(uint8_t dat)
{
    uint8_t t;
    outputMode();
    GPIO_ResetBits(this->scl_port, this->scl_pin); //拉低时钟开始数据传输
    for (t = 0; t < 8; t++)
    {
        if ((dat & 0x80) >> 7)
            GPIO_SetBits(this->sda_port, this->sda_pin);
        else
            GPIO_ResetBits(this->sda_port, this->sda_pin);
        dat <<= 1;
        iic_delay(); //对TEA5767这三个延时都是必须的
        GPIO_SetBits(this->scl_port, this->scl_pin);
        iic_delay();
        GPIO_ResetBits(this->scl_port, this->scl_pin);
        iic_delay();
    }
}

uint8_t IIC_Object::getByte(bool a)
{
    uint8_t i, receive = 0;
    inputMode(); // SDA设置为输入
    for (i = 0; i < 8; i++)
    {
        GPIO_ResetBits(this->scl_port, this->scl_pin);
        iic_delay();
        GPIO_SetBits(this->scl_port, this->scl_pin);
        receive <<= 1;
        if (GPIO_ReadInputDataBit(this->sda_port, this->sda_pin))
            receive++;
        iic_delay();
    }
    ack(a);
    return receive;
}

bool IIC_Object::sendData(uint8_t slvAdd, uint8_t regAdd, uint8_t len, uint8_t *dat)
{
    if (this->start())
        return !false;
    this->sendByte(slvAdd << 1);
    if (!this->wait_ack())
    {
        this->end();
        return !false;
    }
    this->sendByte(regAdd);
    this->wait_ack();
    while (len--)
    {
        this->sendByte(*dat++);
        if (!this->wait_ack())
        {
            this->end();
            return !false;
        }
    }
    this->end();
    return !true;
}

bool IIC_Object::sendData(uint8_t slvAdd, uint8_t regAdd, uint8_t dat)
{
    if (this->start())
        return !false;
    this->sendByte(slvAdd << 1);
    if (!this->wait_ack())
    {
        this->end();
        return !false;
    }
    this->sendByte(regAdd);
    this->wait_ack();
    this->sendByte(dat);
    if (!this->wait_ack())
    {
        this->end();
        return !false;
    }
    this->end();
    return !true;
}

bool IIC_Object::readData(uint8_t slvAdd, uint8_t regAdd, uint8_t len, uint8_t *buf)
{
    if (!this->start())
        return !false;
    this->sendByte(slvAdd << 1);
    if (!this->wait_ack())
    {
        this->end();
        return !false;
    }
    this->sendByte(regAdd);
    this->wait_ack();
    this->start();
    this->sendByte((slvAdd << 1) + 1);
    this->wait_ack();
    while (--len)
        *buf++ = this->getByte(true);
    *buf++ = this->getByte(false);
    this->end();
    return !true;
}

uint8_t IIC_Object::readData(uint8_t slvAdd, uint8_t regAdd)
{
    uint8_t r;
    this->start();
    this->sendByte(slvAdd << 1);
    this->wait_ack();
    this->sendByte(regAdd);
    this->wait_ack();
    this->start();
    this->sendByte((slvAdd << 1) + 1);
    this->wait_ack();
    r = this->getByte(false);
    this->end();
    return r;
}
