/*
 * EcuM_Interface.c
 *
 *  Created on: 01-Nov-2026
 *      Author: Shardul
 */
#include "EcuM_Interface.h"
#include "NFC_CDD_Middleware.h"
#include "Mcu.h"
#include "SysTimer.h"

MCU_States_t genu_vMCUState = MCU_STATE_STARTUP1;

void STARTUP1(void)
{

	/* Initialize the clock tree system clock */
	Mcu_InitClock();

	/* Initialize all pins using the Port driver */
	Port_Init();

	/* Initialize Interrupt Settings */
	Platform_Init();

	/* Initializes the MCU Peripherals */
	Peripheral_Init();

	/* Start the 10ms scheduler tick */
	SysTick_Init();

	genu_vMCUState = MCU_STATE_STARTUP2;
}


void STARTUP2(void)
{
	/* Initialize NFC Task */
	CDD_NFC_Task();
}


void RUN(void)
{
	CDD_NFC_Task();
}


void SHUTDOWN1(void)
{
	/* Add Shutdown Sequence */
	/* Here de-initialize Peripherals */
}

void SHUTDOWN2(void)
{
	/* Configure Mcu for sleep mode */
}
