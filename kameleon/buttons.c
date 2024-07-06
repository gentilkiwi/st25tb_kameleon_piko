#include "buttons.h"

void BUTTONS_Init()
{
    gpio_init(PIKO_GPIO_SW1_IRQ);
    gpio_pull_up(PIKO_GPIO_SW1_IRQ);
    gpio_set_irq_enabled(PIKO_GPIO_SW1_IRQ, GPIO_IRQ_EDGE_FALL, false);

    gpio_init(PIKO_GPIO_SW2_IRQ);
    gpio_pull_up(PIKO_GPIO_SW2_IRQ);
    gpio_set_irq_enabled(PIKO_GPIO_SW2_IRQ, GPIO_IRQ_EDGE_FALL, false);
    
}

void BUTTONS_Enable(uint GPIO)
{
    gpio_set_irq_enabled(GPIO, GPIO_IRQ_EDGE_FALL, true);
}

void BUTTONS_Disable(uint GPIO)
{
    gpio_set_irq_enabled(GPIO, GPIO_IRQ_EDGE_FALL, false);
}