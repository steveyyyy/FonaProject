#include "display.h"

Display::Display(const char* deviceBinding,int baudrate, u8_t command=0xFE, u8_t specialCommand=0x7C):UART(deviceBinding,baudrate){
    this->command=command;
    this->specialCommand=specialCommand;
}

void Display::clearDispaly(){
    uartSend((char *)command);
    uartSend((char *)(u8_t)1);
}

void Display::updateBaudrate(int baudrate){
    uartSend((char *)specialCommand);
    //uartSend((char *)(u8_t)baudrate);
}
void Display::setBacklight(u8_t brightness){
    uartSend((char *)specialCommand);
    uartSend((char *)brightness);
}
void Display::setCursorPosition(u8_t position){
    uartSend((char *)command);
    position = 0x80+position;
    if(position <= 0xCF){
        uartSend((char *)position);
    }  
}
void Display::changeSplashScreen(string splashScreen){
    setCursorPosition(0x00);
    uartSend(splashScreen.c_str());
    uartSend((char *)specialCommand);
}