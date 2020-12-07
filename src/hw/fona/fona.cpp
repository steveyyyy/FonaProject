#include "fona.h"

Fona::Fona(const char* deviceBinding,int baudrate):UART(deviceBinding,baudrate)
{
    //this->message="";

    this->ev.setTarget(this);
    this->ev.setDnd(true);
    this->ev.setId(Event::evDefault);

    this->evIni.setTarget(this);
    this->evIni.setDnd(true);
    this->evIni.setId(Event::evInitial);

    this->evErr.setTarget(this);
    this->evErr.setDnd(true);
    this->evErr.setId((Event::evID)evError);

    this->evCmd.setTarget(this);
    this->evCmd.setDnd(true);
    this->evCmd.setId((Event::evID)evCommand);

    this->evRp.setTarget(this);
    this->evRp.setDnd(true);
    this->evRp.setId((Event::evID)evResponse);

 //   selector=dataChoice;
}

Fona::~Fona(){}

void Fona::elaborateMessage(u8_t character){
    if (pos < MAXDATASIZE-2)
                {
                    buffer[pos] = character;
                    if (character == 0x0A)
                    {
                        //we found a 0D0A or a 0A
                        buffer[pos+1] = 0;
                        memcpy(data,buffer,MAXDATASIZE);
                        pos=0;
                        XF::getInstance()->pushEvent(&evRp);
                    }
                    else
                    {
                        pos++;
                    }
                }
                else
                {
                    //emergency exit, no more buffer left
                    buffer[MAXDATASIZE-2] = character;
                    buffer[MAXDATASIZE-1] = 0;
                    memcpy(data,buffer,MAXDATASIZE);
                    pos=0;
                    XF::getInstance()->pushEvent(&evRp);
                }
}

void Fona::send(string message){
    if(state==ST_IDLE){
        message += "\r";
        uartSend(message.c_str());
        XF::getInstance()->pushEvent(&evCmd);
    }
    else if(state==ST_SETUP||state==ST_INIT){
        message += "\r";
        uartSend(message.c_str());
    }
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
                this->state = ST_SETUP;
            }
        break;
        case ST_SETUP:
            if (e->getId() == (Event::evID)evResponse)
            {
                this->state = ST_WAITOK;
            }
            break;
        case ST_WAITOK:
            if (e->getId() == Event::evDefault)
            {
                this->state = ST_IDLE;
            }
            if (e->getId() == (Event::evID)evError)
            {
                this->state = ST_SETUP;
            }
        break;
        case ST_IDLE:
            if (e->getId() == (Event::evID)evCommand)
            {
                this->state = ST_COMMAND;
            }
            if (e->getId() == (Event::evID)evResponse)
            {
                this->state = ST_NOTIFY;
            }
        break;
        case ST_COMMAND:
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
                printk("ST_INIT\n");
                send("ATE0");
            break;
            case ST_SETUP:
                printk("ST_SETUP\n");  
                send("AT");     
            break;
            case ST_WAITOK:
                printk("ST_WAITOK\n");
                // switch (selector){
                //     case dataChoice:
                //         dataSource=&buffer;
                //         break;
                //     case bufferChoice:
                //         dataSource=&data;
                //         break;
                // }
                // if(*dataSource == "OK\r\n"){
                //     XF::getInstance()->pushEvent(&ev);
                // }
                // else{
                //     XF::getInstance()->pushEvent(&evErr);
                // }
                if(strcmp((char *)data,"OK\r\n")){
                    XF::getInstance()->pushEvent(&ev);
                }
            break;
            case ST_IDLE:
                printk("ST_IDLE\n");
            break;
            case ST_COMMAND:
                printk("ST_COMMAND\n");
            break;
            case ST_NOTIFY:
                printk("ST_NOTIFY\n");
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