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


/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "app_env.h"
#include "gpio.h"

/*
 * STRUCTURE DEFINITIONS
 ****************************************************************************************
 */
 
#define	DISCON_COUNT_MAX	100

typedef struct	gsensor_data_tag
{
		int16_t	temp_data;
		int16_t	addr_data;
} gsensor_data;

typedef struct 	discon_reason_tag
{
		uint8_t nb;
		uint8_t reason[DISCON_COUNT_MAX];
} discon_reason;

struct usr_env_tag
{
    uint16_t    led1_on_dur;
    uint16_t    led1_off_dur;

#if (defined(QN_ADV_WDT))
    bool        adv_wdt_enable;
    void        (*adv_wdt_to)(void);
#endif
		uint16_t		test_flag;
		uint8_t i2cbuffer[104];
		gsensor_data mpu6050_data;	
		uint32_t time;
		uint8_t test_result;
		discon_reason usr_discon;
};

struct	test_data_tag
{
		uint8_t		time_flag;
		uint32_t 	time;
#if (CONFIG_ENABLE_DRIVER_MPU6050 == TRUE)
		uint8_t 	mpu6050_flag;
		uint8_t		mpu6050_addr_flag;
		int16_t		mpu6050_addr;
		uint8_t 	mpu6050_temp_flag;
		int16_t		mpu6050_temp;
#endif
};

extern	struct test_data_tag	test_data;

extern struct usr_env_tag usr_env;

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

extern void app_task_msg_hdl(ke_msg_id_t const msgid, void const *param);
extern int app_led_timer_handler(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
extern int app_gap_adv_intv_update_timer_handler(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
extern int app_proxr_alert_to_handler(ke_msg_id_t const msgid, void *param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
extern void usr_sleep_restore(void);
extern void usr_button1_cb(void);
extern void usr_button2_cb(void);
extern int app_button_timer_handler(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
extern void usr_init(void);
extern void gpio_interrupt_callback(enum gpio_pin pin);
extern void	timer0_callback(void);

#if (CONFIG_ENABLE_DRIVER_MPU6050 == TRUE)
extern int app_mpu6050_temperature_test_timer_handler(ke_msg_id_t const msgid, void const *param,
                               ke_task_id_t const dest_id, ke_task_id_t const src_id);
extern int app_mpu6050_addr_read_test_timer_handler(ke_msg_id_t const msgid, void const *param,
                               ke_task_id_t const dest_id, ke_task_id_t const src_id);
#endif
#if FB_OLED
extern int app_oled_state_display_timer_handler(ke_msg_id_t const msgid,
                               void *param,
                               ke_task_id_t const dest_id,
                               ke_task_id_t const src_id);
#endif

#endif
