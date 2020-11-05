#ifndef DIAL
#define DIAL

#include "../xf/xf.h"
#include <vector>

#include "../hw/rotarydial/rotarydial.h"
#include "../hw/button/button.h"
#include "../hw/led/led.h"

class Dial :  public IReactive, public RotaryDial::IRotaryObserver, public Button::IButtonObserver
{
    public:
        typedef enum DIALERSTATE{ ST_INIT, 
                                   ST_WAITHOOKUP, 
                                   ST_DIALING,
                                   ST_VALIDATEDIGIT,
                                   ST_NOTIFY
                                 } DIALERSTATE;

        typedef enum dialerEvents{  evHookUp=200,
                                    evHookDown,
                                    evOnDigit,
                                    evNotify,
                                 } rotaryEvents;
        Dial(Button* switchhook, LED* ledGreen, LED* ledRed);
        ~Dial();
        string getNumber();
        void deleteNumber();
        void onButton(int id, bool pressed);
        void onDigit(int digit);
        bool processEvent(Event* event);
        void startBehaviour();
    private:
        Button* switchhook;
        LED* ledGreen;
        LED* ledRed;
        string number;
        bool listenOnDigits;
        DIALERSTATE state;
        Event in; //inital event
        Event ev; //default event
        Event hu; //hookup event
        Event hd; //hookdown event
        Event od; //OnDigit event
        Event nf; //Notify event
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