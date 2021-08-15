#ifndef _BOUNCER_H_
#define _BOUNCER_H_

#include <Arduino.h>

/**
 * @brief Debounce a given pin
 * 
 */
class Bounce {
    public:
        uint8_t pin_number;
        uint16_t timeout_ms; 

        Bounce(uint8_t pin_number, uint16_t timeout);

        // return true if last update() leads to xxxEdge
        bool fallingEdge();
        bool risingEdge();
        bool change();
        bool update() ;

    private:
        bool last_state;
        bool current_state;
        u_long state_time;
        bool is_debouncing;
};

#endif //_BOUNCER_H_