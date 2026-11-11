/*
 * NFC_CDD_Middleware.c
 *
 *  Created on: 30-Aug-2026
 *      Author: Shardul
 */

#include "NFC_CDD_Middleware.h"
#include "NFC_CDD_DevCfg.h"

static enum NFCInitStatus_t {
    NFCINITSTATUS_NOTSTARTED,
    NFCINITSTATUS_DEVICEID_READ,
    NFCINITSTATUS_PREINIT_READ,
    NFCINITSTATUS_STARTED,
    NFCINITSTATUS_COMPLETED,
} genu_vNFCInitStatus;

/* Data buffers */
uint8_t gau8_vMFRC522_rb[NUMBER_OF_MC_CHIPS][MC_REGINDEX_RESERVED + 1u]; /*
    Description: Buffer to hold the raw cell data received from the ASIC after extraction from the received messages. */
uint8_t gau8_vMFRC522_wr[NUMBER_OF_MC_CHIPS][MC_REGINDEX_RESERVED + 1u];
uint8 gu8_fErrorAggregate = FALSE;

CDDDriverStatus genu_vNFCCDDState = CDD_STANDBY;
uint8_t gu8_cNFCInitRtrCnt = 0u;
uint8_t gu8_cNFCShutRtrCnt = 0u;
CDD_TrxStatus genu_vNFCShutStatus = CDD_TRX_NOT_STARTED;
NFC_CDDErrorType guni_vCDDErrors;
NFC_msgErrorType gauni_vMsgErrors;
uint8_t gu8_vSavedCDDErrors = 0u;
uint8_t gu8_vSavedMsgErrors = 0u;
uint8_t gu8_vRegAddress = 0u;

static void ComposeDynMsg(E_NFC_msgIdType l_vMsgId);

/*******************************************************************************
 **    Function Definitions
 ******************************************************************************/

/*******************************************************************************
 ** Function name    : CDD_NFC_Task
 ** Description      : Entry point for NFC CDD. All required tasks in NFC CDD
 **                    is handled from here
 ** Parameters       : void
 ** Return value     : void
 ******************************************************************************/
void CDD_NFC_Task(void)
{
    /* Check if there are any sequences pending for decoding */
    if(genu_vNFCTrxStatus != CDD_TRX_NOT_STARTED)
    {
        /* Disable interrupts before critical section.
         * Critical section is for decoding the data received in previous cycle */
        NFC_DisableAllInterrupts();

        /* Decode sequence sent (if any) in the previous cycle */
        SeqsDecode();

        /* Update the NFC CDD State in response to various events */
        Update_NFCCDDState();

        /* Clear all flags and parameters in preparation for the next cycle */
        NFC_clearAllParam();
    }
    else
    {
        ;/* Do Nothing */
    }
    /* Take required action as per the state of NFC CDD */
    CDD_NFC_ModeManager();
}

/*******************************************************************************
 ** Function name    : ClearFaultFlags
 ** Description      : Clears all fault flags
 ** Parameters       : void
 ** Return value     : void
 ******************************************************************************/
void ClearFaultFlags(void)
{
    /* Except state error */
    gu8_vSavedCDDErrors = guni_vCDDErrors.u8;
    guni_vCDDErrors.u8 = 0x0u;
    gu8_fErrorAggregate = FALSE;
    gu8_vSavedMsgErrors = gauni_vMsgErrors.u8;
    gauni_vMsgErrors.u8 = 0u;
}

/*******************************************************************************
 ** Function name    : SeqsDecode
 ** Description      : Decodes data received in previous scheduler cycle
 ** Parameters       : void
 ** Return value     : void
 ******************************************************************************/
