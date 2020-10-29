#ifndef DIAL
#define DIAL

#include "../xf/xf.h"
#include <vector>

#include "../hw/rotarydial/rotarydial.h"
#include "../hw/button/button.h"

class Dial :  public IReactive, public RotaryDial::IRotaryObserver, public Button::IButtonObserver
{
    public:
        typedef enum DIALERSTATE{ ST_INIT, 
                                   ST_WAITGREEN, 
                                   ST_DIALING, 
                                //    ST_ADDNUMBER, 
                                   ST_NOTIFY
                                 } DIALERSTATE;

        typedef enum dialerEvents{  evGreenPressed=200,
                                    evRedPressed,
//                                    evOnDigit
                                 } rotaryEvents;
        Dial(Button* buttonAnswer, Button* buttonHangUp);
        ~Dial();
        string getNumber();
        void deleteNumber();
        void onButton(int id, bool pressed);
        void onDigit(int digit);
        bool processEvent(Event* event);
        void startBehaviour();
    private:
        Button* buttonAnswer;
        Button* buttonHangUp;
        string number;
        bool listenOnDigits;
        DIALERSTATE state;
        Event in; //inital event
        Event ev; //default event
        Event gp; //greenpressed event
        Event rp; //redpressed event

};

#endif