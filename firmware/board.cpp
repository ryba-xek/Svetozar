#include "Arduino.h"
#include "board.h"
#include "config.h"

// PWM timers
HardwareTimer htimer1(1);
HardwareTimer htimer2(2);
HardwareTimer htimer3(3);
HardwareTimer htimer4(4);

void setPolarity(uint8 pin, uint8 pol) {
    if (pin >= BOARD_NR_GPIO_PINS) {
        return;
    }
    timer_dev *dev = PIN_MAP[pin].timer_device;
    uint8 cc_channel = PIN_MAP[pin].timer_channel;
    ASSERT(dev && cc_channel);
    timer_cc_set_pol(dev, cc_channel, pol);
}

void BoardSetup(void (*callback)(void), void (*csFaultCallback)(void)) {

    // Inputs
    pinMode(BLIGHT_IN_PIN,  INPUT);
    pinMode(HBEAM_IN_PIN,   INPUT);
    pinMode(LBEAM_IN_PIN,   INPUT);
    pinMode(RTURN_IN_PIN,   INPUT);
    pinMode(LTURN_IN_PIN,   INPUT);
    pinMode(WARN_IN_PIN,    INPUT);
    pinMode(HORN_IN_PIN,    INPUT);
    pinMode(BREAK_IN_PIN,   INPUT);
    pinMode(CSFAULT_IN_PIN, INPUT_PULLUP);
    pinMode(AUX_IN_PIN,     INPUT);
    pinMode(ATEMP_IN_PIN,   INPUT_ANALOG);
    pinMode(LVCS_IN_PIN,    INPUT_ANALOG);
    pinMode(HVCS_IN_PIN,    INPUT_ANALOG);

    // Digital outputs
    pinMode(CSSUPPLY_PIN,   OUTPUT);
    pinMode(LED_OUT_PIN,    OUTPUT);
    pinMode(LED2_OUT_PIN,   OUTPUT);
    pinMode(CAN_SHDN_PIN,   OUTPUT);
    pinMode(LTURN_OUT_PIN,  OUTPUT);
    pinMode(RTURN_OUT_PIN,  OUTPUT);
    pinMode(HBEAM_OUT_PIN,  OUTPUT);
    pinMode(LBEAM_OUT_PIN,  OUTPUT);
    pinMode(HORN_OUT_PIN,   OUTPUT);

    digitalWrite(CSSUPPLY_PIN,    HIGH);  // CS supply on
    digitalWrite(LED_OUT_PIN,    HIGH);  // LED off
    digitalWrite(CAN_SHDN_PIN,   !CAN_BUS_ENABLED);
    digitalWrite(LTURN_OUT_PIN,  HIGH);  // off
    digitalWrite(RTURN_OUT_PIN,  HIGH);  // off
    digitalWrite(HBEAM_OUT_PIN,  LOW);  // off
    digitalWrite(LBEAM_OUT_PIN,  LOW);  // off
    digitalWrite(HORN_OUT_PIN,   LOW);  // off
    
    // PWM outputs
    pinMode(STOP_OUT_PIN,   PWM);
    pinMode(BLIGHT_OUT_PIN, PWM);
    pinMode(FAN_OUT_PIN,    PWM);
    pinMode(AUX_OUT_PIN,    PWM);

    htimer1.setPrescaleFactor(1);
    htimer2.setPrescaleFactor(1);
    htimer3.setPrescaleFactor(1);
    htimer4.setPrescaleFactor(1);
    htimer1.setOverflow(2047);
    htimer2.setOverflow(2047); //32768-1
    htimer3.setOverflow(2047);
    htimer4.setOverflow(2047);

    setPolarity(STOP_OUT_PIN,   HIGH);
    setPolarity(BLIGHT_OUT_PIN, HIGH);
    setPolarity(FAN_OUT_PIN,    HIGH);
    setPolarity(AUX_OUT_PIN,    HIGH);

    pwmWrite(STOP_OUT_PIN,   STOP_HALF_VALUE);
    //pwmWrite(BLIGHT_OUT_PIN, 0);
    pwmWrite(FAN_OUT_PIN,    0);
    pwmWrite(AUX_OUT_PIN,    0);

    // Special functions
    pinMode(CAN_RX_PIN,    INPUT);
    pinMode(CAN_TX_PIN,    OUTPUT);
    adc_set_prescaler(ADC_PRE_PCLK2_DIV_2);
    //Serial3.begin(115200);

    // External interrupts
    attachInterrupt(CSFAULT_IN_PIN, csFaultCallback, FALLING);
    attachInterrupt(BLIGHT_IN_PIN,  callback,        CHANGE);
    attachInterrupt(HBEAM_IN_PIN,   callback,        CHANGE);
    attachInterrupt(LBEAM_IN_PIN,   callback,        CHANGE);
    attachInterrupt(LTURN_IN_PIN,   callback,        CHANGE);
    attachInterrupt(RTURN_IN_PIN,   callback,        CHANGE);
    attachInterrupt(WARN_IN_PIN,    callback,        CHANGE);
    attachInterrupt(HORN_IN_PIN,    callback,        CHANGE);
    attachInterrupt(BREAK_IN_PIN,   callback,        CHANGE);
    #if BOARD_VER != BOARD_VER_1_1
        attachInterrupt(AUX_IN_PIN,    callback, CHANGE);
    #endif

    // Clocks & system timer
    systick_init(SYSTICK); // 50 ms

    #if USE_ANALOG_STOP
//        rcc_clk_disable(RCC_ADC1);
    #endif
//    rcc_clk_disable(RCC_ADC2);
//    rcc_clk_disable(RCC_ADC3);
//    rcc_clk_disable(RCC_AFIO);
//    rcc_clk_disable(RCC_BKP);
//    rcc_clk_disable(RCC_CRC);
//    rcc_clk_disable(RCC_DAC);
//    rcc_clk_disable(RCC_DMA1);
//    rcc_clk_disable(RCC_DMA2);
//    rcc_clk_disable(RCC_FLITF);
//    rcc_clk_disable(RCC_FSMC);
//    rcc_clk_disable(RCC_I2C1);
//    rcc_clk_disable(RCC_I2C2);
//    rcc_clk_disable(RCC_SDIO);
//    rcc_clk_disable(RCC_SPI1);
//    rcc_clk_disable(RCC_SPI2);
//    rcc_clk_disable(RCC_SPI3);
//    rcc_clk_disable(RCC_TIMER1);
//    rcc_clk_disable(RCC_TIMER5);
//    rcc_clk_disable(RCC_TIMER6);
//    rcc_clk_disable(RCC_TIMER7);
//    rcc_clk_disable(RCC_TIMER8);
//    rcc_clk_disable(RCC_TIMER9);
//    rcc_clk_disable(RCC_TIMER10);
//    rcc_clk_disable(RCC_TIMER11);
//    rcc_clk_disable(RCC_TIMER12);
//    rcc_clk_disable(RCC_TIMER13);
//    rcc_clk_disable(RCC_TIMER14);
//    rcc_clk_disable(RCC_USART1);
//    rcc_clk_disable(RCC_USART2);
//    rcc_clk_disable(RCC_USART3);
//    rcc_clk_disable(RCC_UART4);
//    rcc_clk_disable(RCC_UART5);
    if (CAN_BUS_ENABLED) {
        rcc_clk_disable(RCC_CAN);
    }
}
