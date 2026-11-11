/*
 * STM32F4xx.h
 *
 *  Created on: 02-Sept-2026
 *      Author: Shardul
 */

#ifndef SOURCES_CDD_DRIVER_STM32F4XX_H_
#define SOURCES_CDD_DRIVER_STM32F4XX_H_

#include "Std_Define.h"

typedef struct {
	volatile uint32_t RCC_CR;
	volatile uint32_t RCC_PLLCFGR;
	volatile uint32_t RCC_CFGR;
	volatile uint32_t RCC_CIR;
	volatile uint32_t RCC_AHB1RSTR;
	volatile uint32_t RCC_AHB2RSTR;
	uint32_t RESERVED[2];
	volatile uint32_t RCC_APB1RSTR;
	volatile uint32_t RCC_APB2RSTR;
	uint32_t RESERVED_1[2];
	volatile uint32_t RCC_AHB1ENR;
	volatile uint32_t RCC_AHB2ENR;
	uint32_t RESERVED_2[2];
	volatile uint32_t RCC_APB1ENR;
	volatile uint32_t RCC_APB2ENR;
	uint32_t RESERVED_3[2];
	volatile uint32_t RCC_AHB1LPENR;
	volatile uint32_t RCC_AHB2LPENR;
	uint32_t RESERVED_4[2];
	volatile uint32_t RCC_APB1LPENR;
	volatile uint32_t RCC_APB2LPENR;
	uint32_t RESERVED_5[2];
	volatile uint32_t RCC_BDCR;
	volatile uint32_t RCC_CSR;
	uint32_t RESERVED_6[2];
	volatile uint32_t RCC_SSCGR;
	volatile uint32_t RCC_PLLI2SCFGR;
	uint32_t RESERVED_7;
	volatile uint32_t RCC_DCKCFGR;
}RCC_Type;

/** Peripheral RCC base address */
#define IP_RCC_BASE_ADDR                         (0x40023800u)
/** Peripheral RCC base pointer */
#define IP_RCC                                   ((RCC_Type *)IP_RCC_BASE_ADDR)

typedef struct {
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t SR;
	volatile uint32_t DR;
	volatile uint32_t CRCPR;
	volatile uint32_t RXCRCR;
	volatile uint32_t TXCRCR;
	volatile uint32_t SPICFGR;
}LPSPI_Type;

/** Number of instances of the LPSPI module. */
#define LPSPI_INSTANCE_COUNT                     (5u)
/* LPSPI - Peripheral instance base addresses */
/** Peripheral LPSPI_1 base address */
#define IP_LPSPI_1_BASE                          (0x40013000u)
/** Peripheral LPSPI_1 base pointer */
#define IP_LPSPI_1                               ((LPSPI_Type *)IP_LPSPI_1_BASE)
/** Peripheral LPSPI_2 base address */
#define IP_LPSPI_2_BASE                          (0x40003800u)
/** Peripheral LPSPI_2 base pointer */
#define IP_LPSPI_2                               ((LPSPI_Type *)IP_LPSPI_2_BASE)
/** Peripheral LPSPI_3 base address */
#define IP_LPSPI_3_BASE                          (0x40003C00u)
/** Peripheral LPSPI_3 base pointer */
#define IP_LPSPI_3                               ((LPSPI_Type *)IP_LPSPI_3_BASE)
/** Peripheral LPSPI_4 base address */
#define IP_LPSPI_4_BASE                          (0x40013400u)
/** Peripheral LPSPI_4 base pointer */
#define IP_LPSPI_4                               ((LPSPI_Type *)IP_LPSPI_4_BASE)
/** Peripheral LPSPI_5 base address */
#define IP_LPSPI_5_BASE                          (0x40015000u)
/** Peripheral LPSPI_5 base pointer */
#define IP_LPSPI_5                               ((LPSPI_Type *)IP_LPSPI_5_BASE)
/** Array initializer of LPSPI peripheral base addresses */
#define IP_LPSPI_BASE_ADDRS                      {IP_LPSPI_1_BASE, IP_LPSPI_2_BASE, IP_LPSPI_3_BASE, IP_LPSPI_4_BASE, IP_LPSPI_5_BASE}
/** Array initializer of LPSPI peripheral base pointers */
#define IP_LPSPI_BASE_PTRS                       {IP_LPSPI_1, IP_LPSPI_2, IP_LPSPI_3, IP_LPSPI_4, IP_LPSPI_5}
#define LPSPI1                                   0
#define LPSPI2                                   1
#define LPSPI3                                   2
#define LPSPI4                                   3
#define LPSPI5                                   4
#endif /* SOURCES_CDD_DRIVER_STM32F4XX_H_ */
