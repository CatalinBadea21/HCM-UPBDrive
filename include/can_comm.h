/*
* File:        can_comm.h
* Description: Header file for CAN communication between the main controller and external
*              subsystems such as the ESC and ECU. Defines constants, data structures,
*              global variables, and function prototypes required for transmitting and
*              receiving CAN messages, as well as handling CAN interrupts.
*/

#ifndef CAN_COMM_H
#define CAN_COMM_H

/**********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include <project.h>

/**********************************************************************************************************************
* Defines
***********************************************************************************************************************/
#define MAX_RPM 14000u              /* ICE Maximum RPM */
#define RPM_THRESHOLD 1000u         /* Threshold for the engine state */
#define RPM_MULTIPLIER 100u         /* Multiplier for RPM according to ECU divider */
#define MAX_GEAR 5u                 /* Gearbox maximum value */
#define MAX_VSS 130u                /* Vehicle maximum speed */
#define MAX_TPS 100u                /* Maximum throttle since it's percentage */
#define BRAKE_THRESHOLD 0u          /* Threshold for brake pedal state */
#define CMD_MODE 0u                 /* CAN message command to write to ESC */
#define CAN_MAX_FAILED_ATTEMPTS 7u  /* Maximum attempts for failed CAN communication */
#define MASK_7_BITS 0x7F
#define MASK_8_BITS 0xFF

/**********************************************************************************************************************
* Typedefs
***********************************************************************************************************************/
typedef struct {    
    uint16_t rpm;           /* Engine RPM */
    uint8_t engine_state;   /* 1 = on, 0 = off */
    uint8_t gear;           /* Current gear (0-5) */
    uint8_t tps;            /* Throttle position sensor (0-100%) */
    uint8_t vss;            /* Vehicle speed sensor (km/h) */
    uint8_t brake_state;    /* 1 = pressed, 0 = not pressed */
} vehicle_status_t;

/**********************************************************************************************************************
* Global external variables
***********************************************************************************************************************/
extern volatile uint8_t CANisrFlag;
extern volatile vehicle_status_t car_state;

/**********************************************************************************************************************
* Function declarations
***********************************************************************************************************************/
void CAN_Transmit_To_ESC(uint8_t driving_mode, int8_t duty_cycle);
uint8_t CAN_Read_From_ECU();
CY_ISR_PROTO(ISR_CAN);

#endif /* CAN_COMM_H */
/* [] */