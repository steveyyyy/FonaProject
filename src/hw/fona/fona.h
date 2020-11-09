#include "../../xf/xf.h"

#include "../../gpio/intmanager.h"
#include <vector>
#include <algorithm>

#ifndef FONA_ONCE
#define FONA_ONCE


class Fona : public IReactive
{
public:

    class IFonaObserver
    {
        public:
        virtual void onResponse(string response) = 0;
    };

    typedef enum FONASTATES {    ST_INIT,

                            } FONASTATES; 

    Fona();
    ~Fona();
    void initHW();
    bool processEvent(Event* e);
    void startBehaviour();
    void subscribe(IFonaObserver* subscriber);
    void unsubscribe(IFonaObserver* subscriber);
    void notify();

private:                              
    FONASTATES state;                     
    Event ev;                               
    vector<IFonaObserver*> subscribers;
};

#endif
