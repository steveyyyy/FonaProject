#ifndef FACTORY_ONCE
#define FACTORY_ONCE

#include "../xf/xf.h"
#include "../hw/rotarydial/rotarydial.h"
#include "../hw/led/led.h"
#include "../hw/button/button.h"
#include "../controller/dial.h"
#include "../uart/uart.h"
#include "../hw/fona/fona.h"

class Factory
{
public:
    static void init();
    static void build();
    static void start();

    static GPI* pWindUp() {return &_pinWindUp;}
    static GPI* pPulse() {return &_pinPulse;}
    static RotaryDial* rotary(){return &_rotaryDial;}

    static LED* ledGreen() {return &_ledGreen;}
    static LED* ledRed() {return &_ledRed;}

    static GPI* pSwitchhook(){return &_pinSwitchhook;}

    static Button* switchhook(){return &_switchhook;}
    static Dial* dialer(){return &_dialer;}

    static Fona* fona(){return &_fona;}

    static XF* xf(){return XF::getInstance();}
    static IntManager* im(){return IntManager::getInstance();}
    
private:
    Factory(/* args */);
    ~Factory(); 

private:
    static RotaryDial _rotaryDial;
    static GPI _pinWindUp;
    static GPI _pinPulse;

    static LED _ledGreen;
    static LED _ledRed;

    static GPI _pinSwitchhook;
    static Button _switchhook;

    static Dial _dialer;

    static Fona _fona;
};

#endif