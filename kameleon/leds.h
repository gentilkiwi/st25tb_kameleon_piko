/*  Benjamin DELPY `gentilkiwi`
    https://blog.gentilkiwi.com
    benjamin@gentilkiwi.com
    Licence : https://creativecommons.org/licenses/by/4.0/
*/
#pragma once
#include "board.h"

extern const uint LEDS[];

void LEDS_Init();
void LEDS_Bitmask(const uint *LEDS_ARRAY, const uint8_t nbLeds, uint8_t bitmask);
void LEDS_Animation();

#define LEDS_MODES      LEDS
#define NB_LEDS_MODES   5
#define LEDS_SLOTS      LEDS_MODES + NB_LEDS_MODES
#define NB_LEDS_SLOTS   8
#define LEDS_STATUS     LEDS_SLOTS + NB_LEDS_SLOTS
#define NB_LEDS_STATUS  3

#define LEDS_MODES_Bitmask(bitmask) LEDS_Bitmask(LEDS_MODES, NB_LEDS_MODES, bitmask)
#define LEDS_SLOTS_Bitmask(bitmask) LEDS_Bitmask(LEDS_SLOTS, NB_LEDS_SLOTS, bitmask)
#define LEDS_STATUS_Bitmask(bitmask) LEDS_Bitmask(LEDS_STATUS, NB_LEDS_STATUS, bitmask)

#define LED_Slot(index) LEDS_SLOTS_Bitmask(1 << index)

#define LED_INDEX_STATUS_BLUE       NB_LEDS_MODES + NB_LEDS_SLOTS + 0
#define LED_INDEX_STATUS_GREEN      NB_LEDS_MODES + NB_LEDS_SLOTS + 1
#define LED_INDEX_STATUS_RED        NB_LEDS_MODES + NB_LEDS_SLOTS + 2

#define LED_ON(index)               gpio_put(LEDS[index], 1)
#define LED_OFF(index)              gpio_put(LEDS[index], 0)
