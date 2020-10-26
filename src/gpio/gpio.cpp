/* this is the Button class */
#include "gpio.h"
#include <string.h>

u8_t GPIO::uid = 0; /**< Initalize unique ID with 0*/

/**
 * @brief Construct a new GPIO object 
 * 
 * @param p1 Pin Number
 * @param port Port Name
 */
GPIO::GPIO(int p1, const char* port)
{
    this->id = this->uid++;
    this->pin = p1;
    this->driver = NULL;
    this->port = (char*) k_malloc(strlen(port)+1);
    strcpy(this->port, port);
    this->config =0;

}

/**
 * @brief Destroy the GPIO object
 * 
 */
GPIO::~GPIO()
{
    k_free (this->port);
}

/**
 * @brief Initialize the Driver
 * 
 */
void GPIO::initHW()
{
    this->driver = device_get_binding(this->port);
    //printk("driver of pin %d is %x\n", this->pin, this->driver);
}

/**
 * @brief Get Pin Number
 * 
 * @return int 
 */
int GPIO::getPin()
{
    return this->pin;
}

/**
 * @brief Get Uid of this GPIO Object
 * 
 * @return int
 */
int GPIO::getUId()
{
    return this->uid;
}

/**
 * @brief Get Driver of this GPIO Object
 * 
 * @return Driver as a device structure
 */
struct device* GPIO::getDriver()
{
    return this->driver;
}
