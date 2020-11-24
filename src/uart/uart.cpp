#include "uart.h"

UART::UART(const char* deviceBinding,int baudrate){
    uart_dev = NULL;
    this->baudrate = baudrate;
    character=0;
    this->deviceBinding=deviceBinding;
}

UART::~UART(){};

void UART::initHW(){
    //Get device
    uart_dev = device_get_binding(deviceBinding);
	if (!uart_dev) {
		printk("Cannot get UART device\n");
	}

    //Config device
    uart_cfg.baudrate = baudrate;
    uart_cfg.parity = UART_CFG_PARITY_NONE;
    uart_cfg.stop_bits = UART_CFG_STOP_BITS_1;
    uart_cfg.data_bits = UART_CFG_DATA_BITS_8;
    uart_cfg.flow_ctrl = UART_CFG_FLOW_CTRL_NONE;

    int ret = uart_configure(uart_dev, &uart_cfg);
	if (ret != 0) {
		printk("Cannot configure UART device\n");
	}
}

bool UART::enableRXInterrupt(){
    bool processed=false;
    if(!uart_dev){
        printk("Problem to load uart device\n");
    }
    else{
        processed=true;
        //uart_irq_callback_set(uart_dev, uartReceive);
        uart_irq_callback_user_data_set(uart_dev, uartReceive,this);
        uart_irq_rx_enable(uart_dev);
        //printk("UART device loaded... [OK]");
    }
    return processed;
}

void UART::uartSend(const char* txData) 
{
    u8_t txSize = strlen(txData);
    for (int i=0; i<txSize; i++)
    {
        uart_poll_out(uart_dev, txData[i]);
    };
}

void UART::uartReceive(const struct device *uart_dev, void *data){
    uint8_t recvData = 0;
    UART* thisUART = (UART*)data;
    
    if (!uart_irq_update(uart_dev))
    {
        printk("retval should always be 1");
        return;
    }
    if (uart_irq_rx_ready(uart_dev))
    {
        uart_fifo_read(uart_dev,&recvData,1);
        thisUART->character = (char)recvData;
        //thisUART->notify();
        printk("%c",thisUART->character);
    }  
    //remove notify make statwe machine
}

void UART::subscribe(IUARTObserver* subscriber) 
{
    vector<IUARTObserver*>::iterator it;
    it = find(subscribers.begin(), subscribers.end(), subscriber);
    if (it == subscribers.end())
    {
        subscribers.push_back(subscriber);
    }    
}

void UART::unsubscribe(IUARTObserver* subscriber) 
{
    vector<IUARTObserver*>::iterator it;
    it = find(subscribers.begin(), subscribers.end(), subscriber);
    if (it != subscribers.end())
    {
        subscribers.erase(it);
    }   
}

void UART::notify(u8_t character) 
{
    vector<IUARTObserver*>::iterator it;
    for (it = subscribers.begin(); it != subscribers.end(); ++it)
    {
        (*it)->onMessage(character);
    }
}

int UART::getBaudrate(){
    return this->baudrate;
}
void UART::setBaudrate(int baudrate){
    this->baudrate = baudrate;
    updateBaudrate();
}
void UART::updateBaudrate(){
    uart_cfg.baudrate = baudrate;
    int ret = uart_configure(uart_dev, &uart_cfg);
	if (ret != 0) {
		printk("Cannot configure UART device\n");
	}
}