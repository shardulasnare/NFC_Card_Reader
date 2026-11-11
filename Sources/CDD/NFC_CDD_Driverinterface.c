/*
 * NFC_CDD_Driverinterface.c
 *
 *  Created on: 30-Aug-2026
 *      Author: Shardul
 */
#include "NFC_CDD_Middleware.h"
#include "NFC_CDD_Driverinterface.h"
#include "NFC_CDD_DevCfg.h"

/*******************************************************************************
 **    Global Variable declarations
 ******************************************************************************/
static uint32_t su32_vSpiTxIndex; /*
    Description: Index of transmission buffer from message to be transmitted. */
static uint32_t su32_vSpiRxIndex; /*
    Description: Index of receive buffer where received message will be stored. */
static volatile uint32_t su32_vTrxIndex;
                                                                               // PRQA S 1504++
volatile uint8_t gau8_vNFCTxBuff[NFC_BUFF_LEN]; /*
    Description: Buffer for messages to be transmitted to the ASIC */
volatile uint8_t gau8_vNFCRxBuff[NFC_BUFF_LEN]; /*
    Description: Buffer for messages received from the ASIC */
                                                                               // PRQA S 1504--
E_NFC_seqIdType gaenu_vSeqsToSend[MC_LAST_SEQ]; /*
    Description: The list of sequences to be sent in current transmission cycle */
uint8_t gu8_cSeqQCount = 0; /*
    Description: Number of sequences in the transmission queue */
uint8_t gu8_cSeqQIndex;
volatile CDD_TrxStatus genu_vNFCTrxStatus;
static volatile uint8 su8_vCurrSeqTransmitIndex = 0u;
/*******************************************************************************
 **    Function Definitions
 ******************************************************************************/
/*******************************************************************************
 ** Function name    : NFC_clearAllParam
 ** Description      : Resets flags and variables used in NFC CDD
 ** Parameters       : void
 ** Return value     : void
 ******************************************************************************/
void NFC_clearAllParam(void)
{
    uint32_t lu32_cIndex;
    /* Since decoding is complete, reset the TRX Status */
    genu_vNFCTrxStatus = CDD_TRX_NOT_STARTED;
    gu8_cSeqQCount = 0u;
    gu8_cSeqQIndex = 0u;

    su32_vSpiTxIndex = 0u;
    su32_vSpiRxIndex = 0u;

    for(lu32_cIndex = 0u; lu32_cIndex < MC_LAST_SEQ; lu32_cIndex++)
    {
        gaenu_vSeqsToSend[lu32_cIndex] = (E_NFC_seqIdType)0xFFu;
    }
}

/*******************************************************************************
 ** Function name    : NFC_startTransmit
 ** Description      : Wrapper for starting transmission to MC33772C
 **                  : Assumes data is already in buffer and starts TRX
 ** Parameters       : void
 ** Return value     : void
 ******************************************************************************/
void NFC_DriverStartTransmit(void)
{
	su8_vCurrSeqTransmitIndex = 0u;
	su32_vTrxIndex = 0u;
	const NFC_seqType *lstr_vSeq = &gastr_vSeqList[gaenu_vSeqsToSend[su8_vCurrSeqTransmitIndex]];
	uint16_t lu16_vFrameCnt = lstr_vSeq->msgCount;
    genu_vNFCTrxStatus = CDD_TRX_INPROGRESS;
    if(lstr_vSeq->FrameType == WRITE_SEQUENCES) lu16_vFrameCnt *= 2u;

    /* Configure EDMA with the number of subsequences and the length of first
     * subsequence to be transmitted */
    (void)LPSPI_DRV_MasterTransfer(LPSPINFC, (const uint8_t *)&gau8_vNFCTxBuff[su32_vTrxIndex], (uint8_t *)&gau8_vNFCRxBuff[su32_vTrxIndex],  lu16_vFrameCnt);
}

/*******************************************************************************
 ** Function name    : NFC_addToTxBuff
 ** Description      : Add 32Bit data "lu32_vSrcData" to Transmission buffer
 ** Parameters       : lu32_vSrcData - 32bit word of data to be added
 ** Return value     : void
 ******************************************************************************/
void NFC_addToTxBuff(const uint16_t *lu16_vSrcData, E_NFC_msgIdType lu32_vMsgId)
{
    volatile uint8_t *src;
    volatile uint8_t *dst;
    uint8_t lu8_vMsgLen = (lu32_vMsgId < RMSG_LAST) ? 1u : 2u;
    /* Check if data to be added overflows the buffer */
    if((su32_vSpiTxIndex + lu8_vMsgLen) < NFC_BUFF_LEN)
    {
    	dst = (uint8_t *)&gau8_vNFCTxBuff[su32_vSpiTxIndex];
    	src = (uint8_t *)lu16_vSrcData;
    	if(lu8_vMsgLen == 1u){
    		dst[0] = src[0];
    	}
    	else
    	{
    		dst[0u] = src[1u];
    		dst[1u] = src[0u];
    	}
    	su32_vSpiTxIndex += lu8_vMsgLen;
    }
    else
    {
    	; /* Nothing to do here */
    }

}

