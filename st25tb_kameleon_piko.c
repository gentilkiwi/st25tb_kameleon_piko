/*  Benjamin DELPY `gentilkiwi`
    https://blog.gentilkiwi.com
    benjamin@gentilkiwi.com
    Licence : https://creativecommons.org/licenses/by/4.0/
*/
#include "kameleon/board.h"
#include "kameleon/modes.h"

const KAMELEON_MODE Modes[] = {
    {.function = MODE_emulate, .ledsModesBitmask = 1 << 0, .Name = "Emulate", "~"},
    {.function = MODE_rewrite, .ledsModesBitmask = 1 << 1, .Name = "Rewrite", "~"},
    {.function = MODE_detect,  .ledsModesBitmask = 1 << 2, .Name = "Detect", "~"},
    {.function = MODE_select,  .ledsModesBitmask = 1 << 3, .Name = "Select", "~"},
    {.function = MODE_unk,     .ledsModesBitmask = 1 << 4, .Name = "(⊙_⊙)？", "~"},
};

const KAMELEON_MODE Modes_2[] = {
    {.function = MODE_learn,   .ledsModesBitmask = 1 << 3, .Name = "Learn", "~"},
};

const char KIWI_BANNER[] = 	"\x1b[2J\x1b[3J\x1b[H\n"
	"  .#####.         ST25TB Kameleon Piko 0.1\n"
	" .## ^ ##.__ _\n"
	" ## / \\ /   ('>-  /***\n"
	" ## \\ / | K  |     Benjamin DELPY `gentilkiwi` ( benjamin@gentilkiwi.com )\n"
	" '## v #\\____/\n"
	"  '#####' L\\_      ***/\n\n";

int main()
{
    uint8_t maxModes;
    const KAMELEON_MODE *pMode, *cMode;
    pico_unique_board_id_t board_id;
    
    stdio_init_all();
    
    BOARD_init();
    LEDS_Animation();
    SLOTS_Change(FlashStoredData.CurrentSlot);

    while(!gpio_get(PIKO_GPIO_SW2_IRQ)) // to wait terminal to be connected - if needed
    {
        __low_power_mode_0();
    }
    
    pico_get_unique_board_id(&board_id);
    puts(KIWI_BANNER);
    
    printf(
        "PICO Unique Board Id: %016llx\n"
        "RP2040 Chip Version : 0x%02x\n"
        "RP2040 SysFrequency : %lu kHz\n"
        "Flash size          : %d bytes\n"
        "Flash sector size   : %d bytes\n"
        "Flash page size     : %d bytes\n"
        "Flash binary_start  : 0x%p\n"
        "Flash binary_end    : 0x%p\n"
        "Flash next sector   : 0x%p\n"
        "Flash StoredData    : 0x%p for %d bytes\n",
        __builtin_bswap64(*(uint64_t *) &board_id), rp2040_chip_version(), frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_SYS), PICO_FLASH_SIZE_BYTES, FLASH_SECTOR_SIZE, FLASH_PAGE_SIZE, &__flash_binary_start, &__flash_binary_end, SECTOR_ALIGN_UP(&__flash_binary_end), &FlashStoredData, sizeof(FlashStoredData)
        );
    
    if(gpio_get(PIKO_GPIO_SW1_IRQ))
    {
        pMode = cMode = Modes;
        maxModes = count_of(Modes);
    }
    else
    {
        pMode = cMode = Modes_2;
        maxModes = count_of(Modes_2);    
    }

    printf("ST25TB - Current Slot  : %u\n", FlashStoredData.CurrentSlot);

    while(true)
    {
        g_irq_SW1 = false;
        g_irq_SW2 = false;
        LEDS_MODES_Bitmask(cMode->ledsModesBitmask);
        LEDS_STATUS_Bitmask(0);
        
        printf("\n--- Mode: %s ---\n", cMode->Name);
        
        cMode->function();
        
        TRF7970A_SPI_DirectCommand(TRF79X0_STOP_DECODERS_CMD);
        TRF7970A_SPI_Write_SingleRegister(TRF79X0_CHIP_STATUS_CTRL_REG, 0x00);
        cMode++;
        if(cMode >= (pMode + maxModes))
        {
            cMode = pMode + 0;
        }
        TIMER_delay_Milliseconds(150);
    }

    // return 0;
}
