#ifndef USER_INPUT_H
#define USER_INPUT_H

/*
* File:        user_input.h
* Description: TBC
*/

// Includes
#include <project.h>
#include <Selector_In.h>
#include <Boost_Button.h>

// Macros, typedefs, and structs
#define SELECTOR_BIT0 1u
#define SELECTOR_BIT1 2u
#define SELECTOR_BIT2 4u
#define SELECTOR_BIT3 8u
#define SELECTOR_BIT4 16u
#define SELECTOR_BIT5 32u

// Function declarations
uint8_t Read_Rotary_Switch();
uint8_t Read_Binary_Selector();
uint8_t Read_Strategy_Selector();
uint8_t Read_Boost_Button();

#endif /* USER_INPUT_H */
/* [] */