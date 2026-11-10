/*
 * ETS.c
 *
 *  Created on: 10-Nov-2026
 *      Author: Shardul
 */


/*****************************************************************************/
/* Start of file */
/******************************************************************************
Header File Inclusion
*******************************************************************************/
#include "ETS.h"
#include "BswM_Interface.h"
#include "NFC_CDD_Interface.h"

/******************************************************************************
* Macro and function declaration
*******************************************************************************/

/******************************************************************************
Global and Static variable declaration
*******************************************************************************/
static uint8_t s_ETSNFCCDDStatus = 0u;
static uint8 s_ETSNFCInitRetryCount = 0u;

/*****************************************************************************
** Function name: Test_BSW_InitTask
** Description  : This function check NFC device is fully initialized
** 				  and transited to RUN Mode
** Parameter X  : void
** Return value : void
** Remarks      : -
******************************************************************************/
void Test_ETS_InitTask(void)
{
    bool lu8_fAPIAcceptance = false;
    GetNFCDriverStatus(&s_ETSNFCCDDStatus, &s_ETSNFCInitRetryCount);
    /* System transitions to RUN state only if NFC is in RUN State.
     * Alternatively, forceful transition is also possible */
    if((s_ETSNFCCDDStatus == CDD_RUN) || (gu8_fForceMCURun == TRUE))
    {
        SetASWIntialization(&lu8_fAPIAcceptance);
        gu8_fForceMCURun = FALSE;
    }
    else
    {
        if(s_ETSNFCCDDStatus == CDD_STANDBY)
        {
            SetNFCDriverState(CDD_INIT, &lu8_fAPIAcceptance);
        }
        else
        {}
    }
}
