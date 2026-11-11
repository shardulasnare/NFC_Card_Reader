/*
 * lpspi_hal.c
 *
 *  Created on: 30-Aug-2026
 *      Author: Shardul
 */
#include "lpspi_hal.h"

void LPSPI_HAL_Init(LPSPI_Type *base, uint8_t instance)
{
	/* Enable clock access for SPI Module */
	if(instance == LPSPI1)
	{
		IP_RCC->RCC_APB2ENR &= ~(1U << 12U); /* Clear the existing value of SPI1EN bit */
		IP_RCC->RCC_APB2ENR |=  (1U << 12U); /* Set the value of SPI1EN bit */
	}
	else if(instance == LPSPI2)
	{
		IP_RCC->RCC_APB1ENR &= ~(1U << 14U); /* Clear the existing value of SPI2EN bit */
		IP_RCC->RCC_APB1ENR |=  (1U << 14U); /* Set the value of SPI2EN bit */
	}
	else if(instance == LPSPI3)
	{
		IP_RCC->RCC_APB1ENR &= ~(1U << 15U); /* Clear the existing value of SPI3EN bit */
		IP_RCC->RCC_APB1ENR |=  (1U << 15U); /* Set the value of SPI3EN bit */
	}
	else if(instance == LPSPI4)
	{
		IP_RCC->RCC_APB2ENR &= ~(1U << 13U); /* Clear the existing value of SPI4EN bit */
		IP_RCC->RCC_APB2ENR |=  (1U << 13U); /* Set the value of SPI4EN bit */
	}
	else
	{
		IP_RCC->RCC_APB2ENR &= ~(1U << 20U); /* Clear the existing value of SPI5EN bit */
		IP_RCC->RCC_APB2ENR |=  (1U << 20U); /* Set the value of SPI5EN bit */
	}


}

void LPSPI_HAL_DeInit(LPSPI_Type *base, uint8_t instance)
{
	/* Disable clock access for SPI Module */
	if(instance == LPSPI1)
	{
		IP_RCC->RCC_APB2ENR &= ~(1U << 12U); /* Clear the existing value of SPI1EN bit */
	}
	else if(instance == LPSPI2)
	{
		IP_RCC->RCC_APB1ENR &= ~(1U << 14U); /* Clear the existing value of SPI2EN bit */
	}
	else if(instance == LPSPI3)
	{
		IP_RCC->RCC_APB1ENR &= ~(1U << 15U); /* Clear the existing value of SPI3EN bit */
	}
	else if(instance == LPSPI4)
	{
		IP_RCC->RCC_APB2ENR &= ~(1U << 13U); /* Clear the existing value of SPI4EN bit */
	}
	else
	{
		IP_RCC->RCC_APB2ENR &= ~(1U << 20U); /* Clear the existing value of SPI5EN bit */
	}
	base->CR1 &=  ~(1U << 6U); /* Clear the value of SPE bit */
}

status_t LPSPI_HAL_SetMasterSlaveMode(LPSPI_Type * base, bool isMaster)
{

    /* If the module is enabled, return error */
    if (true == LPSPI_HAL_IsModuleEnabled(base))
    {
        return STATUS_ERROR;
    }
    else
    {
        /* MSTR is bit 2 of SPI_CR1 */
        base->CR1 = (base->CR1 & (~(1u << 2U))) | ((uint32_t)isMaster << 2U);
        return STATUS_SUCCESS;
    }
}

status_t LPSPI_HAL_ClearStatusFlag(LPSPI_Type * base, lpspi_status_flag_t statusFlag)
{
    /* If the flag is not w1c capable, return invalid parameter error */
    if ((statusFlag == STATUS_LPSPI_MODULE_BUSY) ||
        (statusFlag == STATUS_LPSPI_RXNE) ||
        (statusFlag == STATUS_LPSPI_TXE))
    {
        return STATUS_ERROR;
    }
    else
    {
        if (statusFlag == LPSPI_ALL_STATUS)
        {
            base->SR &= (uint32_t)LPSPI_ALL_STATUS;
        }
        else
        {
            base->SR &= ~((uint32_t)1U << (uint32_t)statusFlag);
        }
        return STATUS_SUCCESS;
    }
}

status_t LPSPI_HAL_SetPcsHandling(LPSPI_Type * base, bool isEnbaled)
{
    /* If the module is enabled, return error */
    if (true == LPSPI_HAL_IsModuleEnabled(base))
    {
        return STATUS_ERROR;
    }
    else
    {
        /* Set PCS Handling */
        /* PCS Handling is used for SW to drive the PCS before every transaction
         * or PCS is handled at protocol level */
    	if(isEnbaled == TRUE)
    	{
    		base->CR1 &= ~(isEnbaled << 9U); /* Clear the SSM bit */
    		base->CR1 |=  (isEnbaled << 9U); /* Set the SSM bit */
    		base->CR1 &= ~(isEnbaled << 8U); /* Clear the SSI bit */
    		base->CR1 |=  (isEnbaled << 8U); /* Set the SSI bit */
    	}
        return STATUS_SUCCESS;
    }
}

void LPSPI_HAL_SetControlReg1(LPSPI_Type * base, const lpspi_ctrl1reg_config_t * Ctrl1CfgSet)
{
     base->CR1 |= (((uint16_t)Ctrl1CfgSet->clkPhase << 0U) |
                         ((uint16_t)Ctrl1CfgSet->clkPolarity << 1U) |
                         ((uint16_t)Ctrl1CfgSet->BR << 3U) |
                         ((uint16_t)Ctrl1CfgSet->lsbFirst << 7U));
}
