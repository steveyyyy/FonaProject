/* this is the Button class */
#include "button.h"
#include <algorithm>
using namespace std;

/**
 * @brief Construct a new Button object
 * 
 * @param p1 Pin number
 * @param port Port name
 */
Button::Button(GPI* pin)
{
    this->pin=pin;
    this->state = ST_INIT;
    this->ev.setTarget(this);
    this->ev.setDnd(1);
    this->tm.setTarget(this);
    this->tm.setDnd(1);
}

/**
 * @brief Destroy the Button object
 * 
 */
Button::~Button()
{
}

/**
 * @brief Initialize the buttons pin and subscribe to Interrupt Manager
 * 
 */
void Button::initHW()
{
    //initialize the buttons pin

    
    //subscribe to interrupts from the butoons pin
    IntManager::Subscription sub;
    sub.subscriber=this; //need to implement IIIntObserver!!
    sub.pp.pin=this->pin->getPin();
    sub.pp.dev=this->pin->getDriver();
    IntManager::getInstance()->subscribe(sub);
}

/**
 * @brief Add an Observer to this button
 * 
 * @param subscriber Button observer
 */
void Button::subscribe(IButtonObserver* subscriber)
{
    vector<IButtonObserver*>::iterator it;
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
void Button::unsubscribe(IButtonObserver* subscriber)
{
    vector<IButtonObserver*>::iterator it;
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
void Button::notify()
{
    vector<IButtonObserver*>::iterator it;
    for (it=subscribers.begin(); it!=subscribers.end();++it)
    {
        (*it)->onButton(this->pin->getUId(), this->state == ST_PRESSED);
    }
}

/**
 * @brief Fired when an Interupt happens -> Send Interrupt event to event handler
 * 
 */
void Button::onInterrupt(u32_t pin)
{
    //hre we will fire an event to the Button itself
    //be aware: this is inteerrupt territoy, so no mallocs and frees
    //we reuse our event: 

    ev.setId(Event::evInt);
    //fire the event
    XF::getInstance()->pushEvent(&ev);
}

/**
 * @brief Process event and change state accordingly 
 * 
 * @param e event 
 */
bool Button::processEvent(Event* e)
{
    BUTTONSTATES oldstate;
    bool processed=false;

    oldstate = this->state;
    switch (this->state)
    {
        case ST_INIT:
        if (e->getId() == Event::evInitial)
        {
            this->state = ST_DECIDE;
        }
        break;
        case ST_DEBOUNCE:
        if (e->getId() == Event::evTimeout)
        {
            this->state = ST_DECIDE;
        }
        break;
        case ST_DECIDE:
        if (e->getId() == Event::evDefault)
        {
            if (pin->read() == GPIO::PIN_ON)
            {
                this->state = ST_PRESSED;
            }
            else
            {
                this->state = ST_RELEASED;
            }
        }
        break;
        case ST_PRESSED:
            if (e->getId() == Event::evInt)
            {
                if (pin->read() == GPIO::PIN_OFF)
                {
                    this->state = ST_DEBOUNCE;
                }
            }
        break;
        case ST_RELEASED:
            if (e->getId() == Event::evInt)
            {
                if (pin->read() == GPIO::PIN_ON)
                {
                    this->state = ST_DEBOUNCE;
                }
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
            case ST_DEBOUNCE:
                printk("DEBOUNCE\n");
                tm.setId(Event::evTimeout);
                tm.setDelay(DEBOUNCEDELAY);
                XF::getInstance()->pushEvent(&tm);
            break;
            case ST_DECIDE:
                printk("DECIDE\n");
                ev.setId(Event::evDefault);
                ev.setDelay(0);
                XF::getInstance()->pushEvent(&ev);
            break;
            case ST_PRESSED:
                printk("PRESSED\n");
                printk("Button %02d(%d) pressed\n", this->pin->getPin(),this->pin->getUId());
                notify();
            break;
            case ST_RELEASED:
                printk("RELEASED\n");
                printk("Button %02d(%d) released\n", this->pin->getPin(),this->getId());
                notify();
            break;
        }
    }
    return processed;
}

/**
 * @brief Fire initial event to activate Button's initial behaviour
 * 
 */
void Button::startBehaviour()
{
    ev.setId(Event::evInitial);
    ev.setDelay(0);
    XF::getInstance()->pushEvent(&ev);
}

/**
 * @brief Return unique ID of Button 
 * 
 * @return int ID number
 */
int Button::getId()
{
    return this->pin->getUId();
}