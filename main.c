/*
* File:         main.c
* Description:  Main function for the HCM project.
* Copyright:    2025, Catalin Badea - UPB Drive Hybrid Department
*/

// Includes
#include <project.h>
#include <stdio.h>
//#include <include/can_comm.h>
#include <include/hybrid_mode.h>
#include <include/user_input.h>
#include <AIR_enable.h>

void init()
{
    CAN_Start();
    CyIntSetVector(CAN_ISR_NUMBER, ISR_CAN);

    CyGlobalIntEnable;
}

// main() function: program starts here
int main()
{
    init();

    while(1) // Infinite loop
    {
        if (CAN_Read_From_ECU()) // CAN read was successfull
        {
            if (car_state.engine_state) // ICE is running
            {
                AIR_enable_Write(STD_ON); // Enable the AIR
            }
            else
            {
                Set_Strategy_Freewheel();
                AIR_enable_Write(STD_OFF); // Disable the AIR
            }
        }

        CyDelay(50u); // Delay for 50ms
    }
}

/* [] END OF FILE */