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

/* this is the Button class */
#include "../../xf/xf.h"
#include "../../gpio/gpi.h"
#include "../../gpio/intmanager.h"
#include <vector>

using namespace std;

#ifndef BUTTON_ONCE
#define BUTTON_ONCE

#define DEBOUNCEDELAY 10

/**
 * @class Button 
 * @brief This class represents a button on the board
 */
class Button : public IReactive, public IntManager::IIntObserver
{
public:
    /**
     * @interface IButtonObserver
     * @brief This Interface implements the method onButton which is fired when the button has been pressed/released
     */
    class IButtonObserver
    {
        public:
        virtual void onButton(int id, bool pressed) = 0;
    };

    typedef enum BUTTONSTATES {ST_INIT,ST_DEBOUNCE,ST_DECIDE, ST_PRESSED, ST_RELEASED} BUTTONSTATES; /**< enum states of a button */

    explicit Button(GPI* pin);
    ~Button();
    void initHW();
    bool processEvent(Event* e) override;
    void startBehaviour();
    void onInterrupt(u32_t pin) override;
    void subscribe(IButtonObserver* subscriber);
    void unsubscribe(IButtonObserver* subscriber);
    void notify();
    int getId();
    bool getCurrentState();

private:
    GPI* pin;                                /**< General Purpose Input pin */
    BUTTONSTATES state;                     /**< State of button */
    Event ev;
    Event tm;                               /**< event */
    vector<IButtonObserver*> subscribers;   /**< subscriber list */
};

#endif
