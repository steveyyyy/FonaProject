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
#include <algorithm>
#include <string>

#ifndef FONA_ONCE
#define FONA_ONCE

using namespace std;

class Fona : public UART::IUARTObserver
{
public:

    class IFonaObserver
    {
        public:
        virtual void onResponse(char* message) = 0;
    };

    explicit Fona(UART* uart);
    ~Fona();
    void subscribe(IFonaObserver* subscriber);
    void unsubscribe(IFonaObserver* subscriber);
    void notify();
    void send(string command);
    void onMessage(k_msgq* messages) override;
private:
    UART* uart; 
    vector<IFonaObserver*> subscribers;
    char data[MAXDATASIZE];
};

#endif