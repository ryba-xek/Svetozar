#include "can.h"
#include "config.h"
#include "board.h"

/*
 * Svetozar: an open-source e-vehicles peripherals controller
 * Supported board version: v1.2
 *
 * TODO:
 *  - CAN bus
 *  - analog temp sensor
 */

bool blightOn = false;
bool hbeamOn = false;
bool lbeamOn = false;
bool rturnOn = false;
bool lturnOn = false;
bool warnOn = false;
bool hornOn = false;
bool breakOn = false;
bool auxOn = false;

bool routinesRunning = false;

uint breakCtr = 0;
uint turnCtr = 0;

void readInPins() {
    // update input vars
    blightOn = digitalRead(BLIGHT_IN_PIN) == LOW;
    hbeamOn  = digitalRead(HBEAM_IN_PIN)  == LOW;
    lbeamOn  = digitalRead(LBEAM_IN_PIN)  == LOW;
    rturnOn  = digitalRead(RTURN_IN_PIN)  == LOW;
    lturnOn  = digitalRead(LTURN_IN_PIN)  == LOW;
    warnOn   = digitalRead(WARN_IN_PIN)   == LOW;
    hornOn   = digitalRead(HORN_IN_PIN)   == LOW;
    breakOn  = digitalRead(BREAK_IN_PIN)  == LOW;
    auxOn    = digitalRead(AUX_IN_PIN)    == LOW;
}

void systick_cb() { // used for stoplight & turn signals
    bool   turnLightsValue;
    uint16 stopLightValue;

    // update input data
    readInPins();  // sometimes EXTi interrupt is not fired, this
    if (USE_ANALOG_STOP) {
        breakOn = breakOn || (analogRead(ABREAK_IN_PIN) < STOP_ANALOG_THRESHOLD);
    }

    // PIN_MAP[LED_OUT_PIN].gpio_device->regs->ODR ^= BIT(PIN_MAP[LED_OUT_PIN].gpio_bit);  // ledOn = !ledOn;
    digitalWrite(LED_OUT_PIN, !(blightOn || hbeamOn || lbeamOn || rturnOn || lturnOn || warnOn || hornOn || breakOn || auxOn));
    
    // stoplight
    if (!breakOn) {
        breakCtr = 0;
        stopLightValue = STOP_HALF_VALUE;
    } else {
        stopLightValue = (breakCtr / STOP_BLINK_TIME) % 2 == 0 ? STOP_ON_VALUE : 0;
        breakCtr = min(STOP_BLINK_TIME * STOP_BLINK_NUM * 2, breakCtr+1); 
    }
    pwmWrite(STOP_OUT_PIN, stopLightValue);

    // turn signals
    if (!(lturnOn || rturnOn || warnOn)) {
        turnCtr = 0;
        turnLightsValue = false;
    } else {
        turnLightsValue = turnCtr < TURN_BLINK_PERIOD/2; // 0,1,2,3,4 - on, 5,6,7,8,9 - off
        turnCtr++;
        if (turnCtr == TURN_BLINK_PERIOD) turnCtr = 0;
    }
    digitalWrite(LTURN_OUT_PIN, !(turnLightsValue && (lturnOn || warnOn)));
    digitalWrite(RTURN_OUT_PIN, !(turnLightsValue && (rturnOn || warnOn)));

    // high beam, low beam, horn
    digitalWrite(HBEAM_OUT_PIN, hbeamOn);
    digitalWrite(LBEAM_OUT_PIN, lbeamOn || hbeamOn);
    digitalWrite(HORN_OUT_PIN,  hornOn);

    // aux, backlight
    pwmWrite(AUX_OUT_PIN, auxOn ? AUX_ON_VALUE : 0);
    pwmWrite(BLIGHT_OUT_PIN, auxOn ? BLIGHT_ON_VALUE : 0);

    // fan
    pwmWrite(FAN_OUT_PIN, breakOn ? FAN_ON_VALUE : FAN_OFF_VALUE);

    // CPU sleep
    routinesRunning = breakOn || lturnOn || rturnOn || warnOn;
}

void setup() {
    if (CAN_BUS_ENABLED) {
        // CAN conflicts with USB, disable USB
        #ifdef SERIAL_USB
            Serial.end();
        #endif

      CANSetup();  // Initialize the CAN module and prepare the message structures
    }
    
    BoardSetup(readInPins);

    
    systick_attach_callback(systick_cb);
}


void loop() {
    // sleep when no turn lights/stop light are on
    if (!routinesRunning) {
        asm("wfi");
    }
}
