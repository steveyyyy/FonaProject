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

#ifndef ROTARY_DIAL
#define ROTARY_DIAL

#include <vector>
#include <algorithm>

#include "../../xf/xf.h"
#include "../../gpio/gpi.h"
#include "../../gpio/intmanager.h"

class RotaryDial : public IReactive, public IntManager::IIntObserver
{
    public:
    class IRotaryObserver
    {
        public:
        virtual void onDigit(int digit) = 0;
    };
    RotaryDial(GPI* wind, GPI* pulse);
    ~RotaryDial();
    
    void initHW();
    bool processEvent(Event* e) override;
    void startBehaviour();
    void onInterrupt(u32_t pin) override;
    void subscribe(IRotaryObserver* subscriber);
    void unsubscribe(IRotaryObserver* subscriber);
    void notify();
    int getActiveId();
    int getNumberId();
    
    typedef enum rotaryEvents{  evPulseDown=100,
                                evPulseUp,
                                evNotify
                             } rotaryEvents;

    typedef enum ROTARYSTATE {  ST_INIT,
                                ST_IDLE,
                                ST_NOTIFY,
                                ST_PULSEDOWN,
                                ST_DEBPULSEDOWN,
                                ST_PULSEUP,
                                ST_DEBPULSEUP,
                             } ROTARYSTATE;

    private:

    GPI* wind;
    GPI* pulse;
    Event ev; //default
    Event pu; //pulseUp
    Event pd; //pulseDown
    Event in; //initial event
    Event tm; //timeoutEvent

    vector<IRotaryObserver*> subscribers;
    ROTARYSTATE state;
    vector<int> pulses;
    
    int pulsesOver;
    int pulseWidth;
    int pulseRange;
    int digit;
    int delay;

    s64_t time_stamp;

    struct k_timer* t;
    static void onTimeout(struct k_timer* t);
};

#endif