#ifndef UART_ONCE
#define UART_ONCE

#include "../xf/xf.h"
#include <drivers/uart.h>
#include <vector>
#include <algorithm>
#include <cstring>

#define MAXDATASIZE 32 //32 oder 28 anstatt 30 da es durch 4 teilbar ist

using namespace std;

class UART: public IReactive 
{
    public:
        class IUARTObserver
        {
            public:
            virtual void onMessage(uint8_t data[MAXDATASIZE]) = 0;
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
        void notify(uint8_t data[MAXDATASIZE]);

        bool processEvent(Event* e);
        void startBehaviour();

        typedef enum uartEvents{  evResponse=200,
                                    } uartEvents;

        typedef enum UARTSTATE {    ST_INIT,
                                    ST_IDLE,
                                    ST_RECEIVE
                                 } UARTSTATE;
        //uint8_t* getMessageFromQueue();
        //uint8_t* getMessageFromQueue();
    private:
        const struct device* uart_dev;  /**< device structur driver */ 
        struct uart_config uart_cfg;    /**< UART configuration */
        const char* deviceBinding;
        int baudrate;
        static void uartReceive(const struct device *dev, void *data);
        char __aligned(4) messages_buffer[10 * sizeof(uint8_t[MAXDATASIZE])];
        struct k_msgq messages;
        uint8_t message[MAXDATASIZE];
        //uint8_t data[MAXDATASIZE];
        uint8_t pos;
        uint8_t endOfMessage;
        vector<IUARTObserver*> subscribers;
        Event rp; //response event
        Event ev; //default/initial event
        UARTSTATE state;
};

#endif