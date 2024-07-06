/*  Benjamin DELPY `gentilkiwi`
    https://blog.gentilkiwi.com
    benjamin@gentilkiwi.com
    Licence : https://creativecommons.org/licenses/by/4.0/
*/
#include "st25tb_initiator.h"

uint8_t ST25TB_Initiator_ValidateAndGetNBSectorsFromUID(const uint8_t *pui8UID);
uint8_t ST25TB_Initiator_CMD_Initiate(uint8_t *pui8ChipId);
uint8_t ST25TB_Initiator_CMD_Select(const uint8_t ui8ChipId);
uint8_t ST25TB_Initiator_CMD_Get_Uid(uint8_t pui8Data[8]);
uint8_t ST25TB_Initiator_CMD_CONFIRMED_Get_Uid(uint8_t pui8Data[8]);
uint8_t ST25TB_Initiator_CMD_Completion();
uint8_t ST25TB_Initiator_CMD_Read_Block(const uint8_t ui8BlockIdx, uint8_t pui8Data[4]);
uint8_t ST25TB_Initiator_CMD_CONFIRMED_Read_Block(const uint8_t ui8BlockIdx, uint8_t pui8Data[4]);
uint8_t ST25TB_Initiator_CMD_Write_Block(const uint8_t ui8BlockIdx, const uint8_t pui8Data[4]);
uint8_t ST25TB_Initiator_CMD_CONFIRMED_Write_Block(const uint8_t ui8BlockIdx, const uint8_t pui8Data[4]);

uint8_t ST25TB_Initiator_Initiate_Select_UID(uint8_t pui8Data[8])
{
    uint8_t BP_IrqSource, ui8ChipId;

    BP_IrqSource = ST25TB_Initiator_CMD_Initiate(&ui8ChipId);
    if(BP_IrqSource == IRQ_SOURCE_NONE)
    {
        BP_IrqSource = ST25TB_Initiator_CMD_Select(ui8ChipId);
        if(BP_IrqSource == IRQ_SOURCE_NONE)
        {
            BP_IrqSource = ST25TB_Initiator_CMD_CONFIRMED_Get_Uid(pui8Data);
        }

        ST25TB_Initiator_CMD_Completion();
    }

    return BP_IrqSource;
}

uint8_t ST25TB_Initiator_Initiate_Select_Block(uint8_t index, uint8_t pui8Data[4])
{
    uint8_t BP_IrqSource, ui8ChipId;

    BP_IrqSource = ST25TB_Initiator_CMD_Initiate(&ui8ChipId);
    if(BP_IrqSource == IRQ_SOURCE_NONE)
    {
        BP_IrqSource = ST25TB_Initiator_CMD_Select(ui8ChipId);
        if(BP_IrqSource == IRQ_SOURCE_NONE)
        {
            BP_IrqSource = ST25TB_Initiator_CMD_CONFIRMED_Read_Block(index, pui8Data);
        }

        ST25TB_Initiator_CMD_Completion();
    }

    return BP_IrqSource;
}

uint8_t ST25TB_Initiator_Read_Card()
{
    uint8_t BP_IrqSource, ui8ChipId, i, nbSectors;

    BP_IrqSource = ST25TB_Initiator_CMD_Initiate(&ui8ChipId);
    if(BP_IrqSource == IRQ_SOURCE_NONE)
    {
        BP_IrqSource = ST25TB_Initiator_CMD_Select(ui8ChipId);
        if(BP_IrqSource == IRQ_SOURCE_NONE)
        {
            BP_IrqSource = ST25TB_Initiator_CMD_CONFIRMED_Get_Uid((uint8_t *) SLOTS_ST25TB_Current[SLOTS_ST25TB_INDEX_UID]);
            if(BP_IrqSource == IRQ_SOURCE_NONE)
            {
                nbSectors = ST25TB_Initiator_ValidateAndGetNBSectorsFromUID((uint8_t *) SLOTS_ST25TB_Current[SLOTS_ST25TB_INDEX_UID]);
                if(nbSectors)
                {
                    for(i = 0; (i < nbSectors) && (BP_IrqSource == IRQ_SOURCE_NONE); i++)
                    {
                        BP_IrqSource = ST25TB_Initiator_CMD_CONFIRMED_Read_Block(i, SLOTS_ST25TB_Current[i]);
                    }

                    if(BP_IrqSource == IRQ_SOURCE_NONE)
                    {
                        BP_IrqSource = ST25TB_Initiator_CMD_CONFIRMED_Read_Block(ST25TB_IDX_SYSTEM, SLOTS_ST25TB_Current[SLOTS_ST25TB_INDEX_SYSTEM]);
                    }
                }
                else
                {
                    BP_IrqSource = IRQ_SOURCE_ST25TB_PROTOCOL_ERR;
                }
            }
        }
        ST25TB_Initiator_CMD_Completion();
    }

    return BP_IrqSource;
}

