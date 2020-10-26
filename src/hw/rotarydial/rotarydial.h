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
    RotaryDial(int activePin, int numberPin, const char* activePort, const char* numberPort);
    ~RotaryDial();
    
    void initHW();
    bool processEvent(Event* e);
    void startBehaviour();
    void onInterrupt(u32_t pin);
    void subscribe(IRotaryObserver* subscriber);
    void unsubscribe(IRotaryObserver* subscriber);
    void notify();
    int getActiveId();
    int getNumberId();
    //int getDigit();
    
    typedef enum rotaryEvents{  evWindUp=100,
                                evWindDown,
                                evPulse
                             } rotaryEvents;

    typedef enum ROTARYSTATE {  ST_INIT,
                                ST_WAITDIAL,
                                ST_DEBWINDUP,
                                ST_COUNTING,   
                                ST_DEBWINDDOWN,
                                ST_NOTIFY,
                                ST_DEBPULSE,
                                ST_COUNT,
                             } ROTARYSTATE;

    private:
    GPI active;
    GPI number;
    Event ev; //default
    Event wu;//windup event
    Event wd; //winddow event
    Event pl; //pulseevent
    Event in; //initial event
    Event tm;//timeoutEvent
    vector<IRotaryObserver*> subscribers;
    ROTARYSTATE state;
    int digit;

    int delay;
};

#endif