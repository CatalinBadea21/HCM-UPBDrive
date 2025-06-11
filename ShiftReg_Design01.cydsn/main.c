/*******************************************************************************
* File Name: main.c
*
* Version: 2.1
*
* Description:
*  This is source code for example project of the Shift Register component.
*  Parameters used:
*   Length              8,16
*   Shift direction     Right
*
*  Basic functionality of the Shift Register component is shown by this example
*  project: load value to be shifted, shift operation and store shifted data.
*  All necessary information is displayed on LCD.
*
* Note:
*  Control Register is used as component clock source to simplify the project.
*
*  'Shift Out' output is not observed (Can be provided using oscilloscope
*  or logic analyser after output pins will be connected and assigned).
*
********************************************************************************
* Copyright 2012-2015, Cypress Semiconductor Corporation. All rights reserved.
* This software is owned by Cypress Semiconductor Corporation and is protected
* by and subject to worldwide patent and copyright laws and treaties.
* Therefore, you may use this software only as provided in the license agreement
* accompanying the software package from which you obtained this software.
* CYPRESS AND ITS SUPPLIERS MAKE NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* WITH REGARD TO THIS SOFTWARE, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT,
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*******************************************************************************/

#include <project.h>


/*******************************************************************************
* Function Name: main
********************************************************************************
*
* Summary:
*  Main function performs following functions:
*   1. Declares cycle counter variables
*   2. Starts all the components
*   3. Writes values to be shifted into the shift register's input FIFO
*   4. Displays Shift Register Length value, shifted data and stored value
*   5. Controls Load and Store inputs of the Shift Registers
*   6. Indicates the example project execution is complete
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
int main()
{
    /* cycle counter variables */
    uint8 i,j;
    
    /* Start of all components used */
    LCD_1_Start();
    ShiftReg_1_Start();
    ShiftReg_2_Start();
    
    /* Generating the clock pulse which is required to enable the component 
       with the Control Register configured into the Sync Mode
    */
    Clock_1_Write(1u);
    Clock_1_Write(0u);
    Clock_2_Write(1u);
    Clock_2_Write(0u);
    
    LCD_1_Position(0u,0u);
    LCD_1_PrintString("ShiftReg example ");
    CyDelay(3000u);
        
    /* 8-bit Shift Register section*/
    LCD_1_ClearDisplay();
    LCD_1_Position(0u,0u);
    LCD_1_PrintString("8-bit ShiftReg:");
        
    /* Write data to be shifted into the input FIFO  */
    ShiftReg_1_WriteData(0xFFu);
    
    /* Load data from the input FIFO to the shift register */
    Load_1_Write(1u);
    Clock_1_Write(1u);
    CyDelay(10);
    Clock_1_Write(0u);
    CyDelay(30);
    
    /* Right Shift loaded data (supplement with 0s) */
    for(j=0u; j<2u; j++)
    {
        LCD_1_ClearDisplay();
        LCD_1_Position(0u,0u);
        LCD_1_PrintString("8-bit ShiftReg:");
        
        for(i=0u; i<10u; i=i+3u)
        {
            Clock_1_Write(1u);
            Clock_1_Write(0u);
            
            /* Display shifted values */
            LCD_1_Position(1u,i);
            LCD_1_PrintHexUint8(ShiftReg_1_ReadRegValue());
        }
        CyDelay(3000u);
    }
        
    /* Change Shift In value to '1' */
    ShiftIn_1_Write(1u);
    
    /* Right Shift loaded data (supplement with 1s) until 0xFF */
    for(j=0;j<2;j++)
    {
        CyDelay(3000u);
        LCD_1_ClearDisplay();
        LCD_1_Position(0u,0u);
        LCD_1_PrintString("8-bit ShiftReg:");
        
        for(i=0u; i<10u; i=i+3u)
        {
            Clock_1_Write(1u);
            Clock_1_Write(0u);
            
            /* Display shifted values */
            LCD_1_Position(1u,i);
            LCD_1_PrintHexUint8(ShiftReg_1_ReadRegValue());
        }
    }
    CyDelay(3000u);
    
    /* Store shift register value into the output FIFO */
    Store_1_Write(1u);
    Clock_1_Write(1u);
    Clock_1_Write(0u);
    Store_1_Write(0u);
    
    /* Read stored data from the output FIFO */
    LCD_1_Position(1u,0u);
    LCD_1_PrintString("Stored val:");
    LCD_1_PrintHexUint8(ShiftReg_1_ReadData());
        
    /* 16-bit Shift Register section */
    CyDelay(3000u);
    LCD_1_ClearDisplay();
    LCD_1_Position(0u,0u);
    LCD_1_PrintString("16-bit ShiftReg:");
    
    /* Write data to be shifted into the input FIFO */
    ShiftReg_2_WriteData(0xFFFFu);
    
    /* Load data from the input FIFO to the shift register */
    Load_2_Write(1u);
    Clock_2_Write(1u);
    Clock_2_Write(0u);
    Load_2_Write(0u);
    
    /* Right Shift loaded data (supplement with 0s) */
    for(j=0u; j<5u; j++)
    {
        LCD_1_ClearDisplay();
        LCD_1_Position(0u,0u);
        LCD_1_PrintString("16-bit ShiftReg:");
        
        for(i=0u; i<15u; i=i+5u)
        {
            Clock_2_Write(1u);
            Clock_2_Write(0u);
            
            /* Display shifted values */
            LCD_1_Position(1u,i);
            LCD_1_PrintHexUint16(ShiftReg_2_ReadRegValue());
        }
        CyDelay(3000u);
    }
    
    /* Display last shifted value */
    Clock_2_Write(1u);
    Clock_2_Write(0u);
    LCD_1_ClearDisplay();
    LCD_1_Position(0u,0u);
    LCD_1_PrintString("16-bit ShiftReg:");
    LCD_1_Position(1u,0u);
    LCD_1_PrintHexUint16(ShiftReg_2_ReadRegValue());
    
    CyDelay(3000u);
    
    /* Change Shift In value to '1' */
    ShiftIn_2_Write(1u);
    
    /* Right Shift '1' (until 0xFF) */
    for(j=0u; j<5; j++)
    {
        LCD_1_ClearDisplay();
        LCD_1_Position(0u,0u);
        LCD_1_PrintString("16-bit ShiftReg:");
        
        for(i=0u; i<15u; i=i+5u)
        {
            Clock_2_Write(1u);
            Clock_2_Write(0u);
            
            /* Display shifted values */
            LCD_1_Position(1u,i);
            LCD_1_PrintHexUint16(ShiftReg_2_ReadRegValue());
        }
        CyDelay(3000u);
    }
    
    /* Display last shifted value */
    Clock_2_Write(1u);
    Clock_2_Write(0u);
    LCD_1_ClearDisplay();
    LCD_1_Position(0u,0u);
    LCD_1_PrintString("16-bit ShiftReg:");
    LCD_1_Position(1u,0u);
    LCD_1_PrintHexUint16(ShiftReg_2_ReadRegValue());
    
    CyDelay(3000u);
    /* Store shift register value into the output FIFO */
    Store_2_Write(1u);
    Clock_2_Write(1u);
    Clock_2_Write(0u);
    Store_2_Write(0u);
    
    /* Read stored data from the output FIFO */
    LCD_1_Position(1u,0u);
    LCD_1_PrintString("Stored val:");
    LCD_1_PrintHexUint16(ShiftReg_2_ReadData());
    
    /* Display the message that example project execution is complete */
    CyDelay(3000u);
    LCD_1_ClearDisplay();
    LCD_1_Position(0u,0u);
    LCD_1_PrintString("ShiftReg example");
    LCD_1_Position(1u,0u);
    LCD_1_PrintString("is complete! ");
    
    while(1u){;}
}    


/* [] END OF FILE */
