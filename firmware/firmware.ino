#include "can.h"
#include "config.h"
#include "board.h"
#include "HardwareSerial.h"
#include <HardwareCAN.h>

/*
 * Svetozar: an open-source e-vehicles peripherals controller
 * Supported board versions: v1.1 (with minor bugs), v1.2
 *
 * TODO:
 *  - analog temp sensor
 *
 *  IMPORTANT NOTES:
 *   - switch CPU frequency to 48 MHz
 *   - edit config.h
 *   - do not use delay() function
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
uint8 motorTemp = 0;

bool routinesRunning;
bool needQueryTemp = false;
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

    #if USE_ANALOG_STOP
        breakOn = breakOn || (analogRead(ABREAK_IN_PIN) < STOP_ANALOG_THRESHOLD);
    #endif
}

void processPeriodicTask() { // called every 50 ms
    uint16 stopLightValue;
    bool   turnLightsValue;

    // update input data
    readInPins();  // sometimes EXTi interrupt is not fired, this helps
motorTemp = blightOn ? 100 : 60; //TODO: remove
    needQueryTemp = true;

    // light up led if any input is triggered
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
        turnCtr++; //TODO: move to interrupt handler
        if (turnCtr == TURN_BLINK_PERIOD) turnCtr = 0;
    }
    digitalWrite(LTURN_OUT_PIN, !(turnLightsValue && (lturnOn || warnOn)));
    digitalWrite(RTURN_OUT_PIN, !(turnLightsValue && (rturnOn || warnOn)));

    // high beam, low beam, horn
    digitalWrite(HBEAM_OUT_PIN, hbeamOn);
    digitalWrite(LBEAM_OUT_PIN, lbeamOn || hbeamOn);
    digitalWrite(HORN_OUT_PIN,  hornOn);

    // backlight, aux
    //pwmWrite(BLIGHT_OUT_PIN, blightOn ? BLIGHT_ON_VALUE : 0);
    pwmWrite(AUX_OUT_PIN, auxOn ? AUX_ON_VALUE : 0);

    // fan
    pwmWrite(FAN_OUT_PIN, blightOn ? 1024 : 0); //calcFanPwm(motorTemp)
    
    char buffer[100];
    uint16 v;
    v = calcFanPwm(motorTemp);
    sprintf(buffer, "debug: %d", v);
    //Serial3.println(buffer);
     
//    Serial3.print("Motor temp: ");
//    Serial3.print(motorTemp);
//    Serial3.print(", fan PWM value: ");
//    Serial3.println(calcFanPwm(motorTemp));

    // can CPU sleep?
    routinesRunning = breakOn || lturnOn || rturnOn || warnOn;

    //pass control to loop()
}

void setup() {
    #if CAN_BUS_ENABLED
        // CAN conflicts with USB, disable USB
        #ifdef SERIAL_USB
            Serial.end();
        #endif

      CANSetup();  // Initialize the CAN module and prepare the message structures
    #endif
    
    BoardSetup(readInPins);
    
    systick_attach_callback(processPeriodicTask);
    //Serial3.print("Setup over");
}

uint16 calcFanPwm(uint8 temp) {
    if (temp < MOTOR_TRIGGER_TEMP) {
        return 0;
    }

    if (temp >= MOTOR_MAX_TEMP) {
        return FAN_MAX_VALUE;
    }

    return FAN_MIN_VALUE + (temp - MOTOR_TRIGGER_TEMP) * (FAN_MAX_VALUE - FAN_MIN_VALUE) / (MOTOR_MAX_TEMP - MOTOR_TRIGGER_TEMP);
}

void loop() {
    CanMsg *r_msg;
    float temp;

    #if CAN_BUS_ENABLED
        if (needQueryTemp) {
            needQueryTemp = false;
            SendCANmessage(CAN_MOTOR_TEMP_REQ_MSGID, 8, 0x40, 0x08, 0x21, 0x2, 0x0, 0x0, 0x0, 0x0);
        }
    #endif

    #if CAN_BUS_ENABLED
    // process CAN messages
    while ((r_msg = canBus.recv()) != NULL) {
        switch (r_msg->ID) {
            case CAN_BREAK_MSGID:
                breakOn |= bitRead(r_msg->Data[6], 1);
                break;

            case CAN_MOTOR_TEMP_MSGID:
                char tempbytes[4] = {r_msg->Data[4], r_msg->Data[5], r_msg->Data[6], r_msg->Data[7]};
                memcpy(&temp, tempbytes, sizeof(float));
                motorTemp = temp;
                break;

            default:
                break;
        }

        canBus.free();
    }
    #endif

    // sleep when no turn/stop lights are on
    if (!routinesRunning) {
        asm("wfi");
    }
}
