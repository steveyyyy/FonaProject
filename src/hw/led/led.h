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

#ifndef LED_ONCE
#define LED_ONCE

#include <zephyr.h>
#include "../../gpio/gpo.h"

/**
 * @class LED
 * @brief This class represents a LED 
 */
class LED
{
    public:
    LED(int p1, const char* port);
    ~LED();
    void initHW();
    void on();
    void off();

    private: 
    GPO pin; /**< General Purpose Output pin */
};
#endif