/**
 ****************************************************************************************
 *
 * @file usr_design.h
 *
 * @brief Product related design header file.
 *
 * Copyright(C) 2015 NXP Semiconductors N.V.
 * All rights reserved.
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */

#ifndef USR_DESIGN_H_
#define USR_DESIGN_H_


/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "app_env.h"
#include "gpio.h"

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

extern void app_task_msg_hdl(ke_msg_id_t const msgid, void const *param);
extern void usr_sleep_restore(void);
extern void usr_init(void);
extern void gpio_interrupt_callback(enum gpio_pin pin);

#endif
