#include "Arduino.h"
#include "config.h"
#include <HardwareCAN.h>

HardwareCAN canBus(CAN1_BASE);

void CANSetup(void)
{
    CAN_STATUS Stat ;
    
    // Initialize CAN module
    canBus.map(CAN_GPIO_PB8_PB9);
    Stat = canBus.begin(CAN_SPEED_125, CAN_MODE_NORMAL);    // Other speeds go from 125 kbps to 1000 kbps. CAN allows even more choices.

    canBus.filter(0, 0, 0);
    canBus.set_irq_mode();              // Use irq mode (recommended), so the handling of incoming messages
  
}

