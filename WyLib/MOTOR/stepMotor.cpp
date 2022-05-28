#include "motor.hpp"

using namespace motor;

StepMotor::StepMotor(const char *a, const char *b, const char *c, const char *d)
{
    this->a = new GPIO::Gpio_Object(a);
    this->b = new GPIO::Gpio_Object(b);
    this->c = new GPIO::Gpio_Object(c);
    this->d = new GPIO::Gpio_Object(d);

    this->a->reset();
    this->b->reset();
    this->c->reset();
    this->d->reset();
}
void StepMotor::run(uint8_t speed, uint8_t c)
{
    while (c--)
    {
        this->a->set();
        delayMs(speed);
        this->d->reset();
        delayMs(speed);
        this->b->set();
        delayMs(speed);
        this->a->reset();
        delayMs(speed);
        this->c->set();
        delayMs(speed);
        this->b->reset();
        delayMs(speed);
        this->d->set();
        delayMs(speed);
        this->c->reset();
        delayMs(speed);
    }
    this->a->reset();
    this->b->reset();
    this->c->reset();
    this->d->reset();
}
