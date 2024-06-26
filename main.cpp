
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


// Multithreadingggg
/*Mutex mutex;
uint16_t filtered_sound = 0;

void threadfunc(){
    Lock<Mutex> lock(mutex);
    for(int i=0; i<128; i++){
        filtered_sound += readableBuff[i];
    }
    filtered_sound = filtered_sound/128;
    iprintf("in thread, mutex locked %d\n", filtered_sound); 
}*/

int main()
{
    Vumeter meter(ledr1::getPin(),ledr2::getPin(),ledy1::getPin(),ledy2::getPin(),ledg1::getPin());
    //Thread *led_thread;
    auto& driver=TLV320AIC3101::instance();

    /*
    thread leds([]{ 
        while(1){
            ledg1::high();
            Thread::sleep(500);
            ledg1::low();
            Thread::sleep(500);
        }
     });
    leds.detach();
    */


    driver.setup();

    //check I2C
    bool i2cWorked = driver.I2C_Send(0x03,0b00010001);
    unsigned char reg = driver.I2C_Receive(0x0E);

    if(i2cWorked){
        meter.showVal(64300);
        iprintf("I2C OK!\n");
    }else{
        meter.showVal(0);
        iprintf("I2C broken :(\n");
    }

    miosix::delayMs(1500);

    if(reg == 0b10001000){
        meter.showVal(0);
        iprintf("Codec registers ok!\n");
    } else {
        meter.showVal(64300);
        iprintf("Codec registers not ok :(\n");
    }

    miosix::delayMs(1500);

    miosix::delayMs(10);
    // adding thread to manage the vumeter
    //iprintf("In main trying to create thread... \n");
    //led_thread=Thread::create(threadfunc, 2048, 1, Thread::JOINABLE);

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
        /*
        if(driver.I2S_startRx()){
            iprintf("in main, waiting for IRQ...\n");
            readableBuff = driver.getReadableBuff();
            iprintf("read_buffer= %p\n",readableBuff);
            //iprintf("found readable buffer\n");
            for(int i=0; i<255; i++){
               meter.showVal(readableBuff[i]);
               //iprintf("audio_val= %d\n",readableBuff[i]);
            }
            driver.ok();
        }
        */

       /* this is working */
       
        while(!driver.I2S_startRx()){}
        readableBuff = driver.getReadableBuff();
        for(int i=0; i<128; i++){
            meter.showVal(readableBuff[i]);
            Thread::sleep(1);
        }
        driver.I2S_startTx(readableBuff);
        

       //idea here is to get readablebuffer lock a mutex to call a filter thread which writes filtered
       //value on a shared variable (global) 
      /*  while(!driver.I2S_startRx()){}
        readableBuff = driver.getReadableBuff();
        iprintf("in main, data received ok\n");       
        {
            Lock<Mutex> lock(mutex);
            iprintf("in main, mutex locked %d\n", filtered_sound); 
            meter.showVal(filtered_sound);
            delayUs(1000);
            filtered_sound = 0;
        }

        driver.I2S_startTx(readableBuff); */

    }

    //led_thread->join();
}


