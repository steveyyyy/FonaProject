#ifndef UART_ONCE
#define UART_ONCE

#include <drivers/uart.h>
#include <vector>
#include <algorithm>
#include <cstring>

using namespace std;

class UART
{
    public:
        class IUARTObserver
        {
            public:
            virtual void onMesssage(const char* message);
        };
        UART(const char* deviceBinding,int baudrate);
        ~UART();
        void initHW();

        void uartSend(const char* txData);
        
        bool enableRXInterrupt();

        void subscribe(IUARTObserver* subscriber);
        void unsubscribe(IUARTObserver* subscriber);
        void notify();
    protected:
        vector<IUARTObserver*> subscribers;
        struct device* uart_dev;  /**< device structur driver */ 
        struct uart_config uart_cfg;    /**< UART configuration */
        const char* deviceBinding;
        int baudrate;
        const char* message;

        static void uartReceive(void* data);
};

#endif