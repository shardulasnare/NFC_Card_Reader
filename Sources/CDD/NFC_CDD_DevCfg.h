/*
 * NFC_CDD_DevCfg.h
 *
 *  Created on: 30-Aug-2026
 *      Author: Shardul
 */

#ifndef SOURCES_CDD_NFC_CDD_DEVCFG_H_
#define SOURCES_CDD_NFC_CDD_DEVCFG_H_

#include "Std_Define.h"

#define NUMBER_OF_MC_CHIPS  1u
#define NFC_BUFF_LEN        64u

/***************************************** NFC CDD Defines ****************************************/
#ifndef MAXNFCRESETCNT
#define MAXNFCRESETCNT ((uint8)10)
#endif /* MAXNFCRESETCNT */

#ifndef MAXNFCSHUTRTRYCNT
#define MAXNFCSHUTRTRYCNT ((uint8)10)
#endif /* MAXNFCSHUTRTRYCNT */
/*************************************************************************************************/

typedef enum CDD_TrxStatus_
{
    CDD_TRX_NOT_STARTED = 0u,
    CDD_TRX_INPROGRESS = 1u,
    CDD_TRX_COMPLETE = 2u,
    CDD_TRX_MCALERROR = 3u
} CDD_TrxStatus;

#endif /* SOURCES_CDD_NFC_CDD_DEVCFG_H_ */
