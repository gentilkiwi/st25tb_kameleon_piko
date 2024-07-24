/*  Benjamin DELPY `gentilkiwi`
    https://blog.gentilkiwi.com
    benjamin@gentilkiwi.com
    Licence : https://creativecommons.org/licenses/by/4.0/
*/
#pragma once
#include "st25tb.h"

#define ST25TB_INITIATOR_TIMEOUT_INITIATE           20
#define ST25TB_INITIATOR_TIMEOUT_SELECT             10
#define ST25TB_INITIATOR_TIMEOUT_GENERIC            5
#define ST25TB_INITIATOR_DELAY_BEFORE_RETRY         250
#define ST25TB_INITIATOR_DELAY_WRITE_TIME_OTP       3
#define ST25TB_INITIATOR_DELAY_WRITE_TIME_EEPROM    5
#define ST25TB_INITIATOR_DELAY_WRITE_TIME_COUNTER   7

uint8_t ST25TB_Initiator_Initiate_Select_UID_C1_C2(uint8_t UID[8], uint8_t C1[4], uint8_t C2[4]);

uint8_t ST25TB_Initiator_Read_Card();
uint8_t ST25TB_Initiator_Write_Card(uint8_t data[SLOTS_ST25TB_SECTORS_INTERNAL][4]);
#define ST25TB_Initiator_Write_Current_Card()   ST25TB_Initiator_Write_Card(SLOTS_ST25TB_Current)

uint8_t ST25TB_Initiator_Initiate_Select_Read_Block(const uint8_t ui8BlockIdx, uint8_t pui8Data[4]);
uint8_t ST25TB_Initiator_Initiate_Select_ultra_Write_Block(const uint8_t ui8BlockIdx, const uint8_t pui8Data[4]);