uint8_t ST25TB_Initiator_Write_Card()
{
    uint8_t BP_IrqSource, ui8ChipId, ui8UID[8], i, nbSectors;

    BP_IrqSource = ST25TB_Initiator_CMD_Initiate(&ui8ChipId);
    if(BP_IrqSource == IRQ_SOURCE_NONE)
    {
        BP_IrqSource = ST25TB_Initiator_CMD_Select(ui8ChipId);
        if(BP_IrqSource == IRQ_SOURCE_NONE)
        {
            BP_IrqSource = ST25TB_Initiator_CMD_Get_Uid(ui8UID);
            if(BP_IrqSource == IRQ_SOURCE_NONE)
            {
                nbSectors = ST25TB_Initiator_ValidateAndGetNBSectorsFromUID(ui8UID);
                if (nbSectors && ((*(uint64_t*) ui8UID) == (*(uint64_t*) SLOTS_ST25TB_Current[SLOTS_ST25TB_INDEX_UID]))) // implicit check of same size/techno :')
                {
                    for(i = 0; (i < nbSectors) && (BP_IrqSource == IRQ_SOURCE_NONE); i++)
                    {
#if defined(ST25TB_DO_NOT_WRITE_DANGEROUS_SECTOR)
                        if((i == ST25TB_IDX_COUNTER1) || (i == ST25TB_IDX_COUNTER2))
                        {
                            continue;
                        }
#endif
                        BP_IrqSource = ST25TB_Initiator_CMD_CONFIRMED_Write_Block(i, SLOTS_ST25TB_Current[i]);
                    }
#if !defined(ST25TB_DO_NOT_WRITE_DANGEROUS_SECTOR)
                    if(BP_IrqSource == IRQ_SOURCE_NONE)
                    {
                        BP_IrqSource = ST25TB_Initiator_CMD_CONFIRMED_Write_Block(ST25TB_IDX_SYSTEM, SLOTS_ST25TB_Current[SLOTS_ST25TB_INDEX_SYSTEM]);
                    }
#endif
                }
                else
                {
                    BP_IrqSource = IRQ_SOURCE_ST25TB_PROTOCOL_ERR;
                }
            }
        }
        ST25TB_Initiator_CMD_Completion();
    }

    return BP_IrqSource;
}

uint8_t ST25TB_Initiator_SendRecv(const uint8_t *pui8Payload, const uint8_t ui8Length, uint16_t timeout_ms)
{
    uint8_t ret, TRF7970A_irqStatus;
    if(ST25TB_Send(pui8Payload, ui8Length))
    {
        ret = IRQ_Wait_for_SW1_or_SW2_or_TRF_or_Timeout(&TRF7970A_irqStatus, timeout_ms);
        if(!(ret & (IRQ_SOURCE_SW1 | IRQ_SOURCE_SW2 | IRQ_SOURCE_TIMER)))
        {
            if (ret & IRQ_SOURCE_TRF7970A)
            {
                if (ST25TB_Recv(true, TRF7970A_irqStatus))
                {
                    ret = IRQ_SOURCE_NONE;
                }
            }
        }
    }
    else
    {
        ret = IRQ_SOURCE_TRF7970A;
    }

    return ret;
}

