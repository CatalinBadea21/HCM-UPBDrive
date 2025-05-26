/*
* File:        can_comm.c
* Description: Handles CAN communication between the main controller and external subsystems,
*              such as the ECU and ESC. Includes transmission of control commands and
*              reception of vehicle status data over the CAN bus. Also defines the CAN ISR.
*/

#include <include/can_comm.h>
#include <include/hybrid_mode.h>

/**********************************************************************************************************************
* Global variables
***********************************************************************************************************************/
volatile uint8_t CANisrFlag = 0u;
volatile vehicle_status_t car_state = {0, 0, 0, 0, 0, 0};

/**********************************************************************************************************************
* Functions
***********************************************************************************************************************/
/*
 * Function:    CAN_Transmit_To_ESC
 * Description: Sends a control command to the ESC (Electronic Speed Controller) over CAN.
 * Parameters:
 *   - driving_mode: Desired driving mode (e.g., normal, sport, reverse).
 *   - duty_cycle: Signed 8-bit duty cycle value to be mapped and transmitted (throttle/power).
 * Notes:
 *   - The duty cycle is converted to a 15-bit value using Map_Duty_Cycle().
 *   - Data is packed into CAN transmit buffer and sent via ESC control mailbox.
 */
void CAN_Transmit_To_ESC(uint8_t driving_mode, int8_t duty_cycle)
{
    volatile int16_t duty_cycle_15bit = Map_Duty_Cycle(duty_cycle);

    CAN_TX_DATA_BYTE1(CAN_TX_MAILBOX_ESC_control) = CMD_MODE;
    CAN_TX_DATA_BYTE2(CAN_TX_MAILBOX_ESC_control) = driving_mode;
    CAN_TX_DATA_BYTE3(CAN_TX_MAILBOX_ESC_control) = (duty_cycle_15bit >> 8) & 0x7F; // Dutycycle MSB; mask with 7 bits
    CAN_TX_DATA_BYTE4(CAN_TX_MAILBOX_ESC_control) = duty_cycle_15bit & 0xFF;        // Dutycycle LSB; mask with 8 bits

    CAN_SendMsgESC_control();
}

/*
 * Function:    CAN_Read_From_ECU
 * Description: Reads vehicle status data from the ECU (Engine Control Unit) if a CAN message
 *              has been received. Updates the global car_state structure.
 * Returns:
 *   - 1 if data was read and is within valid bounds.
 *   - 0 if no new message or invalid data (out of range).
 * Notes:
 *   - The function uses CANisrFlag to determine if a new message was received.
 *   - Performs range checks for RPM, gear, TPS, and VSS values.
 *   - Clears the CANisrFlag after successful read.
 */
uint8_t CAN_Read_From_ECU()
{
    if (CANisrFlag)
    {
        car_state.rpm = CAN_RX_DATA_BYTE1(CAN_RX_MAILBOX_ECU_receive) * RPM_MULTIPLIER;

        if (car_state.rpm > RPM_THRESHOLD)
            car_state.engine_state = 1;
        else
            car_state.engine_state = 0;

        car_state.gear = CAN_RX_DATA_BYTE2(CAN_RX_MAILBOX_ECU_receive);
        car_state.tps = CAN_RX_DATA_BYTE3(CAN_RX_MAILBOX_ECU_receive);
        car_state.vss = CAN_RX_DATA_BYTE4(CAN_RX_MAILBOX_ECU_receive);

        if (CAN_RX_DATA_BYTE5(CAN_RX_MAILBOX_ECU_receive) > BRAKE_THRESHOLD)
            car_state.brake_state = 1;
        else
            car_state.brake_state = 0;

        if ((car_state.rpm > MAX_RPM) || (car_state.gear > MAX_GEAR) || (car_state.tps > MAX_TPS) || (car_state.vss > MAX_VSS))
            return 0; // Values out of bounds, failed read from can
        
        CANisrFlag = 0; // Clear interrupt flag

        return 1;
    }
    else
        return 0;
}

/*
 * Function:    ISR_CAN
 * Description: Interrupt Service Routine triggered on CAN receive event.
 * Actions:
 *   - Clears the receive interrupt flag.
 *   - Sets the global CANisrFlag to signal that new data is available.
 *   - Acknowledges the received message in the ECU receive mailbox.
 */
CY_ISR(ISR_CAN)
{
    CAN_INT_SR_REG.byte[1u] = CAN_RX_MESSAGE_MASK; // Clear Receive Message flag
    CANisrFlag = 1u; // Set interrupt flag
    CAN_RX_ACK_MESSAGE(CAN_RX_MAILBOX_ECU_receive); // RX ACK
}

/* [] END OF FILE */