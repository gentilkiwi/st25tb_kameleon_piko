/*  Benjamin DELPY `gentilkiwi`
    https://blog.gentilkiwi.com
    benjamin@gentilkiwi.com
    Licence : https://creativecommons.org/licenses/by/4.0/
*/
#include "st25tb_utils.h"

uint8_t st25tb_utils_Display_UIDChip(const uint8_t *pui8UID)
{
    uint8_t nbSectors, chipId;
    const char *chipName;

    printf("\nUID: %016llX\n", (*(uint64_t*) pui8UID));
    nbSectors = ST25TB_Initiator_ValidateAndGetNBSectorsFromUID(pui8UID, &chipId);
    if(nbSectors)
    {
        switch(chipId)
        {
            case 0x33:
                chipName = "ST25TB512-AT";
                break;
            case 0x1b:
                chipName = "ST25TB512-AC";
                break;
            case 0x3f:
                chipName = "ST25TB02K";
                break;
            case 0x1f:
                chipName = "ST25TB04K";
                break;
            case 0x0c: // 12
                chipName = "SRT512";
                break;
            case 0x06: // 6
                chipName = "SRI512";
                break;
            case 0x04: // 4
                chipName = "SRIX512";
                break;
            case 0x0f: // 15
                chipName = "SRI2K";
                break;
            case 0x07: // 7
                chipName = "SRI4K";
                break;
            case 0x03: // 3
                chipName = "SRIX4K";
                break;
            default:
                chipName = NULL;
        }
        printf("Chipset: %s (%hhu sectors)\n", chipName, nbSectors);
    }
    
    return nbSectors;
}

void st25tb_utils_Display_sector(const uint8_t data[SLOTS_ST25TB_SECTORS_INTERNAL][4], const uint8_t sector)
{
    uint8_t idx = ST25TB_Target_AdjustIdxForSpecialAddr(sector);
    printf("%3hhu/0x%02X | %02hhX %02hhX %02hhX %02hhX\n", sector, sector, data[idx][0], data[idx][1], data[idx][2], data[idx][3]);
}

void st25tb_utils_Display_sector_data(const uint8_t data[4], const uint8_t sector)
{
    printf("%3hhu/0x%02X | %02hhX %02hhX %02hhX %02hhX\n", sector, sector, data[0], data[1], data[2], data[3]);
}

void st25tb_utils_Display_dump(const uint8_t data[SLOTS_ST25TB_SECTORS_INTERNAL][4])
{
    uint8_t i, nbSectors;
    
    nbSectors = st25tb_utils_Display_UIDChip(data[SLOTS_ST25TB_INDEX_UID]);
    if(nbSectors)
    {
        printf("\n block#  | data         \n---------+------------\n");
        for(i = 0; i < nbSectors; i++)
        {
            st25tb_utils_Display_sector(data, i);
        }
        st25tb_utils_Display_sector(data, 0xff);
        printf("---------+------------\n");
    }
}