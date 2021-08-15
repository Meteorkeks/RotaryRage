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
 * See slides for more https://docs.google.com/presentation/d/1O3VIUvFkATJrm6YaQaJuq9ZdTl-Y60neox94lireewk/edit#slide=id.ge8b705876d_0_0
 * 
 * @return true 
 * @return false 
 */
bool Bounce::update() {
    // get current state
    int current_state = digitalRead(pin_number);

    if (is_debouncing) {
        u_long new_time_stamp = millis();
        u_long time_elapsed_since_trigger_event = new_time_stamp - state_time;
        if (time_elapsed_since_trigger_event > timeout_ms) {
            // propagate change
            this->current_state = current_state;
            is_debouncing = false;
            return true;
        }
        return false;
    }
    else {
        bool needs_debouncing = current_state ^ last_state;
        if (needs_debouncing) {
            // start debouncing
            state_time = millis();
            is_debouncing = true;
            return false;
        }
        else {
            // pass through state change
            this->last_state = this->current_state;
            this->current_state = current_state;
            return false;
        }
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