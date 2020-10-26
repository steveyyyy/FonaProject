#ifndef FACTORY_ONCE
#define FACTORY_ONCE

#include "../xf/xf.h"
#include "../hw/rotarydial/rotarydial.h"
#include "../hw/led/led.h"
#include "../hw/button/button.h"

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

    static GPI* pAnswer(){return &_pinAnswer;}
    static GPI* pHangUp(){return &_pinHangUp;}
    static Button* btnAnswer(){return &_btnAnswer;}
    static Button* btnHangUp(){return &_btnHangUp;}

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

    static GPI _pinAnswer;
    static GPI _pinHangUp;
    static Button _btnAnswer;
    static Button _btnHangUp;
};

#endif