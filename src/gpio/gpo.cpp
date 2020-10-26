#include "gpo.h"

/**
 * @brief Construct a new GPO object
 * 
 * @param p1 Pin number
 * @param port Port name
 */
GPO::GPO(int p1, const char* port) : GPIO(p1, port)
{;
    this->config = 0;
    this->initialState = PIN_OFF;
    this->strength = FORCENONE;
    this->mode = MODENONE;
}

/**
 * @brief Destroy the GPO object
 * 
 */
GPO::~GPO()
{
}

/**
 * @brief Initialize config based on inital state, amount of force and output mode
 * 
 */
void GPO::initHW()
{
    GPIO::initHW();

    switch (this->initialState)
    {
        case PIN_OFF:
        this->config = GPIO_OUTPUT_LOW;
        break;
        case PIN_ON:
        this->config = GPIO_OUTPUT_HIGH;
        break;
        default:
        break;
    }
    
    switch (this->strength)
    {
        case WEEK:
        this->config = this->config | GPIO_DS_DFLT_LOW;
        break;
        case STRONG:
        this->config = this->config | GPIO_DS_DFLT_HIGH;
        break;
        default:
        break;
    }
    switch (this->mode)
    {
        case OPENDRAIN:
        this->config = this->config | GPIO_OPEN_DRAIN;
        break;
        case OPENSOURCE:
        this->config = this->config | GPIO_OPEN_SOURCE;
        break;
        default:
        break;
    }
    int ret = 0;
    ret = gpio_pin_configure(this->driver,this->pin,this->config);
}

/**
 * @brief Set amount of power to high
 * 
 */
void GPO::setHighPower()
{
    this->strength = STRONG;
}

/**
 * @brief Set amount of power to weak 
 * 
 */
void GPO::setLowPower()
{
    this->strength = WEEK;
}

/**
 * @brief Set inital state to on
 * 
 */
void GPO::setInitialOn()
{
    this->initialState = PIN_ON;
}

/**
 * @brief Set initial state to off
 * 
 */
void GPO::setInitialOff()
{
    this->initialState = PIN_OFF;
}

/**
 * @brief Set output mode to opendrain
 * 
 */
void GPO::setOpenDrain()
{
    this->mode = OPENDRAIN;
}

/**
 * @brief Set output mode to opensource
 * 
 */
void GPO::setOpenSource()
{
    this->mode = OPENSOURCE;
}

/**
 * @brief Turn output pin on
 * 
 */
void GPO::on()
{
    gpio_pin_set(this->driver,this->pin,PIN_ON);
}

/**
 * @brief Turn output pin off
 * 
 */
void GPO::off()
{
    gpio_pin_set(this->driver,this->pin,PIN_OFF);
}

/**
 * @brief Set ouput pin state
 * 
 * @param state of the output pin 
 */
void GPO::set(PINSTATE state)
{
    int ret = 0;
    ret =   gpio_pin_set(this->driver,this->pin,state);
}
