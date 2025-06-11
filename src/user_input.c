/*
* File:        user_input.c
* Description: Implements functions for reading and decoding user input from the binary selector,
*              rotary switch, and boost button. These inputs determine the hybrid strategy to be
*              applied based on driver intent. Includes bitwise decoding logic to convert
*              digital pin states into strategy indices.
*/

#include <include/user_input.h>

/**********************************************************************************************************************
* Functions
***********************************************************************************************************************/

/*
 * Function:    Read_Rotary_Switch
 * Description: Decodes a single-active-bit selector input from a rotary switch.
 * Returns:
 *   - Strategy index in the range [0–5], where each power-of-two input maps to a unique mode.
 * Notes:
 *   - Assumes only one selector bit is active at a time.
 *   - Defaults to 0 if input does not match a known pattern.
 */
uint8_t Read_Rotary_Switch()
{
    uint8_t selector_stragy;
    
    // Read each pin individually
    if (Selector_In_1_Read() == ACTIVE_LOW)
    {
        selector_stragy = MANUAL_S;
    }
    else if (Selector_In_2_Read() == ACTIVE_LOW)
    {
        selector_stragy = LAUNCH_S;
    }
    else if (Selector_In_3_Read() == ACTIVE_LOW)
    {
        selector_stragy = AUTO_NOREGEN_S;
    }
    else if (Selector_In_4_Read() == ACTIVE_LOW)
    {
        selector_stragy = AUTO_BRAKEREGEN_S;
    }
    else if (Selector_In_5_Read() == ACTIVE_LOW)
    {
        selector_stragy = AUTO_ALLREGEN_S;
    }
    else // No strategy selected or out of bounds
    {
        selector_stragy = FREEWHEEL_S;
    }

    return selector_stragy;
}

/*
 * Function:    Read_Boost_Button
 * Description: Reads the state of the boost button.
 * Returns:
 *   - 1 if the boost button is pressed.
 *   - 0 otherwise.
 */
uint8_t Read_Boost_Button()
{
    // Read pin and invert logic due to active-low configuration
    uint8_t boost_button_state = (uint8_t)(!Boost_Button_Read());

    return boost_button_state;
}

/* [] END OF FILE */