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
#define LED_OFF_DUR_CON         0
#define LED_ON_DUR_IDLE         0
#define LED_OFF_DUR_IDLE        0xffff

#define APP_GLPS_STROED_RECORDS_NUM     10
#define APP_GLPS_RACP_RSP_REQ_TO        1300    // 13s
#define APP_GLPS_MEAS_SEND_TO           1200    // 12s
#define APP_GLPS_MEAS_WITH_CTX_REQ_TO   1500    // 15s

#define EVENT_BUTTON1_PRESS_ID            0

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
 * @brief This function is used for demonstration.
 *
 * @param[in] ctx_exist Send Glucose Measurement value with Context or not
 *
 * @return None.
 ****************************************************************************************
 */
static void app_glps_meas_send(uint8_t ctx_exist)
{
    struct glp_meas meas;
    struct glp_meas_ctx ctx;

    //DEVELOPER NOTE: SET REAL VALUE OF USER APPLICATION IN THIS FUNCTION
    meas.flags = GLP_MEAS_TIME_OFF_PRES |
                 GLP_MEAS_GL_CTR_TYPE_AND_SPL_LOC_PRES |
                 GLP_MEAS_GL_CTR_UNITS_MOL_L |
                 GLP_MEAS_SENS_STAT_ANNUN_PRES |
                 (ctx_exist == true ? GLP_MEAS_CTX_INF_FOLW : GLP_MEAS_CTX_INF_NOT_FOLW);

    if (meas.flags & GLP_MEAS_TIME_OFF_PRES)
    {
        meas.time_offset = 13;
    }

    if (meas.flags & GLP_MEAS_GL_CTR_TYPE_AND_SPL_LOC_PRES)
    {
        meas.location = GLP_LOC_EARLOBE;        // see enum glp_meas_loc
        meas.type = GLP_TYPE_CAPILLARY_PLASMA;  // see enum glp_meas_type
    }

    if (meas.flags & GLP_MEAS_GL_CTR_UNITS_MOL_L)
        meas.concentration = 25;
    else
        meas.concentration = 30;

    if (meas.flags & GLP_MEAS_SENS_STAT_ANNUN_PRES)
        meas.status = GLP_MEAS_STATE_DEV_BAT_LOW;   // enum glp_meas_state

    if (meas.flags & GLP_MEAS_CTX_INF_FOLW)
    {
        ctx.flags = GLP_CTX_CRBH_ID_AND_CRBH_PRES | // see enum glp_meas_ctx_flag
                    GLP_CTX_MEAL_PRES |
                    GLP_CTX_TESTER_HEALTH_PRES |
                    GLP_CTX_EXE_DUR_AND_EXE_INTENS_PRES |
                    GLP_CTX_MEDIC_ID_AND_MEDIC_PRES |
                    GLP_CTX_MEDIC_VAL_UNITS_L |
                    GLP_CTX_HBA1C_PRES |
                    GLP_CTX_EXTD_F_PRES;
                    
        if (ctx.flags & GLP_CTX_CRBH_ID_AND_CRBH_PRES)
        {
            ctx.carbo_id = GLP_CID_BREAKFAST;   // see enum glp_meas_ctx_carbo
            ctx.carbo_val = 20;                 // kilograms
        }

        if (ctx.flags & GLP_CTX_MEAL_PRES)
        {
            ctx.meal = GLP_MEAL_POSTPRANDIAL;   //see enum glp_meas_ctx_meal
        }

        if (ctx.flags & GLP_CTX_TESTER_HEALTH_PRES)
        {
            ctx.tester = GLP_TESTER_TESTER_VAL_NOT_AVA;     //see enum glp_meas_ctx_tester_type
            ctx.health = GLP_HEALTH_MINOR_HEALTH_ISSUES;    //see enum glp_meas_ctx_feeling       
        }
        
        if (ctx.flags & GLP_CTX_EXE_DUR_AND_EXE_INTENS_PRES)
        {
            ctx.exercise_dur = 100;     // in seconds
            ctx.exercise_intens = 10;   // percentage
        }

        if (ctx.flags & GLP_CTX_MEDIC_ID_AND_MEDIC_PRES)
        {
            ctx.med_id = GLP_MEDID_INTER_ACTING_INSULIN;    // enum glp_meas_ctx_med_id
            if (ctx.flags & GLP_CTX_MEDIC_VAL_UNITS_L)
                ctx.med_val = 11;                           // liters
            else
                ctx.med_val = 12;                           // kilograms
        }

        if (ctx.flags & GLP_CTX_HBA1C_PRES)
        {
            ctx.hba1c_val = 13;
        }

        if (ctx.flags & GLP_CTX_EXTD_F_PRES)
        {
            ctx.ext_flags = 0;
        }
    }

    meas.base_time.year = 2012;
    meas.base_time.month = 12;
    meas.base_time.day = 22;
    meas.base_time.hour = 12;
    meas.base_time.min = 25;
    meas.base_time.sec = 54;

    if (ctx_exist == true)
        app_glps_meas_with_ctx_req_send(app_glps_env->conhdl, app_glps_env->records_idx++, &meas, &ctx);
    else
        app_glps_meas_without_ctx_req_send(app_glps_env->conhdl, app_glps_env->records_idx++, &meas);
}

