#include "event.h"

Event::Event() {
    this->id = evNone;
    this->t = nullptr;
    this->delay = 0;
    this->deleteAfterConsume = false;
    this->target = nullptr;
}

Event::Event(evID eventID) {
    this->id = evNone;
    this->t = nullptr;
    this->delay = 0;
    this->deleteAfterConsume = false;
    this->target = nullptr;
}

Event::~Event() {
    
}

void Event::setTarget(IReactive* target) {
    this->target = target;
}

IReactive* Event::getTarget() {
    return this->target;
}

void Event::setId(evID eventID) {
    this->id = eventID;
}

Event::evID Event::getId() {
    return this->id;
}

void Event::setDnd(bool doNotDelete) {
    this->deleteAfterConsume = doNotDelete;
}

bool Event::getDnd() {
    return this->deleteAfterConsume;
}

void Event::setDelay(int delay) {
    this->delay = delay;
}

int Event::getDelay() {
    return this->delay;
}

void Event::setT(struct k_timer* t) {
    this->t = t;
}

struct k_timer* Event::getT() {
    return this->t;
}