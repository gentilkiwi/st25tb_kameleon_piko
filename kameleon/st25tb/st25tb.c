/*  Benjamin DELPY `gentilkiwi`
    https://blog.gentilkiwi.com
    benjamin@gentilkiwi.com
    Licence : https://creativecommons.org/licenses/by/4.0/
*/
#include "st25tb.h"

uint8_t g_ui8_ST25TB_Buffer[0x10];
uint8_t g_ui8_cbST25TB_Buffer = 0;

bool __time_critical_func(ST25TB_Recv)(bool bIsinitiator, uint8_t irqProvided)
{
    bool status = false;
    if (irqProvided & TRF79X0_IRQ_STATUS_SRX)
    {
        if (bIsinitiator || (TRF7970A_SPI_Read_SingleRegister(TRF79X0_NFC_TARGET_PROTOCOL_REG) == TRF79X0_NFC_TARGET_PROTOCOL_14B_COMMAND))
        {
            g_ui8_cbST25TB_Buffer = TRF7970A_SPI_Read_SingleRegister(TRF79X0_FIFO_STATUS_REG);
            if (g_ui8_cbST25TB_Buffer && !(g_ui8_cbST25TB_Buffer & TRF79X0_FIFO_OVERFLOW))
            {
                TRF7970A_SPI_Read_ContinuousRegister(TRF79X0_FIFO_REG, g_ui8_ST25TB_Buffer, g_ui8_cbST25TB_Buffer);
                status = true;
            }
        }
    }

    return status;
}

bool __time_critical_func(ST25TB_Send)(const uint8_t *pcbData, const uint8_t cbData)
{
    bool status;

    TRF7970A_SPI_Write_Packet(pcbData, cbData);
    if(TRF7970A_SPI_waitIrq() & TRF79X0_IRQ_STATUS_TX)
    {
        TRF7970A_SPI_DirectCommand(TRF79X0_RESET_FIFO_CMD);
        status = true;
    }
    else
    {
        status = false;
    }

    return status;
}

const uint8_t ST25TB_TRF7970A_Mode_Initiator[] = {
    MK_WS(TRF79X0_ISO_CONTROL_REG), 0x0c,
    MK_WS(TRF79X0_RX_SPECIAL_SETTINGS_REG), 0xf0,
    MK_WS(TRF79X0_CHIP_STATUS_CTRL_REG), TRF79X0_STATUS_CTRL_RF_ON,
    MK_WS(TRF79X0_MODULATOR_CONTROL_REG), TRF79X0_MOD_CTRL_MOD_ASK_10,
    MK_WS(TRF79X0_REGULATOR_CONTROL_REG), TRF79X0_REGULATOR_CTRL_VRS_2_7V,
    MK_WS(TRF79X0_FIFO_IRQ_LEVEL), 0x0f,
    MK_WS(TRF79X0_NFC_LOW_DETECTION_LEVEL), 0x03,
    MK_WS(TRF79X0_NFC_TARGET_LEVEL_REG), 0x07,
    MK_RS(TRF79X0_IRQ_STATUS_REG), 0x00,
};

const uint8_t ST25TB_TRF7970A_Mode_Target[] = {
    MK_WS(TRF79X0_ISO_CONTROL_REG), 0x25,
    MK_WS(TRF79X0_RX_SPECIAL_SETTINGS_REG), 0xf0,
    MK_WS(TRF79X0_CHIP_STATUS_CTRL_REG), TRF79X0_STATUS_CTRL_RF_ON,
    MK_WS(TRF79X0_MODULATOR_CONTROL_REG), TRF79X0_MOD_CTRL_MOD_ASK_10,
    MK_WS(TRF79X0_REGULATOR_CONTROL_REG), TRF79X0_REGULATOR_CTRL_VRS_2_7V,
    MK_WS(TRF79X0_FIFO_IRQ_LEVEL), 0x0f,
    MK_WS(TRF79X0_NFC_LOW_DETECTION_LEVEL), 0x03,
    MK_WS(TRF79X0_NFC_TARGET_LEVEL_REG), 0x07,
    MK_RS(TRF79X0_IRQ_STATUS_REG), 0x00,
};

void ST25TB_TRF7970A_Mode(bool bIsInitiator)
{
    TRF_IRQ_DISABLE();

    TRF7970A_SPI_DirectCommand(TRF79X0_SOFT_INIT_CMD);
    __no_operation();
    TRF7970A_SPI_DirectCommand(TRF79X0_IDLE_CMD);
    TIMER_delay_Milliseconds(2);

    if(bIsInitiator)
    {
        TRF7970A_SPI_Send_raw(ST25TB_TRF7970A_Mode_Initiator, sizeof(ST25TB_TRF7970A_Mode_Initiator));
    }
    else
    {
        TRF7970A_SPI_Send_raw(ST25TB_TRF7970A_Mode_Target, sizeof(ST25TB_TRF7970A_Mode_Target)); // RUN_DECODERS moved to program part
    }

    TIMER_delay_Milliseconds(2);
    TRF_IRQ_ENABLE();
}
