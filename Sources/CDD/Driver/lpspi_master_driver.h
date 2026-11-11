/*
 * lpspi_master_driver.h
 *
 *  Created on: 30-Aug-2026
 *      Author: Shardul
 */

#ifndef SOURCES_CDD_DRIVER_LPSPI_MASTER_DRIVER_H_
#define SOURCES_CDD_DRIVER_LPSPI_MASTER_DRIVER_H_

#include "Std_Define.h"
#include "lpspi_hal.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
extern volatile uint8_t spiErrCounter[LPSPI_INSTANCE_COUNT];

typedef enum
{
    LPSPI_USING_NONE        = 0U,    /*!< The driver supports only a single frame transfer for each request */
    LPSPI_USING_BLOCKING,           /*!< The driver will block until completion of all frames */
    LPSPI_USING_DMA,                /*!< The driver will use DMA to perform SPI transfer */
    LPSPI_USING_INTERRUPTS,         /*!< The driver will use interrupts to perform SPI transfer */
} lpspi_transfer_type;

typedef enum
{
    LPSPI_TRANSFER_NOT_STARTED = 0U,    /*!< Transfer not started */
    LPSPI_TRANSFER_IN_PROGRESS,         /*!< Transfer is in progress */
    LPSPI_TRANSFER_OK,                  /*!< Transfer OK */
    LPSPI_TRANSFER_ERROR,               /*!< Error during transmission */
    LPSPI_TRANSMIT_FAIL,                /*!< Error during transmission */
    LPSPI_RECEIVE_FAIL                  /*!< Error during reception */
} transfer_status_t;

typedef void (*lpspi_callback_t)(transfer_status_t status);
typedef struct
{
    uint16_t bitsPerFrame;               /*!< Number of bits per frame: 8- to 16-bits; needed for programming */
    volatile bool isTransferInProgress;  /*!< True if there is an active transfer */
    const volatile uint8_t * volatile txBuff;    /*!< The buffer from which transmitted bytes are taken */
    volatile uint8_t * volatile rxBuff;          /*!< The buffer into which received bytes are placed */
    volatile uint16_t txFrameCnt;        /*!< Number of bytes from current frame which were already sent */
    volatile uint16_t rxFrameCnt;        /*!< Number of bytes from current frame which were already received */
    volatile bool lsb;                   /*!< True if first bit is LSB and false if first bit is MSB */
    uint8_t rxDMAChannel;                /*!< Channel number for DMA rx channel */
    uint8_t txDMAChannel;                /*!< Channel number for DMA tx channel */
    lpspi_transfer_type transferType;    /*!< Type of LPSPI transfer */
    lpspi_callback_t callBack;           /* callback */
    transfer_status_t status;            /*!< The status of the current */
} lpspi_state_t;

typedef struct
{
	bool IsMaster;                       /*!< Select which Mode to use master/slave */
	bool SPIEnable;                      /*!< Enable Peripheral SPI */
    bool SWSlaveMgm;                     /*!< CS pin is handled by software or by protocol */
    baudrate_config_t baudrate;          /*!< Baudrate Selection as per slave requirement */
    uint8_t bitcount;                    /*!< Number of bits/frame, minimum is 8-bits */
    lpspi_clock_phase_t clkPhase;        /*!< Selects which phase of clock to capture data */
    lpspi_sck_polarity_t clkPolarity;    /*!< Selects clock polarity */
    bool lsbFirst;                       /*!< Option to transmit LSB first */
    lpspi_transfer_type transferType;    /*!< Type of LPSPI transfer */
    uint8_t rxDMAChannel;                /*!< Channel number for DMA rx channel. If DMA mode isn't used this field will be ignored. */
    uint8_t txDMAChannel;                /*!< Channel number for DMA tx channel. If DMA mode isn't used this field will be ignored. */
    lpspi_callback_t callBack;           /* Callback */
} lpspi_master_config_t;
/*******************************************************************************
 * API
 ******************************************************************************/
status_t LPSPI_DRV_MasterDeinit(uint32_t instance);
status_t LPSPI_DRV_MasterTransfer(uint32_t instance,
                                        const volatile uint8_t * const sendBuffer,
                                        volatile uint8_t * receiveBuffer,
                                        uint16_t frameCount);
void LPSPI_DRV_SingleFrameTransfer(uint32_t instance, const uint8_t * sendBuffer);
transfer_status_t LPSPI_DRV_GetTransferResult(uint32_t instance, uint8_t *rxData);
void LPSPI_DRV_MasterInitCfg(uint8_t lu8_vInstance, lpspi_state_t *lstr_vStatePtr, const lpspi_master_config_t *lstr_vMasterConfig);

#endif /* SOURCES_CDD_DRIVER_LPSPI_MASTER_DRIVER_H_ */
