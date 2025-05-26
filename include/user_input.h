#ifndef USER_INPUT_H
#define USER_INPUT_H

/*
* File:        user_input.h
* Description: TBC
*/

// Includes
#include <project.h>

// Macros, typedefs, and structs
#define SELECTOR_BIT0 1u
#define SELECTOR_BIT1 2u
#define SELECTOR_BIT2 4u
#define SELECTOR_BIT3 8u
#define SELECTOR_BIT4 16u

// Function declarations
uint8_t Binary_Selector_Used(uint8_t coded_strategy);
uint8_t Rotary_Switch_Used(uint8_t coded_strategy);
uint8_t Read_Strategy_Selector();
uint8_t Read_Boost_Button();

#endif /* USER_INPUT_H */
/* [] */