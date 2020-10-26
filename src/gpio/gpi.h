/* this is the Button class */
#include <drivers/gpio.h>
#include "gpio.h"
#include "../xf/xf.h"
#include <map>
#include <vector>
using namespace std;

#ifndef GPI_ONCE
#define GPI_ONCE

/**
 * @class General Purpose Input
 * @brief This class implements the attributes and methods of a General Purpose Input
 */
class GPI : public GPIO
{
public:

    GPI(int p1, const char* port);
    virtual ~GPI();
    void initHW();
    void setPullup();
    void setPulldown();
    GPIO::PINSTATE read();

protected:
    enum MODE  {PULLUP, PULLDOWN};  /**< enum mode of input (pullup/pulldown) */
    MODE mode;                      /**< mode of input */
};

#endif
