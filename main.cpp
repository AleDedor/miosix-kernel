
#include <cstdio>
#include "miosix.h"
#include "vumeter.h"
#include <interfaces/delays.h>
#include "TLV320AIC3101.h"
#include <thread>

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
    Vumeter meter(ledr1::getPin(),ledr2::getPin(),ledy1::getPin(),ledy2::getPin(),ledg1::getPin());

    auto& driver=TLV320AIC3101::instance();
    driver.setup();

    thread leds([]{ 
        while(1){
            ledg1::high();
            Thread::sleep(500);
            ledg1::low();
            Thread::sleep(500);
        }
     });

     leds.detach();



    //check I2C
    bool i2cWorked = driver.I2C_Send(0x0E,0b10001000);
    unsigned char reg = driver.I2C_Receive(0x0E);

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

    //TLV320AIC3101::instance().I2S_startRx();

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

    // debug I2S, use test() to start communication with DMA (RX)
    // when communication is completed, interrupt is called. showVal() only at that moment
    /*    if(TLV320AIC3101::IRQ_entrato){
            for(int i=0; i<256; i++){
               meter.showVal(bufferw[i]);
            }
            TLV320AIC3101::instance().test();
        }*/

        // CONFIGURARE ENTRAMBE LE ISR PER TX E RX!!!!!!
        if(driver.I2S_startRx()){
            //iprintf("bmain\n");
            readableBuff = driver.getReadableBuff();
            iprintf("read_buffer= %p\n",readableBuff);
            for(int i=0; i<256; i++){
               meter.showVal(readableBuff[i]);
            }
            driver.ok();
        }
    
    }
}


