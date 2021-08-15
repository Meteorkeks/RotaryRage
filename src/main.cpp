/* 
 * Sample program for ESP32 acting as a Bluetooth keyboard
 * 
 * Copyright (c) 2019 Manuel Bl
 * 
 * Licensed under MIT License
 * https://opensource.org/licenses/MIT
 */

//
// This program lets an ESP32 act as a keyboard connected via Bluetooth.
// When a button attached to the ESP32 is pressed, it will generate the key strokes for a message.
//
// For the setup, a momentary button should be connected to pin 2 and to ground.
// Pin 2 will be configured as an input with pull-up.
//
// In order to receive the message, add the ESP32 as a Bluetooth keyboard of your computer
// or mobile phone:
//
// 1. Go to your computers/phones settings
// 2. Ensure Bluetooth is turned on
// 3. Scan for Bluetooth devices
// 4. Connect to the device called "ESP32 Keyboard"
// 5. Open an empty document in a text editor
// 6. Press the button attached to the ESP32





#define US_KEYBOARD 1

#include <Arduino.h>
#include "BLEDevice.h"
#include "BLEHIDDevice.h"
#include "HIDTypes.h"
#include "HIDKeyboardTypes.h"



// Change the below values if desired
#define BUTTON_PIN 12
#define MESSAGE "Hello from ESP32\n"
#define DEVICE_NAME "ESP32 Keyboard"



// Forward declarations
void bluetoothTask(void*);
void typeText(const char* text);
void reset();



class Encoder;

bool isBleConnected = false;


void setup() {
    Serial.begin(115200);

    // configure pin for button
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    // start Bluetooth task
    xTaskCreate(bluetoothTask, "bluetooth", 20000, NULL, 5, NULL);
}

// States for flow controll
enum EncoderState {
    OpenMenu,
    OpenSubMenu,    
};

bool last_bttn = false;
bool last_a = false;
bool last_b = false;

unsigned long last_bttn_ms;
unsigned long last_ble_ms;

Encoder en;
EncoderState state = OpenMenu;

void loop() {  
    unsigned long now_ms = millis();

    // update encoder info
    en.update();
     
    if (isBleConnected && en.bttn->risingEdge() && (now_ms - last_ble_ms > 100)) {
        if( state == OpenMenu )
        {
            // press tab + F to search
            // type 'Kamerasteuerung' + return
            typeText("Kamerasteuerung\n");
        }
        else if (state = OpenSubMenu)
        {
            // press tab + F to search
            // type 'Beleuchtung' + return
            typeText("Beleuchtung\n");
        }
        else
        {
            reset();
        }
        // button has been pressed: type message
        Serial.println(MESSAGE);
        typeText(MESSAGE);
        last_ble_ms = now_ms;
    }
    else
    {
        state = OpenMenu;
    }



    
}



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


        Bounce(uint8_t pin_number, uint16_t timeout)
        {
            pin_number = pin_number;
            timeout = timeout;
            last_state;
        }

    bool update() {
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

    private:
        bool last_state;
        u_long state_time;
        bool is_debouncing;
}







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
    Encoder(uint8_t encoderA, uint8_t encoderB, uint8_t taster) {

      enA_pin = encoderA;
      enB_pin = encoderB;
      bttn_pin = taster;

      // debouncing pin wrapper
      enA  = new Bounce(enA_pin,   3);
      enB  = new Bounce(enB_pin,   3);
      bttn = new Bounce(bttn_pin, 10);
        
        
      pinMode(enA_pin, INPUT_PULLUP);
      pinMode(enB_pin, INPUT_PULLUP);
      pinMode(taster,  INPUT_PULLUP);
    }
  
    void update() {
        if(enB->update()) 
        {
            if(enB->fallingEdge()) 
            {
                stateB = LOW;
            }
            if(enB->risingEdge()) 
            {
                stateB = HIGH;
            }
        }
            
        if(enA->update()) 
        {
            if(enA->fallingEdge()) 
            {
                value += (stateB)?1:-1;
            }
        }

        if(bttn->update()) 
        {
            if(bttn->fallingEdge()) {
                button ^= true;
            }
        }
    }

    int getValueDiff() {
      int diff = value - value_last;
      value_last = value;
      return (diff>20)?0:diff;
    }

    bool getButtonDiff() {
      bool button_diff = button != button_last;
      button_last = button;
      return button_diff;
    }  
};


// Message (report) sent when a key is pressed or released
struct InputReport {
    uint8_t modifiers;	     // bitmask: CTRL = 1, SHIFT = 2, ALT = 4
    uint8_t reserved;        // must be 0
    uint8_t pressedKeys[6];  // up to six concurrenlty pressed keys
};

// Message (report) received when an LED's state changed
struct OutputReport {
    uint8_t leds;            // bitmask: num lock = 1, caps lock = 2, scroll lock = 4, compose = 8, kana = 16
};


