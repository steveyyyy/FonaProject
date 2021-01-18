#include "ireactive.h"

#ifndef EVENT_ONCE
#define EVENT_ONCE


class Event
{
public:
    typedef enum evID {evNone, evInitial, evTimeout, evDefault, evPressed, evReleased, evInt } evID;
    Event(/* args */);
    explicit Event(evID eventID);
    ~Event();
    void setTarget(IReactive* target);
    IReactive* getTarget();
    void setId(evID eventID);
    evID getId();
    void setDnd(bool doNotDelete);
    bool getDnd();
    void setDelay(int delay);
    int getDelay();
    void setT(struct k_timer* timer);
    struct k_timer* getT();
private:
    evID id;
    IReactive* target;
    struct k_timer* t;
    bool deleteAfterConsume;
    int delay;
};

#endif