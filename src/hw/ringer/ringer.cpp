#include "ringer.h"

Ringer::Ringer(int p1, const char *port, int hertz, int ringTime) : pin(p1, port){
    this->hertz= (1000/hertz)/2;
    if((ringTime<=4)&&(ringTime>0)){
        this->ringTime=(1000*ringTime);
    }
    else{
        this->ringTime=2000;//sets ring to default 2 sec
    }

    oscillator=(struct k_timer*) k_malloc(sizeof(struct k_timer));
    k_timer_init(oscillator, &Ringer::oscillatorToggle, NULL);
    ringTimer=(struct k_timer*) k_malloc(sizeof(struct k_timer));
    k_timer_init(ringTimer, &Ringer::ringTimerTimeout, &Ringer::ringTimerTimeout);
}

Ringer::~Ringer(){}

void Ringer::initHW(){
    pin.setInitialOff();
    pin.setLowPower();
    pin.initHW();
    pinState=false;
    k_timer_user_data_set(oscillator,this);
    k_timer_user_data_set(ringTimer,this);
}
void Ringer::stop(){
    k_timer_stop(ringTimer);
}
void Ringer::ring(){
    k_timer_start(ringTimer,K_MSEC(2000), K_MSEC(0));
    k_timer_start(oscillator,K_MSEC(hertz), K_MSEC(hertz));
}

void Ringer::oscillatorToggle(struct k_timer* oscillator){
    Ringer* thisRinger = static_cast<Ringer*>(k_timer_user_data_get(oscillator));
    if(thisRinger->pinState){
        thisRinger->pinState=false;
        thisRinger->pin.off();
    }else{
        thisRinger->pinState=true;
        thisRinger->pin.on();
    }
}

void Ringer::ringTimerTimeout(struct k_timer* ringTimer){
    Ringer* thisRinger = static_cast<Ringer*>(k_timer_user_data_get(ringTimer));
    k_timer_stop(thisRinger->oscillator);
    thisRinger->pinState=false;
    thisRinger->pin.off();
    thisRinger->notify();
}

void Ringer::subscribe(IRingerObserver* subscriber)
{
    vector<IRingerObserver*>::iterator it;
    it = find(subscribers.begin(), subscribers.end(),subscriber);
    if (it == subscribers.end())
    {
        subscribers.push_back(subscriber);
    }
}

void Ringer::unsubscribe(IRingerObserver* subscriber)
{
    vector<IRingerObserver*>::iterator it;
    it = find(subscribers.begin(), subscribers.end(),subscriber);
    if (it != subscribers.end())
    {
        subscribers.erase(it);
    }
}

void Ringer::notify()
{
    vector<IRingerObserver*>::iterator it;
    for (it=subscribers.begin(); it!=subscribers.end();++it)
    {
        (*it)->onRingOver();
    }
}