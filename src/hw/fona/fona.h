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
        virtual void onResponse(string text) = 0;
    };

    typedef enum fonaEvents{    evResponse1=200,
                                evResponse2
                             } fonaEvents;

    Fona(const char* deviceBinding,int baudrate);
    ~Fona();
    bool processEvent(Event* e);
   
    void subscribe(IFonaObserver* subscriber);
    void unsubscribe(IFonaObserver* subscriber);
    void notify(string text);
    void send(string command);
    string convertToString(uint8_t data[MAXDATASIZE]);
    void elaborateMessage(u8_t character);

    uint8_t data1[MAXDATASIZE];
    uint8_t data2[MAXDATASIZE];
    bool switcher;
    uint8_t buffer[MAXDATASIZE];
    uint8_t pos;

private:
    Event evRp1;     //Response1 event
    Event evRp2;     //Response2 event
    vector<IFonaObserver*> subscribers;
};

#endif