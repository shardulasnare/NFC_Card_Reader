/*
 * lpspi_hal.h
 *
 *  Created on: 30-Aug-2026
 *      Author: Shardul
 */

#ifndef SOURCES_CDD_DRIVER_LPSPI_HAL_H_
#define SOURCES_CDD_DRIVER_LPSPI_HAL_H_

#include "Std_Define.h"
#include "STM32F4xx.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
typedef enum
{
    BAUDRATE_8MHZ = 0u,
    BAUDRATE_4MHZ,
    BAUDRATE_2MHZ,
    BAUDRATE_1MHZ
}baudrate_config_t;

typedef enum
{
    /* SPI interrupt specific values */
    LPSPI_ERROR_INT_ENABLE        = 5U,   /*!< SPI Error Interrupt Enable flag */
    LPSPI_RX_DATA_INT_ENABLE      = 6U,   /*!< RX data Interrupt Enable flag */
    LPSPI_TX_DATA_INT_ENABLE      = 7U,   /*!< TX data Interrupt Enable flag */
}lpspi_int_flag_t;

typedef enum{
	/* SPI specific error codes */
    STATUS_LPSPI_RXNE             = 0U,   /*!< TX Register Empty Status Flag */
    STATUS_LPSPI_TXE              = 1U,   /*!< RX Register Not Empty Status Flag */
    STATUS_LPSPI_UDR              = 3U,   /*!< TX underrun error */
    STATUS_LPSI_CRC_ERROR         = 4U,  /*!< LPSPI CRC error */
    STATUS_LPSPI_MODE_FAULT       = 5U,  /*!< LPSPI Mode error */
    STATUS_LPSPI_OVR              = 6U,   /*!< RX overrun error */
    STATUS_LPSPI_MODULE_BUSY      = 7U, /*!< Module Busy flag */
    STATUS_LPSPI_FRAME_ERROR      = 8U, /*!< LPSPI Frame error */
	LPSPI_ALL_STATUS              = 0x00U, /*!< Used for clearing all w1c status flags */
} lpspi_status_flag_t;

typedef enum
{
    /* Generic error codes */
    STATUS_SUCCESS                = 0x000U, /*!< Generic operation success status */
    STATUS_ERROR                  = 0x001U, /*!< Generic operation failure status */
    STATUS_BUSY                   = 0x002U, /*!< Generic operation busy status */
    STATUS_TIMEOUT                = 0x003U, /*!< Generic operation timeout status */
    STATUS_UNSUPPORTED            = 0x004U, /*!< Generic operation unsupported status */
} status_t;

typedef enum
{
    LPSPI_SCK_ACTIVE_HIGH = 0U, /*!< Signal is Active High (idles low). */
    LPSPI_SCK_ACTIVE_LOW  = 1U  /*!< Signal is Active Low (idles high). */
} lpspi_sck_polarity_t;

typedef enum
{
    LPSPI_ACTIVE_HIGH = 1U, /*!< Signal is Active High (idles low). */
    LPSPI_ACTIVE_LOW  = 0U  /*!< Signal is Active Low (idles high). */
} lpspi_signal_polarity_t;

typedef enum
{
    LPSPI_SDI_IN_SDO_OUT = 0U,     /*!< LPSPI SDI input, SDO output. */
    LPSPI_SDI_IN_OUT     = 1U,     /*!< SDI is used for both input and output data. */
    LPSPI_SDO_IN_OUT     = 2U,     /*!< SDO is used for both input and output data. */
    LPSPI_SDI_OUT_SDO_IN = 3U      /*!< LPSPI SDO input, SDI output. */
} lpspi_pin_config_t;

typedef enum
{
    LPSPI_CLOCK_PHASE_1ST_EDGE = 0U, /*!< Data captured on SCK 1st edge, changed on 2nd. */
    LPSPI_CLOCK_PHASE_2ND_EDGE = 1U  /*!< Data changed on SCK 1st edge, captured on 2nd. */
} lpspi_clock_phase_t;

typedef enum
{
    LPSPI_DATA_OUT_RETAINED = 0U, /*!< Data out retains last value when chip select de-asserted */
    LPSPI_DATA_OUT_TRISTATE = 1U  /*!< Data out is tri-stated when chip select de-asserted */
} lpspi_data_out_config_t;

typedef struct
{
    uint8_t frameSize;              /*!< Number of bits/frame, minimum is 8-bits. */
    bool lsbFirst;                   /*!< Option to transmit LSB first. */
    uint8_t BR;                      /*!< Selects the baud rate setting. */
    lpspi_clock_phase_t clkPhase;    /*!< Selects which phase of clock to capture data. */
    lpspi_sck_polarity_t clkPolarity; /*!< Selects clock polarity. */
} lpspi_ctrl1reg_config_t;

/*******************************************************************************
 * Variables
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

void LPSPI_HAL_Init(LPSPI_Type *base, uint8_t instance);
void LPSPI_HAL_DeInit(LPSPI_Type *base, uint8_t instance);
status_t LPSPI_HAL_SetMasterSlaveMode(LPSPI_Type * base, bool mode);
status_t LPSPI_HAL_ClearStatusFlag(LPSPI_Type * base, lpspi_status_flag_t statusFlag);
status_t LPSPI_HAL_SetPcsHandling(LPSPI_Type * base, bool isEnbaled);
void LPSPI_HAL_SetControlReg1(LPSPI_Type * base, const lpspi_ctrl1reg_config_t * Ctrl1CfgSet);

static inline void LPSPI_HAL_Enable(LPSPI_Type * base)
{
    (base->CR1) &= ~(1U << 6U);
    (base->CR1) |=  (1U << 6U);
}

static inline bool LPSPI_HAL_IsModuleEnabled(const LPSPI_Type * base)
{
    return (bool)((base->CR1) >> 6U);
}

static inline bool LPSPI_HAL_GetStatusFlag(const LPSPI_Type * base,
                                           lpspi_status_flag_t statusFlag)
{
    return (bool)((base->SR)  >> statusFlag);
}

static inline void LPSPI_HAL_SetIntMode(LPSPI_Type * base, lpspi_int_flag_t interruptSrc, bool enable)
{
    if (enable == true)
    {
        base->CR2 |= (uint16_t)1U << (uint8_t)interruptSrc;
    }
    else
    {
        base->CR2 &= ~((uint16_t)1U << (uint8_t)interruptSrc);
    }
}

static inline bool LPSPI_HAL_GetIntMode(const LPSPI_Type * base,
		lpspi_int_flag_t interruptSrc)
{
    return (bool)((base->CR2) >> (uint8_t)interruptSrc);
}

static inline void LPSPI_HAL_SetTxDmaCmd(LPSPI_Type * base, bool enable)
{
    base->CR2 = (base->CR2 & (~2U)) | ((uint16_t)enable << 1U);
}

static inline void LPSPI_HAL_SetRxDmaCmd(LPSPI_Type * base, bool enable)
{
    (base->CR2) = (base->CR2 & (~1U)) | ((uint16_t)enable << 0U);
}

static inline void LPSPI_HAL_WriteData(LPSPI_Type * base, uint8_t data)
{
    base->DR = data;
}

static inline uint8_t LPSPI_HAL_ReadData(const LPSPI_Type * base)
{
    return (uint8_t)base->DR;
}

#endif /* SOURCES_CDD_DRIVER_LPSPI_HAL_H_ */
