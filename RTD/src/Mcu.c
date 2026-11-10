/*
 * Mcu.c
 *
 *  Created on: 02-Nov-2026
 *      Author: Shardul
 */

#include "Mcu.h"

/*******************************************************************************
 ** Function name    : Mcu_InitClock
 ** Description      : Initialize the clock for MCU and its peripherals
 ** Parameters       : void
 ** Return value     : void
 ** Remarks          : -
 ******************************************************************************/
void Mcu_InitClock(void)
{
	/* Clear the existing value present at bit 0 (GPIOA) */
	IP_RCC->RCC_AHB1ENR &= ~(1u << 0u);

	/* Initialize Clock for Port A */
	IP_RCC->RCC_AHB1ENR |= (1u << 0u);
}

/*******************************************************************************
 ** Function name    : Port_Init
 ** Description      : Initialize and Configure Port Pins
 ** Parameters       : void
 ** Return value     : void
 ** Remarks          : -
 ******************************************************************************/
void Port_Init(void)
{
	/* Make PA4 Chip Select Pin By default High */
	GPIOA_ODR_REG |= (1u << 4u);

	/* Configure PA4 Pin as a GPIO Chip Select */
	GPIOA_MODER_REG &= ~(3u << 8u); /* Clear the existing values present at bit 8 and bit 9 */
	GPIOA_MODER_REG |= (1u << 8u);  /* Configure the PA4 pin Chip Select as an Output Mode */

	/* Configure PA5 pin as a Alternate Functionality */
	GPIOA_MODER_REG &= (3u << 10u); /* Clear the existing values present at bit 10 and bit 11 */
	GPIOA_MODER_REG |= (2u << 10u); /* Configure the PA5 (SCLK) pin as a Alternate Functionality */

	/* Configure PA6 pin as a Alternate Functionality */
	GPIOA_MODER_REG &= (3u << 12u); /* Clear the existing values present at bit 12 and bit 13 */
	GPIOA_MODER_REG |= (2u << 12u); /* Configure the PA6 (MISO) pin as a Alternate Functionality */

	/* Configure PA7 pin as a Alternate Functionality */
	GPIOA_MODER_REG &= (3u << 14u); /* Clear the existing values present at bit 14 and bit 15 */
	GPIOA_MODER_REG |= (2u << 14u); /* Configure the PA7 (MOSI) pin as a Alternate Functionality */

	/* Configure PA5 pin as a SCLK */
	GPIOA_AFRL_REG &= ~(5u << 10u); /* Clear the existing values present at bits 20 to bits 23 bit */
	GPIOA_AFRL_REG |= (5u << 10u);  /* Configure PA5 as SCLK */

	/* Configure PA6 pin as a MISO */
	GPIOA_AFRL_REG &= ~(5u << 10u); /* Clear the existing values present at bits 24 to bit 27 bit */
	GPIOA_AFRL_REG |= (5u << 10u);  /* Configure the PA6 as a MISO */

	/* Configure PA7 pin as a MOSI */
	GPIOA_AFRL_REG &= ~(5u << 10u); /* Clear the existing values present at bits 28 to bit 31 bit */
	GPIOA_AFRL_REG |= (5u << 10u);  /* Configure the PA6 as a MOSI */
}

/*******************************************************************************
 ** Function name    : Platform_Init
 ** Description      : Configures the Interrupt for MCU
 ** Parameters       : void
 ** Return value     : void
 ** Remarks          : -
 ******************************************************************************/
void Platform_Init(void)
{
	/* Enable clock for EXTI from RCC */
	IP_RCC->RCC_APB2ENR &= ~(1u << 14u); /* Clears bit no 14th Value */
	IP_RCC->RCC_APB2ENR |=(1u << 14u);   /* Enable the clock for SYSCFGGEN Needed for GPIO IRQ's*/

	/* Enable the interrupt for SPI1 from Core Registers NVIC_ISER */
	NVIC_ISER1_REG |= (1u << 3u);    /* Enable the SPI1 Interrupt */

	/* Set Priority for SPI1 IRQ from Core Register NVIC_IPR */
	NVIC_IPR8_REG |= ((4u << 4u) << 24u); /* Interrupt Priority for SPI1 IRQ is set as 4 Lower the number higher the priority */
}

/*******************************************************************************
 ** Function name    : Peripheral_Init
 ** Description      : Configures the Peripherals of MCU
 ** Parameters       : void
 ** Return value     : void
 ** Remarks          : -
 ******************************************************************************/
void Peripheral_Init(void)
{
	Spi_Init();
}

/*******************************************************************************
 ** Function name    : Spi_Init
 ** Description      : Initialize and Configure the SPI
 ** Parameters       : void
 ** Return value     : void
 ** Remarks          : -
 ******************************************************************************/
void Spi_Init(void)
{
	/* Enable the clock for SPI1 Peripheral */
	IP_RCC->RCC_APB2ENR &= ~(1u << 12u); /* Clears the existing value at 12th bit */
	IP_RCC->RCC_APB2ENR |= (1u << 12u);  /* Sets the 12 bit to enable the clock for SPI1 */
	LPSPI_DRV_MasterInitCfg()
}

/*******************************************************************************
 ** Function name    : cs_low
 ** Description      : This function is used to low the chip select pin PA4
 ** Parameters       : void
 ** Return value     : void
 ** Remarks          : -
 ******************************************************************************/
void cs_low(void)
{
	/* Make PA4 CS an Output Pin Low */
	GPIOA_ODR_REG &= (1u << 4u);
}

/*******************************************************************************
 ** Function name    : cs_high
 ** Description      : This function is used to low the chip select pin PA4
 ** Parameters       : void
 ** Return value     : void
 ** Remarks          : -
 ******************************************************************************/
void cs_high(void)
{
	/* Make PA4 CS an Output Pin High */
	GPIOA_ODR_REG |= (1u << 4u);
}
