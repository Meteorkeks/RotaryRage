#include "bouncer.h"

Bounce::Bounce(uint8_t pin_number, uint16_t timeout)
{
    pin_number = pin_number;
    timeout = timeout;
    last_state;
}

bool Bounce::update() {
    // get current state
    int current_state = digitalRead(pin_number);

    if (is_debouncing) {
        u_long new_time_stamp = millis();
        u_long time_elapsed_since_trigger_event = new_time_stamp - state_time;
        if (time_elapsed_since_trigger_event > timeout_ms) {
            last_state = current_state;
            return true;
        }
    }
    else {
        if (current_state ^ last_state) {
            // start debouncing
            state_time = millis();
            is_debouncing = true;
        }
        else {
            return false;
        }
    }
}