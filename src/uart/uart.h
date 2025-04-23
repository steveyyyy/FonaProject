/* 
    FonaProject – Retro Rotary Phone Meets 3G
    Copyright (C) 2025  steveyyyy

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

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