/*
 * Svetozar: an open-source e-vehicles peripherals controller
 * Supported board version: v1.1
 *
 *
 * Pin assignments:
 *                               _________________
 *                              /       L__J      \
 *               High beam in - | PB12        GND | -
 *                Low beam in - | PB13        GND | -
 *              Right turn in - | PB14       3.3V | -
 *               Left turn in - | PB15      RESET | -
 *                 Warning in - | PA8        PB11 | - Backlight in
 *                    Horn in - | PA9        PB10 | - USART TX3 & LED
 * TIM1_CH3     Stoplight out - | PA10        PB1 | - High beam out           TIM3_CH4
 *                       USB- - | PA11        PB0 | - Low beam out            TIM3_CH3
 *                       USB+ - | PA12        PA7 | - Horn out                TIM3_CH2
 *                ACS711LC V+ - | PA15        PA6 | - LV current sense
 *             Overcurrent in - | PB3         PA5 | - A/D break in
 *                     Aux in - | PB4         PA4 | - Analog temp sensor in
 *            CAN IC shutdown - | PB5         PA3 | - Backlight out           TIM2_CH4
 * TIM4_CH1     Left turn out - | PB6         PA2 | - Fan out                 TIM2_CH3
 * TIM4_CH2    Right turn out - | PB7         PA1 | - Aux out                 TIM2_CH2
 *                     CAN Rx - | PB8         PA0 | - HV current sense
 *                     CAN Tx - | PB9        PC15 | - LSE oscillator in
 *                            - | 5V         PC14 | - LSE oscillator out
 *                            - | GND        PC13 | - Onboard LED out
 *                            - | 3.3V       VBAT |
 *                              \_________________/
 *
 * EXTi map:
 * EXTI0:
 * EXTI1:
 * EXTI2:
 * EXTI3: PB3 - Overcurrent in
 * EXTI4: PB4 - Aux in
 * EXTI5: PA5 - Break in
 * EXTI6:
 * EXTI7:
 * EXTI8: PA8 - Warning in
 * EXTI9: PA9 - Horn in
 * EXTI10:
 * EXTI11: PB11 - Backlight in
 * EXTI12: PB12 - High beam in
 * EXTI13: PB13 - Low beam in
 * EXTI14: PB14 - Right turn in
 * EXTI15: PB15 - Left turn in
 */


#ifndef BOARD_H
#define BOARD_H

#if F_CPU != 48000000UL
    #error "Please select Tools -> CPU Speed -> 48Mhz"
#endif


#define HBEAM_IN_PIN    PB12
#define LBEAM_IN_PIN    PB13
#define RTURN_IN_PIN    PB14
#define LTURN_IN_PIN    PB15
#define WARN_IN_PIN     PA8
#define HORN_IN_PIN     PA9
#define BREAK_IN_PIN    PA10
#if BOARD_VER == BOARD_VER_1_1
    #define AUX_IN_PIN      PA15
#else
    #define AUX_IN_PIN      PB4
#endif
#define CAN_SHDN_PIN    PB5
#define LTURN_OUT_PIN   PB6
#define RTURN_OUT_PIN   PB7
#define CAN_RX_PIN      PB8
#define CAN_TX_PIN      PB9

#define HBEAM_OUT_PIN   PB1
#define LBEAM_OUT_PIN   PB0
#define HORN_OUT_PIN    PA7
#define STOP_OUT_PIN    PA6
#define ABREAK_IN_PIN   PA5
#define ATEMP_IN_PIN    PA4
#define BLIGHT_OUT_PIN  PA3
#define FAN_OUT_PIN     PA2
#define AUX_OUT_PIN     PA1
#define BLIGHT_IN_PIN   PA0
#define LED_OUT_PIN     PC13

void BoardSetup(void (*callback)(void));

#endif //BOARD_H
