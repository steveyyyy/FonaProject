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

#include "gpi.h"
#include <logging/log.h>
LOG_MODULE_REGISTER(gpi, CONFIG_GPI_LOG_LEVEL);

//initialize statics
/**
 * @brief Initialize a new GPI object 
 * 
 * @param p1 Pin Number
 * @param port Port Name
 */
GPI::GPI(int p1, const char* port) : GPIO(p1, port)
{
    this->config = GPIO_INPUT | GPIO_INT_DEBOUNCE;
    this->mode = PULLDOWN;
}

/**
 * @brief Destroy the GPI object
 * 
 */
GPI::~GPI()
{
}

/**
 * @brief Initialize config based on input mode
 * 
 */
void GPI::initHW()
{
    int ret = 0;
    GPIO::initHW();

    switch (this->mode)
    {
        case PULLDOWN:
        this->config = this->config | GPIO_PULL_DOWN;
        break;
        case PULLUP:
        this->config = this->config | GPIO_PULL_UP;
        break;
        default:
        break;
    }
    ret = gpio_pin_configure(this->driver,this->pin,this->config);
    if (ret != 0) {
		LOG_ERR("Cannot configure GPI");
	}
}

/**
 * @brief Set mode to PULLDOWN
 * 
 */
void GPI::setPulldown()
{
    this->mode = PULLDOWN;
}

/**
 * @brief Set mode to PULLUP
 * 
 */
void GPI::setPullup()
{
    this->mode = PULLUP;
}

/**
 * @brief Read current pin state 
 * 
 * @return GPIO::PINSTATE (PIN_ON/PIN_OFF)
 */
GPIO::PINSTATE GPI::read()
{
    GPIO::PINSTATE retval;
    if (gpio_pin_get(this->driver,this->pin))
    {
        retval = PIN_ON;
    }
    else
    {
        retval = PIN_OFF;
    }
    return retval;
}
