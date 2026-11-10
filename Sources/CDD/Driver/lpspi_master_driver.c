/*
 * lpspi_master_driver.c
 *
 *  Created on: 30-Aug-2026
 *      Author: Shardul
 */

#include "lpspi_master_driver.h"
#include "SysTimer.h"
#include "Mcu.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*******************************************************************************
 * Variables
 ******************************************************************************/
static LPSPI_Type * g_lpspiBase[LPSPI_INSTANCE_COUNT] =  IP_LPSPI_BASE_PTRS;

static lpspi_state_t * g_lpspiStatePtr[LPSPI_INSTANCE_COUNT] = {NULL, NULL, NULL, NULL};

/* SPI error counter */
volatile uint8 spiErrCounter[LPSPI_INSTANCE_COUNT] = {0};
lpspi_ctrl1reg_config_t Ctrl1CfgSet[LPSPI_INSTANCE_COUNT];
volatile uint16_t g_spi_byte_count  = 0U; /* Shared between SPI ISR and Normal Code */
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
status_t LPSPI_DRV_MasterDeinit(uint8_t instance)
{
    LPSPI_Type *base = g_lpspiBase[instance];
    status_t errorCode = STATUS_SUCCESS;

    /* Disable clock and Module of LPSPI */
    LPSPI_HAL_DeInit(base, instance);
    /* Disable the interrupt */
    Platform_SetIrq(g_lpspiIrqId[instance], 0);
    /* Clear the state pointer. */
    g_lpspiStatePtr[instance] = NULL;
    return errorCode;
}

status_t LPSPI_DRV_MasterTransfer(uint32 instance,                /*PRQA S 2889 # The return statement before end of function is used for simpler code structure and better readability. */
                                        const volatile uint8_t * const sendBuffer,
										volatile  uint8_t * receiveBuffer,
                                        uint16_t frameCount)
{

    /* Instantiate local variable of type lpspi_master_state_t and point to global state */
    lpspi_state_t * lpspiState = g_lpspiStatePtr[instance];
    LPSPI_Type *base = g_lpspiBase[instance];

    /* Check that we're not busy. */
    if (LPSPI_HAL_GetStatusFlag(base, STATUS_LPSPI_MODULE_BUSY))
    {
        lpspiState->status = LPSPI_TRANSMIT_FAIL;
        //Abort the transfer
       (void)LPSPI_DRV_MasterAbortTransfer(instance);
       //Call the callback function
        lpspiState->callBack(lpspiState->status);
        return STATUS_BUSY;
    }

    /* Clear all interrupts sources */
    (void)LPSPI_HAL_ClearStatusFlag(base, LPSPI_ALL_STATUS);
    (void)base->SR;
    /* Enable fault interrupts sources */
    LPSPI_HAL_SetIntMode(base, LPSPI_ERROR_INT_ENABLE, true);  // PRQA S 1295 # The cast is required to perform a conversion between an unsigned integer and an enum type.
    /* Make CS Low */
    cs_low();
    /* Fill out the other members of the run-time state structure. */
    lpspiState->txBuff = sendBuffer;
    lpspiState->rxBuff = receiveBuffer;
    lpspiState->txFrameCnt = frameCount;
    g_spi_byte_count = 0U;

    // TODO SPI: Replace with HAL
    if(lpspiState->transferType == LPSPI_USING_INTERRUPTS)
    {
        /* Check RX and TX buffers */
        while(~(((base->SR) >> 1U) & 1U));
        while(((base->SR) & 1U));
        LPSPI_HAL_SetIntMode(base, LPSPI_RX_DATA_INT_ENABLE, true);
        LPSPI_HAL_SetControlReg1(base, &Ctrl1CfgSet[instance]);
        LPSPI_HAL_WriteData(base, lpspiState->txBuff[g_spi_byte_count]);
        lpspiState->status = LPSPI_TRANSFER_IN_PROGRESS;
    }
    else
    {
#if 0
        DmaTransferReq_t rxCfg;
        rxCfg.channel = lpspiState->rxDMAChannel;
        rxCfg.srcAddr = (uint32_t *)&base->RDR;
        rxCfg.dstAddr = (uint32_t *)lpspiState->rxBuff;
        rxCfg.numTransfers = frameCount;
        // Multiple by 4 to get the number of bytes to transmit/receive in single frame.
        rxCfg.frameSize = frames << 2u;

        DmaTransferReq_t txCfg;
        txCfg.channel = lpspiState->txDMAChannel;
        txCfg.srcAddr = (uint32_t *)lpspiState->txBuff;
        txCfg.dstAddr = (uint32_t *)&base->TDR;
        txCfg.numTransfers = frameCount;
        txCfg.frameSize = frames << 2u;

        DMA_ConfigTransfer(&txCfg);
        DMA_ConfigTransfer(&rxCfg);

        LPSPI_HAL_SetTxCommandReg(base, &txCmdCfgSet[instance]);

        LPSPI_HAL_SetTxDmaCmd(base, true);
        LPSPI_HAL_SetRxDmaCmd(base, true);
#endif
        ; // Do nothing
    }

    /* Else, return immediately as this is an async transfer */
    return STATUS_SUCCESS;
}

