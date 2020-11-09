#include "fona.h"

Fona::Fona()
{

}

Fona::~Fona()
{
}

/**
 * @brief Initialize the buttons pin and subscribe to Interrupt Manager
 * 
 */
void Fona::initHW()
{
    
}

/**
 * @brief Add an Observer to this button
 * 
 * @param subscriber Button observer
 */
void Fona::subscribe(IFonaObserver* subscriber)
{
    vector<IFonaObserver*>::iterator it;
    it = find(subscribers.begin(), subscribers.end(),subscriber);
    if (it == subscribers.end())
    {
        subscribers.push_back(subscriber);
    }
}

/**
 * @brief Remove an Observer from this button
 * 
 * @param subscriber Button observer
 */
void Fona::unsubscribe(IFonaObserver* subscriber)
{
    vector<IFonaObserver*>::iterator it;
    it = find(subscribers.begin(), subscribers.end(),subscriber);
    if (it != subscribers.end())
    {
        subscribers.erase(it);
    }
}

/**
 * @brief Notify every subscriber that the state of Button has changed
 * 
 */
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
