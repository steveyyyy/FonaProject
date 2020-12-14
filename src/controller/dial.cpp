#include "dial.h"

void Dial::onTimeout(struct k_timer* t){
    printk("timer over\n");
    Event* nf;
    nf = (Event*) k_timer_user_data_get(t);
    XF::getInstance()->pushEvent(nf);
}

Dial::Dial(Button* switchhook, LED* ledGreen, LED* ledRed, Fona* fona){

    this->ledGreen=ledGreen;
    this->ledRed=ledRed;

    this->switchhook=switchhook;

    this->fona=fona;

    listenOnDigits=false;
    state=ST_INIT;
    deleteNumber();

    this->lo.setTarget(this);
    this->lo.setDnd(true);

    this->in.setTarget(this);
    this->in.setDnd(true);
    this->in.setId(Event::evInitial);

    this->ev.setTarget(this);
    this->ev.setDnd(true);
    this->ev.setId(Event::evDefault);

    this->hu.setTarget(this);
    this->hu.setDnd(true);
    this->hu.setId((Event::evID)evHookUp);

    this->hd.setTarget(this);
    this->hd.setDnd(true);
    this->hd.setId((Event::evID)evHookDown);

    this->od.setTarget(this);
    this->od.setDnd(true);
    this->od.setId((Event::evID)evOnDigit);

    this->dl.setTarget(this);
    this->dl.setDnd(true);
    this->dl.setId((Event::evID)evDial);

    this->rg.setTarget(this);
    this->rg.setDnd(true);
    this->rg.setId((Event::evID)evRing);

    this->rs.setTarget(this);
    this->rs.setDnd(true);
    this->rs.setId((Event::evID)evRingStop);

    this->ed.setTarget(this);
    this->ed.setDnd(true);
    this->ed.setId((Event::evID)evHangUp);

    t=(struct k_timer*) k_malloc(sizeof(struct k_timer));
    k_timer_init(t, &Dial::onTimeout, NULL);
    k_timer_user_data_set(t,&dl);
}
Dial::~Dial(){}

string Dial::getNumber(){
    return this->number;
}

void Dial::deleteNumber(){
    number = "";
}

void Dial::onButton(int id, bool pressed){
    if(pressed){
        XF::getInstance()->pushEvent(&hd);
    }
    else{
        XF::getInstance()->pushEvent(&hu);
    }
}

