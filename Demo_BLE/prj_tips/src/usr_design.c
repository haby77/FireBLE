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
#if (defined(QN_ADV_WDT))
#include "wdt.h"
#endif
#include "sleep.h"

/*
 * MACRO DEFINITIONS
 ****************************************************************************************
 */

#define LED_ON_DUR_ADV_FAST        2
#define LED_OFF_DUR_ADV_FAST       (uint16_t)((GAP_ADV_FAST_INTV2*0.625)/10)
#define LED_ON_DUR_ADV_SLOW        2
#define LED_OFF_DUR_ADV_SLOW       (uint16_t)((GAP_ADV_SLOW_INTV*0.625)/10)
#define LED_ON_DUR_CON             0xffff
#define LED_OFF_DUR_CON                 0
#define LED_ON_DUR_IDLE                 0
#define LED_OFF_DUR_IDLE                0xffff

#define APP_SEND_CURRENT_TIME_TIMER     1200 // 12s

#define EVENT_BUTTON1_PRESS_ID          0

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
struct usr_env_tag usr_env = {LED_ON_DUR_IDLE, LED_OFF_DUR_IDLE, false, adv_wdt_to_handler};
#else
struct usr_env_tag usr_env = {LED_ON_DUR_IDLE, LED_OFF_DUR_IDLE};
#endif

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
 * @brief Just used for test - at connection
 * @description
 *  Just used for test - at connection
 ****************************************************************************************
 */
static void app_tips_set_value(void)
{
    //DEVELOPER NOTE: SET loc_time_info to REAL VALUE OF USER APPLICATION HERE
    struct tip_loc_time_info loc_time_info;

    //  -48	        -44        ...      4           56
    //  UTC-12:00   UTC-11:00  ...      UTC+1:00    UTC+14:00
    loc_time_info.time_zone = 8;

    // 0	Standard Time
    // 2	Half An Hour Daylight Time (+0.5h)
    // 4	Daylight Time (+1h)
    // 8	Double Daylight Time (+2h)
    loc_time_info.dst_offset = 8;
    app_tips_upd_local_time_info_req(app_tips_env->conhdl, &loc_time_info);

    //DEVELOPER NOTE: SET ref_time_info to REAL VALUE OF USER APPLICATION HERE
    struct tip_ref_time_info ref_time_info;
    // 0	Unknown
    // 1	Network Time Protocol
    // 2	GPS
    // 3	Radio Time Signal
    // 4	Manual
    // 5	Atomic Clock
    // 6	Cellular Network
    ref_time_info.time_source = 4;

    // Accuracy (drift) of time information in steps of 1/8 of a second (125ms) 
    // compared to a reference time source. Valid range from 0 to 253 (0s to 31.5s).
    // A value of 254 means Accuracy is out of range (> 31.5s). 
    // A value of 255 means Accuracy is unknown.
    ref_time_info.time_accuracy = 252;

    // Days since last update about Reference Source
    // Min value : 0, Max value : 254
    // 255 represents 255 or more days
    ref_time_info.days_update = 240;

    // Hours since update about Reference Source
    // Min value : 0, Max value : 23
    // 255 represents 255 or more days (If Days Since Update = 255, then Hours Since Update shall
    // also be set to 255)
    ref_time_info.hours_update = 22;
    app_tips_upd_ref_time_info_req(app_tips_env->conhdl, &ref_time_info);

    //DEVELOPER NOTE: SET time_with_dst to REAL VALUE OF USER APPLICATION HERE
    struct tip_time_with_dst time_with_dst;
    time_with_dst.date_time.year = 2012;
    time_with_dst.date_time.month = 10;
    time_with_dst.date_time.day = 1;
    time_with_dst.date_time.hour = 12;
    time_with_dst.date_time.min = 58;
    time_with_dst.date_time.sec = 30;
    time_with_dst.dst_offset = 230;
    app_tips_upd_time_dst_req(app_tips_env->conhdl, &time_with_dst);
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
            break;

        case GAP_ADV_REQ_CMP_EVT:
            usr_led1_set(LED_ON_DUR_IDLE, LED_OFF_DUR_IDLE);
            ke_timer_clear(APP_ADV_INTV_UPDATE_TIMER, TASK_APP);
            break;

        case GAP_DISCON_CMP_EVT:
            usr_led1_set(LED_ON_DUR_IDLE, LED_OFF_DUR_IDLE);

            // start adv
            app_gap_adv_start_req(GAP_GEN_DISCOVERABLE|GAP_UND_CONNECTABLE,
                    app_env.adv_data, app_set_adv_data(GAP_GEN_DISCOVERABLE),
                    app_env.scanrsp_data, app_set_scan_rsp_data(app_get_local_service_flag()),
                    GAP_ADV_FAST_INTV1, GAP_ADV_FAST_INTV2);
            break;

        case GAP_LE_CREATE_CONN_REQ_CMP_EVT:
            if(((struct gap_le_create_conn_req_cmp_evt *)param)->conn_info.status == CO_ERROR_NO_ERROR)
            {
                if(GAP_PERIPHERAL_SLV == app_get_role())
                {
                    ke_timer_clear(APP_ADV_INTV_UPDATE_TIMER, TASK_APP);
                    usr_led1_set(LED_ON_DUR_CON, LED_OFF_DUR_CON);
#if (defined(QN_ADV_WDT))
                    usr_env.adv_wdt_enable = false;
#endif

                    // Update cnx parameters
                    if (((struct gap_le_create_conn_req_cmp_evt *)param)->conn_info.con_interval < GAP_PPCP_CONN_INTV_MIN)
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
                        app_gap_param_update_req(((struct gap_le_create_conn_req_cmp_evt *)param)->conn_info.conhdl, &conn_par);
                    }
                }
                
                // Set time information for demo
                app_tips_set_value();
            }
            break;

        case TIPS_DISABLE_IND:
            ke_timer_clear(APP_TIPS_CURRENT_TIME_TIMER, TASK_APP);
            break;

        case TIPS_CURRENT_TIME_CCC_IND:
            if (((struct tips_current_time_ccc_ind *)param)->cfg_val == PRF_CLI_START_NTF) 
            {
                app_tips_env->ntf_sending = false;
                ke_timer_set(APP_TIPS_CURRENT_TIME_TIMER, TASK_APP, APP_SEND_CURRENT_TIME_TIMER);
            }
            else
            {
                ke_timer_clear(APP_TIPS_CURRENT_TIME_TIMER, TASK_APP);
            }
            break;
            
        case TIPS_TIME_UPD_CTNL_PT_IND:
            {
                // Just for demo
                struct tip_time_upd_state time_upd_state;            
                time_upd_state.current_state = TIPS_TIME_UPD_STATE_IDLE;
                time_upd_state.result = TIPS_TIME_UPD_RESULT_SUCCESS;
                app_tips_upd_time_upd_state_req(app_tips_env->conhdl, &time_upd_state);
            }
            break;
    
        default:
            break;
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
 * @brief Handles the current time timer.
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     None
 * @param[in] dest_id   TASK_APP
 * @param[in] src_id    TASK_APP
 *
 * @return If the message was consumed or not.
 * @description
 *  Handles the current time timer.
 ****************************************************************************************
 */
