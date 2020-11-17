#include "fona.h"

Fona::Fona()
{

}

Fona::~Fona()
{
}

void Fona::initHW()
{
    
}

void Fona::onMessage(u8_t character){
    message +=(char)character;
    if(character=='\n'){
        //event auslösen
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
            if (e->getId() == (Event::evID)evBaudCheck)
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
            break;
            case ST_WAITOK:
                printk("ST_WAITOK\n");
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
    ev.setId(Event::evInitial);
    ev.setDelay(0);
    XF::getInstance()->pushEvent(&ev);
}
