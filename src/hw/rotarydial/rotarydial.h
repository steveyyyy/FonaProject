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
    RotaryDial(GPI* active, GPI* number);
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
    //int getDigit();
    
    typedef enum rotaryEvents{  evWindUp=100,
                                evWindDown,
                                evPulseDown,
                                evPulseUp,
                                evNotify
                             } rotaryEvents;

    typedef enum ROTARYSTATE {  ST_INIT,
                                ST_WAITDIAL,
                                ST_DEBWINDUP,
                                ST_COUNTING,   
                                ST_DEBWINDDOWN,
                                ST_DECIDENOTIFY,
                                ST_NOTIFY,
                                ST_DEBPULSEDOWN,
                                ST_WAITPULSEUP,
                                ST_DEBPULSEUP,
                                ST_COUNT,
                             } ROTARYSTATE;

    private:
    GPI* active;
    GPI* number;
    Event ev; //default
    Event wu;//windup event
    Event wd; //winddow event
    Event pu; //pulseUp
    Event pd; //pulseDown
    Event in; //initial event
    Event tm;//timeoutEvent
    Event nf; //notify
    vector<IRotaryObserver*> subscribers;
    ROTARYSTATE state;
    int digit;

    int pulseDelay;
    int windDelay;
};

#endif