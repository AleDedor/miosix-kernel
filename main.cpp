
#include <cstdio>
#include "miosix.h"
#include "vumeter.h"
#include <interfaces/delays.h>
#include "TLV320AIC3101.h"

using namespace std;
using namespace miosix;

const unsigned short *readableBuff;

typedef Gpio<GPIOA_BASE,12> ledg1;
//typedef Gpio<GPIOA_BASE,13> ledg2;
typedef Gpio<GPIOA_BASE,10> ledy1;
typedef Gpio<GPIOA_BASE,11> ledy2;
typedef Gpio<GPIOA_BASE,8> ledr1;
typedef Gpio<GPIOA_BASE,9> ledr2;


int main()
{
    //unsigned int val = 1;

    Vumeter meter(ledr1::getPin(),ledr2::getPin(),ledy1::getPin(),ledy2::getPin(),ledg1::getPin());

    TLV320AIC3101::instance().setup();
    //TLV320AIC3101::instance().startRx();

    //check I2C
    bool i2cWorked = true;
    //bool i2cWorked = TLV320AIC3101::instance().I2C_Send(0x0E,0b10001000);
    unsigned char reg = TLV320AIC3101::instance().I2C_Receive(0x0E);

    if(i2cWorked){
        meter.showVal(64300);
    }else{
        meter.showVal(0);
    }

    miosix::delayMs(1500);

    if(reg == 0b10001000){
        meter.showVal(0);
    } else {
        meter.showVal(64300);
    }

    miosix::delayMs(1500);

   // TLV320AIC3101::instance().I2S_startRx();

    while(1){
    /*
        meter.showVal(val);
        delayUs(50);
        //Thread::sleep(1);
        if(val >= 65535){
            val=0;
            meter.clear();
        } 
        val++;
    */
    
        if(TLV320AIC3101::instance().I2S_startRx()){
            readableBuff = TLV320AIC3101::instance().getReadableBuff();
            for(int i=0; i<128; i++){
                meter.showVal(readableBuff[i]);
            }
        }
    }
}
    
//iprintf("Hello world, write your application here\n");

