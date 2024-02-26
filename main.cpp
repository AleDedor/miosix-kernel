
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
    Vumeter meter(ledr1::getPin(),ledr2::getPin(),ledy1::getPin(),ledy2::getPin(),ledg1::getPin());

    TLV320AIC3101::instance().setup();
    //TLV320AIC3101::instance().startRx();

    unsigned int val = 10096;

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

