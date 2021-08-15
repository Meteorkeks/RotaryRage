#include "bouncer.h"

Bounce::Bounce(uint8_t pin_number, uint16_t timeout)
{
    this->pin_number = pin_number;
    timeout_ms = timeout;
    is_debouncing = false;
    last_state = false;
    current_state = false;
}


/**
 * @brief 
 * 
 * See slides for more https://docs.google.com/presentation/d/1vTbfDcuz6WX5NKOsSmGGm6JItUb8chF0BQySve-Xr3M/edit#slide=id.p
 * 
 * @return true 
 * @return false 
 */
bool Bounce::update() {
    // get current state
    int current_state = digitalRead(pin_number);

    if (is_debouncing) {
        // do not update internal state with new state during debounce period
        u_long new_time_stamp = millis();
        u_long time_elapsed_since_trigger_event = new_time_stamp - state_time;
        if (time_elapsed_since_trigger_event > timeout_ms) {
            is_debouncing = false;
        }
        return false;
    }
    else {
        this->last_state = this->current_state;
        this->current_state = current_state;
        bool change = current_state ^ last_state;
        if (change) {
            // start debouncing
            state_time = millis();
            is_debouncing = true;
        }
        return change;
    }
}

bool Bounce::fallingEdge() 
{
    return this->last_state && !this->current_state;
}
bool Bounce::risingEdge()
{
    return !this->last_state && this->current_state;
}

bool Bounce::change()
{
    //https://stackoverflow.com/a/1596681/10045897
    return this->last_state != this->current_state;
}


/*
Example usage

if (bounce->update) {
    if (bounce->risingEdge()) {
        return True
    }
}

*/