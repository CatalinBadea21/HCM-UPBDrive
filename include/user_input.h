/*
* File:        user_input.h
* Description: Header file for reading user input signals in the HCM project. Provides function 
*              declarations for reading the rotary switch, binary selector, and boost button, 
*              which are used to determine the active hybrid driving strategy. Also defines bit 
*              masks used for decoding selector inputs.
*/

#ifndef USER_INPUT_H
#define USER_INPUT_H

/**********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include <project.h>
#include <Selector_In.h>
#include <Boost_Button.h>

/**********************************************************************************************************************
* Defines
***********************************************************************************************************************/
#define SELECTOR_BIT0 1u
#define SELECTOR_BIT1 2u
#define SELECTOR_BIT2 4u
#define SELECTOR_BIT3 8u
#define SELECTOR_BIT4 16u
#define SELECTOR_BIT5 32u

/**********************************************************************************************************************
* Function declarations
***********************************************************************************************************************/
uint8_t Read_Rotary_Switch();
uint8_t Read_Binary_Selector();
uint8_t Read_Strategy_Selector();
uint8_t Read_Boost_Button();

#endif /* USER_INPUT_H */
/* [] */