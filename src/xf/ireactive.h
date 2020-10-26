#ifndef IREACTIVE_ONCE
#define IREACTIVE_ONCE

class Event;

class IReactive
{
public:
    virtual bool processEvent(Event* event) = 0;
    void startBehaviour();
};

#endif