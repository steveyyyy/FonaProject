#ifndef UART_ONCE
#define UART_ONCE

#include <drivers/uart.h>
#include <vector>
#include <algorithm>
#include <string>
#include <cstring>

using namespace std;

class UART
{
    public:
        class IUARTObserver
        {
            public:
            virtual void onMesssage(string message);
        };
        UART();
        ~UART();
        static UART* getInstance();
        void initHW();
        void checkConfig();
        void setBaudrate(int baudrate);

        void uartSend(const char* txData);
        static void uartRecive(struct device* dev);
        bool enableRXInterrupt();

        void subscribe(IUARTObserver* subscriber);
        void unsubscribe(IUARTObserver* subscriber);
        void notify();

    protected:
        vector<IUARTObserver*> subscribers;
        struct device* uart_dev;  /**< device structur driver */ 
        struct uart_config uart_cfg;    /**< UART configuration */
        struct uart_config uart_cfg_check;  /**< UART configuration check */
        int baudrate;
        string message;
        static UART uart;

    
};

#endif