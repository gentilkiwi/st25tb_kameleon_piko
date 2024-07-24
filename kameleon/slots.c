/*  Benjamin DELPY `gentilkiwi`
    https://blog.gentilkiwi.com
    benjamin@gentilkiwi.com
    Licence : https://creativecommons.org/licenses/by/4.0/
*/
#include "slots.h"

void SLOTS_Change(uint8_t index)
{
    SLOTS_Load(index);
    if(index != FlashStoredData.CurrentSlot)
    {
        flash_update_CurrentSlot(index);
    }
#if SLOTS_ST25TB_COUNT > 8
    LEDS_SLOTS_Bitmask(index);
#else
    LED_Slot(index);
#endif
 }

void SLOTS_Load(uint8_t index)
{
    memcpy(SLOTS_ST25TB_Current, FlashStoredData.Slots[index], sizeof(FlashStoredData.Slots[index]));
}

void SLOTS_Save(uint8_t index)
{
    flash_update_Slot(index, SLOTS_ST25TB_Current);
}

uint8_t SLOTS_FindByUID(uint8_t pui8Data[8]) // ret == SLOTS_FIND_INVALID_INDEX -> not found
{
    uint8_t ret = SLOTS_FIND_INVALID_INDEX, i;
    for(i = 0; i < SLOTS_ST25TB_COUNT; i++)
    {
        if((*(uint64_t*) pui8Data) == (*(uint64_t*) FlashStoredData.Slots[i][SLOTS_ST25TB_INDEX_UID]))
        {
            ret = i;
            break;
        }
    }

    return ret;
}

uint8_t SLOTS_ST25TB_Current[SLOTS_ST25TB_SECTORS_INTERNAL][4];

void flash_update_CurrentSlot(uint32_t NewSlot)
{
    uint32_t ints;
    uint8_t Buffer[ROUND_UP(sizeof(FlashStoredData), FLASH_PAGE_SIZE)];
    PFLASH_STORED_DATA p = (PFLASH_STORED_DATA) Buffer;
    *p = FlashStoredData;

    p->CurrentSlot = NewSlot;
    printf("|%s| - [%lu]\n", __FUNCTION__, NewSlot);
    
    ints = save_and_disable_interrupts();
    flash_range_erase(host_safe_hw_ptr(&FlashStoredData) - XIP_BASE, FLASH_SECTOR_SIZE);
    flash_range_program(host_safe_hw_ptr(&FlashStoredData) - XIP_BASE, Buffer, sizeof(Buffer));
    restore_interrupts(ints);
}

void flash_update_Slot(uint8_t index, uint8_t data[SLOTS_ST25TB_SECTORS_INTERNAL][4])
{
    uint32_t ints;
    uint8_t Buffer[ROUND_UP(sizeof(FlashStoredData), FLASH_PAGE_SIZE)];
    PFLASH_STORED_DATA p = (PFLASH_STORED_DATA) Buffer;
    *p = FlashStoredData;

    memcpy(p->Slots[index], data, sizeof(p->Slots[index]));
    printf("|%s| - [%hhu] [UID: %016llx]\n", __FUNCTION__, index, *(uint64_t *) (data + SLOTS_ST25TB_INDEX_UID));

    ints = save_and_disable_interrupts();
    flash_range_erase(host_safe_hw_ptr(&FlashStoredData) - XIP_BASE, FLASH_SECTOR_SIZE);
    flash_range_program(host_safe_hw_ptr(&FlashStoredData) - XIP_BASE, Buffer, sizeof(Buffer));
    restore_interrupts(ints);
}

__attribute__ ((aligned (FLASH_SECTOR_SIZE), section(".flash_storage")))
/*const */FLASH_STORED_DATA FlashStoredData = {
    .CurrentSlot = 0,
    
    .Slots = {
        {   /* Slot 0 */
            #undef SLOT_UID_BYTE
            #define SLOT_UID_BYTE   0x00
            #if defined(SLOT_0_CONTENT)
            #include SLOT_0_CONTENT
            #else
            #include "slots_fixed_content.h"
            #endif
        },
    #if SLOTS_ST25TB_COUNT > 1
        {   /* Slot 1 */
            #undef SLOT_UID_BYTE
            #define SLOT_UID_BYTE   0x11
            #if defined(SLOT_1_CONTENT)
            #include SLOT_1_CONTENT
            #else
            #include "slots_fixed_content.h"
            #endif
        },
    #endif
    #if SLOTS_ST25TB_COUNT > 2
        {   /* Slot 2 */
            #undef SLOT_UID_BYTE
            #define SLOT_UID_BYTE   0x22
            #if defined(SLOT_2_CONTENT)
            #include SLOT_2_CONTENT
            #else
            #include "slots_fixed_content.h"
            #endif
        },
    #endif
    #if SLOTS_ST25TB_COUNT > 3
        {   /* Slot 3 */
            #undef SLOT_UID_BYTE
            #define SLOT_UID_BYTE   0x33
            #if defined(SLOT_3_CONTENT)
            #include SLOT_3_CONTENT
            #else
            #include "slots_fixed_content.h"
            #endif
        },
    #endif
    #if SLOTS_ST25TB_COUNT > 4
        {   /* Slot 4 */
            #undef SLOT_UID_BYTE
            #define SLOT_UID_BYTE   0x44
            #if defined(SLOT_4_CONTENT)
            #include SLOT_4_CONTENT
            #else
            #include "slots_fixed_content.h"
            #endif
        },
    #endif
    #if SLOTS_ST25TB_COUNT > 5
        {   /* Slot 5 */
            #undef SLOT_UID_BYTE
            #define SLOT_UID_BYTE   0x55
            #if defined(SLOT_5_CONTENT)
            #include SLOT_5_CONTENT
            #else
            #include "slots_fixed_content.h"
            #endif
        },
    #endif
    #if SLOTS_ST25TB_COUNT > 6
        {   /* Slot 6 */
            #undef SLOT_UID_BYTE
            #define SLOT_UID_BYTE   0x66
            #if defined(SLOT_6_CONTENT)
            #include SLOT_6_CONTENT
            #else
            #include "slots_fixed_content.h"
            #endif
        },
    #endif
    #if SLOTS_ST25TB_COUNT > 7
        {   /* Slot 7 */
            #undef SLOT_UID_BYTE
            #define SLOT_UID_BYTE   0x77
            #if defined(SLOT_7_CONTENT)
            #include SLOT_7_CONTENT
            #else
            #include "slots_fixed_content.h"
            #endif
        },
    #endif
    }
};