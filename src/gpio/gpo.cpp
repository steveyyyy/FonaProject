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

#include "gpo.h"
#include <logging/log.h>
LOG_MODULE_REGISTER(gpo, CONFIG_GPO_LOG_LEVEL);

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
    if (ret != 0) {
		LOG_ERR("Cannot configure GPO");
	}
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
    if (ret != 0) {
		LOG_WRN("Cannot change GPO pin");
	}
}
