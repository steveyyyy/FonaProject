#include "display.h"
#include <logging/log.h>
LOG_MODULE_REGISTER(display, CONFIG_DISPLAY_LOG_LEVEL);

Display::Display(UART* uart, u8_t command, u8_t specialCommand){
    this->command=command;
    this->specialCommand=specialCommand;

    this->uart=uart;
    this->uart->subscribe(this);
}

void Display::clearDispaly(){
    uart->uartSend((char *)&command);
    uart->uartSend((char *)1);
}

void Display::updateBaudrate(int baudrate){
    switch (baudrate)
    {
    case 2400:
        uart->uartSend((char *)&specialCommand);
        uart->uartSend((char *)0x0B);
        break;
    case 4800:
        uart->uartSend((char *)&specialCommand);
        uart->uartSend((char *)0x0C);
        break;
    case 9600:
        uart->uartSend((char *)&specialCommand);
        uart->uartSend((char *)0x0D);
        break;
    case 4400:
        uart->uartSend((char *)&specialCommand);
        uart->uartSend((char *)0x0E);
        break;
    case 19200:
        uart->uartSend((char *)&specialCommand);
        uart->uartSend((char *)0x0F);
        break;
    case 38400:
        uart->uartSend((char *)&specialCommand);
        uart->uartSend((char *)0x10);
        break;
    default:
        LOG_ERR("Wrong Baudrate");
        break;
    }
}
void Display::setBacklight(u8_t brightness){
    if((brightness > 128)&&(brightness < 157)){
        uart->uartSend((char *)&specialCommand);
        uart->uartSend((char *)&brightness);
    }
    else{
        LOG_ERR("Wrong brightness. The value needs to be between 128-157.");
    }
}
void Display::setCursorPosition(u8_t position){
    uart->uartSend((char *)&command);
    position = 0x80+position;
    if(position <= 0xCF){
         uart->uartSend((char *)&position);
     }  
}
void Display::changeSplashScreen(char* splashScreen){
    clearDispaly();
    uart->uartSend(splashScreen);
    uart->uartSend((char *)&specialCommand);
    uart->uartSend((char *)0x0A);
}