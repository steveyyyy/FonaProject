#include "../../gpio/gpo.h"
#ifndef RINGER_ONCE
#define RINGER_ONCE

class Ringer
{
public:
    Ringer(int p1, const char *port, int hertz=20, int ringTime=2);
    ~Ringer();
    void initHW();
    void stop();
    void ring();
private:
    GPO pin;
    int hertz;
    int ringTime;
    bool pinState;

    struct k_timer* oscillator;
    static void oscillatorToggle(struct k_timer* oscillator);
    struct k_timer* ringTimer;
    static void ringTimerTimeout(struct k_timer* ringTimer);
};
#endif