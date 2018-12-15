#include "Arduino.h"
#include "config.h"
#include <HardwareCAN.h>

HardwareCAN canBus(CAN1_BASE);

void CANSetup(void)
{
    CAN_STATUS Stat;
    
    // Initialize CAN module
    canBus.map(CAN_GPIO_PB8_PB9);
    Stat = canBus.begin(CAN_SPEED_1000, CAN_MODE_NORMAL);    // Other speeds go from 125 kbps to 1000 kbps. CAN allows even more choices.

    canBus.filter(0, 0, 0);
    canBus.set_irq_mode();              // Use irq mode (recommended), so the handling of incoming messages

    if (Stat != CAN_OK) {
        Serial3.println("CAN init failed");   // Initialization failed
    } else {
        Serial3.println("CAN init OK");
    }

    #if F_CPU == 48000000UL
        can_init_enter(canBus.Port);

        // Set proper timings for 48 Mhz http://www.bittiming.can-wiki.info/
        canBus.Port->BTR = 0x00050002;

        if (can_init_leave(canBus.Port) == CAN_OK)
        {
          while (!(canBus.Port->TSR & CAN_TSR_TME0));  // Transmit mailbox 0 is empty
          while (!(canBus.Port->TSR & CAN_TSR_TME1));  // Transmit mailbox 0 is empty
          while (!(canBus.Port->TSR & CAN_TSR_TME2));  // Transmit mailbox 0 is empty
        }

        Stat = canBus.status();
        if (Stat != CAN_OK) {
           Serial3.println("CAN re-init failed");   // Initialization failed
        } else {
          Serial3.println("CAN re-init OK");
        }
    #endif
}

// Send one frame. Parameter is a pointer to a frame structure (above), that has previously been updated with data.
// If no mailbox is available, wait until one becomes empty. There are 3 mailboxes.
CAN_TX_MBX CANsend(CanMsg *pmsg)
{
    CAN_TX_MBX mbx;

    do {
        mbx = canBus.send(pmsg);
    } while(mbx == CAN_TX_NO_MBX);       // Waiting outbound frames will eventually be sent, unless there is a CAN bus failure.

    return mbx;
}

// Send message
// Prepare and send a frame containing some value
void SendCANmessage(long id=0x001, byte dlength=8, byte d0=0x00, byte d1=0x00, byte d2=0x00, byte d3=0x00, byte d4=0x00, byte d5=0x00, byte d6=0x00, byte d7=0x00) {
    // Initialize the message structure
    CanMsg msg;

    // A CAN structure includes the following fields:
    msg.IDE = CAN_ID_STD;          // Indicates a standard identifier ; CAN_ID_EXT would mean this frame uses an extended identifier
    msg.RTR = CAN_RTR_DATA;        // Indicated this is a data frame, as opposed to a remote frame (would then be CAN_RTR_REMOTE)
    msg.ID  = id;                  // Identifier of the frame : 0-2047 (0-0x3ff) for standard idenfiers; 0-0x1fffffff for extended identifiers
    msg.DLC = dlength;             // Number of data bytes to follow

    // Prepare frame : send something
    msg.Data[0] = d0;
    msg.Data[1] = d1;
    msg.Data[2] = d2;
    msg.Data[3] = d3;
    msg.Data[4] = d4;
    msg.Data[5] = d5;
    msg.Data[6] = d6;
    msg.Data[7] = d7;

    CANsend(&msg);      // Send this frame
}
