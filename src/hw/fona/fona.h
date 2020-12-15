#include "../../xf/xf.h"
#include "../../uart/uart.h"
#include <vector>
#include <algorithm>
#include <string>

#ifndef FONA_ONCE
#define FONA_ONCE


class Fona : public UART::IUARTObserver
{
public:

    class IFonaObserver
    {
        public:
        //virtual void onResponse() = 0;
    };

    Fona(UART* uart);
    ~Fona();
    void subscribe(IFonaObserver* subscriber);
    void unsubscribe(IFonaObserver* subscriber);
    void notify();
    void send(string command);
    string convertToString(uint8_t data[MAXDATASIZE]);

    void onMessage(uint8_t data[MAXDATASIZE]);
private:
    UART* uart; 
    vector<IFonaObserver*> subscribers;
};

#endif