#include "ringer.h"

Ringer::Ringer(int p1, const char *port, int hertz, int ringTime) : pin(p1, port){
    this->hertz= hertz;
    this->ringTime=ringTime;

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
    k_timer_start(oscillator,K_MSEC(50), K_MSEC(50));
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
}