// The report map describes the HID device (a keyboard in this case) and
// the messages (reports in HID terms) sent and received.
static const uint8_t REPORT_MAP[] = {
    USAGE_PAGE(1),      0x01,       // Generic Desktop Controls
    USAGE(1),           0x06,       // Keyboard
    COLLECTION(1),      0x01,       // Application
    REPORT_ID(1),       0x01,       //   Report ID (1)
    USAGE_PAGE(1),      0x07,       //   Keyboard/Keypad
    USAGE_MINIMUM(1),   0xE0,       //   Keyboard Left Control
    USAGE_MAXIMUM(1),   0xE7,       //   Keyboard Right Control
    LOGICAL_MINIMUM(1), 0x00,       //   Each bit is either 0 or 1
    LOGICAL_MAXIMUM(1), 0x01,
    REPORT_COUNT(1),    0x08,       //   8 bits for the modifier keys
    REPORT_SIZE(1),     0x01,       
    HIDINPUT(1),        0x02,       //   Data, Var, Abs
    REPORT_COUNT(1),    0x01,       //   1 byte (unused)
    REPORT_SIZE(1),     0x08,
    HIDINPUT(1),        0x01,       //   Const, Array, Abs
    REPORT_COUNT(1),    0x06,       //   6 bytes (for up to 6 concurrently pressed keys)
    REPORT_SIZE(1),     0x08,
    LOGICAL_MINIMUM(1), 0x00,
    LOGICAL_MAXIMUM(1), 0x65,       //   101 keys
    USAGE_MINIMUM(1),   0x00,
    USAGE_MAXIMUM(1),   0x65,
    HIDINPUT(1),        0x00,       //   Data, Array, Abs
    REPORT_COUNT(1),    0x05,       //   5 bits (Num lock, Caps lock, Scroll lock, Compose, Kana)
    REPORT_SIZE(1),     0x01,
    USAGE_PAGE(1),      0x08,       //   LEDs
    USAGE_MINIMUM(1),   0x01,       //   Num Lock
    USAGE_MAXIMUM(1),   0x05,       //   Kana
    LOGICAL_MINIMUM(1), 0x00,
    LOGICAL_MAXIMUM(1), 0x01,
    HIDOUTPUT(1),       0x02,       //   Data, Var, Abs
    REPORT_COUNT(1),    0x01,       //   3 bits (Padding)
    REPORT_SIZE(1),     0x03,
    HIDOUTPUT(1),       0x01,       //   Const, Array, Abs
    END_COLLECTION(0)               // End application collection
};


BLEHIDDevice* hid;
BLECharacteristic* input;
BLECharacteristic* output;

const InputReport NO_KEY_PRESSED = { };


/*
 * Callbacks related to BLE connection
 */
class BleKeyboardCallbacks : public BLEServerCallbacks {

    void onConnect(BLEServer* server) {
        isBleConnected = true;

        // Allow notifications for characteristics
        BLE2902* cccDesc = (BLE2902*)input->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
        cccDesc->setNotifications(true);

        Serial.println("Client has connected");
    }

    void onDisconnect(BLEServer* server) {
        isBleConnected = false;

        // Disallow notifications for characteristics
        BLE2902* cccDesc = (BLE2902*)input->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
        cccDesc->setNotifications(false);

        Serial.println("Client has disconnected");
    }
};


/*
 * Called when the client (computer, smart phone) wants to turn on or off
 * the LEDs in the keyboard.
 * 
 * bit 0 - NUM LOCK
 * bit 1 - CAPS LOCK
 * bit 2 - SCROLL LOCK
 */
 class OutputCallbacks : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic* characteristic) {
        OutputReport* report = (OutputReport*) characteristic->getData();
        Serial.print("LED state: ");
        Serial.print((int) report->leds);
        Serial.println();
    }
};

void bluetoothTask(void*) {

    // initialize the device
    BLEDevice::init(DEVICE_NAME);
    BLEServer* server = BLEDevice::createServer();
    server->setCallbacks(new BleKeyboardCallbacks());

    // create an HID device
    hid = new BLEHIDDevice(server);
    input = hid->inputReport(1); // report ID
    output = hid->outputReport(1); // report ID
    output->setCallbacks(new OutputCallbacks());

    // set manufacturer name
    hid->manufacturer()->setValue("Maker Community");
    // set USB vendor and product ID
    hid->pnp(0x02, 0xe502, 0xa111, 0x0210);
    // information about HID device: device is not localized, device can be connected
    hid->hidInfo(0x00, 0x02);

    // Security: device requires bonding
    BLESecurity* security = new BLESecurity();
    security->setAuthenticationMode(ESP_LE_AUTH_BOND);

    // set report map
    hid->reportMap((uint8_t*)REPORT_MAP, sizeof(REPORT_MAP));
    hid->startServices();

    // set battery level to 100%
    hid->setBatteryLevel(100);

    // advertise the services
    BLEAdvertising* advertising = server->getAdvertising();
    advertising->setAppearance(HID_KEYBOARD);
    advertising->addServiceUUID(hid->hidService()->getUUID());
    advertising->addServiceUUID(hid->deviceInfo()->getUUID());
    advertising->addServiceUUID(hid->batteryService()->getUUID());
    advertising->start();

    Serial.println("BLE ready");
    delay(portMAX_DELAY);
};


void typeText(const char* text) {
    int len = strlen(text);
    for (int i = 0; i < len; i++) {

        // translate character to key combination
        uint8_t val = (uint8_t)text[i];
        if (val > KEYMAP_SIZE)
            continue; // character not available on keyboard - skip
        KEYMAP map = keymap[val];

        // create input report
        InputReport report = {
            .modifiers = map.modifier,
            .reserved = 0,
            .pressedKeys = {
                map.usage,
                0, 0, 0, 0, 0
            }
        };

        // send the input report
        input->setValue((uint8_t*)&report, sizeof(report));
        input->notify();

        delay(5);

        // release all keys between two characters; otherwise two identical
        // consecutive characters are treated as just one key press
        input->setValue((uint8_t*)&NO_KEY_PRESSED, sizeof(NO_KEY_PRESSED));
        input->notify();

        delay(5);
    }
}

/*
 * Reset state of ESP to boot conditions
 *
 */
void reset()
{

}