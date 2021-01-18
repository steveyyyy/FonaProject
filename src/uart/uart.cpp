#include "uart.h"
#include <logging/log.h>
LOG_MODULE_REGISTER(uartt, CONFIG_UART_LOG_LEVEL);

UART::UART(const char* deviceBinding,int baudrate, uint8_t endOfMessage){
    uart_dev = NULL;
    this->baudrate = baudrate;
    this->deviceBinding=deviceBinding;
    this->pos=0;
    this->endOfMessage=endOfMessage;
    memset(message,0,MAXDATASIZE);
    //Config Message Queue
    k_msgq_init(&messages, messages_buffer, sizeof(uint8_t[MAXDATASIZE]), 10);

    this->rp.setTarget(this);
    this->rp.setDnd(true);
    this->rp.setId((Event::evID)evResponse);
}

UART::~UART() {}

void UART::initHW(){    
    //Get device
    uart_dev = device_get_binding(deviceBinding);
	if (!uart_dev) {
		LOG_ERR("Cannot get UART device\n");
	}
    //Config device
    uart_cfg.baudrate = baudrate;
    uart_cfg.parity = UART_CFG_PARITY_NONE;
    uart_cfg.stop_bits = UART_CFG_STOP_BITS_1;
    uart_cfg.data_bits = UART_CFG_DATA_BITS_8;
    uart_cfg.flow_ctrl = UART_CFG_FLOW_CTRL_NONE;

    int ret = uart_configure(uart_dev, &uart_cfg);
	if (ret != 0) {
		LOG_ERR("Cannot configure UART device\n");
	}
}

bool UART::enableRXInterrupt(){
    bool processed=false;
    if(!uart_dev){
        LOG_ERR("Problem to load uart device\n");
    }
    else{
        processed=true;
        uart_irq_callback_user_data_set(uart_dev, uartReceive,this);
        uart_irq_rx_enable(uart_dev);
    }
    return processed;
}

void UART::uartSend(const char* txData) 
{
    for (size_t i=0; i<strlen(txData); i++)
    {
        uart_poll_out(uart_dev, txData[i]);
    };
}

void UART::uartReceive(const struct device *uart_dev, void *data){
    uint8_t recvData = 0;
    UART* thisUART = (UART*)data;
    
    if (!uart_irq_update(uart_dev))
    {
        LOG_WRN("retval should always be 1");
        return;
    }
    else{
        if (uart_irq_rx_ready(uart_dev))
        {
            uart_fifo_read(uart_dev,&recvData,1);
            thisUART->message[thisUART->pos]=recvData;
            thisUART->pos++; 
            if(recvData == 0x0A)
            {
                thisUART->message[thisUART->pos]= 0;
                k_msgq_put(&thisUART->messages, &thisUART->message,K_NO_WAIT);
                thisUART->pos=0;
                XF::getInstance()->pushEvent(&thisUART->rp);
            }
        }
    }
}

void UART::setBaudrate(int baudrate){
    this->baudrate = baudrate;
    uart_cfg.baudrate = baudrate;
    int ret = uart_configure(uart_dev, &uart_cfg);
	if (ret != 0) {
		LOG_WRN("Cannot configure UART device\n");
	}
}

void UART::setEndOfMessage(uint8_t endOfMessage){
    this->endOfMessage = endOfMessage;
}

void UART::subscribe(IUARTObserver* subscriber)
{
    vector<IUARTObserver*>::iterator it;
    it = find(subscribers.begin(), subscribers.end(),subscriber);
    if (it == subscribers.end())
    {
        subscribers.push_back(subscriber);
    }
}

void UART::unsubscribe(IUARTObserver* subscriber)
{
    vector<IUARTObserver*>::iterator it;
    it = find(subscribers.begin(), subscribers.end(),subscriber);
    if (it != subscribers.end())
    {
        subscribers.erase(it);
    }
}

void UART::notify()
{
    vector<IUARTObserver*>::iterator it;
    for (it=subscribers.begin(); it!=subscribers.end();++it)
    {
        (*it)->onMessage(&messages);
    }
}

bool UART::processEvent(Event* e){
    bool processed =false;
    if(e->getId()==(Event::evID)evResponse){
                notify();
                processed=true;
            }
    return processed;
}