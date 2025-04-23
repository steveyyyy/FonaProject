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