void LPSPI_DRV_SingleFrameTransfer(uint32_t instance, const uint8_t * sendBuffer)
{
    /* Instantiate local variable of type dspi_master_state_t and point to global state */
    lpspi_state_t * lpspiState = g_lpspiStatePtr[instance];
    LPSPI_Type *base = g_lpspiBase[instance];

    (void)LPSPI_HAL_ClearStatusFlag(base, LPSPI_ALL_STATUS);

    /* Clean RX and TX buffers */
    (void)base->DR;
    (void)base->SR;
    /* The second flush command is used to avoid the case when one word is still in shifter. */
    (void)base->DR;
    (void)base->SR;
    g_spi_byte_count = 0U;
    LPSPI_HAL_SetControlReg1(base, &Ctrl1CfgSet[instance]);
    LPSPI_HAL_WriteData(base, sendBuffer[g_spi_byte_count]);
    lpspiState->status = LPSPI_TRANSFER_IN_PROGRESS;
}

transfer_status_t LPSPI_DRV_GetTransferResult(uint32_t instance, uint8_t *rxData)
{
    lpspi_state_t * lpspiState = g_lpspiStatePtr[instance];
    LPSPI_Type *base = g_lpspiBase[instance];

    if((lpspiState->transferType == LPSPI_USING_INTERRUPTS) || (lpspiState->transferType == LPSPI_USING_DMA))
    {
        // For these types of transfers, status should be updated in interrupts
        // Received data should be updated in the buffers already provided
        return lpspiState->status;
    }

    if (LPSPI_HAL_GetStatusFlag(base, STATUS_LPSPI_FRAME_ERROR) || LPSPI_HAL_GetStatusFlag(base, STATUS_LPSPI_OVR) || \
        LPSPI_HAL_GetStatusFlag(base, STATUS_LPSPI_UDR) ||  LPSPI_HAL_GetStatusFlag(base, STATUS_LPSPI_MODE_FAULT))
    {
        lpspiState->status = LPSPI_TRANSFER_ERROR;
    }
    else if(LPSPI_HAL_GetStatusFlag(base, STATUS_LPSPI_TXE))
    {
        lpspiState->status = LPSPI_TRANSFER_OK;
    }
    else
    {
        lpspiState->status = LPSPI_TRANSFER_IN_PROGRESS;
    }
    if(lpspiState->status == LPSPI_TRANSFER_OK)
    {
        rxData[g_spi_byte_count] = LPSPI_HAL_ReadData(base);
    }
    return lpspiState->status;
}

static status_t LPSPI_DRV_MasterAbortTransfer(uint32_t instance)
{
    LPSPI_Type *base = g_lpspiBase[instance];
    lpspi_state_t *lpspiState = g_lpspiStatePtr[instance];

    if (lpspiState->transferType == LPSPI_USING_INTERRUPTS)
    {
        /* Disable interrupts */
        LPSPI_HAL_SetIntMode(base, LPSPI_RX_DATA_INT_ENABLE, false);
    }
    else
    {
        ; // Do nothing
    }

    LPSPI_HAL_SetIntMode(base, LPSPI_ERROR_INT_ENABLE, false);

    lpspiState->isTransferInProgress = false;
    return STATUS_SUCCESS;
}

