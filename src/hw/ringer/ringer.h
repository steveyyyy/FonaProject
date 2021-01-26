#include "../../gpio/gpo.h"

#include <vector>
#include <algorithm>
#include <string>

using namespace std;

#ifndef RINGER_ONCE
#define RINGER_ONCE

class Ringer
{
public:
    class IRingerObserver
        {
            public:
            virtual void onRingOver() = 0;
        };

    void subscribe(IRingerObserver* subscriber);
    void unsubscribe(IRingerObserver* subscriber);
    void notify();

    Ringer(int p1, const char *port, int hertz=20, int ringTime=2);
    ~Ringer();
    void initHW();
    void stop();
    void ring();
private:
    vector<IRingerObserver*> subscribers;
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