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
 * Function:    Read_Binary_Selector
 * Description: Reads the strategy selector pins as a binary-encoded value and validates
 *              it against the maximum allowed strategy index.
 * Returns:
 *   - Strategy index in the range [0–6], or 0 if out-of-bounds.
 * Notes:
 *   - This function treats the selector value as a direct binary code.
 */
uint8_t Read_Binary_Selector()
{
    uint8_t decoded_strategy;
    uint8_t coded_strategy = Read_Strategy_Selector();

    if (coded_strategy > 6) // Out of bounds
        decoded_strategy = 0;
    else
        decoded_strategy = coded_strategy;

    return decoded_strategy;
}

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
    uint8_t decoded_strategy;
    uint8_t coded_strategy = Read_Strategy_Selector();

    switch (coded_strategy)
    {
        case 1:
            decoded_strategy = 1;
            break;

        case 2:
            decoded_strategy = 2;
            break;

        case 4:
            decoded_strategy = 3;
            break;

        case 8:
            decoded_strategy = 4;
            break;

        case 16:
            decoded_strategy = 5;
            break;

        case 32:
            decoded_strategy = 6;
            break;

        default:
            decoded_strategy = 0;
    }

    return decoded_strategy;
}

/*
 * Function:    Read_Strategy_Selector
 * Description: Reads and decodes the 6-bit strategy selector input from digital pins.
 * Returns:
 *   - An unsigned 6-bit value (range 0–63) representing the user-selected strategy.
 */
uint8_t Read_Strategy_Selector()
{
    uint8_t selected_strategy;
    selected_strategy = Selector_In_Read() & 0x3F; // Mask to 6 bits
    
    return selected_strategy;
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