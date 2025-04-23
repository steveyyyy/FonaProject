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

#ifndef GPO_ONCE
#define GPO_ONCE

/**
 * @class General Purpose Output 
 * @brief This class implements the attributes of a General Purpose Output
 */
class GPO : public GPIO
{
public:

    GPO(int p1, const char* port);
    virtual ~GPO();
    void setHighPower();
    void setLowPower();
    void setInitialOn();
    void setInitialOff();
    void setOpenDrain();
    void setOpenSource();
    void initHW();
    void on();
    void off();
    void set(PINSTATE state);

protected:

    enum FORCE {FORCENONE,WEEK,STRONG};             /**< enum amount of force */
    enum MODE  {MODENONE, OPENDRAIN, OPENSOURCE};   /**< enum mode of output */
    PINSTATE initialState;                          /**< PINSTATE state of output */
    FORCE strength;                                 /**< FORCE amount of force */
    MODE mode;                                      /**< MODE mode of output */
};

#endif
