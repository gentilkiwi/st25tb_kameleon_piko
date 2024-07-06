#pragma once

/** ST25TB settings
 **/

/* ST25TB memory support
 * - SLOTS_ST25TB_SUPPORT_4K will enable support for ST25TB04K (and below) - but is 512 bytes per slot
 * - SLOTS_ST25TB_SUPPORT_2K will enable support for ST25TB02K (and below) - but is 256 bytes per slot
 * - otherwise, only ST25TB512 is supported - it is only 64 bytes per slot
 * default is SLOTS_ST25TB_SUPPORT_4K
 */
#define SLOTS_ST25TB_SUPPORT_4K
//#define SLOTS_ST25TB_SUPPORT_2K
//#define SLOTS_ST25TB_SUPPORT_512 // not used in code - only to know here what you choose

/* ST25TB available slots number
 * 8 is recommended, if you want more you'll have to adapt slots.c to avoid empty persistent content
 */
#define SLOTS_ST25TB_COUNT  8


/** 'A' settings
 ** at the beginning, ST25TB kameleon was only to emulate/play with ST25TB technology
 ** but at one point, I wanted to play with 'A' emulation and the TRF (spoiler alert: it sucks)
 **
 **/
//#define ST25TB_SUPPORT_A_NTAG210
//#define ST25TB_SUPPORT_A_ST25TA512
