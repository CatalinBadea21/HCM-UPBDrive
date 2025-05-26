/*******************************************************************************
* File Name: Selector_In.h  
* Version 2.20
*
* Description:
*  This file contains the Alias definitions for Per-Pin APIs in cypins.h. 
*  Information on using these APIs can be found in the System Reference Guide.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_Selector_In_ALIASES_H) /* Pins Selector_In_ALIASES_H */
#define CY_PINS_Selector_In_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"


/***************************************
*              Constants        
***************************************/
#define Selector_In_0			(Selector_In__0__PC)
#define Selector_In_0_INTR	((uint16)((uint16)0x0001u << Selector_In__0__SHIFT))

#define Selector_In_1			(Selector_In__1__PC)
#define Selector_In_1_INTR	((uint16)((uint16)0x0001u << Selector_In__1__SHIFT))

#define Selector_In_2			(Selector_In__2__PC)
#define Selector_In_2_INTR	((uint16)((uint16)0x0001u << Selector_In__2__SHIFT))

#define Selector_In_3			(Selector_In__3__PC)
#define Selector_In_3_INTR	((uint16)((uint16)0x0001u << Selector_In__3__SHIFT))

#define Selector_In_4			(Selector_In__4__PC)
#define Selector_In_4_INTR	((uint16)((uint16)0x0001u << Selector_In__4__SHIFT))

#define Selector_In_5			(Selector_In__5__PC)
#define Selector_In_5_INTR	((uint16)((uint16)0x0001u << Selector_In__5__SHIFT))

#define Selector_In_INTR_ALL	 ((uint16)(Selector_In_0_INTR| Selector_In_1_INTR| Selector_In_2_INTR| Selector_In_3_INTR| Selector_In_4_INTR| Selector_In_5_INTR))

#endif /* End Pins Selector_In_ALIASES_H */


/* [] END OF FILE */
