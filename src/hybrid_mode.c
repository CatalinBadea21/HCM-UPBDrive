/*
 * File:        hybrid_mode.c
 * Description: Implements the hybrid drive control strategies for the HCM project. Based on current
 *              vehicle status (gear, RPM, speed, TPS, brake state) and user input, this module 
 *              selects and applies the appropriate driving mode by updating the hybrid_state
 *              and transmitting the command via CAN to the ESC.
*/

#include <include/hybrid_mode.h>

/*
** Hybrid Strategy Modes **
  |----|-----------------------|-------------------------------------------------------|
  | ID | Name                  | Description                                           |
  |----|-----------------------|-------------------------------------------------------|
  | 0  | Freewheel             | Default idle mode                                     |
  | 1  | Manual (Boost)        | Full torque if boost button is active                 |
  | 2  | Launch Assist         | Active at low RPM, low speed, high TPS, gear 1        |
  | 3  | Auto No Regen         | Proportional torque, no regen                         |
  | 4  | Auto Brake Regen      | Regen on brake press                                  |
  | 5  | Auto TPS-Based Regen  | Regen when TPS < threshold or braking                 |
  |----|-----------------------|-------------------------------------------------------|
  *
  * Auto mode is mapping TPS from a threshold to 100
  * ! Boost button overrides any mode, as long as the strategy is not set to Freewheel (Hybrid OFF)
*/

/**********************************************************************************************************************
* Global variables
***********************************************************************************************************************/
volatile HY_strategy_t hybrid_state = {0, 0};

/**********************************************************************************************************************
* Functions
***********************************************************************************************************************/
/*
 * Function:    TPS_to_Duty_Cycle_Percentage
 * Description: Converts the TPS value (over a threshold) to a proportional duty cycle.
 * Parameters:
 *   - tps_percentage: Throttle position sensor value from ECU [0–100].
 * Returns:
 *   - A duty cycle percentage [0–100], mapped linearly for TPS ≥ threshold.
 *     Returns 0 for TPS < threshold.
 */
uint8_t TPS_to_Duty_Cycle_Percentage(uint8_t tps_percentage)
{
    if (tps_percentage < MAPPING_TPS_THRESHOLD)
        return 0;
    else
        return ((tps_percentage - MAPPING_TPS_THRESHOLD ) * 100) / (100 - MAPPING_TPS_THRESHOLD);
}

/*
 * Function:    Map_Duty_Cycle
 * Description: Converts a duty cycle percentage to a 15-bit signed integer for CAN transmission.
 * Parameters:
 *   - duty_cycle_percentage: Desired motor duty (in percent, −100 to 100).
 * Returns:
 *   - 15-bit signed value scaled to match ESC input format.
 */
int16_t Map_Duty_Cycle(int8_t duty_cycle_percentage)
{
    return (int16_t)((duty_cycle_percentage * MAX_VALUE_15BIT) / 100);
}

/*
 * Function:    Emergency_Stop
 * Description: Sets the hybrid mode to freewheel, then disables the AIR.
 */
void Emergency_Stop()
{
    Set_Strategy(FREEWHEEL_S); /* Set the ESC to freewheel before power off */
    CyDelay(500u); /* Wait 500 ms for the motor to slow down */
    AIR_enable_Write(STD_OFF); /* Disable the AIR */
}

/*
 * Function:    Set_Strategy
 * Description: Applies the appropriate hybrid driving strategy based on selector input,
 *              vehicle state, and driver override.
 *
 * Parameters:
 *   - sel_strategy: User-selected strategy index [1–6].
 *
 * Logic:
 *   - If gear is neutral or speed is too low/high -> Freewheel.
 *   - If boost button is pressed, brake is not, and sel_strategy > 0 -> Full torque.
 *   - Otherwise, applies strategy based on sel_strategy.
 *   - Defaults to Freewheel if sel_strategy is invalid.
 *
 * Sends the selected driving mode and duty cycle to the ESC via CAN.
 */
void Set_Strategy(uint8_t sel_strategy)
{
    if ((car_state.gear == NEUTRAL_GEAR) || (car_state.vss <= MIN_VSS_TO_DEPLOY) || (car_state.vss > MAX_VSS_TO_DEPLOY))
        Set_Strategy_Freewheel();
    else if ((Read_Boost_Button() == STD_ON) && (car_state.brake_state == STD_OFF) && (sel_strategy > 0))
    {
        hybrid_state.driving_mode = TORQUE_MODE;
        hybrid_state.duty_cycle_percentage = BOOST_TORQUE_PERCENTAGE;
    }
    else
        switch (sel_strategy)
        {
            case FREEWHEEL_S:
                Set_Strategy_Freewheel();
                break;
                
            case MANUAL_S:
                Set_Strategy_Manual();
                break;

            case LAUNCH_S:
                Set_Strategy_Launch_Assist();
                break;

            case AUTO_NOREGEN_S:
                Set_Strategy_Auto_No_Regen();
                break;

            case AUTO_BRAKEREGEN_S:
                Set_Strategy_Auto_Brake_Regen();
                break;

            case AUTO_ALLREGEN_S:
                Set_Strategy_Auto_Always_Regen();
                break;

            default:
                Set_Strategy_Freewheel();
        }
    
    CAN_Transmit_To_ESC(hybrid_state.driving_mode, hybrid_state.duty_cycle_percentage);
}

