/* this is the interrupt manager class */
#include <drivers/gpio.h>
#include <vector>
#include <map>
#include <string.h>
using namespace std;


#ifndef INTMANAGER_ONCE
#define INTMANAGER_ONCE

/**
 * @class Interrupt Manager
 * @brief This class manages all of the interrupt signals
 */
class IntManager
{
public:
    /**
     * @interface Interrupt Observer
     * @brief This Interface implements the method onInterrupt, which is fired on an Interrupt 
     */
    class IIntObserver
    {
        public: 
        virtual void onInterrupt(u32_t pin) = 0;
    };

    typedef enum EDGE  {EDGENONE, RISING, FALLING, BOTH} EDGE;
    typedef struct PinPort
    {
        u8_t pin;
        const struct device* dev;
        bool operator==(const PinPort& other) const
        {
            return ((pin == other.pin) && (dev == other.dev));
        }
    } PinPort;

    typedef struct Interrupt
    {
        PinPort pp;
        EDGE edge;
        bool operator==(const Interrupt& other) const
        {
            return (pp == other.pp && edge==other.edge);
        }
    } Interrupt;

    typedef struct Subscription
    {
        IIntObserver* subscriber;
        PinPort pp;
        bool operator==(const Subscription& other) const
        {
            return (subscriber == other.subscriber && pp==other.pp);
        }
    } Subscription;

    typedef struct cbPinMap
    {
        const struct device* dev;
        struct gpio_callback cb;
        u32_t pinMap;
        cbPinMap() : pinMap(0) {};
        bool operator==(const cbPinMap& other) const
        {
            return (dev == other.dev);
        };
    } cbPinMap;


    //the public methods        
    void enableInt(Interrupt ntrpt);
    void disableInt(Interrupt ntrpt);
    void subscribe(Subscription sub);
    void unsubscribe(Subscription sub);
    static IntManager* getInstance();

    //the private methods
    private:
    IntManager();
    virtual ~IntManager();
    static void onInterrupt(const struct device* dev, struct gpio_callback* cb, u32_t pins);

    //the private data
    private: 
    static IntManager theIntMan;                /**< Interrupt Manager */
    static vector<IntManager::cbPinMap> cbList; /**< Callback List */
    static vector <Subscription> subscriptions; /**< Subscription List */
};
#endif


