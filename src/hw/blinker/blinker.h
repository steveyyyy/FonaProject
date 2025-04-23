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

#include "../../xf/xf.h"
#include "../../xf/ireactive.h"
#include "../../xf/event.h"
#include "../led/led.h"

#ifndef BLINKER_ONCE
#define BLINKER_ONCE

class Blinker : public IReactive
{
public:
    explicit Blinker(int blinkDelay);
    Blinker(int blinkDelayOn, int blinkDelayOff);
    ~Blinker(); 
    void start();
    void stop();
    void setLed(LED* led);
    void setDelayOn(int delay);
    void setDelayOff(int delay);
    bool processEvent(Event* e) override;
    void startBehaviour();

private:
    LED* led;
    typedef enum BLINKER_STATES{ ST_NONE, ST_INIT, ST_ON, ST_OFF }BLINKER_STATES;   /**< blinker states */
    Event in;
    Event tm;
    BLINKER_STATES state;                                                           /**< state of blinker */
    int blinkDelayOn, blinkDelayOff;                                                /**< on/off delays */
    bool diverseBlink;                                                              /**< differ between regular or different on/off timings */
};

#endif