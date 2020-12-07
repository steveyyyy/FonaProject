#include "../../xf/xf.h"
#include "../../uart/uart.h"
#include "../../gpio/intmanager.h"
#include <vector>
#include <algorithm>

#ifndef FONA_ONCE
#define FONA_ONCE
#define MAXDATASIZE 255

class Fona : public IReactive, public UART
{
public:

    class IFonaObserver
    {
        public:
        virtual void onResponse() = 0;
    };

    typedef enum fonaEvents{    evError=200,
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

    Fona(const char* deviceBinding,int baudrate);
    ~Fona();
    bool processEvent(Event* e);
    void startBehaviour();
    void subscribe(IFonaObserver* subscriber);
    void unsubscribe(IFonaObserver* subscriber);
    void notify();
    void send(string command);

    void elaborateMessage(u8_t character);

    // string data;
    // string buffer;

    uint8_t data[MAXDATASIZE];
    uint8_t buffer[MAXDATASIZE];
    uint8_t pos;

    bool compareDataTo(char * compare);

private:                           
    FONASTATES state;                     
    Event ev;       //Default event
    Event evIni;    //Inital event
    Event evErr;    //Error event
    Event evCmd;    //Command event
    Event evRp;     //Response event                             
    vector<IFonaObserver*> subscribers;
    string* dataSource;
};

#endif