uint8_t ST25TB_Initiator_ValidateAndGetNBSectorsFromUID(const uint8_t *pui8UID)
{
    uint8_t ret = 0, chipId;

    if(pui8UID[7] == 0xd0) // SR*/ST25TB*
    {
        chipId = pui8UID[5];

        if((chipId == 0x33) || (chipId == 0x1b)) // ST25TB512-AT, ST25TB512-AC
        {
            ret = 16;
        }
        else if(chipId == 0x3f) // ST25TB02K
        {
            ret = 64;
        }
        else if(chipId == 0x1f) // ST25TB04K
        {
            ret = 128;
        }
        else
        {
            chipId >>= 2; // let's go legacy! (but not SR176)

            if((chipId == 12) || (chipId == 6) || (chipId == 4)) // SRT512, SRI512, SRIX512
            {
                ret = 16;
            }
            else if(chipId == 15) // SRI2K
            {
                ret = 64;
            }
            else if((chipId == 7) || (chipId == 3)) // SRI4K, SRIX4K
            {
                ret = 128;
            }
        }
    }

    return ret;
}

const uint8_t ST25TB_Initiator_CMD_Initiate_data[] = { ST25TB_CMD_INITIATE, 0x00 };
uint8_t ST25TB_Initiator_CMD_Initiate(uint8_t *pui8ChipId)
{
    uint8_t ret;

    ret = ST25TB_Initiator_SendRecv(ST25TB_Initiator_CMD_Initiate_data, sizeof(ST25TB_Initiator_CMD_Initiate_data), ST25TB_INITIATOR_TIMEOUT_INITIATE);
    if (ret == IRQ_SOURCE_NONE)
    {
        if (g_ui8_cbST25TB_Buffer == 1)
        {
            *pui8ChipId = *g_ui8_ST25TB_Buffer;
            ret = IRQ_SOURCE_NONE;
        }
        else
        {
            ret = IRQ_SOURCE_ST25TB_PROTOCOL_ERR;
        }
    }

    return ret;
}

uint8_t ST25TB_Initiator_CMD_Select(const uint8_t ui8ChipId)
{
    uint8_t ret, ST25TB_Initiator_CMD_Select_data[] = { ST25TB_CMD_SELECT, ui8ChipId };

    ret = ST25TB_Initiator_SendRecv(ST25TB_Initiator_CMD_Select_data, sizeof(ST25TB_Initiator_CMD_Select_data), ST25TB_INITIATOR_TIMEOUT_SELECT);
    if (ret == IRQ_SOURCE_NONE)
    {
        if((g_ui8_cbST25TB_Buffer == 1) && (ui8ChipId == g_ui8_ST25TB_Buffer[0]))
        {
            ret = IRQ_SOURCE_NONE;
        }
        else
        {
            ret = IRQ_SOURCE_ST25TB_PROTOCOL_ERR;
        }
    }

    return ret;
}

const uint8_t ST25TB_Initiator_CMD_Get_Uid_data[] = { ST25TB_CMD_GET_UID };
uint8_t ST25TB_Initiator_CMD_Get_Uid(uint8_t pui8Data[8])
{
    uint8_t ret;

    ret = ST25TB_Initiator_SendRecv(ST25TB_Initiator_CMD_Get_Uid_data, sizeof(ST25TB_Initiator_CMD_Get_Uid_data), ST25TB_INITIATOR_TIMEOUT_GENERIC);
    if (ret == IRQ_SOURCE_NONE)
    {
        if (g_ui8_cbST25TB_Buffer == sizeof(uint64_t))
        {
            *(uint64_t *) pui8Data = *(uint64_t *) g_ui8_ST25TB_Buffer;
            ret = IRQ_SOURCE_NONE;
            TIMER_delay_Milliseconds(1);
        }
        else
        {
            ret = IRQ_SOURCE_ST25TB_PROTOCOL_ERR;
        }
    }

    return ret;
}

uint8_t ST25TB_Initiator_CMD_CONFIRMED_Get_Uid(uint8_t pui8Data[8])
{
    uint8_t ret, buffer[8];

    ret = ST25TB_Initiator_CMD_Get_Uid(buffer);
    if (ret == IRQ_SOURCE_NONE)
    {
        ret = ST25TB_Initiator_CMD_Get_Uid(pui8Data);
        if (ret == IRQ_SOURCE_NONE)
        {
            if ((*(uint64_t*) buffer) != (*(uint64_t*) pui8Data))
            {
                ret = IRQ_SOURCE_ST25TB_PROTOCOL_ERR;
            }
        }
    }

    return ret;
}

