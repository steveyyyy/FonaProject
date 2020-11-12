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
            //this->state = ST_DECIDE;
        }
        break;
    }

    if (oldstate != this->state)
    {
        processed=true;
        switch (this->state)
        {
            case ST_INIT:
                printk("INIT\n");
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
