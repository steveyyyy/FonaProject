#include "led.h"

/**
 * @brief Construct a new LED object
 * 
 * @param p1 Pin number
 * @param port Port name
 */
LED::LED(int p1, const char *port) : pin(p1, port)
{
}

/**
 * @brief Destroy the LED object
 * 
 */
LED::~LED()
{
}

/**
 * @brief Initialize led pin
 * 
 */
void LED::initHW()
{
    pin.setInitialOff();
    pin.setLowPower();
    // pin.setOpenSource();
    pin.initHW();
}

/**
 * @brief Turn led on
 * 
 */
void LED::on()
{
    pin.on();
}

/**
 * @brief Turn led off
 * 
 */
void LED::off()
{
    pin.off();
}
