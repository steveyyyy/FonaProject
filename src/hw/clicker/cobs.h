#ifndef COBS_ONCE
#define COBS_ONCE

#include "../../xf/xf.h"
#include "../../xf/ireactive.h"
#include "../../xf/event.h"
#include "../button/button.h"
#include <vector>

/**
 * @class Click Observer
 * @brief Process button inputs to decide between single, double and long clicks
 * 
 */
class ClickObserver : public IReactive, public Button::IButtonObserver
{
public:
    /**
     * @class Interface Click Observer
     * @brief This Interface implements the method onClick which is fired when the button has been pressed/released in a specific duration and amount
     */
    class IClickObserver
    {
        public:
        virtual void onClick(int signal) = 0;
    };

    typedef enum CLICKSTATES {ST_INIT,ST_IDLE,ST_CLICK_AWAIT,ST_DOUBLE_AWAIT,ST_LONG,ST_SHORT,ST_DOUBLE,ST_CLICK} CLICKSTATES;

    ClickObserver(/* args */);
    ~ClickObserver();
    void subscribe(IClickObserver* subscriber);
    void unsubscribe(IClickObserver* subscriber);
    void notify();
    void onButton(int id, bool pressed) override;
    bool processEvent(Event* e) override;
    void startBehavior();
    
private:
    #define LONG_CLICK_DELAY 1000000        /**< Required pressing time for a long click */
    #define DOUBLE_CLICK_DELAY 200000       /**< Required time between clicks */
    Button* button;                         /**< Observed Button */
    Event event;                            /**< Regular Event */
    Event evTm;                             /**< Event Timer */
    CLICKSTATES state;                      /**< Current State */ 
    vector<IClickObserver*> subscribers;    /**< Subscribers */
};

#endif