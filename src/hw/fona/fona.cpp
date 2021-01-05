#include "fona.h"

Fona::Fona(UART* uart)
{
    this->uart=uart;
    //Error doesn happen here
    this->uart->subscribe(this);
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
    // if(k_is_in_isr()){
    //     printk("i am iqr: true\n");
    // }
    // else{
    //     printk("i am not iqr: false\n");
    // };
    
    k_msgq_get(messages, &data, K_NO_WAIT);
    //string s = (char *)data;
    // printk("DATA: ");
    //printk(s.c_str());
    // printk((char *)data);
    // printk("\n");
    // if((char *)data!="\r\r\n"){
    //     notify();
    // }
    char textbeginning[2] = {0};
    strncpy(textbeginning, (char *)data, 2);
    if(!(strcmp(textbeginning,"\r\n")==0)){
        notify();
    }
}

// string Fona::convertToString(uint8_t data[MAXDATASIZE]) 
// { 
//     bool end= true;
//     int i=0;
//     string s = "";
//     while(end){
//         if(data[i]==0x0D){
//             end=false;
//             break;
//         }
//         else{
//             s = s + (char)data[i];
//             i++;
//             }
//     }
//     return s; 
// }