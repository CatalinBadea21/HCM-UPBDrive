#ifndef CAN_COMM_H
#define CAN_COMM_H

/*
* File:        can_comm.h
* Description: TBC
*/

// Includes
#include <project.h>

// Defines
#define MAX_RPM 14000u      // ICE Maximum RPM
#define RPM_THRESHOLD 1000u // Threshold for the engine state
#define RPM_MULTIPLIER 100u // Multiplier for RPM according to ECU divider
#define BRAKE_THRESHOLD 80u // Threshold for brake pedal state
#define CMD_MODE 0u         // Can message command to write to ESC
// CAN IDs (already set in configuration file)
#define ESC_CAN_ID 207u     // SID (25) left shifted 3 bits (25*8) + sender address (7) -> 0xCF
#define ECU_CAN_ID 1000u    // From EMU Master, CAN ID with message data for HCM is 0x3E8

// Typedefs
typedef struct {
    uint8_t engine_state;   // 1 = on, 0 = off
    uint16_t rpm;           // engine RPM
    uint8_t tps;            // throttle position sensor (0-100%)
    uint8_t vss;            // vehicle speed sensor (km/h)
    uint8_t brake_state;    // 1 = pressed, 0 = not pressed
    uint8_t gear;           // current gear (0-5)
} vehicle_status_t;

// Global external variables
extern volatile uint8_t CANisrFlag;
extern volatile vehicle_status_t car_state;

// Function declarations
void CAN_Transmit_To_ESC(uint8_t driving_mode, int16_t duty_cycle);
uint8_t CAN_Read_From_ECU();
CY_ISR_PROTO(ISR_CAN);

#endif /* CAN_COMM_H */
/* [] */