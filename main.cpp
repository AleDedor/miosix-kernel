
#include <cstdio>
#include "miosix.h"

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
    ledg1::mode(Mode::OUTPUT);
    ledy1::mode(Mode::OUTPUT);
    ledy2::mode(Mode::OUTPUT);
    ledr1::mode(Mode::OUTPUT);
    ledr2::mode(Mode::OUTPUT);


    while(1){
        ledg1::high();
        ledy2::high();
        Thread::sleep(500);
        ledg1::low();
        ledy2::low();
        Thread::sleep(500);
    }
    
    //iprintf("Hello world, write your application here\n");
}
