/*
 * lpspiNFC.c
 *
 *  Created on: 30-Aug-2026
 *      Author: Shardul
 */


#include "lpspiNFC.h"
#include "lpspi_hal.h"

lpspi_state_t lpspiNFCState;

const lpspi_master_config_t lpspiNFCCfg = {
  .IsMaster = true,
  .SPIEnable = true,
  .SWSlaveMgm = true,
  .baudrate = BAUDRATE_8MHZ,
  .bitcount = 8u,
  .clkPhase = LPSPI_CLOCK_PHASE_1ST_EDGE,
  .clkPolarity = LPSPI_SCK_ACTIVE_HIGH,
  .lsbFirst = false,
  .transferType = LPSPI_USING_INTERRUPTS,
  .rxDMAChannel = 0x00u,
  .txDMAChannel = 0x00u,
  .callBack = lpspiNFCCB,
};