void SeqsDecode(void)
{
    E_NFC_seqIdType lenu_vCurrSeqId;

    /* Check if any MCAL error has occurred */
    if(genu_vNFCTrxStatus != CDD_TRX_MCALERROR)
    {
        /* Check if SequenceIncomplete error has occurred */
        if(genu_vNFCTrxStatus != CDD_TRX_INPROGRESS)
        {
            // Discard first message so that Tx and Rx buffers are aligned
            NFC_popFromRxBuff(NULL, 1u);

            /* Since no error has occurred, start decoding sequence wise */
            while(gu8_cSeqQIndex < gu8_cSeqQCount)
            {
                /* Extract sent sequence ID from queue */
                lenu_vCurrSeqId = gaenu_vSeqsToSend[gu8_cSeqQIndex];

                // Start parsing the other read messages
                ParseMsg(gastr_vSeqList[lenu_vCurrSeqId].msgList,
                        (uint32_t)gastr_vSeqList[lenu_vCurrSeqId].msgCount);
                gu8_cSeqQIndex++;
            }
        }
        else
        {
            /* #ERROR: Sequence Transmission incomplete */
            guni_vCDDErrors.bf.SEQINCOMP = TRUE;
            gu8_fErrorAggregate = TRUE;
        }
    }
    else
    {
        /* #ERROR: SPI communication error */
        guni_vCDDErrors.bf.SPICOMM = TRUE;
        gu8_fErrorAggregate = TRUE;
    }
}

/*******************************************************************************
 ** Function name    : Update_NFCCDDState
 ** Description      : This function updates the NFC CDD States based on
 **                    various conditions
 ** Parameters       : void
 ** Return value     : void
 ******************************************************************************/
void Update_NFCCDDState(void)
{
    /* Highest priority for RUN and STANDBY states */
    switch(genu_vNFCCDDState)
    {
    case CDD_INIT:
        if(genu_vNFCInitStatus == NFCINITSTATUS_STARTED)
        {
            /* Check if any CDD fault is present(other than State error) */
            if(TRUE == gu8_fErrorAggregate)
            {
                /* Check if initialization can be retried */
                if(gu8_cNFCInitRtrCnt < MAXNFCRESETCNT)
                {
                    /* Increment Retry Count since we have retries left */
                    gu8_cNFCInitRtrCnt++;
                    genu_vNFCInitStatus = NFCINITSTATUS_DEVICEID_READ;
                }
                else
                {
                    /* Put NFC CDD into STANDBY, wait for ASW to take action */
                    genu_vNFCCDDState = CDD_STANDBY;
                    genu_vNFCInitStatus = NFCINITSTATUS_DEVICEID_READ;
                }
            }
            else
            {
                /* Initialization successful. Put NFC CDD into RUN state */
                genu_vNFCCDDState = CDD_RUN;
                genu_vNFCInitStatus = NFCINITSTATUS_NOTSTARTED;
            }
        }
        else
        {
            // @TODO NFC PREINIT: Handle errors in reading pre-init sequence
        }
        break;

    case CDD_SHUTDOWN:
        /* Check if any fault occurred during PDT sequence decoding */
        if(TRUE == gu8_fErrorAggregate)
        {
            /* Check if shutdown can be retried */
            if(gu8_cNFCShutRtrCnt < MAXNFCSHUTRTRYCNT)
            {
                gu8_cNFCShutRtrCnt++;
                genu_vNFCShutStatus = CDD_TRX_NOT_STARTED;
            }
            else
            {
                /* PDT sequence failed, put NFC CDD into STANDBY anyway */
                genu_vNFCShutStatus = CDD_TRX_COMPLETE;
                genu_vNFCCDDState = CDD_STANDBY;
                /* Disable all interrupts i.e. GPT0, INTP4, DMA9 and CSIH0DCE */
                NFC_DisableAllInterrupts();
            }
        }
        else
        {
            /* Since there were no faults, put NFC CDD into STANDBY */
            genu_vNFCShutStatus = CDD_TRX_COMPLETE;
            genu_vNFCCDDState = CDD_STANDBY;
            /* Disable all interrupts */
            NFC_DisableAllInterrupts();
        }
        break;

    default:
    {
        ; /* Do Nothing */
    }
    break;
    }
}

/*******************************************************************************
 ** Function name    : CDD_NFC_ModeManager
 ** Description      : This function manages the different tasks to be executed
 **                  : in different modes of NFC CDD
 ** Parameters       : void
 ** Return value     : void
 ** Remarks          :
 ******************************************************************************/
void CDD_NFC_ModeManager(void)
{
    switch(genu_vNFCCDDState)
    {
    case CDD_STANDBY:
        /* DO NOTHING */
        break;

    case CDD_INIT:
        /* Composing & starting init Sequence */
        CDD_NFC_StartInitSequence();
        break;

    case CDD_RUN:
        break;

    case CDD_SHUTDOWN:
        /* Composing & starting POWERDOWN Sequence */
        CDD_NFC_StartPDSequence();
        break;

    default:
    {
        ; /* Do Nothing */
    }
    break;
    }
}

