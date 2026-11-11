/*
 * NFC_CDD_Driverinterface.h
 *
 *  Created on: 30-Aug-2026
 *      Author: Shardul
 */

#ifndef SOURCES_CDD_NFC_CDD_DRIVERINTERFACE_H_
#define SOURCES_CDD_NFC_CDD_DRIVERINTERFACE_H_

#include "NFC_CDD_Cfg.h"
#include "lpspiNFC.h"

extern volatile CDD_TrxStatus genu_vNFCTrxStatus;
extern E_NFC_seqIdType gaenu_vSeqsToSend[MC_LAST_SEQ];
extern uint8_t gu8_cSeqQCount;
extern uint8_t gu8_cSeqQIndex;

void NFC_clearAllParam(void);
void NFC_DriverStartTransmit(void);
void NFC_addToTxBuff(const uint16_t *lu16_vSrcData, E_NFC_msgIdType lu32_vMsgId);
void NFC_popFromRxBuff(uint8_t *lptru8_vDst, uint32_t lu32_vLength);
void NFC_DisableAllInterrupts(void);

#endif /* SOURCES_CDD_NFC_CDD_DRIVERINTERFACE_H_ */