void Dial::onDigit(int digit){
    if(listenOnDigits){
        number += std::to_string(digit);
    }
    if(state==ST_DIALING){
        XF::getInstance()->pushEvent(&od);
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
                this->state = ST_SETUP;
            }
            break;
        case ST_SETUP:
            if (e->getId() == Event::evDefault){
                this->state = ST_WAITOK;
            }
            break;
        case ST_WAITOK:
            if (e->getId() == Event::evDefault){
                this->state = ST_CHECKLOCK;
            }else if (e->getId() == (Event::evID)evError){
                this->state = ST_SETUP;
            }
            break;    
        case ST_CHECKLOCK:
            if (e->getId() == (Event::evID)evNoLock){
                this->state = ST_IDLE;
            }
            if (e->getId() == (Event::evID)evLocked){
                this->state = ST_LOCKED;
            }
            break;
        case ST_LOCKED:
            if (e->getId() == (Event::evID)evHookDown){
                this->state = ST_UNLOCK;
            }
            break;
        case ST_UNLOCK:
            if (e->getId() == Event::evDefault){
                this->state = ST_CHECKLOCK;
            }
            break;
        case ST_IDLE:
            if(e->getId()==(Event::evID)evHookUp){
                state=ST_DIALING;     
            }
            if(e->getId()==(Event::evID)evRing){
                state=ST_RING;     
            }
            break;
        case ST_DIALING:
            if(e->getId()==(Event::evID)evDial){
                state=ST_DIAL;     
            }
            if(e->getId()==(Event::evID)evOnDigit){
                state=ST_VALIDATEDIGIT;     
            }
            if(e->getId()==(Event::evID)evHookDown){
                state=ST_IDLE;     
            }
            break;
        case ST_VALIDATEDIGIT:
            if(e->getId()==Event::evDefault){
                state=ST_DIALING;     
            }
            if(e->getId()==(Event::evID)evDial){
                state=ST_DIAL;     
            }
            break;
        case ST_DIAL:
            if (e->getId() == Event::evDefault){
                this->state = ST_INCALL;
            }
            break;    
        case ST_INCALL:
            if(e->getId()==(Event::evID)evHookDown){
                state=ST_ENDCALL;  
            }
            break;
        case ST_ENDCALL:
            if(e->getId()==Event::evDefault){
                state=ST_IDLE;  
            }
            break;
        case ST_RING:
            if (e->getId() == (Event::evID)evHookUp){
                this->state = ST_TAKECALL;
            }
            if (e->getId() == (Event::evID)evRingStop){
                this->state = ST_TAKECALL;
            }
            break;
        case ST_TAKECALL:
            if (e->getId() == Event::evDefault){
                this->state = ST_INCALL;
            }
            break;
    }
    if(oldState!=state){
        processed=true;
        switch(state){
            case ST_INIT:
                break;
            case ST_SETUP:
                XF::getInstance()->pushEvent(&ev);
                fona->send("ATE0");
                break;
            case ST_WAITOK:
                break;
            case ST_CHECKLOCK:
                printk("ST_CHECKLOCK\n");
                fona->send("AT+CPIN?");
                break;
            case ST_LOCKED:
                printk("ST_LOCKED\n");
                listenOnDigits=true;
                break;
            case ST_UNLOCK:
                printk("ST_UNLOCK\n");
                listenOnDigits=false;
                fona->send("AT+CPIN="+number);
                break;
            case ST_IDLE:
                printk("ST_WAITHOOKUP\n");
                ledGreen->off();
                ledRed->off();
                listenOnDigits=false;
                deleteNumber();
                break;
            case ST_DIALING:
                ledRed->on();
                printk("ST_DIALING\n");
                listenOnDigits=true;
                if(number.length()>=3){
                    printk("timer startet\n");
                    k_timer_start(t,K_MSEC(5000), K_MSEC(0));
                }
                break;
            case ST_VALIDATEDIGIT:
                printk("ST_VALIDATEDIGIT\n");
                //button can be added to stop timer
                k_timer_stop(t);
                listenOnDigits=false;
                for(string *emergencyNumber: emergencyNumbers){
                    if(emergencyNumber[0]==number){
                        XF::getInstance()->pushEvent(&dl);
                    }
                }
                if(number.length()==10&&number[0]=='0'&&number[1]!='0'){
                    XF::getInstance()->pushEvent(&dl);
                }
                else{
                    XF::getInstance()->pushEvent(&ev);
                }
                break;
            case ST_DIAL:
                listenOnDigits=false;
                printk("ST_DIAL\n");
                fona->send("ATD"+number+"i;");
                printk(number.c_str());
                printk("\n");
                ledRed->off();
                ledGreen->on();
                break;
            case ST_INCALL:
                printk("ST_INIT\n");
                break;
            case ST_ENDCALL:
                fona->send("AT+CHUP");
                break;
            case ST_RING:
                printk("ST_INIT\n");
                break;
            case ST_TAKECALL:
                printk("ST_INIT\n");
                fona->send("ATA");
                break;
        }
    }
    return processed;
}
void Dial::onResponse(string text){
    switch (state)
    {
    case ST_WAITOK:
        if(text == "OK\r\n"){
            XF::getInstance()->pushEvent(&ev);
        }
        break;
    case ST_CHECKLOCK:
        if(text=="\n+CPIN: SIM PIN"){
            printk("locked");
            this->lo.setId((Event::evID)evLocked);
        }else if(text=="READY"){
            printk("unlocked");
            this->lo.setId((Event::evID)evNoLock);
        }
        XF::getInstance()->pushEvent(&lo);
        break;
    case ST_IDLE:
        if(text=="RING"){
            XF::getInstance()->pushEvent(&rg);
        }
        break;
    case ST_TAKECALL:
        if(text=="VOICE CALL: BEGIN"){
            XF::getInstance()->pushEvent(&ev);
        }
        break;
    case ST_INCALL:
        if(text=="VOICE CALL: END"){
            XF::getInstance()->pushEvent(&ed);
        }
        break;
    case ST_ENDCALL:
        if(text=="VOICE CALL: END"){
            XF::getInstance()->pushEvent(&ev);
        }
        break;
    }
}