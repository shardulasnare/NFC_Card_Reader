/*
 * NFC_CDD_Driverinterface.h
 *
 *  Created on: 30-Aug-2026
 *      Author: Shardul
 */

#ifndef SOURCES_CDD_NFC_CDD_DRIVERINTERFACE_H_
#define SOURCES_CDD_NFC_CDD_DRIVERINTERFACE_H_

#include "lpspi_hal.h"

void NFC_clearAllParam(void);
void NFC_DriverStartTransmit(void);
void NFC_addToTxBuff(const uint16_t *lu16_vSrcData, E_NFC_msgIdType lu32_vMsgId);
void NFC_popFromRxBuff(uint16_t * lptru16_vDst,  uint32_t_t lu32_vLength);

#endif /* SOURCES_CDD_NFC_CDD_DRIVERINTERFACE_H_ */
