/*
 * systick_handler.h
 *
 *  Created on: 04-Sept-2026
 *      Author: Shardul
 */

#ifndef SOURCES_CDD_DRIVER_SYSTICK_HANDLER_H_
#define SOURCES_CDD_DRIVER_SYSTICK_HANDLER_H_

#include "Std_Define.h"

typedef struct systick_regs{
	volatile uint32_t SYST_CSR;
	volatile uint32_t SYST_RVR;
	volatile uint32_t SYST_CVR;
	volatile uint32_t SYST_CALIB;
}systick_regs_t;

#define BASE_SYSTICK_REG_VAL         0xE000E010u
#define SYSTICK                      ((systick_regs_t *)BASE_SYSTICK_REG_VAL)
#define COUNTER_ENABLE               (1u << 0u)
#define ENABLE_SYSTICK_EXPECTION     (1u << 1u)
#define PROCESSOR_CLOCK              (1u << 2u)
#define DEFAULT_PROCESSOR_CLOCK_FREQ (16000000u)

void SysTick_Init(void);
uint32_t SetInterruptTimer(uint32_t value);
void SysTimer_waitElapse(void);

#endif /* SOURCES_CDD_DRIVER_SYSTICK_HANDLER_H_ */
