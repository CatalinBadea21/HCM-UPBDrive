/*
 * File:         main.c
 * Description:  Main control loop for the HCM project. Initializes CAN communication and handles
 *               real-time vehicle control logic based on engine state, driver input, and hybrid
 *               drive strategy selection. Interfaces with ECU via CAN, manages AIR relay,
 *               and switches driving strategies accordingly.
 * Copyright:    Catalin Badea & UPB Drive Hybrid Department, 2025
*/

/**********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include <project.h>
#include <stdio.h>
#include <AIR_enable.h>
#include <include/can_comm.h>
#include <include/hybrid_mode.h>
#include <include/user_input.h>

/*
 * Function:    init
 * Description: Initializes the CAN module and sets up the CAN interrupt handler.
 * Actions:
 *   - Starts the CAN peripheral.
 *   - Registers the ISR for CAN receive interrupts.
 *   - Enables global interrupts.
 */
void init()
{
    CAN_Start();
    CyIntSetVector(CAN_ISR_NUMBER, ISR_CAN);
    CyGlobalIntEnable;
}

/*
 * Function:    main
 * Description: Main loop for the HCM embedded control system. Continuously reads data from the
 *              ECU over CAN and selects a hybrid drive strategy based on driver input.
 */
int main()
{
    uint8_t user_strategy, rx_reattempt;
    init(); // Initialize CAN and interrupts

    while(1) // Infinite loop
    {
        if (CAN_Read_From_ECU()) // CAN read was successful
        {
            rx_reattempt = 0;
            
            if (STD_ON == car_state.engine_state) // ICE is running
            {
                /* Select which function based on HW selector */
                //user_strategy = Read_Binary_Selector();
                //user_strategy = Read_Rotary_Switch();
                user_strategy = AUTO_NOREGEN_S;
                
                AIR_enable_Write(STD_ON); // Enable the AIR
                
                Set_Strategy(user_strategy);
            }
            else // ICE is off
            {
                Emergency_Stop(); // Stop the motor and disable the AIR
            }
        }
        else // CAN Read was unsuccessfull
        {
            if (rx_reattempt > CAN_MAX_FAILED_ATTEMPTS)
            {
                Emergency_Stop(); // Stop the motor and disable the AIR
            }
            else
            {
                rx_reattempt++;
            }
        }
               
    CyDelay(50u); // Delay for 50 ms
    }
}

/* [] END OF FILE */