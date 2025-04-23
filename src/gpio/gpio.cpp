/* 
    FonaProject – Retro Rotary Phone Meets 3G
    Copyright (C) 2025  steveyyyy

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

/* this is the Button class */
#include "gpio.h"
#include <string.h>
#include <logging/log.h>
LOG_MODULE_REGISTER(gpio, CONFIG_GPI_GPO_LOG_LEVEL);

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
    LOG_INF("driver of pin %d is %s", this->pin, this->driver->name);
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
    return this->id;
}

/**
 * @brief Get Driver of this GPIO Object
 * 
 * @return Driver as a device structure
 */
const struct device* GPIO::getDriver()
{
    return this->driver;
}
