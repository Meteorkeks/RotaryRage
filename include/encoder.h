#include <Arduino.h>

class Encoder {
  public:
    // pin numbers
    int enA_pin, enB_pin, bttn_pin;

    uint8_t enA_triggers; 
    uint8_t enB_triggers; 
    uint8_t bttn_triggers; 

    uint32_t enA_last_ms; 
    uint32_t enB_last_ms; 
    uint32_t bttn_last_ms; 

    
    void IRAM_ATTR encoderISR();
    void IRAM_ATTR buttonISR();
    
    /**
     * @brief Constructs a new rotary encoder object
     * 
     * @param encoderA pin encoder A
     * @param encoderB pin encoder B
     * @param taster   pin push button
     * @param encoder_debounce_ms   
     * @param button_debounce_ms
     */
    Encoder(    uint8_t encoderA, uint8_t encoderB, uint8_t taster, uint8_t encoder_debounce_ms, uint8_t button_debounce_ms);
    

  
    
};
