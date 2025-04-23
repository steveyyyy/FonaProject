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

#ifndef DIAL
#define DIAL

#include "../xf/xf.h"
#include <vector>

#include "../hw/rotarydial/rotarydial.h"
#include "../hw/button/button.h"
#include "../hw/fona/fona.h"
#include "../hw/ringer/ringer.h"

class Dial :  public IReactive, public RotaryDial::IRotaryObserver, public Button::IButtonObserver, public Fona::IFonaObserver
{
    public:
        typedef enum DIALERSTATE{   ST_INIT,
                                    ST_SETUP,
                                    ST_WAITOK,
                                    ST_CHECKLOCK,
                                    ST_LOCKED,
                                    ST_UNLOCK,
                                    ST_IDLE, 
                                    ST_DIALING,
                                    ST_VALIDATEDIGIT,
                                    ST_DIAL,
                                    ST_INCALL,
                                    ST_ENDCALL,
                                    ST_RING,
                                    ST_RINGIDLE,
                                    ST_TAKECALL
                                } DIALERSTATE;

        typedef enum dialerEvents{  evError=300,
                                    evLocked,
                                    evNoLock,
                                    evHookUp,
                                    evHookDown,
                                    evOnDigit,
                                    evDial,
                                    evCallReject,
                                    evRing,
                                    evRingStop,
                                    evRingIdle
                                 } dialerEvents;
        Dial(Button* switchhook, Fona* fona, Ringer* ring);
        ~Dial();
        string getNumber();
        void deleteNumber();
        void onButton(int id, bool pressed) override;
        void onDigit(int digit) override;
        bool processEvent(Event* event) override;
        void startBehaviour();
        void onResponse(char * text) override;

    private:
        void playDialingTone();
        Button* switchhook;
        Fona* fona;
        Ringer* ring;
        string number;
        bool listenOnDigits;
        DIALERSTATE state;
        Event lo; //Lock event
        Event in; //inital event
        Event ev; //default event
        Event hu; //hookup event
        Event hd; //hookdown event
        Event od; //OnDigit event
        Event dl; //Dial event
        Event cr; //Call rejected event
        Event rg; //Ring event
        Event rs; //Stop Ring event
        Event ri; //Ring Idle event
        string emergencyNumbers[6][2]={ {"112","Notruf"},
                                        {"117","Polizei"},
                                        {"118","Feuerwehr"},
                                        {"144","Sanität"},
                                        {"1414","REGA"},
                                        {"145","Toxinfo"}};
        struct k_timer* t;
        static void onTimeout(struct k_timer* t);
};

#endif