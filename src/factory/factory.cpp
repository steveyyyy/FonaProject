#include "factory.h"

GPI Factory::_pinWindUp(5,"GPIOB"); //Greeen Cable on D11
GPI Factory::_pinPulse(4,"GPIOB"); //Brown Cable on D12
RotaryDial Factory::_rotaryDial(&_pinWindUp,&_pinPulse);

LED Factory::_ledGreen(0,"GPIOA");
LED Factory::_ledRed(1,"GPIOA");

GPI Factory::_pinAnswer(11,"GPIOA");
GPI Factory::_pinHangUp(8,"GPIOA");
Button Factory::_btnAnswer(&_pinAnswer);
Button Factory::_btnHangUp(&_pinHangUp);
Dial Factory::_dialer(&_btnAnswer,&_btnHangUp,&_ledGreen,&_ledRed);


Factory::Factory(/* args */) {
    
}

Factory::~Factory() {
    
}

void Factory::init() {
    xf()->init();

    //set to pulldown
    pWindUp()->setPulldown();
    pPulse()->setPulldown();
    pAnswer()->setPulldown();
    pHangUp()->setPulldown(); 
    //initialize the pins
    pWindUp()->initHW();
    pPulse()->initHW();
    pAnswer()->initHW();
    pHangUp()->initHW(); 

    

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

    //enable interrupts on pAnswer
    ntrpt.edge = IntManager::BOTH;
    ntrpt.pp.pin = pAnswer()->getPin();
    ntrpt.pp.dev = pAnswer()->getDriver();
    im()->enableInt(ntrpt);

    //enable interrupts on pHangUp
    ntrpt.edge = IntManager::BOTH;
    ntrpt.pp.pin = pHangUp()->getPin();
    ntrpt.pp.dev = pHangUp()->getDriver();
    im()->enableInt(ntrpt);

    rotary()->initHW();
    
    ledGreen()->initHW();
    ledRed()->initHW();

    btnAnswer()->initHW();
    btnHangUp()->initHW();
}

void Factory::build() {
    // Nothing to do
    // blinkerA()->setLed(ledGreen());
    btnAnswer()->subscribe(dialer());
    btnHangUp()->subscribe(dialer());
    rotary()->subscribe(dialer());
    
}

void Factory::start() {
    rotary()->startBehaviour();

    // ledGreen()->off();
    // ledRed()->off();

    btnAnswer()->startBehaviour();
    btnHangUp()->startBehaviour();
    dialer()->startBehaviour();
}

