#ifndef LED_ONCE
#define LED_ONCE

#include <zephyr.h>
#include "../../gpio/gpo.h"

/**
 * @class LED
 * @brief This class represents a LED 
 */
class LED
{
    public:
    LED(int p1, const char* port);
    ~LED();
    void initHW();
    void on();
    void off();

    private: 
    GPO pin; /**< General Purpose Output pin */
};
#endif