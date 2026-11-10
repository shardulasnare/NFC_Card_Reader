/*
 * lpspiNFC.h
 *
 *  Created on: 30-Aug-2026
 *      Author: Shardul
 */

#ifndef SOURCES_CDD_CONFIG_LPSPINFC_H_
#define SOURCES_CDD_CONFIG_LPSPINFC_H_

#include "lpspi_master_driver.h"

#define LPSPINFC (0U)

extern lpspi_state_t lpspiNFCState;

extern const lpspi_master_config_t lpspiNFCCfg;

extern void lpspiNFCCB(transfer_status_t status);


#endif /* SOURCES_CDD_CONFIG_LPSPINFC_H_ */
