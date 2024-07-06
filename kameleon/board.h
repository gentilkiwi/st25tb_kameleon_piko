/*  Benjamin DELPY `gentilkiwi`
    https://blog.gentilkiwi.com
    benjamin@gentilkiwi.com
    Licence : https://creativecommons.org/licenses/by/4.0/
*/
#pragma once
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/pio.h"
#include "../config.h"
#include "slots.h"
#include "leds.h"
#include "buttons.h"
#include "trf7970a.h"

typedef struct _GLOBAL_SETTINGS {
    uint8_t CurrentSlot;
} GLOBAL_SETTINGS, *PGLOBAL_SETTINGS;

extern GLOBAL_SETTINGS Settings;

extern volatile bool g_irq_TA0, g_irq_SW1, g_irq_SW2, g_irq_TRF;


#define PIKO_SPI                    spi0

#define PIKO_GPIO_TX                0
#define PIKO_GPIO_RX                1
#define PIKO_GPIO_LED_STATUS_BLUE   2
#define PIKO_GPIO_SW2_IRQ           3
#define PIKO_GPIO_SPI_MISO          4
#define PIKO_GPIO_TRF_EN            5
#define PIKO_GPIO_SPI_CLK           6
#define PIKO_GPIO_SPI_MOSI          7
#define PIKO_GPIO_LED_STATUS_GREEN  8
#define PIKO_GPIO_LED_STATUS_RED    9
#define PIKO_GPIO_SPI_CS            10
#define PIKO_GPIO_TRF_IRQ           11
#define PIKO_GPIO_SW1_IRQ           12
#define PIKO_GPIO_LED_MOD_EMULATE   13
#define PIKO_GPIO_LED_MOD_REWRITE   14
#define PIKO_GPIO_LED_MOD_DETECT    15
#define PIKO_GPIO_LED_MOD_LEARN     16
#define PIKO_GPIO_LED_MOD_UNK       17
#define PIKO_GPIO_LED_SLOT0         18
#define PIKO_GPIO_LED_SLOT1         19
#define PIKO_GPIO_LED_SLOT2         20
#define PIKO_GPIO_LED_SLOT3         21
#define PIKO_GPIO_LED_SLOT4         22
#define PIKO_GPIO_LED_SLOT5         23
#define PIKO_GPIO_LED_SLOT6         24
#define PIKO_GPIO_LED_SLOT7         25

void BOARD_init();

#define TIMER_stop()                    TA0CTL &= ~(TAIE | MC)
#define TIMER_delay_Milliseconds(n_ms)  sleep_ms(n_ms)
#define TIMER_start_Milliseconds(n_ms)  TIMER_start_Milliseconds_internal(n_ms * 33)
#define TIMER_delay_Microseconds(n_us)  sleep_us(n_us)

void TIMER_start_Milliseconds_internal(uint16_t n_unit_ms); // max is UINT16_MAX ( 1985 ms * 33 = ~ UINT16_MAX )

#define IRQ_SOURCE_NONE                 0x00
#define IRQ_SOURCE_TRF7970A             0x01
#define IRQ_SOURCE_SW1                  0x02
#define IRQ_SOURCE_SW2                  0x04
#define IRQ_SOURCE_TIMER                0x08
#define IRQ_SOURCE_ST25TB_PROTOCOL_ERR  0x10

uint8_t IRQ_Wait_for_SW1();
uint8_t IRQ_Wait_for_SW1_or_SW2();
uint8_t IRQ_Wait_for_SW1_or_SW2_or_TRF(uint8_t *pTRF7970A_irqStatus);
uint8_t IRQ_Wait_for_SW1_or_TRF(uint8_t *pTRF7970A_irqStatus);
uint8_t IRQ_Wait_for_SW1_or_SW2_or_Timeout(uint16_t timeout_ms);
uint8_t IRQ_Wait_for_SW1_or_SW2_or_TRF_or_Timeout(uint8_t *pTRF7970A_irqStatus, uint16_t timeout_ms);