const uint8_t ST25TB_Initiator_CMD_Completion_data[] = { ST25TB_CMD_COMPLETION };
uint8_t ST25TB_Initiator_CMD_Completion()
{
    uint8_t ret;

    if(ST25TB_Send(ST25TB_Initiator_CMD_Completion_data, sizeof(ST25TB_Initiator_CMD_Completion_data)))
    {
        ret = IRQ_SOURCE_NONE;
    }
    else
    {
        ret = IRQ_SOURCE_TRF7970A;
    }

    return ret;
}

uint8_t ST25TB_Initiator_CMD_Read_Block(const uint8_t ui8BlockIdx, uint8_t pui8Data[4])
{
    uint8_t ret, ST25TB_Initiator_CMD_Read_Block_data[] = { ST25TB_CMD_READ_BLOCK, ui8BlockIdx };

    ret = ST25TB_Initiator_SendRecv(ST25TB_Initiator_CMD_Read_Block_data, sizeof(ST25TB_Initiator_CMD_Read_Block_data), ST25TB_INITIATOR_TIMEOUT_GENERIC);
    if (ret == IRQ_SOURCE_NONE)
    {
        if(g_ui8_cbST25TB_Buffer == sizeof(uint32_t))
        {
            *((uint32_t *) pui8Data) = *(uint32_t *) g_ui8_ST25TB_Buffer;
            ret = IRQ_SOURCE_NONE;
            TIMER_delay_Milliseconds(4);
        }
        else
        {
            ret = IRQ_SOURCE_ST25TB_PROTOCOL_ERR;
        }
    }

    return ret;
}

uint8_t ST25TB_Initiator_CMD_CONFIRMED_Read_Block(const uint8_t ui8BlockIdx, uint8_t pui8Data[4])
{
    uint8_t ret, buffer[4];

    ret = ST25TB_Initiator_CMD_Read_Block(ui8BlockIdx, buffer);
    if (ret == IRQ_SOURCE_NONE)
    {
        ret = ST25TB_Initiator_CMD_Read_Block(ui8BlockIdx, pui8Data);
        if (ret == IRQ_SOURCE_NONE)
        {
            if ((*(uint32_t*) buffer) != (*(uint32_t*) pui8Data))
            {
                ret = IRQ_SOURCE_ST25TB_PROTOCOL_ERR;
            }
        }
    }

    return ret;
}

uint8_t ST25TB_Initiator_CMD_Write_Block(const uint8_t ui8BlockIdx, const uint8_t pui8Data[4])
{
    uint8_t ret, ST25TB_Initiator_CMD_Write_Block_data[2 + 4] = { ST25TB_CMD_WRITE_BLOCK, ui8BlockIdx };

    *(uint32_t *) (ST25TB_Initiator_CMD_Write_Block_data + 2) = *((uint32_t *) pui8Data);
    if (ST25TB_Send(ST25TB_Initiator_CMD_Write_Block_data, sizeof(ST25TB_Initiator_CMD_Write_Block_data)))
    {
        ret = IRQ_SOURCE_NONE;
        TIMER_delay_Milliseconds(((ui8BlockIdx == ST25TB_IDX_COUNTER1) || (ui8BlockIdx == ST25TB_IDX_COUNTER2)) ? ST25TB_INITIATOR_DELAY_WRITE_TIME_COUNTER : ST25TB_INITIATOR_DELAY_WRITE_TIME_EEPROM);
    }
    else
    {
        ret = IRQ_SOURCE_TRF7970A;
    }

    return ret;
}

uint8_t ST25TB_Initiator_CMD_CONFIRMED_Write_Block(const uint8_t ui8BlockIdx, const uint8_t pui8Data[4])
{
    uint8_t ret, buffer[4];

    ret = ST25TB_Initiator_CMD_Write_Block(ui8BlockIdx, pui8Data);
    if (ret == IRQ_SOURCE_NONE)
    {
        ret = ST25TB_Initiator_CMD_Read_Block(ui8BlockIdx, buffer);
        if (ret == IRQ_SOURCE_NONE)
        {
            if ((*(uint32_t*) buffer) != (*(uint32_t*) pui8Data))
            {
                ret = IRQ_SOURCE_ST25TB_PROTOCOL_ERR;
            }
        }
    }

    return ret;
}
