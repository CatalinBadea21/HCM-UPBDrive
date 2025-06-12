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
#include <Boost_Button.h>

/**********************************************************************************************************************
* Defines
***********************************************************************************************************************/
#define ACTIVE_LOW 0u
#define ACTIVE_HIGH 1u
/* __________ DRIVING STRATEGIES __________ */
#define FREEWHEEL_S 0u
#define MANUAL_S 1u
#define LAUNCH_S 2u
#define AUTO_NOREGEN_S 3u
#define AUTO_BRAKEREGEN_S 4u
#define AUTO_ALLREGEN_S 5u
    
/**********************************************************************************************************************
* Function declarations
***********************************************************************************************************************/
uint8_t Read_Rotary_Switch();
uint8_t Read_Boost_Button();

#endif /* USER_INPUT_H */
/* [] */