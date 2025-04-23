/* 
    FonaProject – Retro Rotary Phone Meets 3G
    Copyright (C) 2025  steveyyyy

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "factory.h"

GPI Factory::_pinWindUp(5,"GPIOB"); //Greeen Cable on D11
GPI Factory::_pinPulse(4,"GPIOB"); //Brown Cable on D12
RotaryDial Factory::_rotaryDial(&_pinWindUp,&_pinPulse);

GPI Factory::_pinSwitchhook(11,"GPIOA");
Button Factory::_switchhook(&_pinSwitchhook);

UART Factory::_uart("UART_1", 115200);
Fona Factory::_fona(&_uart);

Ringer Factory::_ringer(7,"GPIOA");

Dial Factory::_dialer(&_switchhook,&_fona,&_ringer);

gpio_callback Factory::_cbPortA;
gpio_callback Factory::_cbPortB;


Factory::Factory(/* args */) {
    
}

Factory::~Factory() {
    
}

void Factory::init() {
    xf()->init();

    //set to pulldown
    pWindUp()->setPulldown();
    pPulse()->setPulldown();
    pSwitchhook()->setPulldown();

    //initialize the pins
    pWindUp()->initHW();
    pPulse()->initHW();
    pSwitchhook()->initHW();

    // IntManager::Interrupt ntrpt;
    // //enable interrupts on pWindUp
    // ntrpt.edge = IntManager::BOTH;
    // ntrpt.pp.pin = pWindUp()->getPin();
    // ntrpt.pp.dev = pWindUp()->getDriver();
    // im()->enableInt(ntrpt);

    // //enable interrupts on pPulse
    // ntrpt.edge = IntManager::BOTH;
    // ntrpt.pp.pin = pPulse()->getPin();
    // ntrpt.pp.dev = pPulse()->getDriver();
    // im()->enableInt(ntrpt);

    // //enable interrupts on pSwitchhook
    // ntrpt.edge = IntManager::BOTH;
    // ntrpt.pp.pin = pSwitchhook()->getPin();
    // ntrpt.pp.dev = pSwitchhook()->getDriver();
    // im()->enableInt(ntrpt);

    ringer()->initHW();  
    switchhook()->initHW();
    rotary()->initHW();
    
    //init UART
    uart()->initHW();
    //enable UART interrupt
    uart()->enableRXInterrupt();

    if(gpio_pin_interrupt_configure(pSwitchhook()->getDriver(),pSwitchhook()->getPin(),GPIO_INT_EDGE_BOTH) != 0){
        printk("error\n");
    }
    if(gpio_pin_interrupt_configure(pPulse()->getDriver(),pPulse()->getPin(),GPIO_INT_EDGE_BOTH) != 0){
        printk("error\n");
    }
    if(gpio_pin_interrupt_configure(pWindUp()->getDriver(),pWindUp()->getPin(),GPIO_INT_EDGE_BOTH) != 0){
        printk("error\n");
    }
    
    
    gpio_init_callback(cbPortA(),Factory::onInterrupt,BIT(pSwitchhook()->getPin()));
    gpio_init_callback(cbPortB(),Factory::onInterrupt,BIT(pPulse()->getPin())|BIT(pWindUp()->getPin())); //ODER oder PLUS ist beides möglich 

    if(gpio_add_callback(pSwitchhook()->getDriver(),cbPortA()) != 0){
        printk("error\n");
    }
    if(gpio_add_callback(pPulse()->getDriver(),cbPortB()) != 0){
        printk("error\n");
    }
    if(gpio_add_callback(pWindUp()->getDriver(),cbPortB()) != 0){
        printk("error\n");
    }


}

void Factory::build() {
    switchhook()->subscribe(dialer());
    rotary()->subscribe(dialer());
    fona()->subscribe(dialer());

}

void Factory::start() {
    rotary()->startBehaviour();
    switchhook()->startBehaviour();
    dialer()->startBehaviour();
}

void Factory::onInterrupt(const struct device* dev, struct gpio_callback* cb, u32_t pins){
    //printk("interrupt von %s,%i\n",dev->name,pins);
    int key = irq_lock();
    if ((pSwitchhook()->getDriver() == dev) && (BIT(pSwitchhook()->getPin()) & pins))
    {
        switchhook()->onInterrupt(pSwitchhook()->getPin()); //onInterrupt from the Interface gets fired
    }

    if ((pPulse()->getDriver() == dev) && (BIT(pPulse()->getPin()) & pins))
    {
        rotary()->onInterrupt(pPulse()->getPin()); //onInterrupt from the Interface gets fired
    }

    if ((pWindUp()->getDriver() == dev) && (BIT(pWindUp()->getPin()) & pins))
    {
        rotary()->onInterrupt(pWindUp()->getPin()); //onInterrupt from the Interface gets fired
    }
    irq_unlock(key);
}

