/*  Benjamin DELPY `gentilkiwi`
    https://blog.gentilkiwi.com
    benjamin@gentilkiwi.com
    Licence : https://creativecommons.org/licenses/by/4.0/
*/
#pragma once
#include "st25tb.h"

typedef enum __attribute__((__packed__)) _tSt25TbState {
    Invalid,
    PowerOff,
    Ready,
    Inventory,
    Selected,
    Deselected,
    Deactivated,
} tSt25TbState;

void ST25TB_Target_ResetState();
tSt25TbState ST25TB_Target_StateMachine();
uint8_t ST25TB_Target_AdjustIdxForSpecialAddr(uint8_t original);

/*
 * SRT512
 *  t0 Antenna reversal delay 128/fS  151 탎
 *  t1 Synchronization delay          151 탎
 *  t2 Answer to new request delay >= 132 탎
 *
 * ST25TB512-AT
 *  t0 Antenna reversal delay 128/fS  159 탎
 *  t1 Synchronization delay          151 탎
 *  t2 Answer to new request delay >= 132 탎
 *
 *  -> focus on t0 <= 160 탎 ?
 */
#define ST25TB_TARGET_DELAY_US_GLOBAL  86
#define ST25TB_TARGET_DELAY_US_MEDIUM  13
#define ST25TB_TARGET_DELAY_US_SMALL   7