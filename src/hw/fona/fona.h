#include "../../xf/xf.h"
#include "../../uart/uart.h"
#include <vector>
#include <algorithm>
#include <string>

#ifndef FONA_ONCE
#define FONA_ONCE

using namespace std;

class Fona : public UART::IUARTObserver
{
public:

    class IFonaObserver
    {
        public:
        virtual void onResponse(char* message) = 0;
    };

    explicit Fona(UART* uart);
    ~Fona();
    void subscribe(IFonaObserver* subscriber);
    void unsubscribe(IFonaObserver* subscriber);
    void notify();
    void send(string command);
    void onMessage(k_msgq* messages) override;
private:
    UART* uart; 
    vector<IFonaObserver*> subscribers;
    char data[MAXDATASIZE];
};

#endif