/*******************************************************************************
 ** Function name    : CDD_NFC_StartInitSequence
 ** Description      : Initiates sending of Init sequence to MFRC522
 ** Parameters       : void
 ** Return value     : void
 ** Remarks          :
 ******************************************************************************/
void CDD_NFC_StartInitSequence(void)
{
    /* Clear flags and parameters before sending sequences */
    ClearFaultFlags();
    if(genu_vNFCInitStatus == NFCINITSTATUS_NOTSTARTED)
    {
        AddSeqToQueue(&gastr_vSeqList[MC_SOFTRESET]);
        genu_vNFCInitStatus = NFCINITSTATUS_DEVICEID_READ;
        NFC_DriverStartTransmit();
    }
    else if(genu_vNFCInitStatus == NFCINITSTATUS_DEVICEID_READ)
    {
        if((gau8_vMFRC522_rb[0u][MC_REGINDEX_VERSIONREG] & 0xFFu) == 0x82U)
        {
            genu_vNFCInitStatus = NFCINITSTATUS_STARTED;
            AddSeqToQueue(&gastr_vSeqList[MC_READDEVID]);
            NFC_DriverStartTransmit();
        }
        else
        {
            AddSeqToQueue(&gastr_vSeqList[MC_READDEVID]);
            genu_vNFCInitStatus = NFCINITSTATUS_DEVICEID_READ;
            NFC_DriverStartTransmit();
        }
    }
    else
    {
        return;
    }
}

/*******************************************************************************
 ** Function name    : CDD_NFC_StartPDSequence
 ** Description      : Initiates sending of Power-down sequence to MFRC522
 ** Parameters       : void
 ** Return value     : void
 ** Remarks          :
 ******************************************************************************/
void CDD_NFC_StartPDSequence(void)
{
    /* TODO: add a power-down sequence to gastr_vSeqList and queue it here */
}


/*******************************************************************************
 ** Function name    : AddSeqToQueue
 ** Description      : Adds sequence pointed by currSeq to Transmission Queue
 ** Parameters       : const NFC_seqType * const lps_vCurrSeq
 ** Return value     : void
 ** Remarks          :
 ******************************************************************************/
void AddSeqToQueue(const NFC_seqType * const lps_vCurrSeq)
{
	E_NFC_msgIdType lu32_vMsgId;
	NFC_msg16Type luni_vTempMsg;                                               // PRQA S:UnionType 0759 # Union required for simplifying code

	gaenu_vSeqsToSend[gu8_cSeqQCount++] = lps_vCurrSeq->id;
	for(uint32 lu32_cMsgIndex = 0u; lu32_cMsgIndex < lps_vCurrSeq->msgCount; lu32_cMsgIndex++)
	{
		/* Extract message ID which is to be transmitted */
		lu32_vMsgId = lps_vCurrSeq->msgList[lu32_cMsgIndex];
		if(lu32_vMsgId < RMSG_LAST)
		{
			/* Since its neither a static message nor a read message,
			 * it must be a dynamic read Data/Reg address message. Pass it to ComposeDynMsg_RD_WR() */
			/* Check what type of message this is */
			luni_vTempMsg.u16 = gauni_vMsgsList[lu32_vMsgId].u16;
			NFC_addToTxBuff(&luni_vTempMsg.u16, lu32_vMsgId);
		}
		else if(lu32_vMsgId < SMSG_LAST)
		{   /* Its a static message. Add it directly */
			luni_vTempMsg.u16 = gauni_vMsgsList[lu32_vMsgId].u16;
			NFC_addToTxBuff(&luni_vTempMsg.u16, lu32_vMsgId);
		}
		else if(lu32_vMsgId < DMSG_LAST)
		{
			/* Since its neither a static message nor a read message,
			 * it must be a dynamic write Data only message. Pass it to ComposeDynMsg() */
			ComposeDynMsg(lu32_vMsgId);
		}
		else
		{
			/* Since its neither a static message nor a read message,
			 * it must be a dynamic write Data/Reg address message. Pass it to ComposeDynMsg_RD_WR() */
			ComposeDynMsg_RD_WR(lu32_vMsgId);
		}
	}
}

