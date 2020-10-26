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

    Button(int p1, const char* port);
    ~Button();
    void initHW();
    bool processEvent(Event* e);
    void startBehaviour();
    void onInterrupt(u32_t pin);
    void subscribe(IButtonObserver* subscriber);
    void unsubscribe(IButtonObserver* subscriber);
    void notify();
    int getId();

private:
    GPI pin;                                /**< General Purpose Input pin */
    BUTTONSTATES state;                     /**< State of button */
    Event ev;                               /**< event */
    vector<IButtonObserver*> subscribers;   /**< subscriber list */
};

#endif
