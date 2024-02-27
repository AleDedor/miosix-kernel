
#include <cstdio>
#include "miosix.h"
#include "vumeter.h"
#include <interfaces/delays.h>
#include "TLV320AIC3101.h"

using namespace std;
using namespace miosix;

typedef Gpio<GPIOA_BASE,12> ledg1;
//typedef Gpio<GPIOA_BASE,13> ledg2;
typedef Gpio<GPIOA_BASE,10> ledy1;
typedef Gpio<GPIOA_BASE,11> ledy2;
typedef Gpio<GPIOA_BASE,8> ledr1;
typedef Gpio<GPIOA_BASE,9> ledr2;


int main()
{
    unsigned int val = 10096;

    Vumeter meter(ledr1::getPin(),ledr2::getPin(),ledy1::getPin(),ledy2::getPin(),ledg1::getPin());

    miosix::delayMs(50);

    TLV320AIC3101::instance().setup();
    //TLV320AIC3101::instance().startRx();

    // check I2C
    miosix::delayMs(50);
    unsigned char reg = TLV320AIC3101::instance().I2C_Receive(0x11);

    if(reg == 0b00001111){
        meter.showVal(val);
    } else {
        meter.showVal(65535);
    } 

    while(1){
        meter.showVal(val);
        delayUs(50);
        //Thread::sleep(1);
        if(val >= 65535){
            val=0;
            meter.clear();
        } 
        val++;
    }
}
    
//iprintf("Hello world, write your application here\n");

