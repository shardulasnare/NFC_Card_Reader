/*
 * NFC_CDD_Interface.h
 *
 *  Created on: 30-Aug-2026
 *      Author: Shardul
 */

#ifndef SOURCES_CDD_DRIVER_NFC_CDD_INTERFACE_H_
#define SOURCES_CDD_DRIVER_NFC_CDD_INTERFACE_H_

#include "NFC_CDD_DevCfg.h"

void GetNFCDriverStatus(CDDDriverStatus* const lptrenu_vNFCCDDStatus,
        uint8_t * const lptru8_cNFCRetryCount);
void SetNFCDriverState(CDDDriverStatus const lenu_vNFCRequestState,
        bool* const lptru8_fStateChangeAccepted);
void SetNFCToPowerDown(bool *lu8_fNFCPDAccepted);

#endif /* SOURCES_CDD_DRIVER_NFC_CDD_INTERFACE_H_ */
