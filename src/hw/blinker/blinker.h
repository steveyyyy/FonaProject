#include "../../xf/xf.h"
#include "../../xf/ireactive.h"
#include "../../xf/event.h"
#include "../led/led.h"

#ifndef BLINKER_ONCE
#define BLINKER_ONCE

class Blinker : public IReactive
{
public:
    explicit Blinker(int blinkDelay);
    Blinker(int blinkDelayOn, int blinkDelayOff);
    ~Blinker(); 
    void start();
    void stop();
    void setLed(LED* led);
    void setDelayOn(int delay);
    void setDelayOff(int delay);
    bool processEvent(Event* e) override;
    void startBehaviour();

private:
    LED* led;
    typedef enum BLINKER_STATES{ ST_NONE, ST_INIT, ST_ON, ST_OFF }BLINKER_STATES;   /**< blinker states */
    Event in;
    Event tm;
    BLINKER_STATES state;                                                           /**< state of blinker */
    int blinkDelayOn, blinkDelayOff;                                                /**< on/off delays */
    bool diverseBlink;                                                              /**< differ between regular or different on/off timings */
};

#endif