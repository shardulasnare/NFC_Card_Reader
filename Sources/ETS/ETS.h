/*
 * ETS.h
 *
 *  Created on: 10-Nov-2026
 *      Author: Shardul
 */

#ifndef SOURCES_ETS_ETS_H_
#define SOURCES_ETS_ETS_H_

#include "Std_Define.h"

/* Set from the debugger to force MCU into RUN state */
extern volatile bool gu8_fForceMCURun;

void Test_ETS_InitTask(void);
void ETS_Task(void);

#endif /* SOURCES_ETS_ETS_H_ */
