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

#include <zephyr.h>
#include <list>
#include "event.h"

#ifndef XF_ONCE
#define XF_ONCE


using namespace std;

/**
 * @class The XF 
 * @brief Our Event processing machine. 
 */
class XF
{
public:
    void init();
    static XF* getInstance();
    void pushEvent(Event* e);
    void execute();
    void unscheduleTM(Event* e);
private:
    XF(/* args */);
    ~XF();
    Event* popEvent();
    void scheduleTM(Event* e);
    static void onTimeout(struct k_timer* t);
    static void onStop(struct k_timer* t);
    static XF theXF;
    //list<Event*> evQueue;
    // struct k_mutex m;
    struct k_msgq queue;
    char __aligned(4) queue_buffer[10 * sizeof(Event*)];
};

#endif