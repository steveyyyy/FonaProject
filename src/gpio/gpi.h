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
#include <drivers/gpio.h>
#include "gpio.h"
#include "../xf/xf.h"
#include <map>
#include <vector>
using namespace std;

#ifndef GPI_ONCE
#define GPI_ONCE

/**
 * @class General Purpose Input
 * @brief This class implements the attributes and methods of a General Purpose Input
 */
class GPI : public GPIO
{
public:

    GPI(int p1, const char* port);
    virtual ~GPI();
    void initHW();
    void setPullup();
    void setPulldown();
    GPIO::PINSTATE read();

protected:
    enum MODE  {PULLUP, PULLDOWN};  /**< enum mode of input (pullup/pulldown) */
    MODE mode;                      /**< mode of input */
};

#endif
