#include "fona.h"

Fona::Fona(UART* uart)
{
    this->uart=uart;
    this->message="";

    this->ev.setTarget(this);
    this->ev.setDnd(true);
    this->ev.setId(Event::evDefault);

    this->evIni.setTarget(this);
    this->evIni.setDnd(true);
    this->evIni.setId(Event::evInitial);

    this->evErr.setTarget(this);
    this->evErr.setDnd(true);
    this->evErr.setId((Event::evID)evError);

    this->evCmd.setTarget(this);
    this->evCmd.setDnd(true);
    this->evCmd.setId((Event::evID)evCommand);

    this->evRp.setTarget(this);
    this->evRp.setDnd(true);
    this->evRp.setId((Event::evID)evResponse);
}

Fona::~Fona(){}

void Fona::initHW()
{
    uart->subscribe(this);
}

void Fona::onMessage(u8_t character){
    printk("%c",character);
    message += character;
    //event auslösen
    if(state==ST_WAITOK){
        if(message=="\r\nOK"){
            XF::getInstance()->pushEvent(&ev);
            message="";
        }
    }
    else if(state==ST_COMMAND){

    }
    else if(state==ST_IDLE){

    }
}

void Fona::send(string message){
    if(state==ST_IDLE||state==ST_WAITOK){
        message += "\r";
        uart->uartSend(message.c_str());
        XF::getInstance()->pushEvent(&evCmd);
    }
}

void Fona::subscribe(IFonaObserver* subscriber)
{
    vector<IFonaObserver*>::iterator it;
    it = find(subscribers.begin(), subscribers.end(),subscriber);
    if (it == subscribers.end())
    {
        subscribers.push_back(subscriber);
    }
}

void Fona::unsubscribe(IFonaObserver* subscriber)
{
    vector<IFonaObserver*>::iterator it;
    it = find(subscribers.begin(), subscribers.end(),subscriber);
    if (it != subscribers.end())
    {
        subscribers.erase(it);
    }
}

void Fona::notify()
{
    vector<IFonaObserver*>::iterator it;
    for (it=subscribers.begin(); it!=subscribers.end();++it)
    {
        // (*it)->onResponse();
    }
}

bool Fona::processEvent(Event* e)
{
    FONASTATES oldstate;
    bool processed=false;

    oldstate = this->state;
    switch (this->state)
    {
        case ST_INIT:
            if (e->getId() == Event::evInitial)
            {
                this->state = ST_SETUP;
            }
        break;
        case ST_SETUP:
            if (e->getId() == Event::evDefault)
            {
                this->state = ST_WAITOK;
            }
            break;
        case ST_WAITOK:
            if (e->getId() == Event::evDefault)
            {
                this->state = ST_IDLE;
            }
            if (e->getId() == (Event::evID)evError)
            {
                this->state = ST_SETUP;
            }
        break;
        case ST_IDLE:
            if (e->getId() == (Event::evID)evCommand)
            {
                this->state = ST_COMMAND;
            }
            if (e->getId() == (Event::evID)evResponse)
            {
                this->state = ST_NOTIFY;
            }
        break;
        case ST_COMMAND:
            if (e->getId() == (Event::evID)evResponse)
            {
                this->state = ST_NOTIFY;
            }
        break;
        case ST_NOTIFY:
            if (e->getId() == Event::evDefault)
            {
                this->state = ST_IDLE;
            }
        break;
    }

    if (oldstate != this->state)
    {
        processed=true;
        switch (this->state)
        {
            case ST_INIT:
                printk("ST_INIT\n");
            break;
            case ST_SETUP:
                printk("ST_SETUP\n");
                XF::getInstance()->pushEvent(&ev);               
            break;
            case ST_WAITOK:
                printk("ST_WAITOK\n");
                //dont send it here
                send("ATE0");
                send("AT");
            break;
            case ST_IDLE:
                printk("ST_IDLE\n");
            break;
            case ST_COMMAND:
                printk("ST_COMMAND\n");
            break;
            case ST_NOTIFY:
                printk("ST_NOTIFY\n");
            break;
        }
    }
    return processed;
}


void Fona::startBehaviour()
{
    evIni.setId(Event::evInitial);
    evIni.setDelay(0);
    XF::getInstance()->pushEvent(&evIni);
}