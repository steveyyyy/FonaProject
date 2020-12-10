#include "factory.h"

GPI Factory::_pinWindUp(5,"GPIOB"); //Greeen Cable on D11
GPI Factory::_pinPulse(4,"GPIOB"); //Brown Cable on D12
RotaryDial Factory::_rotaryDial(&_pinWindUp,&_pinPulse);

LED Factory::_ledGreen(0,"GPIOA");
LED Factory::_ledRed(1,"GPIOA");

GPI Factory::_pinSwitchhook(11,"GPIOA");
Button Factory::_switchhook(&_pinSwitchhook);

Fona Factory::_fona("UART_1", 115200);

Dial Factory::_dialer(&_switchhook,&_ledGreen,&_ledRed,&_fona);


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

    IntManager::Interrupt ntrpt;
    //enable interrupts on pWindUp
    ntrpt.edge = IntManager::BOTH;
    ntrpt.pp.pin = pWindUp()->getPin();
    ntrpt.pp.dev = pWindUp()->getDriver();
    im()->enableInt(ntrpt);

    //enable interrupts on pPulse
    ntrpt.edge = IntManager::BOTH;
    ntrpt.pp.pin = pPulse()->getPin();
    ntrpt.pp.dev = pPulse()->getDriver();
    im()->enableInt(ntrpt);

    //enable interrupts on pSwitchhook
    ntrpt.edge = IntManager::BOTH;
    ntrpt.pp.pin = pSwitchhook()->getPin();
    ntrpt.pp.dev = pSwitchhook()->getDriver();
    im()->enableInt(ntrpt);

    rotary()->initHW();
    
    ledGreen()->initHW();
    ledRed()->initHW();

    switchhook()->initHW();

    fona()->initHW();
    fona()->enableRXInterrupt();
}

void Factory::build() {
    switchhook()->subscribe(dialer());
    rotary()->subscribe(dialer()); 
    fona()->subscribe(dialer());
}

void Factory::start() {
    rotary()->startBehaviour();
    switchhook()->startBehaviour(); 
    fona()->startBehaviour();
    dialer()->startBehaviour();
}

