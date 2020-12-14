#include "fona.h"

Fona::Fona(const char* deviceBinding,int baudrate):UART(deviceBinding,baudrate)
{
    this->evRp1.setTarget(this);
    this->evRp1.setDnd(true);
    this->evRp1.setId((Event::evID)evResponse1);

    this->evRp2.setTarget(this);
    this->evRp2.setDnd(true);
    this->evRp2.setId((Event::evID)evResponse2);

    pos='0';
    switcher =true;
}

Fona::~Fona(){}

void Fona::elaborateMessage(u8_t character){
    buffer[pos]=character;
    pos++;
    if(character == 0x0A){
        if(pos <= 3){
            pos=0;
        }
        else{
            //buffer[pos]=0x9F;
            if(switcher){
                 memcpy(data1,buffer,MAXDATASIZE);
                 XF::getInstance()->pushEvent(&evRp1);
            }
            else{
                 memcpy(data2,buffer,MAXDATASIZE);
                 XF::getInstance()->pushEvent(&evRp2);
            }
            switcher = !switcher;
            
            pos=0; 
        }
    }
}

void Fona::send(string message){
        message += "\r";
        uartSend(message.c_str());
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

void Fona::notify(string text)
{
    vector<IFonaObserver*>::iterator it;
    for (it=subscribers.begin(); it!=subscribers.end();++it)
    {
        (*it)->onResponse(text);
    }
}

bool Fona::processEvent(Event* e)
{
    bool processed=false;    
            if (e->getId() == (Event::evID)evResponse1)
            {
                notify(convertToString(data1));
                processed=true;
            }
            if (e->getId() == (Event::evID)evResponse2)
            {
                notify(convertToString(data2));
                processed=true;
            }
        return processed;
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
    //printk(s.c_str());
    return s; 
}