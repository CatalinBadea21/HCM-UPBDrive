/*
* File:        can_comm.c
* Description: TBC
*/

#include <include/can_comm.h>

volatile uint8_t CANisrFlag = 0u;

vehicle_status_t car_state;

void CAN_Transmit_To_ESC()
{
    CAN_TX_DATA_BYTE1(CAN_TX_MAILBOX_ESC_control) = CMD_MODE;
    CAN_TX_DATA_BYTE2(CAN_TX_MAILBOX_ESC_control) = 3u; //DRIVING MODE (0 - fw, 2 - curr, 8 - brk)
    CAN_TX_DATA_BYTE3(CAN_TX_MAILBOX_ESC_control) = 0x0C; //duty cycle - first byte
    CAN_TX_DATA_BYTE4(CAN_TX_MAILBOX_ESC_control) = 0xCC; //duty cycle - 2nd byte
    CAN_SendMsgESC_control();
}

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

        if ((car_state.rpm > MAX_RPM) || (car_state.gear > 5) || (car_state.tps > 100) || (car_state.vss > 130))
            return 0; // Values out of bounds, failed read from can
        
        CANisrFlag = 0; // Clear interrupt flag

        return 1;
    }
    else
        return 0;
}

CY_ISR(ISR_CAN)
{
    CAN_INT_SR_REG.byte[1u] = CAN_RX_MESSAGE_MASK; // Clear Receive Message flag
    CANisrFlag = 1u; // Set interrupt flag
    CAN_RX_ACK_MESSAGE(CAN_RX_MAILBOX_ECU_receive); // RX ACK
}

/* [] END OF FILE */