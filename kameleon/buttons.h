/*  Benjamin DELPY `gentilkiwi`
    https://blog.gentilkiwi.com
    benjamin@gentilkiwi.com
    Licence : https://creativecommons.org/licenses/by/4.0/
*/
#pragma once
#include "board.h"

void BUTTONS_Init();
void BUTTONS_Enable(uint GPIO);

#define BUTTONS_Enable_SW1()    BUTTONS_Enable(PIKO_GPIO_SW1_IRQ)
#define BUTTONS_Enable_SW2()    BUTTONS_Enable(PIKO_GPIO_SW2_IRQ)

#define BUTTONS_Disable_SW1()    BUTTONS_Disable(PIKO_GPIO_SW1_IRQ)
#define BUTTONS_Disable_SW2()    BUTTONS_Disable(PIKO_GPIO_SW2_IRQ)