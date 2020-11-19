#include "../../uart/uart.h"
#include <vector>

#ifndef DISPLAY_ONCE
#define DISPLAY_ONCE


class Display : public UART::IUARTObserver
{
public:
    Display(UART* uart, u8_t command, u8_t specialCommand);
    ~Display();
    void initHW();
    void clearDispaly();

private:
    UART* uart;
    u8_t command;
    u8_t specialCommand;                         
};

#endif