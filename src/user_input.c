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
 *   - Strategy index in the range [0–6], where each power-of-two input maps to a unique mode.
 * Notes:
 *   - Assumes only one selector bit is active at a time.
 *   - Defaults to 0 if input does not match a known pattern.
 */
uint8_t Read_Rotary_Switch()
{
    uint8_t coded_strategy, decoded_strategy;
    coded_strategy = Selector_In_Read() & 0x3F; // Mask to 6 bits

    switch (coded_strategy)
    {
        case 1:
            decoded_strategy = MANUAL_S;
            break;

        case 2:
            decoded_strategy = LAUNCH_S;
            break;

        case 4:
            decoded_strategy = AUTO_NOREGEN_S;
            break;

        case 8:
            decoded_strategy = AUTO_BRAKEREGEN_S;
            break;

        case 16:
            decoded_strategy = AUTO_ALLREGEN_S;
            break;

        case 32:
            decoded_strategy = TORQUE_FILL_S;
            break;

        default:
            decoded_strategy = FREEWHEEL_S;
    }

    return decoded_strategy;
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
    uint8_t boost_button_state = Boost_Button_Read();

    return boost_button_state;
}

/* [] END OF FILE */