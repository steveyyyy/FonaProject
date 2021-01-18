#include "cobs.h"
#include <algorithm>
#include <logging/log.h>
LOG_MODULE_REGISTER(clicker, CONFIG_CLICKER_LOG_LEVEL);

/**
 * @brief Construct a new Click Observer object
 * 
 */
ClickObserver::ClickObserver()
{
    this->state = ST_INIT;
    button = NULL;
    evTm.setDnd(1);
    evTm.setId(Event::evTimeout);
    evTm.setTarget(this);

    event.setDnd(1);
    event.setId(Event::evInitial);
    event.setTarget(this);
    
}

/**
 * @brief Destroy the Click Observer object
 * 
 */
ClickObserver::~ClickObserver()
{
}

/**
 * @brief Fire initial event to activate ClickObserver's initial behaviour
 * 
 */
void ClickObserver::startBehavior()
{
    event.setId(Event::evInitial);
    event.setDelay(0);      
    XF::getInstance()->pushEvent(&event);
}

/**
 * @brief Subscribe to Observer
 * 
 * @param subscriber 
 */
void ClickObserver::subscribe(IClickObserver* subscriber)
{
    vector<IClickObserver*>::iterator it;
    it = find(subscribers.begin(), subscribers.end(),subscriber);
    if (it == subscribers.end())
    {
        subscribers.push_back(subscriber);
    }
}

/**
 * @brief Unsubscribe from Observer
 * 
 * @param subscriber 
 */
void ClickObserver::unsubscribe(IClickObserver* subscriber)
{
    vector<IClickObserver*>::iterator it;
    it = find(subscribers.begin(), subscribers.end(),subscriber);
    if (it != subscribers.end())
    {
        subscribers.erase(it);
    }
}

/**
 * @brief Notify all subscribers of any changes
 * 
 */
void ClickObserver::notify()
{
    int signal;
    vector<IClickObserver*>::iterator it;
    for (it=subscribers.begin(); it!=subscribers.end();++it)
    {
        switch (this->state)
        {
        case ST_CLICK:
            signal = 0;
            break;
        case ST_LONG:
            signal = 1;
            break;
        case ST_DOUBLE:
            signal = 2;
            break;
        default:
            signal = 3;
            break;
        }
        (*it)->onClick(signal);
    }
}

/**
 * @brief Forward signal from button to event processor
 * 
 * @param id Button ID
 * @param pressed pressed/released bool
 */
void ClickObserver::onButton(int id, bool pressed)
{
    if (pressed)
    {
        LOG_INF("PRESSED");
        event.setId(Event::evPressed);
    }else
    {
        LOG_INF("RELEASED");
        event.setId(Event::evReleased);
    }
    XF::getInstance()->pushEvent(&event);
}

/**
 * @brief Handle events
 * 
 * @param e event
 */
bool ClickObserver::processEvent(Event* e)
{
    CLICKSTATES oldstate;
    bool processed=false;

    oldstate = this->state;
    switch (this->state)
    {
    case ST_INIT:
        if (e->getId() == Event::evInitial)
        {
            this->state = ST_IDLE;
        }
        break;
    case ST_IDLE:
        if (e->getId() == Event::evPressed)
        {
            this->state = ST_CLICK_AWAIT;
        }
        break;
    case ST_CLICK_AWAIT:
        if (e->getId() == Event::evReleased)
        {
            this->state = ST_SHORT;
        }else if (e->getId() == Event::evTimeout)
        {
            this->state = ST_LONG;
        }
        break;
    case ST_LONG:
        if (e->getId() == Event::evReleased)
        {
            this->state = ST_IDLE;
        }
        break;
    case ST_SHORT:
        if (e->getId() == Event::evPressed)
        {
            this->state = ST_DOUBLE_AWAIT;
        }else if (e->getId() == Event::evTimeout)
        {
            this->state = ST_CLICK;
        }
        break;
    case ST_CLICK:
        if (e->getId() == Event::evReleased)
        {
            this->state = ST_IDLE;
        }
        break;
    case ST_DOUBLE_AWAIT:
        if (e->getId() == Event::evReleased)
        {
            this->state = ST_DOUBLE;
        }
        break;
    case ST_DOUBLE:
        if (e->getId() == Event::evReleased)
        {
            this->state = ST_IDLE;
        }
        break;
    default:
        break;
    }

    if (oldstate != this->state)
    {
        processed=true;
        switch (this->state)
        {
        case ST_INIT:
            break;
        case ST_IDLE:
            break;
        case ST_CLICK_AWAIT:
            evTm.setDelay(LONG_CLICK_DELAY);
            XF::getInstance()->pushEvent(&evTm);
            break;
        case ST_LONG:
            notify();
            break;
        case ST_SHORT:
            XF::getInstance()->unscheduleTM(&evTm);
            evTm.setDelay(DOUBLE_CLICK_DELAY);
            XF::getInstance()->pushEvent(&evTm);
            break;
        case ST_CLICK:
            event.setId(Event::evReleased);
            event.setDelay(0);
            notify();
            XF::getInstance()->pushEvent(&event);
            break;
        case ST_DOUBLE_AWAIT:
            break;
        case ST_DOUBLE:
            event.setId(Event::evReleased);
            event.setDelay(0);
            notify();
            XF::getInstance()->pushEvent(&event);
            break;
        default:
            break;
        }
    }
    return processed;
    
}