void SPI1_IRQHandler(void)
{
    lpspi_state_t * lpspiState = g_lpspiStatePtr[instance];
    LPSPI_Type *base = g_lpspiBase[instance];

    /* If an error is detected the transfer will be aborted */
    if (LPSPI_HAL_GetStatusFlag(base, STATUS_LPSPI_FRAME_ERROR) || LPSPI_HAL_GetStatusFlag(base, STATUS_LPSPI_OVR) || LPSPI_HAL_GetStatusFlag(base, STATUS_LPSPI_MODE_FAULT))
    {
        spiErrCounter[instance]++;
        LPSPI_HAL_SetIntMode(base, LPSPI_RX_DATA_INT_ENABLE, false);
        lpspiState->status = LPSPI_TRANSFER_ERROR;
        if(lpspiState->callBack != NULL_PTR)
        {
            lpspiState->callBack(lpspiState->status);
        }
        // Change status only if new transfer has not been started
        if(lpspiState->status != LPSPI_TRANSFER_IN_PROGRESS)
        {
            lpspiState->status = LPSPI_TRANSFER_NOT_STARTED;
        }
        return;
    }
    LPSPI_HAL_SetIntMode(base, LPSPI_RX_DATA_INT_ENABLE, false);
    lpspiState->rxBuff[g_spi_byte_count] = LPSPI_HAL_ReadData(base);
    g_spi_byte_count++;
    lpspiState->txFrameCnt --;
    if(lpspiState->txFrameCnt != 0u)
    {
         LPSPI_HAL_WriteData(base, lpspiState->txBuff[g_spi_byte_count]);
         LPSPI_HAL_SetIntMode(base, LPSPI_RX_DATA_INT_ENABLE, true);
    }
    else
    {
    	LPSPI_HAL_SetIntMode(base, LPSPI_RX_DATA_INT_ENABLE, false);
    	LPSPI_HAL_SetIntMode(base, LPSPI_ERROR_INT_ENABLE, false);
    	while(LPSPI_HAL_GetStatusFlag(base,STATUS_LPSPI_MODULE_BUSY));
        lpspiState->status = LPSPI_TRANSFER_OK;
        cs_high();
        if(lpspiState->callBack != NULL_PTR)
        {
            lpspiState->callBack(lpspiState->status);
        }
        // Change status only if new transfer has not been started
        if(lpspiState->status != LPSPI_TRANSFER_IN_PROGRESS)
        {
            lpspiState->status = LPSPI_TRANSFER_NOT_STARTED;
        }
    }
}

#if 0
void LPSPI_DMA_Notification(DmaChStatus_t status, DmaSource_t source)
{
    //Calculates the LPSPI instance using the DMA Source
    //Divided by 2 because each LPSPI instance has two DMA sources (Rx and Tx)
    uint8 instance = (source - DMAMUX0_SOURCE_LPSPI_0_TX) / 2u;

    lpspi_state_t * lpspiState = g_lpspiStatePtr[instance];
    LPSPI_Type * base = g_lpspiBase[instance];

    LPSPI_HAL_SetRxDmaCmd(base, false);
    LPSPI_HAL_SetTxDmaCmd(base, false);

    if(status == DMA_STATUS_ERROR)
    {
        lpspiState->status = LPSPI_TRANSFER_ERROR;
    }
    else
    {
        lpspiState->status = LPSPI_TRANSFER_OK;
    }

    if(lpspiState->callBack != NULL_PTR)
    {
        lpspiState->callBack(lpspiState->status);
    }

    // Change status only if new transfer has not been started
    if(lpspiState->status != LPSPI_TRANSFER_IN_PROGRESS)
    {
        lpspiState->status = LPSPI_TRANSFER_NOT_STARTED;
    }
}
#endif

void LPSPI_DRV_MasterInitCfg(uint8_t lu8_vInstance, lpspi_state_t *lstr_vStatePtr, const lpspi_master_config_t *lstr_vMasterConfig)
{
    LPSPI_Type * base = g_lpspiBase[lu8_vInstance];
    g_lpspiStatePtr[lu8_vInstance] = lstr_vStatePtr;

    // Reset LPSPI module and bring it out of reset.
    // Also disables Debug Mode and Disables the module
    LPSPI_HAL_Init(base, lu8_vInstance);
    LPSPI_HAL_SetMasterSlaveMode(base, true);
    LPSPI_HAL_SetPcsHandling(base,true);
    LPSPI_HAL_Enable(base);    // Enable Module

    Ctrl1CfgSet[lu8_vInstance].lsbFirst = lstr_vMasterConfig->lsbFirst;
    Ctrl1CfgSet[lu8_vInstance].BR =  lstr_vMasterConfig->baudrate;
    Ctrl1CfgSet[lu8_vInstance].clkPhase = lstr_vMasterConfig->clkPhase;
    Ctrl1CfgSet[lu8_vInstance].clkPolarity = lstr_vMasterConfig->clkPolarity;

    lstr_vStatePtr->callBack = lstr_vMasterConfig->callBack;
    lstr_vStatePtr->transferType = lstr_vMasterConfig->transferType;
    lstr_vStatePtr->bitsPerFrame = lstr_vMasterConfig->bitcount;
    lstr_vStatePtr->status = LPSPI_TRANSFER_NOT_STARTED;
#if 0
    lstr_vStatePtr->rxDMAChannel = lstr_vMasterConfig->rxDMAChannel;
    lstr_vStatePtr->txDMAChannel = lstr_vMasterConfig->txDMAChannel;
#endif
}
