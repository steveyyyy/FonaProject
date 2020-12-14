#include "fona.h"

Fona::Fona(const char* deviceBinding,int baudrate):UART(deviceBinding,baudrate)
{
    this->ev.setTarget(this);
    this->ev.setDnd(true);
    this->ev.setId(Event::evDefault);

    this->evIni.setTarget(this);
    this->evIni.setDnd(true);
    this->evIni.setId(Event::evInitial);

    this->evRp.setTarget(this);
    this->evRp.setDnd(true);
    
    pos='0';
    switcher =true;
}

Fona::~Fona(){}

void Fona::elaborateMessage(u8_t character){
    buffer[pos]=character;
    pos++;
    if(character == 0x0A){
        if(buffer[0]==0x0D&&buffer[1]==0x0A){
            pos=0;
        }
        else{
            //buffer[pos]=0x9F;
            if(switcher){
                memcpy(data1,buffer,MAXDATASIZE);
                this->evRp.setId((Event::evID)evResponse1);
            }
            else{
                memcpy(data2,buffer,MAXDATASIZE);
                this->evRp.setId((Event::evID)evResponse2);
            }
            switcher = !switcher;
            XF::getInstance()->pushEvent(&evRp);
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
    FONASTATES oldstate;
    bool processed=false;

    oldstate = this->state;
    switch (this->state)
    {
        case ST_INIT:
            if (e->getId() == Event::evInitial)
            {
                this->state = ST_IDLE;
            }
        break;
        case ST_IDLE:
            if (e->getId() == (Event::evID)evResponse1)
            {
                this->state = ST_NOTIFY1;
            }
            if (e->getId() == (Event::evID)evResponse2)
            {
                this->state = ST_NOTIFY2;
            }
        break;
        case ST_NOTIFY1:
            if (e->getId() == Event::evDefault)
            {
                this->state = ST_IDLE;
            }
        break;
        case ST_NOTIFY2:
            if (e->getId() == Event::evDefault)
            {
                this->state = ST_IDLE;
            }
        break;
    }

    if (oldstate != this->state)
    {
        processed=true;
        switch (this->state)
        {
            case ST_INIT:
            break;
            case ST_IDLE:
            break;
            case ST_NOTIFY1:
                notify(convertToString(data1));
                XF::getInstance()->pushEvent(&ev);
            break;
            case ST_NOTIFY2:
                notify(convertToString(data2));
                XF::getInstance()->pushEvent(&ev);
            break;
        }
    }
    return processed;
}

void Fona::startBehaviour()
{
    evIni.setId(Event::evInitial);
    evIni.setDelay(0);
    XF::getInstance()->pushEvent(&evIni);
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