#include "display.h"

Display::Display(UART* uart, u8_t command=254, u8_t specialCommand=124){
    this->uart=uart;
    this->command=command;
    this->specialCommand=specialCommand;
}

void Display::initHW(){
    uart->subscribe(this);
}

void Display::clearDispaly(){
    uart->uartSend((char *)command);
    uart->uartSend((char *)(u8_t)1);
}