/*
 * Function:    Set_Strategy_Freewheel
 * Description: Disables hybrid power by setting mode to Freewheel and zero duty cycle.
 */
void Set_Strategy_Freewheel()
{
    hybrid_state.driving_mode = FREEWHEEL_MODE;
    hybrid_state.duty_cycle_percentage = STD_OFF;
}

/*
 * Function:    Set_Strategy_Manual
 * Description: Enables full power (boost mode) if the boost button is pressed and the brake is not.
 *              Falls back to Freewheel otherwise.
 */
void Set_Strategy_Manual()
{
    /* The logic for the boost button is in the main Set_Strategy() function;
     * If the program enters here it means the selector is on manual,
     * but boost button is not pressed -> freewheel */
    Set_Strategy_Freewheel();
}

/*
 * Function:    Set_Strategy_Launch_Assist
 * Description: Engages full torque at launch under these conditions:
 *              - RPM and VSS under a set threshold
 *              - TPS over a set threshold
 *              - Gear = 1
 *              - Brake not pressed
 *              Falls back to Freewheel otherwise.
 */
void Set_Strategy_Launch_Assist()
{
    if ((car_state.rpm <= LAUNCH_MAX_RPM) && (car_state.vss <= LAUNCH_MAX_VSS) && (car_state.tps >= LAUNCH_MIN_TPS) \
        && (car_state.gear == 1) && (car_state.brake_state == STD_OFF))
    {
        hybrid_state.driving_mode = TORQUE_MODE;
        hybrid_state.duty_cycle_percentage = LAUNCH_TORQUE_PERCENTAGE;
    }
    else
        Set_Strategy_Freewheel();
}

/*
 * Function:    Set_Strategy_Auto_No_Regen
 * Description: Applies torque proportional to TPS when brake is not pressed.
 *              Falls back to Freewheel if brake is active.
 */
void Set_Strategy_Auto_No_Regen()
{
    if (car_state.brake_state == STD_OFF)
    {
        hybrid_state.driving_mode = TORQUE_MODE;
        hybrid_state.duty_cycle_percentage = TPS_to_Duty_Cycle_Percentage(car_state.tps);
    }
    else
        Set_Strategy_Freewheel();
}

/*
 * Function:    Set_Strategy_Auto_Brake_Regen
 * Description: Applies torque proportional to TPS when brake is not pressed.
 *              Switches to full regenerative braking (BRAKE mode) when brake is pressed.
 */
void Set_Strategy_Auto_Brake_Regen()
{
    if (car_state.brake_state == STD_OFF)
    {
        hybrid_state.driving_mode = TORQUE_MODE;
        hybrid_state.duty_cycle_percentage = TPS_to_Duty_Cycle_Percentage(car_state.tps);
    }
    else /* Braking */
    {
        hybrid_state.driving_mode = BRAKE_MODE;
        hybrid_state.duty_cycle_percentage = BRAKE_REGEN_INTENSITY_PERCENTAGE;
    }
}

/*
 * Function:    Set_Strategy_Auto_Always_Regen
 * Description: Applies torque proportional to TPS if TPS > 50 and brake is not pressed.
 *              Regenerative braking otherwise (different intensity for coasting or braking)
 */
void Set_Strategy_Auto_Always_Regen()
{
    if ((car_state.brake_state == STD_OFF) && (car_state.tps > ALWAYS_REGEN_TPS_THRESHOLD))
    {
        hybrid_state.driving_mode = TORQUE_MODE;
        hybrid_state.duty_cycle_percentage = TPS_to_Duty_Cycle_Percentage(car_state.tps);
    }
    else if (car_state.brake_state == STD_OFF) /* TPS under threshold, brake not pressed */
    {
        hybrid_state.driving_mode = BRAKE_MODE;
        hybrid_state.duty_cycle_percentage = COASTING_REGEN_INTENSITY_PERCENTAGE;
    }
    else /* Brake pressed */
    {
        hybrid_state.driving_mode = BRAKE_MODE;
        hybrid_state.duty_cycle_percentage = BRAKE_REGEN_INTENSITY_PERCENTAGE;
    }
}

/* [] END OF FILE */