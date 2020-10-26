#include <zephyr.h>
#include <list>
#include "event.h"

#ifndef XF_ONCE
#define XF_ONCE


using namespace std;

/**
 * @class The XF 
 * @brief Our Event processing machine. 
 */
class XF
{
public:
    void init();
    static XF* getInstance();
    void pushEvent(Event* e);
    void execute();
    void unscheduleTM(Event* e);
private:
    XF(/* args */);
    ~XF();
    Event* popEvent();
    void scheduleTM(Event* e);
    static void onTimeout(struct k_timer* t);
    static void onStop(struct k_timer* t);
    static XF theXF;
    //list<Event*> evQueue;
    // struct k_mutex m;
    struct k_msgq queue;
    char __aligned(4) queue_buffer[10 * sizeof(Event*)];
};

#endif