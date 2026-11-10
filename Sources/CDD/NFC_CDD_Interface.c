/*
 * NFC_CDD_Interface.c
 *
 *  Created on: 30-Aug-2026
 *      Author: Shardul
 */

#include "NFC_CDD_Interface.h"
#include "NFC_CDD_Middleware.h"

/*******************************************************************************
 ** Function name    : GetNFCDriverStatus(CDDDriverStatus *NFCCDDStatus,
 **                  : uint8* NFCRetryCount)
 ** Description      : Returns the current state of NFC CDD and number of
 **                  : retries for initialization sequence
 ** Parameters       : CDDDriverStatus* const lptrenu_vNFCCDDStatus,
 **                  : uint8* const lptru8_cNFCRetryCount
 ** Return value     : void
 ** Remarks          :
 ******************************************************************************/
void GetNFCDriverStatus(CDDDriverStatus* const lptrenu_vNFCCDDStatus,
        uint8* const lptru8_cNFCRetryCount)
{
#ifdef SPECIAL_TEST_BUILD
    if(SET == CTF_NFC_CDDState)
    {
        *lptrenu_vNFCCDDStatus = CTV_NFC_CDDState;
    }
    else
    {
        *lptrenu_vNFCCDDStatus = genu_vNFCCDDState;
    }
#else
    *lptrenu_vNFCCDDStatus = genu_vNFCCDDState;
#endif
    *lptru8_cNFCRetryCount = gu8_cNFCInitRtrCnt;
}

/*******************************************************************************
 ** Function name    : SetNFCDriverState
 ** Description      : This function commands the NFC CDD state transition
 ** Parameters       : CDDDriverStatus const lenu_vNFCRequestState,
 **                  : BOOL* const lptru8_fStateChangeAccepted
 ** Return value     : void
 ** Remarks          :
 ******************************************************************************/
void SetNFCDriverState(CDDDriverStatus const lenu_vNFCRequestState,
        BOOL* const lptru8_fStateChangeAccepted)
{
    if((CDD_INIT == lenu_vNFCRequestState) && (genu_vNFCTrxStatus == CDD_TRX_NOT_STARTED))
    {
        gu8_cNFCInitRtrCnt = 0u;
        CDD_NFC_StartInitSequence();
    }
    else if(CDD_SHUTDOWN == lenu_vNFCRequestState)
    {
        genu_vNFCShutStatus = CDD_TRX_NOT_STARTED;
        gu8_cNFCShutRtrCnt = 0u;
    }
    else if(CDD_STANDBY == lenu_vNFCRequestState)
    {
        genu_vNFCShutStatus = CDD_TRX_NOT_STARTED;
    }
    else
    {
        *lptru8_fStateChangeAccepted = FALSE;
        return;
    }
    genu_vNFCCDDState = lenu_vNFCRequestState;
    *lptru8_fStateChangeAccepted = TRUE;
}


/*******************************************************************************
 ** Function name    : SetNFCToPowerDown
 ** Description      : This function starts MFRC522 power down sequence
 ** Parameters       : BOOL* const lptru8_fPowerDownAccepted
 ** Return value     : void
 ** Remarks          :
 ******************************************************************************/
void SetNFCToPowerDown(bool* const lptru8_fPowerDownAccepted)
{
    if(CDD_RUN == genu_vNFCCDDState)
    {
        genu_vNFCCDDState = CDD_SHUTDOWN;
        *lptru8_fPowerDownAccepted = TRUE;
    }
    else
    {
        *lptru8_fPowerDownAccepted = FALSE;
        /* #ERROR: State Plausibility error */
        guni_vCDDErrors.bf.STATEERR = TRUE;
    }
}
