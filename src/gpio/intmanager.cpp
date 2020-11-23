/* this is the interrupt manager class */
#include "intmanager.h"
#include <algorithm>
using namespace std;

//this is the singleton instance
IntManager IntManager::theIntMan;
vector<IntManager::cbPinMap> IntManager::cbList;
vector <IntManager::Subscription> IntManager::subscriptions;

/**
 * @brief Construct a new Interrupt Manager object
 * 
 */
IntManager::IntManager()
{
}

/**
 * @brief Destroy the Interrupt Manager object
 * 
 */
IntManager::~IntManager()
{
}

/**
 * @brief Configure Interrupt 
 * 
 * @param ntrpt Interrupt
 */
void IntManager::enableInt(Interrupt ntrpt)
{
    if (ntrpt.pp.dev)
    {
        int flags = 0;
        switch (ntrpt.edge)
        {
            case (RISING):
            flags = GPIO_INT_EDGE_RISING;
            break;
            case (FALLING):
            flags = GPIO_INT_EDGE_FALLING;
            break;
            case (BOTH):
            flags = GPIO_INT_EDGE_BOTH;
            break;
            case (EDGENONE):
            flags = GPIO_INT_DISABLE;
            break;
            default:
            break;
        }
        gpio_pin_interrupt_configure(ntrpt.pp.dev,ntrpt.pp.pin,flags);
    }
    else
    {
        printk("device binding for %x invalid\n",(unsigned int) ntrpt.pp.dev);
    }
}

/**
 * @brief Disable Interrupt
 * 
 * @param ntrpt 
 */
void IntManager::disableInt(Interrupt ntrpt)
{
    if (ntrpt.pp.dev)
    {
        gpio_pin_interrupt_configure(ntrpt.pp.dev,ntrpt.pp.pin,GPIO_INT_DISABLE);
    }
    else
    {
        printk("device binding for %x invalid\n",(unsigned int) ntrpt.pp.dev);
    }    
}

/**
 * @brief Check if subscription already exists and add it if not
 * 
 * @param sub Subscription
 */
void IntManager::subscribe(Subscription sub)
{
    vector<Subscription>::iterator it;
    it = find(subscriptions.begin(), subscriptions.end(),sub);
    if (it != subscriptions.end())
    {
        printk("subscription already exists\n");
    }
    else
    {
        subscriptions.push_back(sub);   
        if (sub.pp.dev)
        {
            bool exist = false;
            int pos = 0;
            vector<cbPinMap>::size_type i;
            for (i=0; i<cbList.size(); i++)
            {
                if (cbList[i].dev == sub.pp.dev)
                {
                    pos = i; 
                    exist = true;
                }
            }

            //fndr = find(cbList.begin(), cbList.end(),dev);
            if (exist)
            {
                cbList[pos].pinMap |= BIT(sub.pp.pin);
            }
            else
            {
                cbPinMap cpm;
                cpm.dev = sub.pp.dev;
                cpm.pinMap = BIT(sub.pp.pin);
                cbList.push_back(cpm);
                pos = cbList.size()-1;        
            }
            //Initialize the callback as should be
            gpio_init_callback(&(cbList[pos].cb),IntManager::onInterrupt,cbList[pos].pinMap);
            //maybe this is an existing callback and only pinmap changed
            gpio_remove_callback(sub.pp.dev,&(cbList[pos].cb));
            //add the new callback
            gpio_add_callback(sub.pp.dev,&(cbList[pos].cb));
        }
        else
        {
            printk("device binding for %x invalid\n",(unsigned int) sub.pp.dev);
        }
    }
}

/**
 * @brief Remove the given subscription if it exists
 * 
 * @param sub Subscription
 */
void IntManager::unsubscribe(Subscription sub)
{
    vector<Subscription>::iterator it;
    it = find(subscriptions.begin(), subscriptions.end(),sub);
    if (it != subscriptions.end())
    {
        subscriptions.erase(it);
        if (sub.pp.dev)
        {
            bool exist = false;
            int pos = 0;
            vector<cbPinMap>::size_type i;
            for (i=0; i<cbList.size(); i++)
            {
                if (cbList[i].dev == sub.pp.dev)
                {
                    pos = i; 
                    exist = true;
                }
            }
            if (exist)
            {
                cbList[pos].pinMap &= ~BIT(sub.pp.pin);
                //Initialize the callback as should be
                gpio_init_callback(&(cbList[pos].cb),IntManager::onInterrupt,cbList[pos].pinMap);
                //maybe this is an existing callback and only pinmap changed
                gpio_remove_callback(sub.pp.dev,&(cbList[pos].cb));
                //add the new callback
                gpio_add_callback(sub.pp.dev,&(cbList[pos].cb));
            }
            else
            {
                printk("no such device (%x) found in pinmap\n",(unsigned int) sub.pp.dev);
            }
        }
        else
        {
            printk("device binding for %x invalid\n",(unsigned int) sub.pp.dev);
        }
    }
    else
    {
        printk("no such subscription\n");
    }
}

//the static methods of the interrupt manager
/**
 * @brief Return instance of the Interrupt Manager
 * 
 * @return IntManager* 
 */
IntManager* IntManager::getInstance()
{
    return &theIntMan;
}

/**
 * @brief Fire Interupt on all observers that are subscribed
 * 
 * @param dev Driver
 * @param cb Callback
 * @param pins pins
 */
void IntManager::onInterrupt(const struct device* dev, struct gpio_callback* cb, u32_t pins)
{
    int key = irq_lock();
    vector<u8_t>::size_type i;
    for (i=0; i < subscriptions.size(); i++)
    {
        if ((subscriptions[i].pp.dev == dev) && 
            (BIT(subscriptions[i].pp.pin) & pins))
        {
            subscriptions[i].subscriber->onInterrupt(subscriptions[i].pp.pin); //onInterrupt from the Interface gets fired
        }
    }
    irq_unlock(key);
}