/**
 ****************************************************************************************
 * @brief This function is used for demonstration.
 *
 * @param[in] racp_req The operator of Record Access Control Point(RACP)
 *
 * @return None.
 ****************************************************************************************
 */
static uint8_t app_glps_racp_procedure_handle(struct glp_racp_req *racp_req)
{
    uint8_t res = GLP_RSP_SUCCESS;

    switch (racp_req->filter.operator)
    {
        case GLP_OP_LT_OR_EQ:
        case GLP_OP_GT_OR_EQ:
        case GLP_OP_WITHIN_RANGE_OF:
            res = GLP_RSP_OPERATOR_NOT_SUP;
            break;
        case GLP_OP_ALL_RECS:
        case GLP_OP_FIRST_REC:
            app_glps_env->records_idx = 0;
            break;
        case GLP_OP_LAST_REC:
            app_glps_env->records_idx = APP_GLPS_STROED_RECORDS_NUM;
            break;
        default:
            res = GLP_RSP_INVALID_OPERATOR;
            break;
    }
    return res;
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
            }
            break;

        case GLPS_DISABLE_IND:
            ke_timer_clear(APP_GLPS_MEAS_SEND_TIMER, TASK_APP);
            break;

        case GLPS_CFG_INDNTF_IND:
            if (app_glps_env->evt_cfg & GLPS_MEAS_NTF_CFG && !(((struct glps_cfg_indntf_ind *)param)->evt_cfg & GLPS_MEAS_NTF_CFG))
            {
                ke_timer_clear(APP_GLPS_MEAS_SEND_TIMER, TASK_APP);
            }

            if (app_glps_env->evt_cfg & GLPS_MEAS_CTX_NTF_CFG && !(((struct glps_cfg_indntf_ind *)param)->evt_cfg & GLPS_MEAS_CTX_NTF_CFG))
            {
                ke_timer_clear(APP_GLPS_MEAS_SEND_TIMER, TASK_APP);
            }
            break;

        case GLPS_RACP_REQ_IND:
        {
            uint8_t res;
            switch (((struct glps_racp_req_ind *)param)->racp_req.op_code)
            {
                case GLP_REQ_REP_STRD_RECS:         // Report stored records (Operator: Value from Operator Table)
                    res = app_glps_racp_procedure_handle(&app_glps_env->racp_req);
                    if (res == GLP_RSP_SUCCESS)
                    {    
                        ke_timer_set(APP_GLPS_MEAS_SEND_TIMER, TASK_APP, APP_GLPS_MEAS_SEND_TO);
                    }
                    else
                    {    
                        app_glps_racp_rsp_req_send(app_glps_env->conhdl, 0, GLP_REQ_REP_STRD_RECS, res);
                    }
                    break;

                case GLP_REQ_DEL_STRD_RECS:         // Delete stored records (Operator: Value from Operator Table)
                    app_glps_racp_rsp_req_send(app_glps_env->conhdl, 0, GLP_REQ_DEL_STRD_RECS, GLP_RSP_SUCCESS);
                    break;

                case GLP_REQ_ABORT_OP:              // Abort operation (Operator: Null 'value of 0x00 from Operator Table')
                    ke_timer_clear(APP_GLPS_MEAS_SEND_TIMER, TASK_APP);
                    app_glps_racp_rsp_req_send(app_glps_env->conhdl, 0, GLP_REQ_ABORT_OP, GLP_RSP_SUCCESS);
                    break;

                case GLP_REQ_REP_NUM_OF_STRD_RECS:  // Report number of stored records (Operator: Value from Operator Table)
                    res = app_glps_racp_procedure_handle(&app_glps_env->racp_req);
                    app_glps_racp_rsp_req_send(app_glps_env->conhdl, APP_GLPS_STROED_RECORDS_NUM, GLP_REQ_REP_NUM_OF_STRD_RECS, res);
                    break;

                default:
                    app_glps_racp_rsp_req_send(app_glps_env->conhdl, 0, ((struct glps_racp_req_ind *)param)->racp_req.op_code, GLP_RSP_OP_CODE_NOT_SUP);
                    break;
            }
            break;
        }

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
 * @brief Handles the demo Glucose measurement timer.
 *
 * @param[in] msgid     Id of the message received
 * @param[in] param     None
 * @param[in] dest_id   TASK_APP
 * @param[in] src_id    TASK_APP
 *
 * @return If the message was consumed or not.
 * @description 
 *  Handles the demo Glucose measurement timer.
 ****************************************************************************************
 */
