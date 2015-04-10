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

#include "bletime.h"
#include "app_env.h"
#include "gpio.h"

/*
 * MACRO DEFINITIONS
 ****************************************************************************************
 */

#define APP_HTPT_MEAS_INTV                          HTPT_MEAS_INTV_DFLT_MAX
#define APP_HTPT_MEAS_INTV_DEF                      (APP_HTPT_MEAS_INTV * 100)

// the event id of adc sample complete
#define EVENT_ADC_SAMPLE_CMP_ID                     1

// The number of sample to temperature
#define HTPT_TEMP_SAMPLE_NUMBER                     16

// if true, use farenheit, otherwise, use celcius.
#define HTPT_USE_FARENHEIT                          false

// tansform celcius to farenheit with 10times
#define HTPT_CELCIUS_TO_FARENHEIT_X10(c_10times)    ((c_10times) * 9/5 + 320)

// HTPT store temperature value array size,
// it is recommended that at least one day of time-stamped data can be stored.
// If the device is configured to send data at intervals of 30 minutes over a
// 24-hour period, this equates to a recommendation of at least 48 stored data
// measurements. For more basic scenarios, the storing of at least 30 data
// measurements is recommended.
#define HTPT_TEMP_STORE_MEAS_SIZE                   30

// HTPT idle connection timeout
#define HTPT_IDLE_CONNECTION_TIMEOUT                500 //5sec

// HTPT period measure time, and this value is also intermediate measure interval.
#define HTPT_PERIOD_MEAS_TIME                       100 //1sec

#define HTPT_INTERM_MEAS_STABLE_COUNTER_MAX         10

#define HTPT_INTERM_MEAS_STABLE_TOLERABLE_LIMIT     10

/*
 * STRUCTURE DEFINITIONS
 ****************************************************************************************
 */

struct usr_env_tag
{
    uint16_t    led1_on_dur;
    uint16_t    led1_off_dur;

    // temperature ADC register value buffer
    int16_t     htpt_reg_buf[HTPT_TEMP_SAMPLE_NUMBER];

    // stored temperature measure fifo struct
    struct
    {
        struct htp_temp_meas data[HTPT_TEMP_STORE_MEAS_SIZE];
        uint32_t    head;
        uint32_t    tail;
    }store_meas_fifo;

    // intermediate measure stable struct
    struct
    {
        int32_t max;
        int32_t min;
        int32_t stable_counter;
    }imeas;

    // measure is running
    bool is_meas_running;

    // connection idle state
    bool is_idle_connection_overtime;

    // temperature measure busy state (Temperature Measurement or Intermediate Temperature)
    bool is_meas_send_busy;

    bool is_imeas_send_busy;

    // measure characteristic config state
    bool is_temp_meas_config;

    // intermediate measure characteristic config state
    bool is_temp_imeas_config;

    // measure interval characteristic config state
    bool is_temp_interval_config;
    
    // measure should indicate
    bool is_should_indicate;

    // measure counter
    size_t meas_counter;

#if (defined(QN_ADV_WDT))
    bool        adv_wdt_enable;
    void        (*adv_wdt_to)(void);
#endif
};

extern struct usr_env_tag usr_env;

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

extern void app_task_msg_hdl(ke_msg_id_t const msgid, void const *param);

extern int app_led_timer_handler                 (ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
extern int app_gap_adv_intv_update_timer_handler (ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
extern int app_htpt_period_meas_timer_handler    (ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
extern int app_htpt_idle_connection_timeout_timer_handler (ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
extern int app_button_timer_handler              (ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id);

extern void usr_sleep_restore(void);
extern void usr_button1_cb(void);

extern void usr_init(void);
extern void gpio_interrupt_callback(enum gpio_pin pin);

#endif
