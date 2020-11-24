#ifndef UART_ONCE
#define UART_ONCE

#include <drivers/uart.h>
#include <vector>
#include <algorithm>
#include <cstring>
#include "../xf/xf.h"

using namespace std;

class UART : public IReactive
{
    public:
        class IUARTObserver
        {
            public:
            virtual void onMessage(u8_t character)=0;
        };
        UART(const char* deviceBinding,int baudrate);
        ~UART();
        void initHW();

        void uartSend(const char* txData);
        
        bool enableRXInterrupt();

        void subscribe(IUARTObserver* subscriber);
        void unsubscribe(IUARTObserver* subscriber);
        void notify(u8_t character);
        int getBaudrate();
        void setBaudrate(int baudrate);
        void updateBaudrate();

        bool processEvent(Event* e);
        //void startBehaviour();

    protected:
        vector<IUARTObserver*> subscribers;
        const struct device* uart_dev;  /**< device structur driver */ 
        struct uart_config uart_cfg;    /**< UART configuration */
        const char* deviceBinding;
        int baudrate;
        u8_t character;

        Event ev;

        static void uartReceive(const struct device *dev, void *data);
};

#endif