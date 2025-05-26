/*
* File:        hybrid_mode.c
* Description: TBC
*/

#include <include/hybrid_mode.h>

/*
* Hybrid Strategies:
* 0 - Freewheel
* 1 - Manual mode (boost button)
* 2 - Launch assist (<7500 RPM, <45 km/h, TPS >80%, 1st gear)
* 3 - Auto, no regen
* 4 - Auto, regen on brake press
* 5 - Auto, regen for TPS<40%
* 6 - Torque fill (2000-8000 RPM, TPS>60%) , regen on brake press
* 
* Auto mode is mapping TPS from 50 to 100
* !! Boost button overrides auto mode
*/

// Global variables
volatile HY_strategy_t hybrid_state = {0, 0};

// Functions
uint8_t TPS_to_Duty_Cycle_Percentage(uint8_t tps_percentage)
{
    return 2 * (tps_percentage - 50);
}

int16_t Map_Duty_Cycle(int8_t duty_cycle_percentage)
{
    return (int16_t)((duty_cycle_percentage * MAX_VALUE_15BIT) / 100);
}

void Set_Strategy(uint8_t sel_strategy)
{
    if ((car_state.gear == NEUTRAL_GEAR) || (car_state.vss <= MIN_VSS_TO_DEPLOY) || (car_state.vss <= MAX_VSS_TO_DEPLOY))
        Set_Strategy_Freewheel();
    else
        switch (sel_strategy)
        {
            case 1:
                Set_Strategy_Manual();
                break;

            case 2:
                Set_Strategy_Launch_Assist();
                break;

            case 3:
                Set_Strategy_Auto_No_Regen();
                break;

            case 4:
                Set_Strategy_Auto_Brake_Regen();
                break;

            case 5:
                Set_Strategy_Auto_Always_Regen();
                break;

            case 6:
                Set_Strategy_Torque_Fill();
                break;

            default:
                Set_Strategy_Freewheel();
        }
}

void Set_Strategy_Freewheel()
{
    hybrid_state.driving_mode = FREEWHEEL_MODE;
    hybrid_state.duty_cycle_percentage = 0;
}

void Set_Strategy_Manual()
{
    // Boost button pressed and brake not pressed
    if ((Read_Boost_Button() == STD_ON) && (car_state.brake_state == STD_OFF))
    {
        hybrid_state.driving_mode = TORQUE_MODE;
        hybrid_state.duty_cycle_percentage = 100;
    }
    else // Freewheel in rest
        Set_Strategy_Freewheel();
}

void Set_Strategy_Launch_Assist()
{
    // RPM under 7500, VSS under 45 km/h, TPS over 80%, 1st gear and brake not pressed
    if ((car_state.rpm <= 7500) && (car_state.vss <= 45) && (car_state.tps >= 80) \
        && (car_state.gear == 1) && (car_state.brake_state == STD_OFF))
    {
        hybrid_state.driving_mode = TORQUE_MODE;
        hybrid_state.duty_cycle_percentage = 100;
    }
    else // Freewheel in rest
        Set_Strategy_Freewheel();
}

void Set_Strategy_Auto_No_Regen()
{
    // Proportional to TPS, brake not pressed
    if ((car_state.brake_state == STD_OFF))
    {
        hybrid_state.driving_mode = TORQUE_MODE;
        hybrid_state.duty_cycle_percentage = TPS_to_Duty_Cycle_Percentage(car_state.tps);
    }
    else // Freewheel in rest
        Set_Strategy_Freewheel();
}

void Set_Strategy_Auto_Brake_Regen()
{
    // Proportional to TPS, brake not pressed
    if ((car_state.brake_state == STD_OFF))
    {
        hybrid_state.driving_mode = TORQUE_MODE;
        hybrid_state.duty_cycle_percentage = TPS_to_Duty_Cycle_Percentage(car_state.tps);
    }
    else // Regen on braking
    {
        hybrid_state.driving_mode = BRAKE_MODE;
        hybrid_state.duty_cycle_percentage = 100u;
    }
}

void Set_Strategy_Auto_Always_Regen()
{
    // Proportional to TPS, brake not pressed
    if ((car_state.brake_state == STD_OFF) && (car_state.tps > 50))
    {
        hybrid_state.driving_mode = TORQUE_MODE;
        hybrid_state.duty_cycle_percentage = TPS_to_Duty_Cycle_Percentage(car_state.tps);
    }
    else // Regen on coasting
    {
        hybrid_state.driving_mode = BRAKE_MODE;
        hybrid_state.duty_cycle_percentage = 100u;
    }
}

void Set_Strategy_Torque_Fill()
{
    // TPS > 50, RPM between 2000-8000, brake not pressed
    if ((car_state.brake_state == STD_OFF) && (car_state.tps > 50) && (car_state.rpm >= 2000) && (car_state.rpm <= 8000))
    {
        hybrid_state.driving_mode = TORQUE_MODE;
        hybrid_state.duty_cycle_percentage = 100u;
    }
    else if (car_state.brake_state == STD_ON) // Regen on braking
    {
        hybrid_state.driving_mode = BRAKE_MODE;
        hybrid_state.duty_cycle_percentage = 100u;
    }
    else // Freewheel in rest
        Set_Strategy_Freewheel();
}

/* [] END OF FILE */