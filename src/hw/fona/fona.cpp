#include "fona.h"

Fona::Fona(UART* uart)
{
    this->uart=uart;
    //Error doesn happen here
    this->uart->subscribe(this);
    message="";
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
        (*it)->onResponse(message);
    }
}

void Fona::onMessage(k_msgq* messages){
    // string s="2398475923847593284sadfhaskljdfnksajdfnaslkjdnf";
    // printk(s.c_str());  
    k_msgq_get(messages, &data, K_NO_WAIT);
    //convertToString();
    message=string("MISSED_CALL: 08:40AM 0788903126");
    //printk(message.c_str());
    //printk to loggin 
    // char textbeginning[2] = {0};
    // strncpy(textbeginning, data, 2);
    if(message!=""){
        notify();
    }
}

void Fona::convertToString()
{ 
    bool condition= true;
    int i=0;
    message = "";
    while(condition){
        if(data[i]==0x0D){
            condition=false;
            break;
        }
        else{
            message = message + (char)data[i];
            i++;
        }
    }
    //message="9834u598324753924875sakdjnflasödjkf";
}