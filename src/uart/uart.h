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
        UART(const char* deviceBinding,int baudrate);
        ~UART();
        void initHW();

        void uartSend(const char* txData);
        
        bool enableRXInterrupt();

        int getBaudrate();
        void setBaudrate(int baudrate);
        void updateBaudrate();

    private:
        const struct device* uart_dev;  /**< device structur driver */ 
        struct uart_config uart_cfg;    /**< UART configuration */
        const char* deviceBinding;
        int baudrate;
        u8_t character;

        static void uartReceive(const struct device *dev, void *data);
};

#endif