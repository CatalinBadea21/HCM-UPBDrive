/*
* File:        user_input.c
* Description: TBC
*/

#include <include/user_input.h>

uint8_t Read_Binary_Selector()
{
    uint8_t decoded_strategy;
    uint8_t coded_strategy = Read_Strategy_Selector();

    if (coded_strategy > 6) // if out of bounds
        decoded_strategy = 0;
    else
        decoded_strategy = coded_strategy;

    return decoded_strategy;
}

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

uint8_t Read_Strategy_Selector()
{
    uint8_t selected_strategy;
    uint8_t input_value = Selector_In_Read();
    uint8_t selector_array[7];

    selector_array[0] = (input_value >> 0) & 0x01;
    selector_array[1] = (input_value >> 1) & 0x01;
    selector_array[2] = (input_value >> 2) & 0x01;
    selector_array[3] = (input_value >> 3) & 0x01;
    selector_array[4] = (input_value >> 4) & 0x01;
    selector_array[5] = (input_value >> 5) & 0x01;

    // convert selector value to decimal
    selected_strategy = \
        SELECTOR_BIT0 * selector_array[0] + \
        SELECTOR_BIT1 * selector_array[1] + \
        SELECTOR_BIT2 * selector_array[2] + \
        SELECTOR_BIT3 * selector_array[3] + \
        SELECTOR_BIT4 * selector_array[4] + \
        SELECTOR_BIT5 * selector_array[5];

    return selected_strategy;
}

uint8_t Read_Boost_Button()
{
    uint8_t boost_button_state = Boost_Button_Read();

    return boost_button_state;
}

/* [] END OF FILE */