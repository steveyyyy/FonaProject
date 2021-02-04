#include "../../gpio/gpo.h"

#ifndef RINGER_ONCE
#define RINGER_ONCE

class Ringer
{
public:
    Ringer(int p1, const char *port, float hertz=16, float prozent=0.3333);
    ~Ringer();
    void initHW();
    void stop();
    void ring();
private:
    GPO pin;
    float pulseUp;
    float pulseDown;
    bool pinState;

    struct k_timer* oscillator;
    static void oscillatorToggle(struct k_timer* oscillator);
};
#endif