int app_glps_meas_send_timer_handler(ke_msg_id_t const msgid,
                                     void *param,
                                     ke_task_id_t const dest_id,
                                     ke_task_id_t const src_id)
{
    //DEVELOPER NOTE: SET REAL VALUE OF USER APPLICATION IN THIS FUNCTION
    if ((app_glps_env->evt_cfg & GLPS_MEAS_NTF_CFG) || (app_glps_env->evt_cfg & GLPS_MEAS_CTX_NTF_CFG))
    {
        app_glps_meas_send((app_glps_env->evt_cfg & GLPS_MEAS_NTF_CFG) ? false : true);

        if (app_glps_env->racp_req.filter.operator == GLP_OP_FIRST_REC)
        {
            app_glps_racp_rsp_req_send(app_glps_env->conhdl, 0, app_glps_env->racp_req.op_code, GLP_RSP_SUCCESS);
        }
        else if (app_glps_env->racp_req.filter.operator == GLP_OP_LAST_REC)
        {
            app_glps_racp_rsp_req_send(app_glps_env->conhdl, 0, app_glps_env->racp_req.op_code, GLP_RSP_SUCCESS);
        }
        else if (app_glps_env->racp_req.filter.operator == GLP_OP_ALL_RECS)
        {
            if (app_glps_env->records_idx >= APP_GLPS_STROED_RECORDS_NUM)
            {
                app_glps_racp_rsp_req_send(app_glps_env->conhdl, 0, app_glps_env->racp_req.op_code, GLP_RSP_SUCCESS);
                app_glps_env->records_idx = 0;
            }
            else
                ke_timer_set(APP_GLPS_MEAS_SEND_TIMER, TASK_APP, APP_GLPS_MEAS_SEND_TO);
        }
    }
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
                    struct app_glps_env_tag *app_glps_env = &app_env.glps_ev;
                    if(!app_glps_env->enabled)
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

