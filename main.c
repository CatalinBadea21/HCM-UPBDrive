/*
* File:         main.c
* Description:  Main function for the HCM project.
* Copyright:    2025, Catalin Badea - UPB Drive Hybrid Department
*/

// Includes
#include <project.h>
#include <stdio.h>
#include <include/can_comm.h>
#include <include/hybrid_mode.h>
#include <include/user_input.h>

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

    for(;;)
    {
        // my loop

        if (CAN_Read_From_ECU()) //can read was successfull
        {
            if (car_state.engine_state) //engine is on
            {
                //do smth
            }
            else
            {
                //turn HY off
            }
        }

        CyDelay(50u);
    }
}

/* [] END OF FILE */