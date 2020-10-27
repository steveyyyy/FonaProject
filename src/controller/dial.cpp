#include "dial.h"

Dial::Dial(){
    listenOnDigits=false;
    state=ST_INIT;
    number="";

    this->in.setTarget(this);
    this->in.setDnd(true);
    this->in.setId(Event::evInitial);

    this->ev.setTarget(this);
    this->ev.setDnd(true);
    this->ev.setId(Event::evDefault);

    this->gp.setTarget(this);
    this->gp.setDnd(true);
    this->gp.setId((Event::evID)evGreenPressed);

    this->rp.setTarget(this);
    this->rp.setDnd(true);
    this->rp.setId((Event::evID)evRedPressed);
}
Dial::~Dial(){}

string Dial::getNumber(){
    return this->number;
}

void Dial::deleteNumber(){
    number = "";
}

void Dial::onButton(int id, bool pressed){
//    if
}

void Dial::onDigit(int digit){
    if(listenOnDigits){
        number += digit;
    }
}

void Dial::startBehaviour(){
    XF::getInstance()->pushEvent(&in);
}

bool Dial::processEvent(Event* e){
    DIALERSTATE oldState=state;
    bool processed=false;
    switch(state){
        case ST_INIT:
            if (e->getId() == Event::evInitial){
                this->state = ST_WAITGREEN;
            }
            break;
        case ST_WAITGREEN:
            if(e->getId()==(Event::evID)evGreenPressed){
                state=ST_DIALING;     
            }
            break;
        case ST_DIALING:
            if(e->getId()==(Event::evID)evGreenPressed){
                state=ST_NOTIFY;     
            }
            if(e->getId()==(Event::evID)evRedPressed){
                state=ST_WAITGREEN;     
            }
            break;
        case ST_NOTIFY:
            if(e->getId()==Event::evDefault){
                state=ST_WAITGREEN;     
            }
            break;
    }
    if(oldState!=state){
        processed=true;
        switch(state){
            case ST_INIT:
                printk("ST_INIT");
                break;
            case ST_WAITGREEN:
                listenOnDigits=false;
                deleteNumber();
                printk("ST_WAITGREEN");
                break;
            case ST_DIALING:
                printk("ST_DIALING");
                listenOnDigits=true;
                break;
            case ST_NOTIFY:
                listenOnDigits=false;
                printk("ST_NOTIFY");
                XF::getInstance()->pushEvent(&ev);
                break;
        }
    }
    return processed;
}