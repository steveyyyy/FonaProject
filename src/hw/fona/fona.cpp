#include "fona.h"

Fona::Fona(UART* uart)
{
    this->uart=uart;
    //Error doesn happen here
    //this->uart->subscribe(this);
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
        //(*it)->onResponse();
    }
}

string Fona::convertToString(uint8_t data[MAXDATASIZE]) 
{ 
    bool condition= true;
    int i=0;
    string s = "";
    while(condition){
        s = s + (char)data[i];
        if(data[i]==0x0A){
            condition=false;
            break;
        }
        i++;
    }
    return s; 
}

void Fona::onMessage(){
    // if(k_is_in_isr()){
    //     printk("i am iqr: true\n");
    // }
    // else{
    //     printk("i am not iqr: false\n");
    // }
    uint8_t* text = uart->getMessageFromQueue();
    //uint8_t* txt = uart->getMessageFromQueue();
    printk((char*)text);
}
bool Fona::processEvent(Event* e){

}
void Fona::startBehaviour(){

}