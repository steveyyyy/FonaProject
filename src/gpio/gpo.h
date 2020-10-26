/* this is the Button class */
#include <drivers/gpio.h>
#include "gpio.h"

#ifndef GPO_ONCE
#define GPO_ONCE

/**
 * @class General Purpose Output 
 * @brief This class implements the attributes of a General Purpose Output
 */
class GPO : public GPIO
{
public:

    GPO(int p1, const char* port);
    virtual ~GPO();
    void setHighPower();
    void setLowPower();
    void setInitialOn();
    void setInitialOff();
    void setOpenDrain();
    void setOpenSource();
    void initHW();
    void on();
    void off();
    void set(PINSTATE state);

protected:

    enum FORCE {FORCENONE,WEEK,STRONG};             /**< enum amount of force */
    enum MODE  {MODENONE, OPENDRAIN, OPENSOURCE};   /**< enum mode of output */
    PINSTATE initialState;                          /**< PINSTATE state of output */
    FORCE strength;                                 /**< FORCE amount of force */
    MODE mode;                                      /**< MODE mode of output */
};

#endif
