/* this is the Button class */
#include <drivers/gpio.h>


#ifndef GPIO_ONCE
#define GPIO_ONCE

/**
 * @class General Purpose Input/Output
 * @brief This class implements the Attributes and Methods of a General Purpose Input/Output.
 */
class GPIO
{
public:

    GPIO(int p1, const char* port);
    virtual ~GPIO();
    void initHW();
    enum PINSTATE {PIN_OFF=0, PIN_ON=1}; /**< enum state of pin (on/off)*/
    int getPin();
    int getUId();
    struct device* getDriver();

protected:
    u8_t id;                /**< unique ID */
    int pin;                /**< pin number */
    struct device* driver;  /**< device structur driver */ 
    char* port;             /**< port name */
    int config;             /**< configuration number */
    static u8_t uid;        /**< unique ID generator */
};

#endif
