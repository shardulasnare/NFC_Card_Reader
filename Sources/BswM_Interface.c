/*
 * BswM_Interface.c
 *
 *  Created on: 02-Nov-2026
 *      Author: Shardul
 */

/*****************************************************************************/
/* Start of file */
/******************************************************************************
 ** Include files
 ******************************************************************************/
#include "BswM_Interface.h"
#include "EcuM_Interface.h"
#include "SysTimer.h"
#include "ETS.h"
#include "NFC_CDD_Middleware.h"
#include "NFC_CDD_Interface.h"

/******************************************************************************
 ** Global and Static Variable declarations
 ******************************************************************************/
uint8_t genu_vMCUPwrDwnMode = 0u;

/******************************************************************************
 ** Function definitions
 ******************************************************************************/
void SetASWIntialization(bool *acceptance)
{
    genu_vMCUState = MCU_STATE_RUN;
    *acceptance = TRUE;
}

void SetASWShutdownRequest(bool lu8_NVMRequest, bool lu8_PowerDownMode,
        bool* lptru8_Acceptance)
{
    bool lu8_fAcceptance = TRUE;
    bool lu8_fNFCPDAccepted = 0u;

    if(SET == lu8_PowerDownMode)
    {
        if(SET == lu8_NVMRequest)
        {
            /*MCU power down mode = shutdown mode*/
            genu_vMCUPwrDwnMode = MCU_SHUTDOWN;
        }
        else
        {
            /* invalid scenario - power down mode requested = shutdown mode but
             NVM write request is not set ; request will not be served */
            genu_vMCUPwrDwnMode = MCU_SHUTDOWN;
        }
    }
    else if(RESET == lu8_PowerDownMode)
    {
        genu_vMCUPwrDwnMode = MCU_VLPS;
    }
    else
    {
        /* invalid argument - request will not be served */
        lu8_fAcceptance = FALSE;
    }

    SetNFCToPowerDown(&lu8_fNFCPDAccepted);

    if(TRUE == lu8_fAcceptance)
    {
        genu_vMCUState = MCU_STATE_SHUTDOWN1;
    }
    else
    {
        ;
    }
    *lptru8_Acceptance = lu8_fAcceptance;
}

/******************************************************************************
 ** End of File
 ******************************************************************************/
