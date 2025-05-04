/*  Benjamin DELPY `gentilkiwi`
    https://blog.gentilkiwi.com
    benjamin@gentilkiwi.com
    Licence : https://creativecommons.org/licenses/by/4.0/
*/
#pragma once
#include "st25tb.h"

bool ST25TB_Target_ResponseTo();
uint8_t ST25TB_Target_AdjustIdxForSpecialAddr(uint8_t original);

/*
 * SRT512
 *  t0 Antenna reversal delay 128/fS  151 �s
 *  t1 Synchronization delay          151 �s
 *  t2 Answer to new request delay >= 132 �s
 *
 * ST25TB512-AT
 *  t0 Antenna reversal delay 128/fS  159 �s
 *  t1 Synchronization delay          151 �s
 *  t2 Answer to new request delay >= 132 �s
 *
 *  -> focus on t0 <= 160 �s ?
 */
#define ST25TB_TARGET_DELAY_US_GLOBAL  86
#define ST25TB_TARGET_DELAY_US_MEDIUM  13
#define ST25TB_TARGET_DELAY_US_SMALL   7