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