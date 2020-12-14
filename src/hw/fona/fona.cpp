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
    this->evRp.setId((Event::evID)evResponse);
    pos='0';
}

Fona::~Fona(){}

void Fona::elaborateMessage(u8_t character){
    buffer[pos]=character;
    pos++;
    if(character == 0x0A){
        //buffer[pos]=0x9F;
        memcpy(data,buffer,MAXDATASIZE);
        pos=0;
        XF::getInstance()->pushEvent(&evRp);
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

void Fona::notify()
{
    vector<IFonaObserver*>::iterator it;
    for (it=subscribers.begin(); it!=subscribers.end();++it)
    {
        (*it)->onResponse(data);
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
            if (e->getId() == (Event::evID)evResponse)
            {
                this->state = ST_NOTIFY;
            }
        break;
        case ST_NOTIFY:
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
            case ST_NOTIFY:
                notify();
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