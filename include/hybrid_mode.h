#ifndef HYBRID_MODE_H
#define HYBRID_MODE_H

/*
* File:        hybrid_mode.h
* Description: TBC
*/

// Includes
#include <project.h>
#include <include/can_comm.h>
#include <include/user_input.h>

// Macros, typedefs, and structs
#define FREEWHEEL_MODE 0u
#define TORQUE_MODE 2u
#define SPEED_MODE 3u
#define BRAKE_MODE 8u
#define MAX_VALUE_15BIT 32767u
#define NEUTRAL_GEAR 0u
#define MIN_VSS_TO_DEPLOY 5u
#define MAX_VSS_TO_DEPLOY 90u
#define STD_ON 1u
#define STD_OFF 0u

typedef struct {
    uint8_t driving_mode;
    int8_t duty_cycle_percentage;
} HY_strategy_t;

// Function declarations
uint8_t TPS_to_Duty_Cycle_Percentage(uint8_t tps_percentage);
int16_t Map_Duty_Cycle(int8_t duty_cycle_percentage);
void Set_Strategy(uint8_t sel_strategy);
void Set_Strategy_Freewheel();
void Set_Strategy_Manual();
void Set_Strategy_Launch_Assist();
void Set_Strategy_Auto_No_Regen();
void Set_Strategy_Auto_Brake_Regen();
void Set_Strategy_Auto_Always_Regen();
void Set_Strategy_Torque_Fill();

#endif /* HYBRID_MODE_H */
/* [] */