
#include <cstdio>
#include "miosix.h"
#include "vumeter.h"

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


    while(1){
        meter.setHigh();
        Thread::sleep(500);
        meter.clear();
        Thread::sleep(500);
    }
    
    //iprintf("Hello world, write your application here\n");
}
