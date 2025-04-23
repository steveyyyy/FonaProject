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

#include "event.h"

Event::Event() {
    this->id = evNone;
    this->t = nullptr;
    this->delay = 0;
    this->deleteAfterConsume = false;
    this->target = nullptr;
}

Event::Event(evID eventID) {
    this->id = evNone;
    this->t = nullptr;
    this->delay = 0;
    this->deleteAfterConsume = false;
    this->target = nullptr;
}

Event::~Event() {
    
}

void Event::setTarget(IReactive* target) {
    this->target = target;
}

IReactive* Event::getTarget() {
    return this->target;
}

void Event::setId(evID eventID) {
    this->id = eventID;
}

Event::evID Event::getId() {
    return this->id;
}

void Event::setDnd(bool doNotDelete) {
    this->deleteAfterConsume = doNotDelete;
}

bool Event::getDnd() {
    return this->deleteAfterConsume;
}

void Event::setDelay(int delay) {
    this->delay = delay;
}

int Event::getDelay() {
    return this->delay;
}

void Event::setT(struct k_timer* t) {
    this->t = t;
}

struct k_timer* Event::getT() {
    return this->t;
}