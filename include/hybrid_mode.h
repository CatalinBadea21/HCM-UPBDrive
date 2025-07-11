/*
* File:        hybrid_mode.h
* Description: Header file for hybrid drive strategy logic in the HCM project. Defines strategy modes,
*              thresholds, state structure, and public functions used to determine and apply hybrid
*              driving behavior based on vehicle status and user input. This includes manual control,
*              launch assist, and various automatic strategies with and without regenerative braking.
*/

#ifndef HYBRID_MODE_H
#define HYBRID_MODE_H

/**********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include <project.h>
#include <include/can_comm.h>
#include <include/user_input.h>

/**********************************************************************************************************************
* Defines
***********************************************************************************************************************/
#define MAX_VALUE_15BIT 32767u
#define NEUTRAL_GEAR 0u
#define STD_ON 1u
#define STD_OFF 0u
#define BOOST_TORQUE_PERCENTAGE 100u
#define LAUNCH_TORQUE_PERCENTAGE 100u
#define BRAKE_REGEN_INTENSITY_PERCENTAGE 100u
#define COASTING_REGEN_INTENSITY_PERCENTAGE 50u
/* __________ THRESHOLDS __________ */
#define MIN_VSS_TO_DEPLOY 3u
#define MAX_VSS_TO_DEPLOY 90u
#define LAUNCH_MAX_RPM 7500u
#define LAUNCH_MAX_VSS 45u
#define LAUNCH_MIN_TPS 80u
#define ALWAYS_REGEN_TPS_THRESHOLD 50u
#define MAPPING_TPS_THRESHOLD 50u
/* __________ ESC MODES __________ */
#define FREEWHEEL_MODE 0u
#define TORQUE_MODE 2u
#define SPEED_MODE 3u
#define BRAKE_MODE 8u

/**********************************************************************************************************************
* Typedefs
***********************************************************************************************************************/
typedef struct {
    uint8_t driving_mode;
    int8_t duty_cycle_percentage;
} HY_strategy_t;

/**********************************************************************************************************************
* Global external variables
***********************************************************************************************************************/
extern volatile HY_strategy_t hybrid_state;

/**********************************************************************************************************************
* Function declarations
***********************************************************************************************************************/
uint8_t TPS_to_Duty_Cycle_Percentage(uint8_t tps_percentage);
int16_t Map_Duty_Cycle(int8_t duty_cycle_percentage);
void Emergency_Stop();
void Set_Strategy(uint8_t sel_strategy);
void Set_Strategy_Freewheel();
void Set_Strategy_Manual();
void Set_Strategy_Launch_Assist();
void Set_Strategy_Auto_No_Regen();
void Set_Strategy_Auto_Brake_Regen();
void Set_Strategy_Auto_Always_Regen();

#endif /* HYBRID_MODE_H */
/* [] */