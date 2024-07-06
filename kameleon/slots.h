/*  Benjamin DELPY `gentilkiwi`
    https://blog.gentilkiwi.com
    benjamin@gentilkiwi.com
    Licence : https://creativecommons.org/licenses/by/4.0/
*/
#pragma once
#include "board.h"

#if defined(SLOTS_ST25TB_SUPPORT_4K)
#define SLOTS_ST25TB_SECTORS_INTERNAL   0x83
#elif defined(SLOTS_ST25TB_SUPPORT_2K)
#define SLOTS_ST25TB_SECTORS_INTERNAL   0x43
#else
#define SLOTS_ST25TB_SECTORS_INTERNAL   0x13
#endif

#define SLOTS_ST25TB_INDEX_SYSTEM   (SLOTS_ST25TB_SECTORS_INTERNAL - 3)
#define SLOTS_ST25TB_INDEX_UID      (SLOTS_ST25TB_SECTORS_INTERNAL - 2)
#define SLOTS_ST25TB_INDEX_UID_2    (SLOTS_ST25TB_SECTORS_INTERNAL - 1)


/* SLOTS_ST25TB_PERSISTENT_DATA has attribute PERSISTENT in C file */
extern uint8_t SLOTS_ST25TB_PERSISTENT_DATA[SLOTS_ST25TB_COUNT][SLOTS_ST25TB_SECTORS_INTERNAL][4];
extern uint8_t SLOTS_ST25TB_Current[SLOTS_ST25TB_SECTORS_INTERNAL][4];

void SLOTS_Change(uint8_t index);
void SLOTS_Load(uint8_t index);
void SLOTS_Save(uint8_t index);

#define SLOTS_FIND_INVALID_INDEX    0xff
uint8_t SLOTS_FindByUID(uint8_t pui8Data[8]); // ret == SLOTS_FIND_INVALID_INDEX -> not found

#define SLOTS_Save_Current()        SLOTS_Save(Settings.CurrentSlot)
