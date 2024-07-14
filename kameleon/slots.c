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
    LED_Slot(index);
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
            #include "slots_fixed_content.h"
            { 0xff, 0x00, 0x00, 0x00 }, // SLOTS_ST25TB_INDEX_UID // Test with libnfc and ACR122U did not like 0x00 at the end (?)
            { 0x00, 0x33, 0x02, 0xd0 }, // SLOTS_ST25TB_INDEX_UID_2
        },
    #if SLOTS_ST25TB_COUNT > 1
        {   /* Slot 1 */
            #include "slots_fixed_content.h"
            { 0xff, 0x11, 0x11, 0x11 }, // SLOTS_ST25TB_INDEX_UID
            { 0x11, 0x33, 0x02, 0xd0 }, // SLOTS_ST25TB_INDEX_UID_2
        },
    #endif
    #if SLOTS_ST25TB_COUNT > 2
        {   /* Slot 2 */
            #include "slots_fixed_content.h"
            { 0xff, 0x22, 0x22, 0x22 }, // SLOTS_ST25TB_INDEX_UID
            { 0x22, 0x33, 0x02, 0xd0 }, // SLOTS_ST25TB_INDEX_UID_2
        },
    #endif
    #if SLOTS_ST25TB_COUNT > 3
        {   /* Slot 3 */
            #include "slots_fixed_content.h"
            { 0xff, 0x33, 0x33, 0x33 }, // SLOTS_ST25TB_INDEX_UID
            { 0x33, 0x33, 0x02, 0xd0 }, // SLOTS_ST25TB_INDEX_UID_2
        },
    #endif
    #if SLOTS_ST25TB_COUNT > 4
        {   /* Slot 4 */
            #include "slots_fixed_content.h"
            { 0xff, 0x44, 0x44, 0x44 }, // SLOTS_ST25TB_INDEX_UID
            { 0x44, 0x33, 0x02, 0xd0 }, // SLOTS_ST25TB_INDEX_UID_2
        },
    #endif
    #if SLOTS_ST25TB_COUNT > 5
        {   /* Slot 5 */
            #include "slots_fixed_content.h"
            { 0xff, 0x55, 0x55, 0x55 }, // SLOTS_ST25TB_INDEX_UID
            { 0x55, 0x33, 0x02, 0xd0 }, // SLOTS_ST25TB_INDEX_UID_2
        },
    #endif
    #if SLOTS_ST25TB_COUNT > 6
        {   /* Slot 6 */
            #include "slots_fixed_content.h"
            { 0xff, 0x66, 0x66, 0x66 }, // SLOTS_ST25TB_INDEX_UID
            { 0x66, 0x33, 0x02, 0xd0 }, // SLOTS_ST25TB_INDEX_UID_2
        },
    #endif
    #if SLOTS_ST25TB_COUNT > 7
        {   /* Slot 7 */
            #include "slots_fixed_content.h"
            { 0xff, 0x77, 0x77, 0x77 }, // SLOTS_ST25TB_INDEX_UID
            { 0x77, 0x33, 0x02, 0xd0 }, // SLOTS_ST25TB_INDEX_UID_2
        },
    #endif
    }
};