/*******************************************************************************
 ** Function name    : ParseMsg
 ** Description      : Parse 48bit message to find various errors
 ** Parameters       : const NFC_seqType * const lptrstr_vCurrSeq
 ** Return value     : void
 ** Remarks          : -
 ******************************************************************************/
void ParseMsg(const E_NFC_msgIdType lptr_vMsgList[], const uint32 lu32_vMsgCount)
{
    uint32_t lu32_cMsgIndex = 0u;
    E_NFC_msgIdType lenu_vMsg;
    uint8_t data;
    E_NFC_regIndexType regIndex;
//    NFC_msgErrorType *lptruni_fMsgErrors = &gauni_vMsgErrors;

    while(lu32_cMsgIndex < lu32_vMsgCount)
    {
        lenu_vMsg = lptr_vMsgList[lu32_cMsgIndex];

        if(lenu_vMsg < RMSG_LAST)
        {
        	NFC_popFromRxBuff(&data, 1);
        	regIndex = genu_MCRegAddrToIndex[(((gauni_vMsgsList[lenu_vMsg].u8[0]) >> 1u) & 0x3Fu)];
        	gau8_vMFRC522_rb[0u][regIndex] = data;
        }
//        else
//        {
//        	/* #ERROR: DEV ID mismatch */
//        	lptruni_fMsgErrors->m48.DEVIDERR = TRUE;
//        	gu8_fErrorAggregate = TRUE;
//        }
//        	            }
//            else
//            {
//                /* #ERROR: CRC mismatch */
//                lptruni_fMsgErrors->m48.CRCERROR = TRUE;
//                gu8_fErrorAggregate = TRUE;
////            }
//        }
        else
        {
        	/* We have the number of static messages. Discard their response */
        	NFC_popFromRxBuff(NULL, 2);
        }
        lu32_cMsgIndex++;
    }
}

/*******************************************************************************
 ** Function name    : ComposeDynMsg
 ** Description      : Composes a Dynamic Message taking data from MFRC522Reg_WR
 **                  : array in preparation for transmission
 ** Parameters       : E_NFC_msgIdType
 ** Return value     : void
 ** Remarks          :
 ******************************************************************************/
static void ComposeDynMsg(E_NFC_msgIdType l_vMsgId)
{
    NFC_msg16Type luni_vMsg;
    E_NFC_regIndexType regIndex;

    /* Extract the message list to local variable */
    luni_vMsg.u16 = gauni_vMsgsList[l_vMsgId].u16;
    /* Calculate CRC of selected message and add it to buffer */
    regIndex = genu_MCRegAddrToIndex[luni_vMsg.bf.reg];
    luni_vMsg.bf.data = gau8_vMFRC522_wr[0u][regIndex];
    NFC_addToTxBuff(&luni_vMsg.u16, l_vMsgId);
}

/*******************************************************************************
 ** Function name    : ComposeDynMsg_RD_WR
 ** Description      : Composes a Dynamic Message taking data from MFRC522Reg_WR
 **                  : array in preparation for transmission
 ** Parameters       : E_NFC_msgIdType
 ** Return value     : void
 ** Remarks          :
 ******************************************************************************/
void ComposeDynMsg_RD_WR(E_NFC_msgIdType l_vMsgId)
{
    NFC_msg16Type luni_vMsg;
    E_NFC_regIndexType regIndex;

    /* Extract the message list to local variable */
    luni_vMsg.u16 = gauni_vMsgsList[l_vMsgId].u16;
    luni_vMsg.bf.reg = gu8_vRegAddress;
    if(luni_vMsg.bf.rw == MC_NFC_READ)
    {
        luni_vMsg.bf.data = 0x00u;
    }
    else if(luni_vMsg.bf.rw == MC_NFC_WRITE)
    {
        regIndex = genu_MCRegAddrToIndex[luni_vMsg.bf.reg];
        luni_vMsg.bf.data = gau8_vMFRC522_wr[0u][regIndex];
    }
    else
    {
        ;/* Do Nothing */
    }
    NFC_addToTxBuff(&luni_vMsg.u16, l_vMsgId);
}
