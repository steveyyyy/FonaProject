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

#include "ireactive.h"

#ifndef EVENT_ONCE
#define EVENT_ONCE


class Event
{
public:
    typedef enum evID {evNone, evInitial, evTimeout, evDefault, evPressed, evReleased, evInt } evID;
    Event(/* args */);
    explicit Event(evID eventID);
    ~Event();
    void setTarget(IReactive* target);
    IReactive* getTarget();
    void setId(evID eventID);
    evID getId();
    void setDnd(bool doNotDelete);
    bool getDnd();
    void setDelay(int delay);
    int getDelay();
    void setT(struct k_timer* timer);
    struct k_timer* getT();
private:
    evID id;
    IReactive* target;
    struct k_timer* t;
    bool deleteAfterConsume;
    int delay;
};

#endif