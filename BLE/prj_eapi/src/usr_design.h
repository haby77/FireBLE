/**
 ****************************************************************************************
 *
 * @file usr_design.h
 *
 * @brief Product related design header file.
 *
 * Copyright (C) Quintic 2012-2013
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */

#ifndef USR_DESIGN_H_
#define USR_DESIGN_H_
#include "gpio.h"

/*
 * MACRO DEFINITIONS
 ****************************************************************************************
 */
#define APP_HTPT_MEAS_INTV      15          // 15s
#define APP_HTPT_MEAS_INTV_DEF  (APP_HTPT_MEAS_INTV * 100)

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

extern void usr_sleep_restore(void);
extern void usr_init(void);
extern void gpio_interrupt_callback(enum gpio_pin pin);

#endif
