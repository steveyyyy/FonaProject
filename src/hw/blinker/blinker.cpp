#include "blinker.h"
#include <logging/log.h>
LOG_MODULE_REGISTER(flashing_light, CONFIG_FLASHING_LIGHT_LOG_LEVEL);

Blinker::Blinker(int blinkDelay) {
    in.setTarget(this);
    in.setDnd(1);
    tm.setTarget(this);
    tm.setDnd(1);
    state = ST_INIT;
    blinkDelayOn = blinkDelayOff = blinkDelay;
    diverseBlink = 0;
}

Blinker::Blinker(int blinkDelayOn, int blinkDelayOff) {
    in.setTarget(this);
    in.setDnd(1);
    tm.setTarget(this);
    tm.setDnd(1);
    state = ST_INIT;
    this->blinkDelayOn = blinkDelayOn;
    this->blinkDelayOff = blinkDelayOff;
    diverseBlink = 1;
}

Blinker::~Blinker() {
    
}

void Blinker::start() {
    this->state = ST_INIT;
    in.setId(Event::evInitial);
    in.setDelay(0);
    XF::getInstance()->pushEvent(&in);
}

void Blinker::stop() {
    this->state = ST_OFF;
    this->led->off();
    in.setId(Event::evInitial);
    in.setDelay(0);
    XF::getInstance()->pushEvent(&in);
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
        if (e->getId() == Event::evInitial)
        {
            state = ST_ON;
        }

        break;
    case ST_ON:
        if (e->getId() == Event::evTimeout)
        {
            state = ST_OFF;
        }
        
        break;
    case ST_OFF:
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
            LOG_INF("ST_INIT");
            break;
        case ST_NONE:
            LOG_INF("ST_NONE");
            break;
        case ST_ON:
            LOG_INF("ST_ON");
            led->on();
            tm.setId(Event::evTimeout);
            tm.setDelay(blinkDelayOn);
            XF::getInstance()->pushEvent(&tm);
            break;
        case ST_OFF:
            LOG_INF("ST_OFF");
            led->off();
            tm.setId(Event::evTimeout);
            tm.setDelay(blinkDelayOff);
            XF::getInstance()->pushEvent(&tm);
            break;
        }
        return true;
    }
    return false;
}

void Blinker::startBehaviour() {
    in.setId(Event::evInitial);
    in.setDelay(0);
    XF::getInstance()->pushEvent(&in);
}
