#ifndef CAN_COMM_H
#define CAN_COMM_H

/*
* File:        can_comm.h
* Description: TBC
*/

// Includes
#include <project.h>

// Defines
#define MAX_RPM 14000u
#define RPM_THRESHOLD 1000u
#define RPM_MULTIPLIER 100u
#define BRAKE_THRESHOLD 80u
#define CMD_MODE 0u

// CAN IDs (already set in configuration file)
#define ESC_CAN_ID 207u // SID (25) left shifted 3 bits (25*8) + sender address (7) -> 0xCF
#define ECU_CAN_ID 1000u // From EMU Master, CAN ID with message data for HCM is 0x3E8

// Typedefs
typedef struct {
    uint8_t engine_state;  // 1 = on, 0 = off
    uint16_t rpm;       // engine revolutions per minute
    uint8_t tps;        // throttle position sensor (0-100%)
    uint8_t vss;        // vehicle speed sensor
    uint8_t brake_state;   // 1 = pressed; 0 = not pressed
    uint8_t gear;        // current gear
} vehicle_status_t;


// Function declarations
void CAN_Transmit_To_ESC();
uint8_t CAN_Read_From_ECU();
CY_ISR_PROTO(ISR_CAN);

#endif /* CAN_COMM_H */
/* [] */