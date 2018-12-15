
#ifndef CONFIG_H
#define CONFIG_H

#define BOARD_VER_1_1            101
#define BOARD_VER_1_2            102

#define BOARD_VER                BOARD_VER_1_1
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

#define FAN_MIN_VALUE            4000     // min PWM for motor to start from standstill, max 2048
#define FAN_MAX_VALUE            2047   // full-on state, max 2048

#define MOTOR_TRIGGER_TEMP       60
#define MOTOR_MAX_TEMP           100

#define CAN_BREAK_MSGID          0x1b0
#define CAN_MOTOR_TEMP_MSGID     0x5b0
#define CAN_MOTOR_TEMP_REQ_MSGID 0x630
#endif //CONFIG_H
