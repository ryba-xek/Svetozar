/*
 * Svetozar: an open-source e-vehicles peripherals controller
 * Supported board version: v1.1
 *
 *
 * Pin assignments:
 *                               _________________
 *                              /       L__J      \
 *                Low beam in - | PB12        GND | -
 *               High beam in - | PB13        GND | -
 *               Left turn in - | PB14       3.3V | -
 *              Right turn in - | PB15      RESET | -
 *                 Warning in - | PA8        PB11 | - Backlight in
 *                    Horn in - | PA9        PB10 | - USART TX3 & LED
 * TIM1_CH3     Stoplight out - | PA10        PB1 | - Low beam out            TIM3_CH4
 *                       USB- - | PA11        PB0 | - High beam out           TIM3_CH3
 *                       USB+ - | PA12        PA7 | - Horn out                TIM3_CH2
 *                ACS711LC V+ - | PA15        PA6 | - Analog temp sensor in
 *             Overcurrent in - | PB3         PA5 | - A/D break in
 *                     Aux in - | PB4         PA4 | - LV current sense
 *            CAN IC shutdown - | PB5         PA3 | - Fan out                 TIM2_CH4
 * TIM4_CH1    Right turn out - | PB6         PA2 | - Backlight out           TIM2_CH3
 * TIM4_CH2     Left turn out - | PB7         PA1 | - Aux out                 TIM2_CH2
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
 * EXTI12: PB12 - Low beam in
 * EXTI13: PB13 - High beam in
 * EXTI14: PB14 - Left turn in
 * EXTI15: PB15 - Right turn in
 */


#ifndef BOARD_H
#define BOARD_H

#if F_CPU != 48000000UL
    #error "Please select Tools -> CPU Speed -> 48Mhz"
#endif


#define LBEAM_IN_PIN    PB12
#define HBEAM_IN_PIN    PB13
#define LTURN_IN_PIN    PB14
#define RTURN_IN_PIN    PB15
#define WARN_IN_PIN     PA8
#define HORN_IN_PIN     PA9
#define STOP_OUT_PIN    PA10
#define CSSUPPLY_PIN    PA15
#define CSFAULT_IN_PIN  PB3
#define AUX_IN_PIN      PB4
#define CAN_SHDN_PIN    PB5
#define RTURN_OUT_PIN   PB6
#define LTURN_OUT_PIN   PB7
#define CAN_RX_PIN      PB8
#define CAN_TX_PIN      PB9

#define BLIGHT_IN_PIN   PB11
#define LED2_OUT_PIN    PB10
#define LBEAM_OUT_PIN   PB1
#define HBEAM_OUT_PIN   PB0
#define HORN_OUT_PIN    PA7
#define ATEMP_IN_PIN    PA6
#define BREAK_IN_PIN    PA5
#define LVCS_IN_PIN     PA4
#define FAN_OUT_PIN     PA3
#define BLIGHT_OUT_PIN  PA2
#define AUX_OUT_PIN     PA1
#define HVCS_IN_PIN     PA0
#define LED_OUT_PIN     PC13

void BoardSetup(void (*callback)(void), void (*csFaultCallback)(void));

#endif //BOARD_H