/*******************************************************************************
 ** Function name    : NFC_popFromRxBuff
 ** Description      : Pops data from SPI Receive buffer to "lptru32_vDst"
 **                  : assuming destination is (lu8_vCount * 32) bits long
 ** Parameters       : uint32_t *const lptru32_vDst, uint8 const lu8_vCount
 ** Return value     : void
 ** Remarks          :
 ******************************************************************************/
void NFC_popFromRxBuff(uint8_t *lptru8_vDst,  uint32_t lu32_vLength)
{
    uint32_t lu32_cLenCtr;
    volatile uint8_t *src;
    volatile uint8_t *dst;
    /* Check if we have as much data as requested and ensure it doesn't
     * underflow the buffer */
    if((su32_vSpiRxIndex + lu32_vLength) > su32_vSpiTxIndex)
    {
        ; /* Nothing to do here */
    }
    else if(NULL == lptru8_vDst)
    {
        /* If null pointer is passed as destination, data is discarded
         * i.e. stack index is incremented */
        su32_vSpiRxIndex += lu32_vLength;
    }
    else
    {
        /* If data is to be written to buffer, this function will copy
         * lu32_cLenCtr count of values */
        src = (uint8_t *)&gau8_vNFCRxBuff[su32_vSpiRxIndex];
        dst = lptru8_vDst;
        for(lu32_cLenCtr = 0u; lu32_cLenCtr < lu32_vLength; lu32_cLenCtr++)
        {
            dst[0u] = src[0u];
            dst ++;
            src ++;
        }
        su32_vSpiRxIndex += lu32_vLength;
    }
}

/*******************************************************************************
 ** Function name    : NFC_DisableAllInterrupts
 ** Description      : Disables all interrupts used in NFC CDD
 ** Parameters       : void
 ** Return value     : void
 ******************************************************************************/
void NFC_DisableAllInterrupts(void)
{
    /* Disable LPSPI Interrupts */
    LPSPI_HAL_SetIntMode(IP_LPSPI_1, LPSPI_RX_DATA_INT_ENABLE, false);                     // PRQA S:HWRegAccess 0306 # Typecast required for h/w register access
    LPSPI_HAL_SetIntMode(IP_LPSPI_1, LPSPI_ERROR_INT_ENABLE, false);
    LPSPI_HAL_SetIntMode(IP_LPSPI_1, LPSPI_TX_DATA_INT_ENABLE, false);
}

/*******************************************************************************
 ** Function name    : lpspiNFCCB
 ** Description      : Callback for after transfer completion
 ** Parameters       : transfer_status_t status
 ** Return value     : void
 ******************************************************************************/
void lpspiNFCCB(transfer_status_t status)
{
    const NFC_seqType *lstr_vSeq = &gastr_vSeqList[gaenu_vSeqsToSend[su8_vCurrSeqTransmitIndex]];
    if(status != LPSPI_TRANSFER_OK)
    {
        genu_vNFCTrxStatus = CDD_TRX_MCALERROR;
        return;     // Early return in case of error
    }
    else
    {
        // Do Nothing
    }
    if(lstr_vSeq->FrameType == READ_SEQUENCES){
    	su32_vTrxIndex += lstr_vSeq->msgCount;
    }
    else{
    	su32_vTrxIndex += (lstr_vSeq->msgCount * 2);
    }
    /* Check if a delay is required before the next transfer */
    if(0u != (*lstr_vSeq).timeStall)
    {
    	/* Need to replace logic as per STM32F411RE */
        /* Enable timer for MC33772C conversion to get over */
        /* TODO: no GPT driver yet - count, gastr_vSubSeqParam, su32_vSubSeqIndex and Gpt_* are undefined */
        // count = (gastr_vSubSeqParam[su32_vSubSeqIndex].timeStall * 30u) - 1u;
        // Gpt_EnableNotification(GptConf_GptChannelConfiguration_CDD_NFC_TransferStallTimer);
        // Gpt_StartTimer(GptConf_GptChannelConfiguration_CDD_NFC_TransferStallTimer, count);
        su8_vCurrSeqTransmitIndex ++;
    }
    else
    {
    	su8_vCurrSeqTransmitIndex ++;
        /* Check if there is anything to be transmitted */
        if(su8_vCurrSeqTransmitIndex <  gu8_cSeqQCount)
        {
            (void)LPSPI_DRV_MasterTransfer(LPSPINFC, (uint8_t *)&gau8_vNFCTxBuff[su32_vTrxIndex], (uint8_t *)&gau8_vNFCRxBuff[su32_vTrxIndex], // PRQA S:IntentionalTypecast 0312 # Typecast from volatile acceptable
            gastr_vSeqList[gaenu_vSeqsToSend[su8_vCurrSeqTransmitIndex]].msgCount);
        }
        else
        {
            /* Reset the subsequence parameters since next sequence may be
             * added at any time after this point */
            su8_vCurrSeqTransmitIndex = 0u;
            su32_vTrxIndex = 0u;
            /* Mark this transfer as complete */
            genu_vNFCTrxStatus = CDD_TRX_COMPLETE;
        }
    }
}

/******************************************************************************
 * End of File
 *****************************************************************************/
