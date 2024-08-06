/*  Benjamin DELPY `gentilkiwi`
    https://blog.gentilkiwi.com
    benjamin@gentilkiwi.com
    Licence : https://creativecommons.org/licenses/by/4.0/
*/
#pragma once
#include "board.h"

#define REFERENCES_ST25TB_SECTORS_INTERNAL   (( 512 / 8 / 4) + 3)

#define REFERENCES_ST25TB_INDEX_SYSTEM   (REFERENCES_ST25TB_SECTORS_INTERNAL - 3)
#define REFERENCES_ST25TB_INDEX_UID      (REFERENCES_ST25TB_SECTORS_INTERNAL - 2)
#define REFERENCES_ST25TB_INDEX_UID_2    (REFERENCES_ST25TB_SECTORS_INTERNAL - 1)

extern const uint8_t REFERENCES_ST25TB[][REFERENCES_ST25TB_SECTORS_INTERNAL][4];

uint8_t REFERENCES_FindAndLoadByUID(uint8_t pui8Data[8]);
