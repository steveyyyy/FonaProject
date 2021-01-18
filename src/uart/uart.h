#ifndef UART_ONCE
#define UART_ONCE

#include "../xf/xf.h"
#include <drivers/uart.h>
#include <vector>
#include <algorithm>
#include <cstring>
#include <string>

#define MAXDATASIZE 32//32 //32 oder 28 anstatt 30 da es durch 4 teilbar ist

using namespace std;

class UART: public IReactive 
{
    public:
        class IUARTObserver
        {
            public:
            virtual void onMessage(k_msgq* messages) = 0;
        };

        UART(const char* deviceBinding,int baudrate, uint8_t endOfMessage=0x0A);
        ~UART();
        void initHW();

        void uartSend(const char* txData);
        
        bool enableRXInterrupt();

        void setBaudrate(int baudrate);

        void setEndOfMessage(uint8_t endOfMessage);

        void subscribe(IUARTObserver* subscriber);
        void unsubscribe(IUARTObserver* subscriber);
        void notify();

        bool processEvent(Event* e) override;

        typedef enum uartEvents{  evResponse=200,
                                    } uartEvents;

    private:
        const struct device* uart_dev;  /**< device structur driver */ 
        struct uart_config uart_cfg;    /**< UART configuration */
        const char* deviceBinding;
        int baudrate;
        static void uartReceive(const struct device *dev, void *data);
        char __aligned(4) messages_buffer[10 * sizeof(uint8_t[MAXDATASIZE])];
        struct k_msgq messages;
        uint8_t message[MAXDATASIZE];
        uint8_t pos;
        uint8_t endOfMessage;
        vector<IUARTObserver*> subscribers;
        Event rp; //response event
};

#endif