/*
 * systick_handler.c
 *
 *  Created on: 04-Sept-2026
 *      Author: Shardul
 */

#include "SysTimer.h"

volatile bool gvu8_fCycleCompleted = false;
volatile bool gu8_fSchedulerOverrun = false;
volatile bool gvu8_f10msCycle = FALSE;

void SysTick_Init(void)
{
	/* Clear the value in CSR (Control and Status) Register */
	SYSTICK->SYST_CSR = 0x00u;

	/* Clear the value in RVL (Reload Value) Register */
	SYSTICK->SYST_RVR = 0x00u;

	/* Clear the value in CVR (Current Value) Register */
	SYSTICK->SYST_CVR = 0x00u;

	/* Set the value for which we required interrupt i.e. 10ms */
	SYSTICK->SYST_RVR = SetInterruptTimer(10);

	/* Enable the Counter from CSR register */
	SYSTICK->SYST_CSR = COUNTER_ENABLE|ENABLE_SYSTICK_EXPECTION|PROCESSOR_CLOCK;
}

uint32_t SetInterruptTimer(uint32_t value /* Ms */)
{
	/* Calculate the number of ticks required for given value */
	return (((DEFAULT_PROCESSOR_CLOCK_FREQ / 1000u) * value) - 1u);
}

void SysTimer_waitElapse(void)
{
	static bool slu8_fCycleInitialized = false;/* For the first cycle, wait until scheduler elapse */
	/* Reset these flags now that their cycle is complete */
	gvu8_f10msCycle = false;
	if (false == slu8_fCycleInitialized)
	{
		slu8_fCycleInitialized = true;
		gvu8_fCycleCompleted = false;
	}

	/* Wait for 10ms to complete */
	while (false == gvu8_fCycleCompleted);

	/* Update the status of variables */
	gvu8_f10msCycle = true;
	gvu8_fCycleCompleted = false;
	gu8_fSchedulerOverrun = false;
}

void SysTick_Handler(void)
{
	if(gvu8_f10msCycle == true) gu8_fSchedulerOverrun = true;
    /* Hardware automatically clears the SysTick pending state upon ISR entry */
	gvu8_fCycleCompleted = true;
}
