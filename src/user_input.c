/*
* File:        user_input.c
* Description: TBC
*/

#include <include/user_input.h>

uint8_t Binary_Selector_Used(uint8_t coded_strategy)
{
    uint8_t decoded_strategy;

    if (coded_strategy > 8) // if out of bounds
        decoded_strategy = 0;
    else
        decoded_strategy = coded_strategy;

    return decoded_strategy;
}

uint8_t Rotary_Switch_Used(uint8_t coded_strategy)
{
    uint8_t decoded_strategy;

    switch (coded_strategy)
    {
        case 0:
            decoded_strategy = 0;
            break;

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

        default:
            decoded_strategy = 0;
    }

    return decoded_strategy;
}

uint8_t Read_Strategy_Selector()
{
    uint8_t selected_strategy;

    // convert selector value to decimal
    selected_strategy = \
        SELECTOR_BIT0 * CyPins_ReadPin(Selector_In_0) + \
        SELECTOR_BIT1 * CyPins_ReadPin(Selector_In_1) + \
        SELECTOR_BIT2 * CyPins_ReadPin(Selector_In_2) + \
        SELECTOR_BIT3 * CyPins_ReadPin(Selector_In_3) + \
        SELECTOR_BIT4 * CyPins_ReadPin(Selector_In_4);

    return selected_strategy;
}

uint8_t Read_Boost_Button()
{
    uint8_t boost_button_state = CyPins_ReadPin(Boost_Button_0);

    return boost_button_state;
}

/* [] END OF FILE */