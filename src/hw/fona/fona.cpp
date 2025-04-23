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

#include "fona.h"

Fona::Fona(UART* uart)
{
    this->uart=uart;
    //Error doesn happen here
    this->uart->subscribe(this);
}

Fona::~Fona(){}

void Fona::send(string message){
        message += "\r";
        uart->uartSend(message.c_str());
}

void Fona::subscribe(IFonaObserver* subscriber)
{
    vector<IFonaObserver*>::iterator it;
    it = find(subscribers.begin(), subscribers.end(),subscriber);
    if (it == subscribers.end())
    {
        subscribers.push_back(subscriber);
    }
}

void Fona::unsubscribe(IFonaObserver* subscriber)
{
    vector<IFonaObserver*>::iterator it;
    it = find(subscribers.begin(), subscribers.end(),subscriber);
    if (it != subscribers.end())
    {
        subscribers.erase(it);
    }
}

void Fona::notify()
{
    vector<IFonaObserver*>::iterator it;
    for (it=subscribers.begin(); it!=subscribers.end();++it)
    {
        (*it)->onResponse((char*)data);
    }
}

void Fona::onMessage(k_msgq* messages){  
    k_msgq_get(messages, &data, K_NO_WAIT);
    if(!(strncmp(data, "\r\n",2)==0)){
        notify();
    }
}