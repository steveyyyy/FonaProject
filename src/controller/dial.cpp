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
    number="";

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

    this->nf.setTarget(this);
    this->nf.setDnd(true);
    this->nf.setId((Event::evID)evCall);

    t=(struct k_timer*) k_malloc(sizeof(struct k_timer));
    k_timer_init(t, &Dial::onTimeout, NULL);
    k_timer_user_data_set(t,&nf);
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
    XF::getInstance()->pushEvent(&od);
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
        case ST_RING:

            break;
        case ST_DIALING:
            if(e->getId()==(Event::evID)evCall){
                state=ST_CALL;     
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
            if(e->getId()==(Event::evID)evCall){
                state=ST_CALL;     
            }
            break;
        case ST_CALL:
            if(e->getId()==(Event::evID)evHookDown){
                state=ST_ENDCALL;  
            }
            break;
        case ST_ENDCALL:
            if(e->getId()==Event::evDefault){
                state=ST_IDLE;  
            }
            break;
    }
    if(oldState!=state){
        processed=true;
        switch(state){
            case ST_INIT:
                printk("ST_INIT\n");
                break;
            case ST_IDLE:
                ledGreen->off();
                ledRed->off();
                listenOnDigits=false;
                deleteNumber();
                printk("ST_WAITHOOKUP\n");
                break;
            case ST_RING:

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
                        XF::getInstance()->pushEvent(&nf);
                    }
                }
                if(number.length()==10&&number[0]=='0'&&number[1]!='0'){
                    XF::getInstance()->pushEvent(&nf);
                }
                else{
                    XF::getInstance()->pushEvent(&ev);
                }
                break;
            case ST_CALL:
                ledRed->off();
                ledGreen->on();
                listenOnDigits=false;
                printk("ST_NOTIFY\n");
                fona->send("ATD"+number+"i;");
                printk(number.c_str());
                printk("\n");
                break;
            case ST_ENDCALL:
                fona->send("AT+CHUP");
                break;
        }
    }
    return processed;
}
void Dial::onResponse(){
    if(fona->compareDataTo("RING")){
        printk("ITS RINGING");
    }
    if(fona->compareDataTo("VOICE CALL: END")){
        XF::getInstance()->pushEvent(&ev);
    }
}