#include "fona.h"

Fona::Fona(UART* uart)
{
    this->uart=uart;
    //Error doesn happen here
    this->uart->subscribe(this);
    s="";
}

Fona::~Fona(){}

void Fona::send(string message){
        message += "\r";
        uart->uartSend(message.c_str());
}

void Fona::subscribe(IFonaObserver* subscriber)
{
    vector<IFonaObserver*>::iterator it;
    it = find(subscribers.begin(), subscribers.end(),subscriber);
    if (it == subscribers.end())
    {
        subscribers.push_back(subscriber);
    }
}

void Fona::unsubscribe(IFonaObserver* subscriber)
{
    vector<IFonaObserver*>::iterator it;
    it = find(subscribers.begin(), subscribers.end(),subscriber);
    if (it != subscribers.end())
    {
        subscribers.erase(it);
    }
}

void Fona::notify()
{
    vector<IFonaObserver*>::iterator it;
    for (it=subscribers.begin(); it!=subscribers.end();++it)
    {
        (*it)->onResponse((char*)data);
    }
}

void Fona::onMessage(k_msgq* messages){  
    k_msgq_get(messages, &data, K_NO_WAIT);
    //int key = irq_lock();
    s="                 ";//only 15 char for some reasons reasons found need to declare this thing in fona.h
    //irq_unlock(key);      //printk to loggin 
    char textbeginning[2] = {0};
    strncpy(textbeginning, data, 2);
    if(!(strcmp(textbeginning,"\r\n")==0)){
        notify();
    }
}