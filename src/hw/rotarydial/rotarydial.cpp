#include "rotarydial.h"
//Powered by carina
#include <logging/log.h>
LOG_MODULE_REGISTER(rotarydial, CONFIG_ROTARYDIAL_LOG_LEVEL);



RotaryDial::RotaryDial(GPI* wind, GPI* pulse){
    this->wind=wind;
    this->pulse=pulse;

    pulseDelay=3;
    windDelay=7;
    pulsesOver=70;

    this->state = ST_INIT;

    this->ev.setTarget(this);
    this->ev.setDnd(true);
    this->ev.setId(Event::evDefault);

    this->tm.setTarget(this);
    this->tm.setDnd(true);
    // this->tm.setDelay(windDelay);
    this->tm.setId(Event::evTimeout);

    this->pu.setTarget(this);
    this->pu.setDnd(true);
    this->pu.setId((Event::evID)evPulseUp);

    this->pd.setTarget(this);
    this->pd.setDnd(true);
    this->pd.setId((Event::evID)evPulseDown);

    this->in.setTarget(this);
    this->in.setDnd(true);
    this->in.setId(Event::evInitial);

    t=(struct k_timer*) k_malloc(sizeof(struct k_timer));
    k_timer_init(t, &RotaryDial::onTimeout, NULL);
    k_timer_user_data_set(t,&tm);
}
RotaryDial::~RotaryDial() {}

void RotaryDial::initHW() {
    //interruptmanager of number pin
    IntManager::Subscription subNumber;
    subNumber.subscriber=this;
    subNumber.pp.pin=wind->getPin();
    subNumber.pp.dev=wind->getDriver();
    IntManager::getInstance()->subscribe(subNumber);

    //interruptmanager of active pin
    IntManager::Subscription subActive;
    subActive.subscriber=this;
    subActive.pp.pin=pulse->getPin();
    subActive.pp.dev=pulse->getDriver();
    IntManager::getInstance()->subscribe(subActive);
}

bool RotaryDial::processEvent(Event* e) {
    ROTARYSTATE oldState = state;
    bool processed=false;

    switch (state)
    {
    case ST_INIT:
        if(e->getId()==Event::evInitial){
            state=ST_IDLE;
        }
        break;
    case ST_IDLE:
        if(e->getId()==(Event::evID)evPulseDown){
           state=ST_PULSEDOWN;     
        }
        break;
    case ST_PULSEDOWN:
        if(e->getId()==(Event::evID)evPulseUp){
            state=ST_PULSEUP;
        }
        break;
    case ST_PULSEUP:
        if(e->getId()==(Event::evID)evPulseDown){
           state=ST_PULSEDOWN;     
        }
        else if(e->getId()==Event::evTimeout){
           state=ST_NOTIFY;     
        }
        break;
    case ST_NOTIFY:
        if(e->getId()==Event::evDefault){
            state=ST_IDLE;
        }
        break;
    }

    if(oldState!=state){
        processed=true;
        switch (state)
        {
        case ST_INIT:
            break;
        case ST_IDLE:
            break;
        case ST_PULSEDOWN:
            time_stamp = k_uptime_get();
            k_timer_stop(t);
            break;
        case ST_PULSEUP:
            pulses.push_back(k_uptime_delta(&time_stamp));
            k_timer_start(t,K_MSEC(pulsesOver), K_MSEC(0));
            break;
        case ST_NOTIFY:
            notify();
            break;
        }
    }   
    return processed;
}

void RotaryDial::onTimeout(struct k_timer* t){
    LOG_INF("Timer timeout");
    Event* tm;
    tm = static_cast<Event*>(k_timer_user_data_get(t));
    XF::getInstance()->pushEvent(tm);
}

void RotaryDial::onInterrupt(u32_t pin) {
if(pin==(u32_t)pulse->getPin()){
        if(pulse->read()==GPIO::PIN_ON){      
            XF::getInstance()->pushEvent(&pu);
        }
        else{
            XF::getInstance()->pushEvent(&pd);
        }
    }
}

void RotaryDial::subscribe(IRotaryObserver* subscriber) {
        vector<IRotaryObserver*>::iterator it;
    it = find(subscribers.begin(), subscribers.end(),subscriber);
    if (it == subscribers.end())
    {
        subscribers.push_back(subscriber);
    }
}

void RotaryDial::unsubscribe(IRotaryObserver* subscriber) {
        vector<IRotaryObserver*>::iterator it;
    it = find(subscribers.begin(), subscribers.end(),subscriber);
    if (it != subscribers.end())
    {
        subscribers.erase(it);
    }
}

void RotaryDial::notify() {
    vector<IRotaryObserver*>::iterator it;
    for (it=subscribers.begin(); it!=subscribers.end();++it)
    {
        (*it)->onPulses(pulses);
    }
}

int RotaryDial::getActiveId(){
    return wind->getUId();
}

int RotaryDial::getNumberId(){
    return pulse->getUId();
}

void RotaryDial::startBehaviour()
{
    XF::getInstance()->pushEvent(&in);
}