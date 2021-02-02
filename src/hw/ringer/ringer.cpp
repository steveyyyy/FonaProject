#include "ringer.h"

Ringer::Ringer(int p1, const char *port, int hertz) : pin(p1, port){
    this->hertz= (1000/hertz)/2;
    oscillator=(struct k_timer*) k_malloc(sizeof(struct k_timer));
    k_timer_init(oscillator, &Ringer::oscillatorToggle, NULL);
}

Ringer::~Ringer(){}

void Ringer::initHW(){
    pin.setInitialOff();
    pin.setLowPower();
    pin.initHW();
    pinState=false;
    k_timer_user_data_set(oscillator,this);
}
void Ringer::stop(){
    k_timer_stop(oscillator);
    pinState=false;
    pin.off();
}
void Ringer::ring(){
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