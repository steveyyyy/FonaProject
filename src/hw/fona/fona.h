#include "../../xf/xf.h"
#include "../../uart/uart.h"
#include "../../gpio/intmanager.h"
#include <vector>
#include <algorithm>

#ifndef FONA_ONCE
#define FONA_ONCE


class Fona : public UART::IUARTObserver, public IReactive
{
public:

    class IFonaObserver
    {
        public:
        virtual void onResponse(string response) = 0;
    };

    typedef enum fonaEvents{    evBaudCheck=200,
                                evError,
                                evCommand,
                                evResponse,
                                evNotify
                             } fonaEvents;

    typedef enum FONASTATES {   ST_INIT,
                                ST_SETUP,
                                ST_WAITOK,
                                ST_IDLE,
                                ST_COMMAND,
                                ST_NOTIFY
                            } FONASTATES; 

    Fona();
    ~Fona();
    void initHW();
    bool processEvent(Event* e);
    void startBehaviour();
    void subscribe(IFonaObserver* subscriber);
    void unsubscribe(IFonaObserver* subscriber);
    void notify();
    void onMessage(u8_t character);

private:                              
    FONASTATES state;                     
    Event ev;                               
    vector<IFonaObserver*> subscribers;
    string message;
};

#endif
