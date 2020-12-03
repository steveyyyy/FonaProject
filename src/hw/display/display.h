#include "../../uart/uart.h"
#include <vector>
#include <string>

#ifndef DISPLAY_ONCE
#define DISPLAY_ONCE

class Display: public UART
{
public:
    Display(const char* deviceBinding,int baudrate,u8_t command, u8_t specialCommand);
    ~Display();
    void initHW();
    void clearDispaly();

    void updateBaudrate(int baudrate);
    void setBacklight(u8_t brightness);
    void setCursorPosition(u8_t position);
    void changeSplashScreen(string splashScreen);
private:
    u8_t command;
    u8_t specialCommand;  
    int baudrateOptions[6]={2400, 
                            4800, 
                            9600, 
                            4400, 
                            19200, 
                            38400};                       
};

#endif