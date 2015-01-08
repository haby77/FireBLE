/**
 ****************************************************************************************
 *
 * @file usr_design.c
 *
 * @brief Product related design.
 *
 * Copyright (C) Quintic 2012-2013
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup  USR
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include <stdint.h>
#include "app_env.h"
#include "led.h"
#include "uart.h"
#include "lib.h"
#include "usr_design.h"
#include "gpio.h"
#include "button.h"
#include "adc.h"
#include "analog.h"
#include "nvds.h"
#if (defined(QN_ADV_WDT))
#include "wdt.h"
#endif
#include "sleep.h"

/*
 * MACRO DEFINITIONS
 ****************************************************************************************
 */

#define LED_ON_DUR_ADV_FAST                         2
#define LED_OFF_DUR_ADV_FAST                        (uint16_t)((GAP_ADV_FAST_INTV2*0.625)/10)
#define LED_ON_DUR_ADV_SLOW                         2
#define LED_OFF_DUR_ADV_SLOW                        (uint16_t)((GAP_ADV_SLOW_INTV*0.625)/10)
#define LED_ON_DUR_CON                              0xffff
#define LED_OFF_DUR_CON                             0
#define LED_ON_DUR_IDLE                             0
#define LED_OFF_DUR_IDLE                            0xffff

// BUTTON1 use for advertising (start/stop), this is its event id
#define EVENT_BUTTON1_PRESS_ID                      0

#if (QN_DEEP_SLEEP_EN)
    #warning "Htpt don't sopport deep sleep because of the bletime requesting. If you don't care the bletime, please ignore this message."
#endif

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */
#if (defined(QN_ADV_WDT))
static void adv_wdt_to_handler(void)
{
    ke_state_set(TASK_APP, APP_IDLE);

    // start adv
    app_gap_adv_start_req(GAP_GEN_DISCOVERABLE|GAP_UND_CONNECTABLE,
                          app_env.adv_data, app_set_adv_data(GAP_GEN_DISCOVERABLE),
                          app_env.scanrsp_data, app_set_scan_rsp_data(app_get_local_service_flag()),
                          GAP_ADV_FAST_INTV1, GAP_ADV_FAST_INTV2);
}
#endif

struct usr_env_tag usr_env = {LED_ON_DUR_IDLE,      // led1_on_dur
                              LED_OFF_DUR_IDLE,     // led1_off_dur
                              {0},                  // htpt_reg_buf
                              {0},                  // store_meas_fifo
                              {0},                  // imeas
                              false,                // is_meas_running
                              true,                 // is_idle_connection_overtime
                              false,                // is_meas_send_busy
                              false,                // is_imeas_send_busy
                              false,                // is_temp_meas_config
                              false,                // is_temp_interval_config
                              false,                // is_should_indicate
                              0,                    // meas_counter
                              0,                    // temp_offset
#if (defined(QN_ADV_WDT))
                              false,
                              adv_wdt_to_handler
#endif
                              };


/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief   Led1 for BLE status
 ****************************************************************************************
 */
static void usr_led1_set(uint16_t timer_on, uint16_t timer_off)
{
    usr_env.led1_on_dur = timer_on;
    usr_env.led1_off_dur = timer_off;

    if (timer_on == 0 || timer_off == 0)
    {
        if (timer_on == 0)
        {
            led_set(1, LED_OFF);
        }
        if (timer_off == 0)
        {
            led_set(1, LED_ON);
        }
        ke_timer_clear(APP_SYS_LED_1_TIMER, TASK_APP);
    }
    else
    {
        led_set(1, LED_OFF);
        ke_timer_set(APP_SYS_LED_1_TIMER, TASK_APP, timer_off);
    }
}

/**
 ****************************************************************************************
 * @brief   Led 1 flash process
 ****************************************************************************************
 */
static void usr_led1_process(void)
{
    if(led_get(1) == LED_ON)
    {
        led_set(1, LED_OFF);
        ke_timer_set(APP_SYS_LED_1_TIMER, TASK_APP, usr_env.led1_off_dur);
    }
    else
    {
        led_set(1, LED_ON);
        ke_timer_set(APP_SYS_LED_1_TIMER, TASK_APP, usr_env.led1_on_dur);
    }
}


/**
 ****************************************************************************************
 * @brief   temperature measurement fifo reset
 ****************************************************************************************
 */
