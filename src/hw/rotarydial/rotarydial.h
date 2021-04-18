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
        virtual void onPulses(vector<int> pulses) = 0;
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
                                ST_PULSEUP,
                             } ROTARYSTATE;

    private:

    GPI* wind;
    GPI* pulse;
    Event ev; //default
    Event pu; //pulseUp
    Event pd; //pulseDown
    Event in; //initial event
    Event tm;//timeoutEvent

    vector<IRotaryObserver*> subscribers;
    ROTARYSTATE state;
    vector<int> pulses;
    
    int pulseDelay;
    int windDelay;
    int pulsesOver;

    s64_t time_stamp;

    struct k_timer* t;
    static void onTimeout(struct k_timer* t);
};

#endif