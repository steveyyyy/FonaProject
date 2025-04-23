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

#include "../../gpio/gpo.h"

#ifndef RINGER_ONCE
#define RINGER_ONCE

class Ringer
{
public:
    Ringer(int p1, const char *port, float hertz=16, float prozent=0.3333);
    ~Ringer();
    void initHW();
    void stop();
    void ring();
private:
    GPO pin;
    float pulseUp;
    float pulseDown;
    bool pinState;

    struct k_timer* oscillator;
    static void oscillatorToggle(struct k_timer* oscillator);
};
#endif