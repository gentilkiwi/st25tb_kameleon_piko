/*  Benjamin DELPY `gentilkiwi`
    https://blog.gentilkiwi.com
    benjamin@gentilkiwi.com
    Licence : https://creativecommons.org/licenses/by/4.0/
*/
#include "board.h"

volatile bool g_irq_TA0, g_irq_SW1, g_irq_SW2, g_irq_TRF;

void __isr __time_critical_func(BOARD_IRQ_Callback)(uint gpio, uint32_t event_mask)
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

int64_t alarm_callback(__attribute__((unused)) alarm_id_t id, __attribute__((unused)) void *user_data)
{
    g_irq_TA0 = true;
    return 0;
}

void BOARD_init()
{
    set_sys_clock_48mhz();
    
    LEDS_Init();
    
    gpio_init(PIKO_GPIO_SW1_IRQ);
    gpio_pull_up(PIKO_GPIO_SW1_IRQ);
    gpio_init(PIKO_GPIO_SW2_IRQ);
    gpio_pull_up(PIKO_GPIO_SW2_IRQ);
    gpio_set_irq_enabled(PIKO_GPIO_SW1_IRQ, GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_enabled(PIKO_GPIO_SW2_IRQ, GPIO_IRQ_EDGE_FALL, true);

    gpio_init(PIKO_GPIO_TRF_EN);
    gpio_set_dir(PIKO_GPIO_TRF_EN, GPIO_OUT);
    gpio_init(PIKO_GPIO_SPI_CS);
    gpio_set_dir(PIKO_GPIO_SPI_CS, GPIO_OUT);
    gpio_init(PIKO_GPIO_TRF_IRQ);
    TRF_IRQ_DISABLE();
    
    spi_init(PIKO_SPI, 4000000);
    spi_set_format(PIKO_SPI, 8, SPI_CPOL_0, SPI_CPHA_1, SPI_MSB_FIRST);
    gpio_set_function(PIKO_GPIO_SPI_CLK, GPIO_FUNC_SPI);
    gpio_set_function(PIKO_GPIO_SPI_MOSI, GPIO_FUNC_SPI);
    gpio_set_function(PIKO_GPIO_SPI_MISO, GPIO_FUNC_SPI);

    TRF7970A_init();

    gpio_set_irq_callback(BOARD_IRQ_Callback);
    irq_set_enabled(IO_IRQ_BANK0, true);
}

/*
 * TODO: Can be interesting to see a generic function with a bitmask of what is wanted instead of multiple functions
 */
uint8_t IRQ_Wait_for_SW1()
{
    g_irq_SW1 = false;
    while(!g_irq_SW1)
    {
       __low_power_mode_0();
    }
    g_irq_SW1 = false;

    return IRQ_SOURCE_SW1;
}

uint8_t IRQ_Wait_for_SW1_or_SW2()
{
    uint8_t ret = IRQ_SOURCE_NONE;

    g_irq_SW1 = false;
    g_irq_SW2 = false;

    while(!g_irq_SW1 && !g_irq_SW2)
    {
       __low_power_mode_0();
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

uint8_t __time_critical_func(IRQ_Wait_for_SW1_or_SW2_or_TRF)(uint8_t *pTRF7970A_irqStatus)
{
    uint8_t ret = IRQ_SOURCE_NONE;

    g_irq_SW1 = false;
    g_irq_SW2 = false;

    g_irq_TRF = TRF_IRQ_READ();
    while(!g_irq_TRF && !g_irq_SW1 && !g_irq_SW2)
    {
        __low_power_mode_0();
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

uint8_t IRQ_Wait_for_SW1_or_TRF(uint8_t *pTRF7970A_irqStatus)
{
    uint8_t ret = IRQ_SOURCE_NONE;

    g_irq_SW1 = false;

    g_irq_TRF = TRF_IRQ_READ();
    while(!g_irq_TRF && !g_irq_SW1)
    {
        __low_power_mode_0();
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

    return ret;
}

uint8_t IRQ_Wait_for_SW1_or_SW2_or_Timeout(uint16_t timeout_ms)
{
    uint8_t ret = IRQ_SOURCE_NONE;
    alarm_id_t id;

    g_irq_SW1 = false;
    g_irq_SW2 = false;

    TIMER_start_Milliseconds(id, timeout_ms);
    while(!g_irq_TA0 && !g_irq_SW1 && !g_irq_SW2)
    {
        __low_power_mode_0();
    }
    TIMER_stop(id);

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

    if(g_irq_TA0)
    {
        ret |= IRQ_SOURCE_TIMER;
    }

    return ret;
}

uint8_t IRQ_Wait_for_SW1_or_SW2_or_TRF_or_Timeout(uint8_t *pTRF7970A_irqStatus, uint16_t timeout_ms)
{
    uint8_t ret = IRQ_SOURCE_NONE;
    alarm_id_t id;
    
    g_irq_SW1 = false;
    g_irq_SW2 = false;

    TIMER_start_Milliseconds(id, timeout_ms);
    g_irq_TRF = TRF_IRQ_READ();
    while(!g_irq_TRF && !g_irq_TA0 && !g_irq_SW1 && !g_irq_SW2)
    {
        __low_power_mode_0();
    }
    TIMER_stop(id);

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

    if(g_irq_TA0)
    {
        ret |= IRQ_SOURCE_TIMER;
    }

    return ret;
}