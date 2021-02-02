#include "../../gpio/gpo.h"

#ifndef RINGER_ONCE
#define RINGER_ONCE

class Ringer
{
public:
    Ringer(int p1, const char *port, int hertz=20);
    ~Ringer();
    void initHW();
    void stop();
    void ring();
private:
    GPO pin;
    int hertz;
    bool pinState;

    struct k_timer* oscillator;
    static void oscillatorToggle(struct k_timer* oscillator);
};
#endif