static void app_store_meas_fifo_reset(void)
{
    usr_env.store_meas_fifo.tail = usr_env.store_meas_fifo.head = 0;
}

/**
 ****************************************************************************************
 * @brief   temperature measurement fifo length
 ****************************************************************************************
 */
//static size_t app_store_meas_fifo_len(void)
//{
//    return usr_env.store_meas_fifo.tail >= usr_env.store_meas_fifo.head ?
//           usr_env.store_meas_fifo.tail - usr_env.store_meas_fifo.head :
//           HTPT_TEMP_STORE_MEAS_SIZE - (usr_env.store_meas_fifo.head - usr_env.store_meas_fifo.tail);
//}

/**
 ****************************************************************************************
 * @brief   temperature measurement fifo in
 ****************************************************************************************
 */
static void app_store_meas_fifo_in(const struct htp_temp_meas *pmeas)
{
    usr_env.store_meas_fifo.data[usr_env.store_meas_fifo.tail++] = *pmeas;

    if(usr_env.store_meas_fifo.tail >= HTPT_TEMP_STORE_MEAS_SIZE)
        usr_env.store_meas_fifo.tail = 0;

    if(usr_env.store_meas_fifo.head == usr_env.store_meas_fifo.tail)
    {
        if(++usr_env.store_meas_fifo.head >= HTPT_TEMP_STORE_MEAS_SIZE)
            usr_env.store_meas_fifo.head = 0;
    }
}

/**
 ****************************************************************************************
 * @brief   temperature measurement fifo out
 ****************************************************************************************
 */
static bool app_store_meas_fifo_out(struct htp_temp_meas *pmeas)
{
    // if the fifo is empty, return false
    if(usr_env.store_meas_fifo.head == usr_env.store_meas_fifo.tail)
        return false;

    *pmeas = usr_env.store_meas_fifo.data[usr_env.store_meas_fifo.head++];

    if(usr_env.store_meas_fifo.head >= HTPT_TEMP_STORE_MEAS_SIZE)
        usr_env.store_meas_fifo.head = 0;
        
    return true;
}

/**
 ****************************************************************************************
 * @brief   whether intermediate temperature is stable.
 * User may overwrite this function to ensure the temperature is stable.
 ****************************************************************************************
 */
bool app_is_interm_temp_stable(int32_t itemp_x10)
{
    int32_t diff;

    if(itemp_x10 > usr_env.imeas.max)
    {
        usr_env.imeas.max = itemp_x10;
    }
    else if(itemp_x10 < usr_env.imeas.min)
    {
        usr_env.imeas.min = itemp_x10;
    }

    diff = usr_env.imeas.max - usr_env.imeas.min;

    if(diff > 0 && diff < HTPT_INTERM_MEAS_STABLE_TOLERABLE_LIMIT)
    {
        ++usr_env.imeas.stable_counter;
    }
    else
    {
        usr_env.imeas.stable_counter = 0;
    }

    if(usr_env.imeas.stable_counter > HTPT_INTERM_MEAS_STABLE_COUNTER_MAX)
    {
        QPRINTF("intermediate temperature is stable\r\n");
        return true;
    }
    else
        return false;
}

/**
 ****************************************************************************************
 * @brief   intermediate temperature value reset.
 * User may overwrite this function.
 ****************************************************************************************
 */
void app_interm_temp_stable_status_reset(void)
{
    usr_env.imeas.max = -1000;
    usr_env.imeas.min = 1000;
    usr_env.imeas.stable_counter = 0;
}

/**
 ****************************************************************************************
 * @brief idle connection timeout timer event.
 *
 * @param[in] msgid     APP_HTPT_IDLE_CONNECTION_TIMEOUT_TIMER
 * @param[in] param     None
 * @param[in] dest_id   TASK_APP
 * @param[in] src_id    TASK_APP
 *
 * @return If the message was consumed or not.
 * @description
 ****************************************************************************************
 */
