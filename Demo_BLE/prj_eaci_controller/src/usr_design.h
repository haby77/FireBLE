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

#include "app_env.h"
#include "gpio.h"

/*
 * MACRO DEFINITIONS
 ****************************************************************************************
 */
#define APP_HTPT_MEAS_INTV      10          // 10s
#define APP_HTPT_MEAS_INTV_DEF  (APP_HTPT_MEAS_INTV * 100)

struct usr_env_tag
{
    uint16_t    led1_on_dur;
    uint16_t    led1_off_dur;

    uint8_t     sleep_allow;
    uint32_t    dev_active_bf;

};

extern struct usr_env_tag usr_env;

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

extern void usr_led1_set(uint16_t timer_on, uint16_t timer_off);
extern int app_led_timer_handler(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
extern void usr_sleep_restore(void);
extern void eaci_wakeup_init(void);
#if (defined(QN_EACI_GPIO_WAKEUP_EX_MCU))
extern void eaci_wakeup_ex_mcu_start(void);
extern void eaci_wakeup_ex_mcu_stop(void);
#endif
extern void usr_init(void);
extern void gpio_interrupt_callback(enum gpio_pin pin);

#endif
