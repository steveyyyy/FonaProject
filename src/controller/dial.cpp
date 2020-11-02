#include "dial.h"

Dial::Dial(Button* buttonAnswer, Button* buttonHangUp, LED* ledGreen, LED* ledRed){

    this->ledGreen=ledGreen;
    this->ledRed=ledRed;

    this->buttonAnswer=buttonAnswer;
    this->buttonHangUp=buttonHangUp;

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
    if(pressed==true&&buttonAnswer->getId()==id){
        XF::getInstance()->pushEvent(&gp);
    }
    if(pressed==true&&buttonHangUp->getId()==id){
        XF::getInstance()->pushEvent(&rp);
    }
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
                printk(number.c_str());
                state=ST_WAITGREEN;     
            }
            break;
    }
    if(oldState!=state){
        processed=true;
        switch(state){
            case ST_INIT:
                printk("ST_INIT\n");
                break;
            case ST_WAITGREEN:
                ledGreen->off();
                ledRed->off();
                listenOnDigits=false;
                deleteNumber();
                printk("ST_WAITGREEN\n");
                break;
            case ST_DIALING:
                ledGreen->on();
                ledRed->on();
                printk("ST_DIALING\n");
                listenOnDigits=true;
                break;
            case ST_NOTIFY: //not done yet
                ledRed->off();
                listenOnDigits=false;
                printk("ST_NOTIFY\n");
                XF::getInstance()->pushEvent(&ev);
                break;
        }
    }
    return processed;
}