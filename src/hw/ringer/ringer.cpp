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

#include "ringer.h"

Ringer::Ringer(int p1, const char *port, float hertz, float prozent) : pin(p1, port){
    this->pulseUp= (1000/hertz)*prozent;
    this->pulseDown= (1000/hertz)*(1-prozent);
    oscillator=(struct k_timer*) k_malloc(sizeof(struct k_timer));
    k_timer_init(oscillator, &Ringer::oscillatorToggle, NULL);
}

Ringer::~Ringer(){}

void Ringer::initHW(){
    pin.setInitialOff();
    pin.setLowPower();
    pin.initHW();
    pinState=false;
    k_timer_user_data_set(oscillator,this);
}
void Ringer::stop(){
    k_timer_stop(oscillator);
    pinState=false;
    pin.off();
}
void Ringer::ring(){
    k_timer_start(oscillator,K_MSEC(pulseUp), K_MSEC(pulseUp));
}

void Ringer::oscillatorToggle(struct k_timer* oscillator){
    Ringer* thisRinger = static_cast<Ringer*>(k_timer_user_data_get(oscillator));
    if(thisRinger->pinState){
        thisRinger->pinState=false;
        thisRinger->pin.off();
        k_timer_start(oscillator,K_MSEC(thisRinger->pulseDown), K_MSEC(thisRinger->pulseDown));
    }else{
        thisRinger->pinState=true;
        thisRinger->pin.on();
        k_timer_start(oscillator,K_MSEC(thisRinger->pulseUp), K_MSEC(thisRinger->pulseUp));
    }
}