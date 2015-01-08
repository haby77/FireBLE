/**
 ****************************************************************************************
 *
 * @file intc.h
 *
 * @brief Declaration of the Interrupt Controller API.
 *
 * Copyright (C) Quintic 2009-2012
 *
 * $Rev: $
 *
 ****************************************************************************************
 */

#ifndef _INTC_H_
#define _INTC_H_

/**
 ****************************************************************************************
 * @addtogroup INTC INTC
 * @ingroup DRIVERS
 *
 * @brief Declaration of the Interrupt Controller API.
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "LPC17xx.h"

/*
 * DEFINES
 ****************************************************************************************
 */

/** @brief Enable interrupts globally in the system.
 * This macro must be used when the initialization phase is over and the interrupts
 * can start being handled by the system.
 */                                              

#define GLOBAL_INT_START() __set_PRIMASK(0)

/** @brief Disable interrupts globally in the system.
 * This macro must be used when the system wants to disable all the interrupt
 * it could handle.
 */
#define GLOBAL_INT_STOP() __set_PRIMASK(1)

/** @brief Disable interrupts globally in the system.
 * This macro must be used in conjunction with the @ref GLOBAL_INT_RESTORE macro since this
 * last one will close the brace that the current macro opens.  This means that both
 * macros must be located at the same scope level.
 */

#define GLOBAL_INT_DISABLE()            \
do {                                    \
    uint32_t int_restore;                \
    int_restore = NVIC->ISER[0];        \
    NVIC->ICER[0] = 0xffffffff;                    

/** @brief Restore interrupts from the previous global disable.
 * @sa GLOBAL_INT_DISABLE
 */

#define GLOBAL_INT_RESTORE()            \
    NVIC->ISER[0] = int_restore;        \
} while(0)

/// @} INTC

#endif // _INTC_H_
