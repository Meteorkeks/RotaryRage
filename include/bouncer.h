#include <Arduino.h>

/**
 * @brief Debounce a given pin
 * 
 */
class Bounce {
    public:
        uint8_t pin_number;
        uint16_t timeout_ms; 

        // return true if last update() leads to xxxEdge
        bool fallingEdge();
        bool risingEdge();


        Bounce(uint8_t pin_number, uint16_t timeout);

        bool update();

    private:
        bool last_state;
        unsigned long state_time;
        bool is_debouncing;
}