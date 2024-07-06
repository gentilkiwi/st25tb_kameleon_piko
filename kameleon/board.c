/*  Benjamin DELPY `gentilkiwi`
    https://blog.gentilkiwi.com
    benjamin@gentilkiwi.com
    Licence : https://creativecommons.org/licenses/by/4.0/
*/
#include "board.h"

volatile bool g_irq_TA0, g_irq_SW1, g_irq_SW2, g_irq_TRF;

//#pragma PERSISTENT(Settings)
GLOBAL_SETTINGS Settings = {
    .CurrentSlot = 0,
};

void __isr BOARD_IRQ_Callback(uint gpio, uint32_t event_mask)
{
    if((gpio == PIKO_GPIO_TRF_IRQ) && (event_mask & GPIO_IRQ_EDGE_RISE))
    {
        g_irq_TRF = true;
    }
    else if((gpio == PIKO_GPIO_SW1_IRQ) && (event_mask & GPIO_IRQ_EDGE_FALL))
    {
        g_irq_SW1 = true;
    }
    else if((gpio == PIKO_GPIO_SW2_IRQ) && (event_mask & GPIO_IRQ_EDGE_FALL))
    {
        g_irq_SW2 = true;
    }
}

void BOARD_init()
{
    /*
     * MCU @ 48 MHz
     * because why need more at this time?
     */
    set_sys_clock_48mhz();
    

    LEDS_Init();
    BUTTONS_Init();

    TRF7970A_init();



    BUTTONS_Enable_SW1();
    BUTTONS_Enable_SW2();


    /*
     * SPI parameters
     */

    
    
    gpio_set_irq_callback(BOARD_IRQ_Callback);
    irq_set_enabled(IO_IRQ_BANK0, true);
}


uint8_t IRQ_Wait_for_SW1_or_SW2_or_TRF(uint8_t *pTRF7970A_irqStatus)
{
    uint8_t ret = IRQ_SOURCE_NONE;

    g_irq_SW1 = false;
    g_irq_SW2 = false;

    g_irq_TRF = TRF_IRQ_READ();
    while(!g_irq_TRF && !g_irq_SW1 && !g_irq_SW2)
    {
        //__low_power_mode_0();
    }

    if(g_irq_TRF)
    {
        *pTRF7970A_irqStatus = TRF7970A_getIrqStatus();
        g_irq_TRF = false;
        ret |= IRQ_SOURCE_TRF7970A;
    }

    if(g_irq_SW1)
    {
        g_irq_SW1 = false;
        ret |= IRQ_SOURCE_SW1;
    }

    if(g_irq_SW2)
    {
        g_irq_SW2 = false;
        ret |= IRQ_SOURCE_SW2;
    }

    return ret;
}