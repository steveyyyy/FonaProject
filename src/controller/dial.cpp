#include "dial.h"
#include <string>
#include <logging/log.h>
LOG_MODULE_REGISTER(dialer, CONFIG_DIALER_LOG_LEVEL);

using namespace std;

void Dial::onTimeout(struct k_timer* t){
    LOG_INF("Timer timeout");
    Event* nf;
    nf = static_cast<Event*>(k_timer_user_data_get(t));
    XF::getInstance()->pushEvent(nf);
}

Dial::Dial(Button* switchhook, LED* ledGreen, LED* ledRed, Fona* fona, Ringer* ring){

    this->ledGreen=ledGreen;
    this->ledRed=ledRed;

    this->switchhook=switchhook;

    this->fona=fona;

    this->ring=ring;

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

    this->rl.setTarget(this);
    this->rl.setDnd(true);
    this->rl.setId((Event::evID)evRingIdle);

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
        LOG_INF("Digit: %i",digit);
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
            if (e->getId() == (Event::evID)evNoLock){
                this->state = ST_IDLE;
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
            if(e->getId()==Event::evDefault){
                state=ST_IDLE;  
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
                this->state = ST_IDLE;
            }
            if (e->getId() == (Event::evID)evRingIdle){
                this->state = ST_RINGIDLE;
            }
            break;
        case ST_RINGIDLE:
            if (e->getId() == (Event::evID)evHookUp){
                this->state = ST_TAKECALL;
            }
            if (e->getId() == (Event::evID)evRingStop){
                this->state = ST_IDLE;
            }
            if (e->getId() == (Event::evID)evRing){
                this->state = ST_RING;
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
                LOG_INF("ST_CHECKLOCK");
                deleteNumber();
                fona->send("AT+CPIN?");
                break;
            case ST_LOCKED:
                LOG_INF("ST_LOCKED");
                listenOnDigits=true;
                break;
            case ST_UNLOCK:
                LOG_INF("ST_UNLOCK");
                listenOnDigits=false;
                fona->send("AT+CPIN="+number);
                break;
            case ST_IDLE:
                LOG_INF("ST_IDLE");
                ring->stop();
                ledGreen->off();
                ledRed->off();
                listenOnDigits=false;
                deleteNumber();
                break;
            case ST_DIALING:
                ledRed->on();
                LOG_INF("ST_DIALING");
                listenOnDigits=true;
                if(number.length()>=3){
                    LOG_INF("Timer started");
                    k_timer_start(t,K_MSEC(5000), K_MSEC(0));
                }
                break;
            case ST_VALIDATEDIGIT:
                LOG_INF("ST_VALIDATEDIGIT");
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
                LOG_INF("ST_DIAL");
                fona->send("ATD"+number+"i;");
                LOG_INF("Number: %s", number.c_str());
                ledRed->off();
                ledGreen->on();
                break;
            case ST_INCALL:
                LOG_INF("ST_INCALL");
                break;
            case ST_ENDCALL:
                LOG_INF("ST_ENDCALL");
                fona->send("AT+CHUP");
                break;
            case ST_RING:
                LOG_INF("ST_RING");
                ring->ring();
                break;
            case ST_RINGIDLE:
                LOG_INF("ST_RINGIDLE");
                break;
            case ST_TAKECALL:
                LOG_INF("ST_TAKECALL");
                ring->stop();
                fona->send("ATA");
                break;
        }
    }
    return processed;
}
void Dial::onResponse(char * text){
    LOG_INF("DATA: %s", text);
    switch (state)
    {
    case ST_INIT:
    case ST_SETUP:
    case ST_LOCKED:
    case ST_DIALING:
    case ST_VALIDATEDIGIT:
        break;
    case ST_WAITOK:
        if(strcmp(text, "OK\r\n")==0){
            XF::getInstance()->pushEvent(&ev);
        }
        break;
    case ST_UNLOCK:
        if(strcmp(text, "OK\r\n")==0){
            this->lo.setId((Event::evID)evNoLock);
            XF::getInstance()->pushEvent(&lo);
        }
        if((strcmp(text, "ERROR\r\n")==0) || (strncmp(text, "+CME ERROR:", 11)==0)){
            XF::getInstance()->pushEvent(&ev);
        }
        break;
    case ST_CHECKLOCK:
        if(strcmp(text, "+CPIN: SIM PIN\r\n")==0){
            LOG_INF("SIM: LOCKED");
            this->lo.setId((Event::evID)evLocked);
            XF::getInstance()->pushEvent(&lo);
        }
        if(strcmp(text, "+CPIN: READY\r\n")==0){
            LOG_INF("SIM: UNLOCKED");
            this->lo.setId((Event::evID)evNoLock);
            XF::getInstance()->pushEvent(&lo);
        }
        break;
    case ST_IDLE:
    case ST_RINGIDLE:
        if(strcmp(text, "RING\r\n")==0){
            XF::getInstance()->pushEvent(&rg);
        }
        break;
    case ST_RING:
        if(strncmp(text, "MISSED_CALL:", 12)==0){
            XF::getInstance()->pushEvent(&rs);
        }
        break;
    case ST_DIAL:    
    case ST_TAKECALL:
        if(strcmp(text, "VOICE CALL: BEGIN\r\n")==0){
            XF::getInstance()->pushEvent(&ev);
        }
        break;
    case ST_INCALL:
    case ST_ENDCALL:
        if(strncmp(text, "VOICE CALL: END", 15)==0){
            XF::getInstance()->pushEvent(&ev);
        }
        break;
    }
}
void Dial::onRingOver(){
    if(state==ST_RING){
        XF::getInstance()->pushEvent(&rl);
    }
}