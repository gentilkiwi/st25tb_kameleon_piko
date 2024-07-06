/*  Benjamin DELPY `gentilkiwi`
    https://blog.gentilkiwi.com
    benjamin@gentilkiwi.com
    Licence : https://creativecommons.org/licenses/by/4.0/
*/
#include "kameleon/board.h"
#include "kameleon/modes.h"
extern char __flash_binary_start, __flash_binary_end;
const KAMELEON_MODE Modes[] = {
    {MODE_emulate, 1 << 0},
    {MODE_rewrite, 1 << 1},
    {MODE_detect,  1 << 2},
    {MODE_select,  1 << 3},
    {MODE_unk,     1 << 4},
};

const KAMELEON_MODE Modes_2[] = {
    {MODE_learn,   1 << 3},
};

int main()
{
    uint8_t maxModes;
    const KAMELEON_MODE *pMode, *cMode;
    
    stdio_init_all();

    BOARD_init();
    LEDS_Animation();
    SLOTS_Change(Settings.CurrentSlot);

    pMode = cMode = Modes;
    maxModes = sizeof(Modes) / sizeof(Modes[0]);



    while(true)
    {
        g_irq_SW1 = false;
        g_irq_SW2 = false;
        LEDS_MODES_Bitmask(cMode->ledsModesBitmask);
        LEDS_STATUS_Bitmask(0);
        cMode->current();
        cMode++;
        if(cMode >= (pMode + maxModes))
        {
            cMode = pMode + 0;
        }
        TIMER_delay_Milliseconds(150);
        
    printf("%p\r\n", &__flash_binary_start);
    printf("%p\r\n", &__flash_binary_end);
    printf("%d\r\n", &__flash_binary_end - &__flash_binary_start);
    }

    // return 0;
}