int app_htpt_idle_connection_timeout_timer_handler(ke_msg_id_t const msgid, void const *param, ke_task_id_t
                                                   const dest_id, ke_task_id_t const src_id)
{
    // there are two place to disconnect, one is idle connection timeout,
    // the other is temperature measure send complete
    usr_env.is_idle_connection_overtime = true;

    QPRINTF("idle connection timeout\r\n");

    if(!usr_env.is_meas_send_busy && !usr_env.is_imeas_send_busy)
    {
        QPRINTF("Force disconnect\r\n");
        app_gap_discon_req(app_htpt_env->conhdl);
    }
    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief initialize idle connection timeout timer
 ****************************************************************************************
 */
void app_set_idle_connection(void)
{
    usr_env.is_idle_connection_overtime = false;
    ke_timer_set(APP_HTPT_IDLE_CONNECTION_TIMEOUT_TIMER, TASK_APP, HTPT_IDLE_CONNECTION_TIMEOUT);
}

/**
 ****************************************************************************************
 * @brief Clear all stored measure value
 ****************************************************************************************
 */
void app_clear_stored_meas(void)
{
    app_store_meas_fifo_reset();
}

/**
 ****************************************************************************************
 * @brief Send stored measure value, if no data to send, will disconnect
 ****************************************************************************************
 */
void app_send_stored_meas(void)
{
    if(usr_env.is_temp_meas_config)
    {
        struct htp_temp_meas temp_meas;

        // if not empty, send the indication
        if(app_store_meas_fifo_out(&temp_meas))
        {
            usr_env.is_meas_send_busy = true;
            app_htpt_temp_send(app_htpt_env->conhdl, &temp_meas, TEMPERATURE_MEASUREMENT);
        }
        else
        {
            // there are two place to disconnect, one is idle connection timeout,
            // the other is temperature measure send complete
            usr_env.is_meas_send_busy = false; // set measure send complete
            if(usr_env.is_idle_connection_overtime && !usr_env.is_imeas_send_busy) // idle connection timeout
            {
                QPRINTF("Force disconnect\r\n");
                app_gap_discon_req(app_htpt_env->conhdl);
            }
        }
    }
}

/**
 ****************************************************************************************
 * @brief Store measure value to fifo
 ****************************************************************************************
 */
void app_stored_meas_indicate(const struct htp_temp_meas* pmeas)
{
    app_store_meas_fifo_in(pmeas);
}

/**
 ****************************************************************************************
 * @brief parametter update
 ****************************************************************************************
 */
void app_param_update(const struct gap_le_create_conn_req_cmp_evt * pconnect)
{
    ke_timer_clear(APP_ADV_INTV_UPDATE_TIMER, TASK_APP);
    usr_led1_set(LED_ON_DUR_CON, LED_OFF_DUR_CON);

    // Update cnx parameters
    if (pconnect->conn_info.con_interval < GAP_PPCP_CONN_INTV_MIN)
    {
        // Update connection parameters here
        struct gap_conn_param_update conn_par;
        /// Connection interval minimum
        conn_par.intv_min = GAP_PPCP_CONN_INTV_MIN;
        /// Connection interval maximum
        conn_par.intv_max = GAP_PPCP_CONN_INTV_MAX;
        /// Latency
        conn_par.latency = GAP_PPCP_SLAVE_LATENCY;
        /// Supervision timeout, Time = N * 10 msec
        conn_par.time_out = GAP_PPCP_STO_MULT;
        app_gap_param_update_req(pconnect->conn_info.conhdl, &conn_par);
    }
}

/**
 ****************************************************************************************
 * @brief Handles LED status timer.
 *
 * @param[in] msgid      APP_SYS_UART_DATA_IND
 * @param[in] param      Pointer to struct app_uart_data_ind
 * @param[in] dest_id    TASK_APP
 * @param[in] src_id     TASK_APP
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
int app_led_timer_handler(ke_msg_id_t const msgid, void const *param,
                               ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    if(msgid == APP_SYS_LED_1_TIMER)
    {
        usr_led1_process();
    }

    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief Handles advertising mode timer event.
 *
 * @param[in] msgid     APP_ADV_INTV_UPDATE_TIMER
 * @param[in] param     None
 * @param[in] dest_id   TASK_APP
 * @param[in] src_id    TASK_APP
 *
 * @return If the message was consumed or not.
 * @description
 *
 * This handler is used to inform the application that first phase of adversting mode is timeout.
 ****************************************************************************************
 */
int app_gap_adv_intv_update_timer_handler(ke_msg_id_t const msgid, void const *param,
                                          ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    if(APP_ADV == ke_state_get(TASK_APP))
    {
        usr_led1_set(LED_ON_DUR_IDLE, LED_OFF_DUR_IDLE);

        // Update Advertising Parameters
        app_gap_adv_start_req(GAP_GEN_DISCOVERABLE|GAP_UND_CONNECTABLE,
                                app_env.adv_data, app_set_adv_data(GAP_GEN_DISCOVERABLE),
                                app_env.scanrsp_data, app_set_scan_rsp_data(app_get_local_service_flag()),
                                GAP_ADV_SLOW_INTV, GAP_ADV_SLOW_INTV);
    }

    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief   Start the device to advertising process.
 ****************************************************************************************
 */
void app_start_adv(void)
{
    if(! app_htpt_env->enabled)
    {
        if(APP_IDLE == ke_state_get(TASK_APP))
        {
            app_gap_adv_start_req(GAP_GEN_DISCOVERABLE|GAP_UND_CONNECTABLE,
                    app_env.adv_data, app_set_adv_data(GAP_GEN_DISCOVERABLE),
                    app_env.scanrsp_data, app_set_scan_rsp_data(app_get_local_service_flag()),
                    GAP_ADV_FAST_INTV1, GAP_ADV_FAST_INTV2);
        }
    }
}

/**
 ****************************************************************************************
 * @brief Get celsius temperature
 ****************************************************************************************
 */
int32_t app_get_celsius_temperature(void)
{
    int32_t temp_average_adc_value = 0;
    int32_t temperature_x10;
    int     i;

    // Calculate average value
    for(i = 0; i < HTPT_TEMP_SAMPLE_NUMBER; ++i)
        temp_average_adc_value += usr_env.htpt_reg_buf[i];

    temp_average_adc_value /= HTPT_TEMP_SAMPLE_NUMBER;

    QPRINTF("ADC: %d\r\n", temp_average_adc_value);
    temperature_x10 = TEMPERATURE_X10(temp_average_adc_value);

    return temperature_x10;
}

/**
 ****************************************************************************************
 * @brief Get measure indicate data
 ****************************************************************************************
 */
int32_t app_get_meas_indicate_data(struct htp_temp_meas* pmeas)
{
    int32_t temperature_x10;
    qn_tm_t time = {0};

    temperature_x10 = app_get_celsius_temperature();

#if HTPT_USE_FARENHEIT
    temperature_x10 = HTPT_CELCIUS_TO_FARENHEIT_X10(temperature_x10);
#endif

    // The temperature value is 32-BIT FLOATING POINT DATA TYPE (FLOAT-TYPE)
    // which is defined in "ISO/IEEE Std. 11073"
    pmeas->temp = 0xFF000000 | temperature_x10;

    // Get time. Before use this function, please use qn_time_set() function
    // to set a real time.
    qn_time_get(&time);

    // Time stamp
    pmeas->time_stamp.year  = time.year;
    pmeas->time_stamp.month = time.month;
    pmeas->time_stamp.day   = time.day;
    pmeas->time_stamp.hour  = time.hour;
    pmeas->time_stamp.min   = time.minutes;
    pmeas->time_stamp.sec   = time.seconds;

    pmeas->type = ARMPIT;

#if HTPT_USE_FARENHEIT
    pmeas->flags = HTPT_FLAG_FAHRENHEIT | HTPT_FLAG_TIME | HTPT_FLAG_TYPE;
#else
    pmeas->flags = HTPT_FLAG_CELSIUS | HTPT_FLAG_TIME | HTPT_FLAG_TYPE;
#endif

    QPRINTF("indicate: %.1f%s %04d/%02d/%02d - %02d:%02d:%02d\r\n", 
                       temperature_x10/10.0, HTPT_USE_FARENHEIT ? "F" : "C", 
                       pmeas->time_stamp.year, pmeas->time_stamp.month, pmeas->time_stamp.day,
                       pmeas->time_stamp.hour, pmeas->time_stamp.min, pmeas->time_stamp.sec);

    return temperature_x10;
}

/**
 ****************************************************************************************
 * @brief Get measure notify data
 ****************************************************************************************
 */
int32_t app_get_imeas_notify_data(struct htp_temp_meas* pmeas)
{
    int32_t temperature_x10;

    temperature_x10 = app_get_celsius_temperature();

#if HTPT_USE_FARENHEIT
    temperature_x10 = HTPT_CELCIUS_TO_FARENHEIT_X10(temperature_x10);
#endif

    // The temperature value is 32-BIT FLOATING POINT DATA TYPE (FLOAT-TYPE)
    // which is defined in "ISO/IEEE Std. 11073"
    pmeas->temp = 0xFF000000 | temperature_x10;

#if HTPT_USE_FARENHEIT
    pmeas->flags = HTPT_FLAG_FAHRENHEIT;
#else
    pmeas->flags = HTPT_FLAG_CELSIUS;
#endif

    QPRINTF("notify: %.1f%s\r\n", temperature_x10/10.0, HTPT_USE_FARENHEIT ? "F" : "C");

    return temperature_x10;
}

/**
 ****************************************************************************************
 * @brief  temperature measure data indicate to client. If disconnect, start to advertise.
 ****************************************************************************************
 */
void app_temp_meas_indicate(void)
{
    struct htp_temp_meas temp_meas = {0};

    app_get_meas_indicate_data(&temp_meas);

    app_stored_meas_indicate(&temp_meas);

    if(app_htpt_env->enabled)
    {
        if(! usr_env.is_meas_send_busy)
        {
            app_send_stored_meas();
        }
    }
    else
    {
        //advertise measurement is ready
        app_start_adv();
    }
}

/**
 ****************************************************************************************
 * @brief  If the intermediate temperature value is stable, indicate to client and stop notify. 
 *         otherwise notify to client.
 *
 * To avoid transmitting unnecessary data, the Time Stamp and Temperature Type fields
 * should not be used in the Intermediate Temperature characteristic.
 ****************************************************************************************
 */
void app_interm_temp_notify(void)
{
    int32_t temperature_x10;
    struct htp_temp_meas temp_meas;

    temperature_x10 = app_get_imeas_notify_data(&temp_meas);

    if(app_is_interm_temp_stable(temperature_x10))
    {
        usr_env.is_imeas_send_busy = false;
        usr_env.is_temp_imeas_config = false;
        if(usr_env.is_temp_meas_config)
        {
            app_temp_meas_indicate();
        }

        if(! usr_env.is_temp_meas_config && usr_env.is_meas_running)
        {
            usr_env.is_meas_running = false;
            ke_timer_clear(APP_HTPT_PERIOD_MEAS_TIMER, TASK_APP);
        }
    }
    else
    {
        // send the notify
        usr_env.is_imeas_send_busy = true;
        app_htpt_temp_send(app_htpt_env->conhdl, &temp_meas, INTERMEDIATE_MEASUREMENT);
    }

}

/**
 ****************************************************************************************
 * @brief ADC sample complete handler
 ****************************************************************************************
 */
void app_event_adc_sample_cmp_handler(void)
{
    ke_evt_clear(1UL << EVENT_ADC_SAMPLE_CMP_ID);

    // CLose ADC and temp sensor
    temp_sensor_enable(MASK_DISABLE);
    adc_clock_off();
    adc_power_off();
    
    if(usr_env.is_temp_meas_config && usr_env.is_should_indicate)
    {
        usr_env.is_should_indicate = false;
        app_temp_meas_indicate();
    }

    if (usr_env.is_temp_imeas_config)
    {
        app_interm_temp_notify();
    }
}

/**
 ****************************************************************************************
 * @brief callback of adc sample complete.
 ****************************************************************************************
 */
void adc_sample_complete_callback(void)
{
    ke_evt_set(1UL << EVENT_ADC_SAMPLE_CMP_ID);        
}

/**
 ****************************************************************************************
 * @brief start measure temperature
 ****************************************************************************************
 */
void start_measure_temperature(void)
{
    
    // eanble temperature sensor
    temp_sensor_enable(MASK_ENABLE);
 
    // ADC initialize
    adc_init(ADC_DIFF_WITH_BUF_DRV, ADC_CLK_1000000, ADC_INT_REF, ADC_12BIT);
    
    // Read voltage. use interrupt
    adc_read_configuration read_cfg;
    read_cfg.trig_src = ADC_TRIG_SOFT;
    read_cfg.mode = CONTINUE_MOD;
    read_cfg.start_ch = TEMP;
    read_cfg.end_ch = TEMP;
    adc_read(&read_cfg, usr_env.htpt_reg_buf, HTPT_TEMP_SAMPLE_NUMBER, adc_sample_complete_callback);
 
}


/**
 ****************************************************************************************
 * @brief  dealwith new configuration value
 ****************************************************************************************
 */
void app_char_config_desc_change(const struct htpt_cfg_indntf_ind *pcfg)
{
    switch (pcfg->char_code)
    {
        case HTPT_TEMP_MEAS_CHAR:
            if (pcfg->cfg_val == PRF_CLI_START_IND)
            {
                usr_env.is_temp_meas_config = true;

                // send store measure if sending is not busy
                if(! usr_env.is_meas_send_busy)
                    app_send_stored_meas();

                usr_env.meas_counter = 0;

                if(! usr_env.is_meas_running && app_htpt_env->meas_intv != 0)
                {
                    usr_env.is_meas_running = true;
                        
                    // measure and indicate immediately
                    usr_env.is_should_indicate = true;
                    start_measure_temperature();
            
                    ke_timer_set(APP_HTPT_PERIOD_MEAS_TIMER, TASK_APP, HTPT_PERIOD_MEAS_TIME);
                }
            }
            else
            {
                usr_env.is_temp_meas_config = false;
                usr_env.is_should_indicate = false;
                
                // clear store measure
                app_clear_stored_meas();
                
                if(! usr_env.is_temp_imeas_config && usr_env.is_meas_running)
                {
                    usr_env.is_meas_running = false;
                    ke_timer_clear(APP_HTPT_PERIOD_MEAS_TIMER, TASK_APP);
                }
            }
            break;

        case HTPT_INTERM_TEMP_CHAR:
            if (pcfg->cfg_val == PRF_CLI_START_NTF)
            {
                usr_env.is_temp_imeas_config = true;
                app_interm_temp_stable_status_reset();
                if(! usr_env.is_meas_running)
                {
                    usr_env.is_meas_running = true;
                    ke_timer_set(APP_HTPT_PERIOD_MEAS_TIMER, TASK_APP, HTPT_PERIOD_MEAS_TIME);
                }

            }
            else
            {
                usr_env.is_temp_imeas_config = false;
                usr_env.is_imeas_send_busy = false;
                if(! usr_env.is_temp_meas_config && usr_env.is_meas_running)
                {
                    usr_env.is_meas_running = false;
                    ke_timer_clear(APP_HTPT_PERIOD_MEAS_TIMER, TASK_APP);
                }
            }
            break;

        case HTPT_MEAS_INTV_CHAR:
            if (pcfg->cfg_val == PRF_CLI_START_IND)
            {
                usr_env.is_temp_interval_config = true;
                // This is not must
                app_htpt_measurement_intv_send(app_htpt_env->conhdl, app_htpt_env->meas_intv);
            }
            else
            {
                usr_env.is_temp_interval_config = false;
            }
            break;

        default:
            break;
    }
}

/**
 ****************************************************************************************
 * @brief   Application task message handler
 ****************************************************************************************
 */
void app_task_msg_hdl(ke_msg_id_t const msgid, void const *param)
{
    switch(msgid)
    {
        case GAP_SET_MODE_REQ_CMP_EVT:
            if(APP_IDLE == ke_state_get(TASK_APP))
            {
                usr_led1_set(LED_ON_DUR_ADV_FAST, LED_OFF_DUR_ADV_FAST);
                ke_timer_set(APP_ADV_INTV_UPDATE_TIMER, TASK_APP, 30 * 100);
#if (defined(QN_ADV_WDT))
                usr_env.adv_wdt_enable = true;
#endif
            }
            else if(APP_ADV == ke_state_get(TASK_APP))
            {
                usr_led1_set(LED_ON_DUR_ADV_SLOW, LED_OFF_DUR_ADV_SLOW);
#if (defined(QN_ADV_WDT))
                usr_env.adv_wdt_enable = true;
#endif
            }
            else if(APP_INIT == ke_state_get(TASK_APP))
            {
                const qn_tm_t time = {2013,8,20,14,40,00};
                
                // initialize ble time
                qn_time_init();
                
                // Set a real time, this is only a example.
                // User should use this function in other condition.
                qn_time_set(&time);
            }
            break;

        case GAP_ADV_REQ_CMP_EVT:
            usr_led1_set(LED_ON_DUR_IDLE, LED_OFF_DUR_IDLE);
            ke_timer_clear(APP_ADV_INTV_UPDATE_TIMER, TASK_APP);
            break;

        case GAP_DISCON_CMP_EVT:
            usr_led1_set(LED_ON_DUR_IDLE, LED_OFF_DUR_IDLE);
            usr_env.is_idle_connection_overtime = false;
            ke_timer_clear(APP_HTPT_IDLE_CONNECTION_TIMEOUT_TIMER, TASK_APP);
            break;

        case GAP_LE_CREATE_CONN_REQ_CMP_EVT:
            {
                const struct gap_le_create_conn_req_cmp_evt * const pconnect = param;
                if(pconnect->conn_info.status == CO_ERROR_NO_ERROR)
                {
                    if(GAP_PERIPHERAL_SLV == app_get_role())
                    {
                        // Update cnx parameters
                        app_param_update(pconnect);

                        // set idle connection timeout event
                        app_set_idle_connection();

#if (defined(QN_ADV_WDT))
                        usr_env.adv_wdt_enable = false;
#endif
                    }
                }
            }
            break;

        case HTPT_DISABLE_IND:
            if(! usr_env.is_temp_meas_config)
            {
                usr_env.is_meas_running = false;
                //always stop intermediate timer
                ke_timer_clear(APP_HTPT_PERIOD_MEAS_TIMER, TASK_APP);
            }
            break;

        case HTPT_CFG_INDNTF_IND:
            app_char_config_desc_change(param);
            break;

        case HTPT_TEMP_SEND_CFM:
            QPRINTF("Temperature send confirm: %d\r\n", ((struct gatt_notify_cmp_evt const *)param)->status);
            app_send_stored_meas();
            break;

        case HTPT_MEAS_INTV_CHG_IND:
            if(usr_env.is_temp_meas_config)
            {
                if(app_htpt_env->meas_intv != 0)
                {
                    ke_timer_set(APP_HTPT_PERIOD_MEAS_TIMER, TASK_APP,
                                app_htpt_env->meas_intv * 100); // trans unit from 1sec to 10ms
                }
                else
                {
                    ke_timer_clear(APP_HTPT_PERIOD_MEAS_TIMER, TASK_APP);
                }
            }
            break;

        default:
            break;
    }
}

/**
 ****************************************************************************************
 * @brief Handles the temperature timer.
 *
 * @param[in] msgid     APP_HTPT_PERIOD_MEAS_TIMER
 * @param[in] param     None
 * @param[in] dest_id   TASK_APP
 * @param[in] src_id    TASK_APP
 *
 * @return If the message was consumed or not.
 * @description
 *  Handles the temperature timer.
 ****************************************************************************************
 */
int app_htpt_period_meas_timer_handler(ke_msg_id_t const msgid,
                                       void const *param,
                                       ke_task_id_t const dest_id,
                                       ke_task_id_t const src_id)
{
    bool start_meas = false;

    // Measurement Ready - send if Client Configuration is Configured,
    // and regardless of connected
    if(usr_env.is_temp_meas_config)
    {
        ++usr_env.meas_counter;

        ASSERT_ERR(app_htpt_env->meas_intv != 0);
        //if interval is zero, don't send indication
        if( usr_env.meas_counter % app_htpt_env->meas_intv == 0 )
        {
            start_meas = true;
            usr_env.is_should_indicate = true;
        }
    }

    if (usr_env.is_temp_imeas_config)
        start_meas = true;

    if(start_meas)
        start_measure_temperature();

    ke_timer_set(APP_HTPT_PERIOD_MEAS_TIMER, TASK_APP, HTPT_PERIOD_MEAS_TIME);

    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief   Restore peripheral setting after wakeup
 ****************************************************************************************
 */
void usr_sleep_restore(void)
{
#if QN_DBG_PRINT
    uart_init(QN_DEBUG_UART, USARTx_CLK(0), UART_9600);
    uart_tx_enable(QN_DEBUG_UART, MASK_ENABLE);
    uart_rx_enable(QN_DEBUG_UART, MASK_ENABLE);
#endif

#if (defined(QN_ADV_WDT))
    if(usr_env.adv_wdt_enable)
    {
        wdt_init(1007616, WDT_INT_MOD); // 30.75s
    }
#endif
}

/**
 ****************************************************************************************
 * @brief Handles button press after cancel the jitter.
 *
 * @param[in] msgid     APP_SYS_BUTTON_1_TIMER
 * @param[in] param     None
 * @param[in] dest_id   TASK_APP
 * @param[in] src_id    TASK_APP
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
int app_button_timer_handler(ke_msg_id_t const msgid, void const *param,
                               ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    switch(msgid)
    {
        case APP_SYS_BUTTON_1_TIMER:
            // make sure the button is pressed
            if(gpio_read_pin(BUTTON1_PIN) == GPIO_LOW)
            {
                if(APP_IDLE == ke_state_get(TASK_APP))
                {
                    struct app_htpt_env_tag *app_htpt_env = &app_env.htpt_ev;
                    if(!app_htpt_env->enabled)
                    {
                        // start adv
                        app_gap_adv_start_req(GAP_GEN_DISCOVERABLE|GAP_UND_CONNECTABLE,
                                app_env.adv_data, app_set_adv_data(GAP_GEN_DISCOVERABLE),
                                app_env.scanrsp_data, app_set_scan_rsp_data(app_get_local_service_flag()),
                                GAP_ADV_FAST_INTV1, GAP_ADV_FAST_INTV2);

#if (QN_DEEP_SLEEP_EN)
                        // prevent entering into deep sleep mode
                        sleep_set_pm(PM_SLEEP);
#endif
                    }
                }
                else if(APP_ADV == ke_state_get(TASK_APP))
                {
                    // stop adv
                    app_gap_adv_stop_req();

#if (QN_DEEP_SLEEP_EN)
                    // allow entering into deep sleep mode
                    sleep_set_pm(PM_DEEP_SLEEP);
#endif
                }
            }
            break;
        default:
            ASSERT_ERR(0);
            break;
    }

    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief Handles button press before key debounce.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
void app_event_button1_press_handler(void)
{
#if ((QN_DEEP_SLEEP_EN) && (!QN_32K_RCO))
    if (sleep_env.deep_sleep) 
    {
        sleep_env.deep_sleep = false;
        // start 32k xtal wakeup timer
        wakeup_32k_xtal_start_timer();
    }
#endif

    // delay 20ms to debounce
    ke_timer_set(APP_SYS_BUTTON_1_TIMER, TASK_APP, 2);
    ke_evt_clear(1UL << EVENT_BUTTON1_PRESS_ID);
}

/**
 ****************************************************************************************
 * @brief   Button 1 click callback
 * @description
 *  Button 1 is used to enter adv mode.
 ****************************************************************************************
 */
void usr_button1_cb(void)
{
    // If BLE is in the sleep mode, wakeup it.
    if(ble_ext_wakeup_allow())
    {
#if ((QN_DEEP_SLEEP_EN) && (!QN_32K_RCO))
        if (sleep_env.deep_sleep)
        {
            wakeup_32k_xtal_switch_clk();
        }
#endif

        sw_wakeup_ble_hw();

// #if (QN_DEEP_SLEEP_EN)
//         // prevent deep sleep
//         if(sleep_get_pm() == PM_DEEP_SLEEP)
//         {
//             sleep_set_pm(PM_SLEEP);
//         }
// #endif
    }

    // key debounce:
    // We can set a soft timer to debounce.
    // After wakeup BLE, the timer is not calibrated immediately and it is not precise.
    // So We set a event, in the event handle, set the soft timer.
    ke_evt_set(1UL << EVENT_BUTTON1_PRESS_ID);
}

/**
 ****************************************************************************************
 * @brief   All GPIO interrupt callback
 ****************************************************************************************
 */
void gpio_interrupt_callback(enum gpio_pin pin)
{
    switch(pin)
    {
        case BUTTON1_PIN:
            //Button 1 is used to enter adv mode.
            usr_button1_cb();
            break;

#if (defined(QN_TEST_CTRL_PIN))
        case QN_TEST_CTRL_PIN:
            //When test controll pin is changed to low level, this function will reboot system.
            gpio_disable_interrupt(QN_TEST_CTRL_PIN);
            syscon_SetCRSS(QN_SYSCON, SYSCON_MASK_REBOOT_SYS);
            break;
#endif

        default:
            break;
    }
}

/**
 ****************************************************************************************
 * @brief   User initialize
 ****************************************************************************************
 */
void usr_init(void)
{
    if(KE_EVENT_OK != ke_evt_callback_set(EVENT_BUTTON1_PRESS_ID,
                                            app_event_button1_press_handler))
    {
        ASSERT_ERR(0);
    }

    if(KE_EVENT_OK != ke_evt_callback_set(EVENT_ADC_SAMPLE_CMP_ID,
                                            app_event_adc_sample_cmp_handler))
    {
        ASSERT_ERR(0);
    }
}

/// @} USR


