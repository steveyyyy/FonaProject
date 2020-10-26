#include "blinker.h"

Blinker::Blinker(int blinkDelay) {
    event.setTarget(this);
    event.setDnd(1);
    state = ST_INIT;
    blinkDelayOn = blinkDelayOff = blinkDelay;
    diverseBlink = 0;
}

Blinker::Blinker(int blinkDelayOn, int blinkDelayOff) {
    event.setTarget(this);
    event.setDnd(1);
    state = ST_INIT;
    this->blinkDelayOn = blinkDelayOn;
    this->blinkDelayOff = blinkDelayOff;
    diverseBlink = 1;
}

Blinker::~Blinker() {
    
}

void Blinker::start() {
    this->state = ST_INIT;
    event.setId(Event::evInitial);
    event.setDelay(0);
    XF::getInstance()->pushEvent(&event);
}

void Blinker::stop() {
    this->state = ST_OFF;
    this->led->off();
    printk("LED OFF\n");
    event.setId(Event::evInitial);
    event.setDelay(0);
    XF::getInstance()->pushEvent(&event);
}

void Blinker::setDelayOn(int delay) {
    this->diverseBlink = 1;
    this->blinkDelayOn = delay;
}

void Blinker::setDelayOff(int delay) {
    this->diverseBlink = 1;
    this->blinkDelayOff = delay;
}

void Blinker::setLed(LED* led) {
    this->led = led;
}

bool Blinker::processEvent(Event* e) {
    BLINKER_STATES oldState = state;
    switch (state)
    {
    case ST_NONE:
        break;
    case ST_INIT:
        printk("ST_INIT\n");
        if (e->getId() == Event::evInitial)
        {
            state = ST_ON;
        }

        break;
    case ST_ON:
        printk("ST_ON\n");
        if (e->getId() == Event::evTimeout)
        {
            state = ST_OFF;
        }
        
        break;
    case ST_OFF:
        printk("ST_OFF\n");
        if (e->getId() == Event::evTimeout)
        {
            state = ST_ON;
        }
    }
    if (oldState != state)
    {
        switch (state)
        {
        case ST_INIT:
            break;
        case ST_NONE:
            break;
        case ST_ON:
            led->on();
            printk("LED ON\n");
            event.setId(Event::evTimeout);
            event.setDelay(blinkDelayOn);
            XF::getInstance()->pushEvent(&event);
            break;
        case ST_OFF:
            led->off();
            printk("LED OFF\n");
            event.setId(Event::evTimeout);
            event.setDelay(blinkDelayOff);
            XF::getInstance()->pushEvent(&event);
            break;
        }
        return true;
    }
    return false;
}

void Blinker::startBehaviour() {
    printk("starting behaviour of blinker...\n");
    event.setId(Event::evInitial);
    event.setDelay(0);
    XF::getInstance()->pushEvent(&event);
}
