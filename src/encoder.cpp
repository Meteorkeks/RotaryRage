#include "encoder.h"

Encoder::Encoder(uint8_t encoderA, uint8_t encoderB, uint8_t taster, uint8_t encoder_debounce_ms, uint8_t button_debounce_ms)
: enA_pin(encoderA), enB_pin(encoderB), bttn_pin(taster) {
       
    pinMode(enA_pin,  INPUT_PULLUP);
    pinMode(enB_pin,  INPUT_PULLUP);
    pinMode(bttn_pin, INPUT_PULLUP);

    enA_triggers = 0;
    enB_triggers = 0;
    bttn_triggers = 0;

    enA_last_ms = millis();
    enB_last_ms = millis();
    bttn_last_ms = millis();
    
    attachInterrupt(enA_pin, encoderISR, RISING);   // cant be done. must be non-class function
    attachInterrupt(bttn_pin, buttonISR, RISING);
    
}