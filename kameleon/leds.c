/*  Benjamin DELPY `gentilkiwi`
    https://blog.gentilkiwi.com
    benjamin@gentilkiwi.com
    Licence : https://creativecommons.org/licenses/by/4.0/
*/
#include "leds.h"

const uint LEDS[] = {
    // LEDS_MODES
    /* 0 */ PIKO_GPIO_LED_MOD_EMULATE,  // LED_MOD_EMULATE
    /* 1 */ PIKO_GPIO_LED_MOD_REWRITE,  // LED_MOD_REWRITE
    /* 2 */ PIKO_GPIO_LED_MOD_DETECT,   // LED_MOD_DETECT
    /* 3 */ PIKO_GPIO_LED_MOD_LEARN,    // LED_MOD_LEARN
    /* 4 */ PIKO_GPIO_LED_MOD_UNK,      // LED_MOD_UNK
    // LEDS_SLOTS
    /* 0 */ PIKO_GPIO_LED_SLOT0,        // LED_SLOT0
    /* 1 */ PIKO_GPIO_LED_SLOT1,        // LED_SLOT1
    /* 2 */ PIKO_GPIO_LED_SLOT2,        // LED_SLOT2
    /* 3 */ PIKO_GPIO_LED_SLOT3,        // LED_SLOT3
    /* 4 */ PIKO_GPIO_LED_SLOT4,        // LED_SLOT4
    /* 5 */ PIKO_GPIO_LED_SLOT5,        // LED_SLOT5
    /* 6 */ PIKO_GPIO_LED_SLOT6,        // LED_SLOT6
    /* 7 */ PIKO_GPIO_LED_SLOT7,        // LED_SLOT7
    // LEDS_STATUS
    /* 0 */ PIKO_GPIO_LED_STATUS_BLUE,  // LED_STATUS_BLUE
    /* 1 */ PIKO_GPIO_LED_STATUS_GREEN, // LED_STATUS_GREEN
    /* 2 */ PIKO_GPIO_LED_STATUS_RED,   // LED_STATUS_RED
};

void LEDS_Init()
{
    uint8_t i;

    for(i = 0; i < (sizeof(LEDS) / sizeof(LEDS[0])); i++)
    {
        gpio_init(LEDS[i]);
        gpio_set_dir(LEDS[i], GPIO_OUT);
    }    
}

void LEDS_Bitmask(const uint *LEDS_ARRAY, const uint8_t nbLeds, uint8_t bitmask)
{
    uint8_t i;

    for(i = 0; i < nbLeds; i++, bitmask >>= 1)
    {
        gpio_put(LEDS_ARRAY[i], bitmask & 0b00000001);
    }
}

void LEDS_Animation()
{
    uint8_t i;

    for(i = 0; i < (sizeof(LEDS) / sizeof(LEDS[0])); i++)
    {
        LED_ON(i);
        TIMER_delay_Milliseconds(10);
        LED_OFF(i);
    }
}
