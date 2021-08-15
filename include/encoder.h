#ifndef _ENCODER_H_
#define _ENCODER_H_

#include <Arduino.h>
#include "bouncer.h"

class Encoder {
  public:
    // pin numbers
    int enA_pin, enB_pin, bttn_pin;
    bool stateA = LOW;
    bool stateB = LOW;    
    
    // continuous rotary value
    int value = 0;
    int value_last = 0;
    
    // push button state
    bool button = false;
    bool button_last = false;

    // debounce wrapper for pins
    Bounce * enA;
    Bounce * enB;
    Bounce * bttn;
    
    /**
     * @brief Constructs a new rotary encoder object
     * 
     * @param encoderA pin encoder A
     * @param encoderB pin encoder B
     * @param taster pin push button
     */
    Encoder(uint8_t encoderA, uint8_t encoderB, uint8_t taster);
  
    void update();

    int getValueDiff();

    bool getButtonDiff();
};

#endif //_ENCODER_H_