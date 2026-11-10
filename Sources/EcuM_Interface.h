/*
 * EcuM_Interface.h
 *
 *  Created on: 01-Nov-2026
 *      Author: Shardul
 */

#ifndef SOURCES_ECUM_INTERFACE_H_
#define SOURCES_ECUM_INTERFACE_H_

#include "Std_Define.h"

/******************************************************************************
 ** Macro definitions
 ******************************************************************************/
#define MCU_SHUTDOWN  0u
#define MCU_VLPS      1u

/******************************************************************************
 ** Global and Static Variable declarations
 ******************************************************************************/
typedef enum MCU_States_t_
{
    MCU_STATE_STARTUP1 = 0x00,
    MCU_STATE_STARTUP2,
    MCU_STATE_RUN,
    MCU_STATE_SHUTDOWN1,
    MCU_STATE_SHUTDOWN2,
} MCU_States_t;

/******************************************************************************
 ** Function external linkage
 ******************************************************************************/
void STARTUP1(void);
void STARTUP2(void);
void RUN(void);
void SHUTDOWN1(void);
void SHUTDOWN2(void);

/******************************************************************************
 ** End of File
 ******************************************************************************/
#endif /* SOURCES_ECUM_INTERFACE_H_ */
