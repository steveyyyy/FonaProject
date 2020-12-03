#include "display.h"

Display::Display(u8_t command=254, u8_t specialCommand=124){
    //this->uart=uart;
    this->command=command;
    this->specialCommand=specialCommand;
}

void Display::clearDispaly(){
    //uart->uartSend((char *)command);
    //uart->uartSend((char *)(u8_t)1);
}

void Display::changeBaudrate(int baudrate){

}