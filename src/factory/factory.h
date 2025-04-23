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

#ifndef FACTORY_ONCE
#define FACTORY_ONCE

#include "../xf/xf.h"
#include "../hw/rotarydial/rotarydial.h"
#include "../hw/button/button.h"
#include "../controller/dial.h"
#include "../uart/uart.h"
#include "../hw/fona/fona.h"
#include "../hw/ringer/ringer.h"

class Factory
{
public:
    static void init();
    static void build();
    static void start();

    static GPI* pWindUp() {return &_pinWindUp;}
    static GPI* pPulse() {return &_pinPulse;}
    static RotaryDial* rotary(){return &_rotaryDial;}

    static GPI* pSwitchhook(){return &_pinSwitchhook;}

    static Button* switchhook(){return &_switchhook;}
    static Dial* dialer(){return &_dialer;}

    static UART* uart(){return &_uart;}
    static Fona* fona(){return &_fona;}

    static Ringer* ringer(){return &_ringer;}

    static XF* xf(){return XF::getInstance();}

    static gpio_callback* cbPortA(){return &_cbPortA;}
    static gpio_callback* cbPortB(){return &_cbPortB;}
    //static IntManager* im(){return IntManager::getInstance();}

    static void onInterrupt(const struct device* dev, struct gpio_callback* cb, u32_t pins);
    
private:
    Factory(/* args */);
    ~Factory(); 

private:
    static RotaryDial _rotaryDial;
    static GPI _pinWindUp;
    static GPI _pinPulse;

    static GPI _pinSwitchhook;
    static Button _switchhook;

    static Dial _dialer;

    static UART _uart;

    static Fona _fona;

    static Ringer _ringer;

    static struct gpio_callback _cbPortA;
    static struct gpio_callback _cbPortB;
};

#endif