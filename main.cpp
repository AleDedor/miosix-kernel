
#include <cstdio>
#include "miosix.h"
#include "vumeter.h"
#include <interfaces/delays.h>
#include "TLV320AIC3101.h"
#include <thread>

using namespace std;
using namespace miosix;

const unsigned short *readableBuff;

Mutex mutex;

typedef Gpio<GPIOA_BASE,12> ledg1;
//typedef Gpio<GPIOA_BASE,13> ledg2;
typedef Gpio<GPIOA_BASE,10> ledy1;
typedef Gpio<GPIOA_BASE,11> ledy2;
typedef Gpio<GPIOA_BASE,8> ledr1;
typedef Gpio<GPIOA_BASE,9> ledr2;



void threadfunc(void *argv){

    Vumeter meter2(ledr1::getPin(),ledr2::getPin(),ledy1::getPin(),ledy2::getPin(),ledg1::getPin());

    while(1){
        {
            Lock<Mutex> lock(mutex);
            if (readableBuff != nullptr) { 
                //extremely rough (but working) implementation, every 50ms only the first element of the buffer is printed on the vu-meter
                meter2.showVal(readableBuff[0]);               
            }
        }
        Thread::sleep(50);
    }
}


int main()
{
    Vumeter meter(ledr1::getPin(),ledr2::getPin(),ledy1::getPin(),ledy2::getPin(),ledg1::getPin());
    auto& driver=TLV320AIC3101::instance();

    driver.setup();

    //check I2C
    bool i2cWorked = driver.I2C_Send(0x03,0b00010001);
    unsigned char reg = driver.I2C_Receive(0x0E);

    if(i2cWorked){
        meter.showVal(64300);
        iprintf("I2C ok!\n");
    } else {
        meter.showVal(0);
        iprintf("I2C not ok!\n");
    }

    Thread::sleep(1500); //delay to be able to see the LEDS turn on if everything is ok

    if(reg == 0b10001000){
        meter.showVal(0);
        iprintf("Codec registers ok!\n");
    } else {
        meter.showVal(64300);
        iprintf("Codec registers not ok!\n");
    }

    // adding thread to manage the vumeter
    Thread *led_thread;
    led_thread = Thread::create(threadfunc, 256, 1);
    iprintf("Thread creato\n");

    while(1){ 

        while(!driver.I2S_startRx()){
            //to avoid busy waiting we put the thread to sleep for a short while
            Thread::nanoSleep(100000);  
        }

        {
            Lock<Mutex> lock(mutex);
            readableBuff = driver.getReadableBuff();
            driver.I2S_startTx(readableBuff);
        }
   
    }

    led_thread->join();
}


