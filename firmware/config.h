
#ifndef CONFIG_H
#define CONFIG_H

#define CAN_BUS_ENABLED          false
#define SYSTICK                  F_CPU/20 - 1  // 50 ms

#define USE_ANALOG_STOP          false
#define STOP_ANALOG_THRESHOLD    500   // anything lower will trigger stop
#define STOP_HALF_VALUE          450   // half-on state, max 2048
#define STOP_ON_VALUE            2048  // full-on state, max 2048
#define STOP_BLINK_NUM           1     // # of blinks
#define STOP_BLINK_TIME          2     // # of 50 ms cycles for a blink off time

#define TURN_BLINK_PERIOD        10    // # of 50 ms cycles, must be even, European standards require 1.5 +/-0.5 Hz

#define AUX_ON_VALUE             2048  // 0..2048

#define BLIGHT_ON_VALUE          2048  // 0..2048

#define FAN_OFF_VALUE            0     // full-off state, max 2048
#define FAN_ON_VALUE             250   // full-on state, max 2048, around 23437 Hz

#endif //CONFIG_H