int app_tips_current_time_timer_handler(ke_msg_id_t const msgid,
                                        void *param,
                                        ke_task_id_t const dest_id,
                                        ke_task_id_t const src_id)
{
    if (app_tips_env->ntf_sending == false)
    {
        struct tip_curr_time current_time;

        current_time.exact_time_256.day_date_time.date_time.year = 2012;
        current_time.exact_time_256.day_date_time.date_time.month = 8;
        current_time.exact_time_256.day_date_time.date_time.day = 29;
        current_time.exact_time_256.day_date_time.date_time.hour = 12;
        current_time.exact_time_256.day_date_time.date_time.min = 58;
        current_time.exact_time_256.day_date_time.date_time.sec = 30;
        //  1 Monday
        //  2 Tuesday
        //  3 Wednesday
        //  4 Thursday
        //  5 Friday
        //  6 Saturday
        //  7 Sunday
        current_time.exact_time_256.day_date_time.day_of_week = 5;
        current_time.exact_time_256.fraction_256 = 10; /// 0 ~ 255
        current_time.adjust_reason = TIPS_FLAG_MAN_TIME_UPDATE;
        app_tips_upd_curr_time_req(app_tips_env->conhdl, &current_time, 1);
        
        // Allow new notification util last notification complete
        app_tips_env->ntf_sending = true;
    }
    ke_timer_set(APP_TIPS_CURRENT_TIME_TIMER, TASK_APP, APP_SEND_CURRENT_TIME_TIMER);
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
 * @param[in] msgid     Id of the message received
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
                    struct app_tips_env_tag *app_tips_env = &app_env.tips_ev;
                    if(!app_tips_env->enabled)
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
}

/// @} USR

