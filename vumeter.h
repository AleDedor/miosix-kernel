// include guards
#ifndef VUMETER_IMPL_H
#define VUMETER_IMPL_H

#include "miosix.h"
#include "interfaces/gpio.h"

using namespace miosix;
//#include <vector> // allows to create vector of classes

// inside main, initialization 
/*Vumeter vumeter(led1::getPin(),led2::getPin(),led3::getPin(),
                     led4::getPin(),led5::getPin(),led6::getPin());*/

// a namespace for all the related constants
namespace VUCONST{
    const uint8_t NUM_LEDS = 5;
    const int MAX_LEVEL = 31;
    const int MIN_LEVEL = 0;
}


//vector<Gpio> leds; // a vector size can be changed dinamically, array = fixed

class Vumeter // fixed @6 leds
{
public:
    // constructor (init) + adjust levels + clear all

    /**
     * Constructor, initializes the vumeter
     * \param Rled1 a Gpio class specifying the GPIO connected to the Rled1
     * \param Rled2 a Gpio class specifying the GPIO connected to the Rled2
     * \param Yled1 a Gpio class specifying the GPIO connected to the Yled1
     * \param Yled2 a Gpio class specifying the GPIO connected to the Yled2
     * \param Gled1 a Gpio class specifying the GPIO connected to the Gled1
     * \param Gled2 a Gpio class specifying the GPIO connected to the Gled2
     */
    Vumeter(GpioPin Rled1, GpioPin Rled2, GpioPin Yled1, GpioPin Yled2, GpioPin Gled1);
    
    /**
     * turn off all the leds
     */
    void clear();
    void setHigh();
    void showVal(int sound_val);

private:

    // all the pins needed for the class
    GpioPin R1;
    GpioPin R2;
    GpioPin Y1;
    GpioPin Y2;
    GpioPin G1; 

};

// here the definition of the contructor
Vumeter::Vumeter(GpioPin Rled1, GpioPin Rled2, GpioPin Yled1, GpioPin Yled2, GpioPin Gled1) 
: R1(Rled1), R2(Rled2), Y1(Yled1), Y2(Yled2), G1(Gled1) //assign passed parameters to class variables
{
    R1.mode(Mode::OUTPUT);
    R2.mode(Mode::OUTPUT);
    Y1.mode(Mode::OUTPUT);
    Y2.mode(Mode::OUTPUT);
    G1.mode(Mode::OUTPUT);
}

//Reset all LEDs
void Vumeter::clear(){
    R1.low();
    R2.low();
    Y1.low();
    Y2.low();
    G1.low();
}

//Set all LEDs
void Vumeter::setHigh(){
    R1.high();
    R2.high();
    Y1.high();
    Y2.high();
    G1.high();
}

void Vumeter::showVal(int sound_val){
    int threshold = 1;
    int value = 0;

    for(int i=0; i<5; i++){
        if(sound_val >= threshold){
            value = (value << 1) | 1;
        }
        threshold = (threshold << 3) + 7;
    }

    if(value && 0x00000001) G1.high();
    else                    G1.low();

    if(value && 0x00000002) Y2.high();
    else                    Y2.low();

    if(value && 0x00000004) Y1.high();
    else                    Y1.low();

    if(value && 0x00000008) R2.high();
    else                    R2.low();

    if(value && 0x00000010) R1.high();
    else                    R1.low();

}
#endif //VUMETER_IMPL_H