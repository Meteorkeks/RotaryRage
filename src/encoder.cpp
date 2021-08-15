#include "encoder.h"

Encoder::Encoder(uint8_t encoderA, uint8_t encoderB, uint8_t taster)
{

    enA_pin = encoderA;
    enB_pin = encoderB;
    bttn_pin = taster;

    // debouncing pin wrapper
    enA =  new Bounce(enA_pin, 3);
    enB =  new Bounce(enB_pin, 3);
    bttn = new Bounce(bttn_pin, 10);

    pinMode(enA_pin, INPUT_PULLUP);
    pinMode(enB_pin, INPUT_PULLUP);
    pinMode(taster,  INPUT_PULLUP);
}

bool Encoder::update()
{
    bool something_changed = false;

    if (enB->update())
    {
        if (enB->fallingEdge())
        {
            stateB = LOW;
        }
        if (enB->risingEdge())
        {
            stateB = HIGH;
        }
    }

    if (enA->update())
    {
        if (enA->fallingEdge())
        {
            value += (stateB) ? 1 : -1;
            something_changed = true;
            
            
            // DEBUG
            Serial.print("Encoder was rotated ");

            if(stateB) 
                Serial.println("left");
            else    
                Serial.println("right");
                
        }
    }

    if (bttn->update())
    {
        if (bttn->fallingEdge())
        {
            button = true;
            // DEBUG
            Serial.println("Button was pushed down.");
        }
        if (bttn->risingEdge())
        {
            button = false;
            // DEBUG
            Serial.println("Button was released.");
        }
        something_changed = true;
    }

    return something_changed;
}

int Encoder::getValueDiff()
{
    int diff = value - value_last;
    value_last = value;
    return (diff > 20) ? 0 : diff;
}

bool Encoder::getButtonDiff()
{
    bool button_diff = button != button_last;
    button_last = button;
    return button_diff;
}
