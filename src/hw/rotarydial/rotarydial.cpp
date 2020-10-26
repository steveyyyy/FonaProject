#include "rotarydial.h"
//Powered by carina



RotaryDial::RotaryDial(int activePin, int numberPin, const char* activePort, const char* numberPort): active(activePin,activePort), number(numberPin,numberPort) {
    delay=64;
    this->state = ST_INIT;

    this->ev.setTarget(this);
    this->ev.setDnd(true);
    this->ev.setId(Event::evDefault);

    this->tm.setTarget(this);
    this->tm.setDnd(true);
    this->tm.setDelay(delay);
    this->tm.setId(Event::evTimeout);

    this->wd.setTarget(this);
    this->wd.setDnd(true);
    this->wd.setId((Event::evID)evWindDown);

    this->wu.setTarget(this);
    this->wu.setDnd(true);
    this->wu.setId((Event::evID)evWindUp);

    this->pl.setTarget(this);
    this->pl.setDnd(true);
    this->pl.setId((Event::evID)evPulse);

    this->in.setTarget(this);
    this->in.setDnd(true);
    this->in.setId(Event::evInitial);
}
RotaryDial::~RotaryDial() {}

void RotaryDial::initHW() {
    active.setPulldown();
    number.setPulldown();

    active.initHW();
    number.initHW();

    //interruptmanager of number pin
    IntManager::Subscription subNumber;
    subNumber.subscriber=this;
    subNumber.pp.pin=number.getPin();
    subNumber.pp.dev=number.getDriver();
    IntManager::getInstance()->subscribe(subNumber);

    //interruptmanager of active pin
    IntManager::Subscription subActive;
    subActive.subscriber=this;
    subActive.pp.pin=active.getPin();
    subActive.pp.dev=active.getDriver();
    IntManager::getInstance()->subscribe(subActive);
}

bool RotaryDial::processEvent(Event* e) {
    ROTARYSTATE oldState = state;
    bool processed=false;

    switch (state)
    {
    case ST_INIT:
        if(e->getId()==Event::evInitial){
            state=ST_WAITDIAL;
        }
        break;
    case ST_WAITDIAL:
        if(e->getId()==(Event::evID)evWindUp){
           state=ST_DEBWINDUP;     
        }
        break;
    case ST_DEBWINDUP:
        if(e->getId()==Event::evTimeout){
            state=ST_COUNTING;
        }
        break;
    case ST_COUNTING:
        if(e->getId()==(Event::evID)evWindDown){
            state=ST_DEBWINDDOWN;
        }
        if(e->getId()==(Event::evID)evPulse){
            state=ST_DEBPULSE;
        }
        break;
    case ST_DEBWINDDOWN:
        if(e->getId()==Event::evTimeout){
            state=ST_NOTIFY;
        }
        break;
    case ST_NOTIFY:
        if(e->getId()==Event::evDefault){
            state=ST_WAITDIAL;
        }
        break;
    case ST_DEBPULSE:
        if(e->getId()==Event::evTimeout){
            state=ST_COUNT;
        }
        break;
    case ST_COUNT:
        if(e->getId()==Event::evDefault){
            state=ST_COUNTING;
        }
        else if(e->getId()==(Event::evID)evWindDown){
            state=ST_DEBWINDDOWN;
        } 
        break;
    }

    if(oldState!=state){
        processed=true;
        switch (state)
        {
        case ST_INIT:
            printk("INIT\n");
            break;
        case ST_WAITDIAL:
            printk("WAITDIAL\n");
            break;  
        case ST_DEBWINDUP:
            printk("DEBWINDUP\n");
            digit=0;
            this->tm.setDelay(delay);
            XF::getInstance()->pushEvent(&tm);
            break;
        case ST_COUNTING:
            printk("COUNTING\n");
            break;
        case ST_DEBWINDDOWN:
            printk("DEBWINDOWN\n");
            this->tm.setDelay(delay);
            XF::getInstance()->pushEvent(&tm);
            break;
        case ST_NOTIFY:
            printk("NOTIFY\n");
            XF::getInstance()->pushEvent(&ev);
            notify();
            break;
        case ST_DEBPULSE:
            printk("DEBPULSE\n");
            this->tm.setDelay(delay);
            XF::getInstance()->pushEvent(&tm);
            break;
        case ST_COUNT:
            printk("COUNT\n");
            digit++;
            if(digit==10){
                digit=0;
            }
            printk("%d\n",digit);
            XF::getInstance()->pushEvent(&ev);
            break;
        }
    }   
    return processed;
}

void RotaryDial::onInterrupt(u32_t pin) {
    if(pin==(u32_t)active.getPin()){
        if(active.read()==GPIO::PIN_ON){      
            XF::getInstance()->pushEvent(&wu);
        }
        else{     
            XF::getInstance()->pushEvent(&wd);
        }
    }
    if(pin==(u32_t)number.getPin()){
        if(number.read()==GPIO::PIN_OFF){      
            XF::getInstance()->pushEvent(&pl);
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
        (*it)->onDigit(this->digit);
    }
}

int RotaryDial::getActiveId(){
    return active.getUId();
}

int RotaryDial::getNumberId(){
    return number.getUId();
}

void RotaryDial::startBehaviour()
{
    XF::getInstance()->pushEvent(&in);
}