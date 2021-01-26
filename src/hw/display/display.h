#include "../../uart/uart.h"
#include <vector>

#ifndef DISPLAY_ONCE
#define DISPLAY_ONCE

class Display : public UART::IUARTObserver
{
public:
    Display(UART* uart, u8_t command=0xFE, u8_t specialCommand=0x7C);
    ~Display();
    void initHW();
    void clearDispaly();

    void updateBaudrate(int baudrate);
    void setBacklight(u8_t brightness);
    void setCursorPosition(u8_t position);
    void changeSplashScreen(char* splashScreen);
private:
    UART* uart;
    u8_t command;
    u8_t specialCommand;
};

#endif