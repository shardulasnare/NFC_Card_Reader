/*
 * NFC_CDD_Middleware.h
 *
 *  Created on: 30-Aug-2026
 *      Author: Shardul
 */

#ifndef SOURCES_CDD_NFC_CDD_MIDDLEWARE_H_
#define SOURCES_CDD_NFC_CDD_MIDDLEWARE_H_

#include "NFC_CDD_Cfg.h"
#include "NFC_CDD_DevCfg.h"

extern uint8_t gu8_fErrorAggregate;

void CDD_NFC_Task(void);
void ClearFaultFlags(void);
void SeqsDecode(void);
void Update_NFCCDDState(void);
void CDD_NFC_ModeManager(void);
void ParseMsg(const E_NFC_msgIdType lptr_vMsgList[], const uint32_t lu32_vMsgCount);
void ComposeDynMsg_RD_WR(E_NFC_msgIdType l_vMsgId);
void CDD_NFC_StartInitSequence(void);
void AddSeqToQueue(const NFC_seqType * const lps_vCurrSeq);

#endif /* SOURCES_CDD_NFC_CDD_MIDDLEWARE_H_ */
