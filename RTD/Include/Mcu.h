/*
 * Mcu.h
 *
 *  Created on: 02-Nov-2026
 *      Author: Shardul
 */

#ifndef RTD_INCLUDE_MCU_H_
#define RTD_INCLUDE_MCU_H_

#include "STM32F4xx.h"
#include "Std_Define.h"

/* Macro Definitions */
/* Pin Configurtion Macros */
#define GPIOA_MODER_REG          (*((volatile uint32_t *)(0x40020000u)))
#define GPIOA_ODR_REG            (*((volatile uint32_t *)(0x40020014u)))
#define GPIOA_AFRL_REG           (*((volatile uint32_t *)(0x40020020u)))

/* NVIC ISER Register */
#define NVIC_ISER1_REG           (*((volatile uint32_t *)(0xE000E104u)))
#define NVIC_IPR8_REG            (*((volatile uint32_t *)(0xE000E420u)))

/* Function Prototypes */
void Mcu_InitClock(void);
void Port_Init(void);
void Platform_Init(void);
void Peripheral_Init(void);
void Spi_Init(void);
void cs_low(void);
void cs_high(void);

#endif /* RTD_INCLUDE_MCU_H_ */
