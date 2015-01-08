/**
 ****************************************************************************************
 *
 * @file app_eaci_generic.c
 *
 * @brief Application GENERIC ACI
 *
 * Copyright (C) Quintic 2012-2013
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup APP_EACI_GENERIC_ACI
 * @{
 ****************************************************************************************
 */


/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "app_eaci.h"
#include "app_eaci_generic.h"
#include "sleep.h"
#include "usr_design.h"

#if (defined(CFG_EACI))
struct device_name_set device_name = {12, "QUINTIC_EACI"};
#endif

#define LED_ON_DUR_ADV_FAST        2
#define LED_OFF_DUR_ADV_FAST       (uint16_t)((GAP_ADV_FAST_INTV2*0.625)/10)
#define LED_ON_DUR_CON             0xffff
#define LED_OFF_DUR_CON            0
#define LED_ON_DUR_IDLE            0
#define LED_OFF_DUR_IDLE           0xffff

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief EACI command handler
 *
 ****************************************************************************************
 */
void app_eaci_cmd_hdl(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    switch (msg_id)
    {
        case EACI_MSG_CMD_ADV:
            app_eaci_cmd_adv_hdl(param_len, param);
            break;

        case EACI_MSG_CMD_SCAN:
            app_eaci_cmd_scan_hdl(param_len, param);
            break;

        case EACI_MSG_CMD_CONN:
            app_eaci_cmd_conn_hdl(param_len, param);
            break;

        case EACI_MSG_CMD_DISC:
            app_eaci_cmd_disc_hdl(param_len, param);
            break;

        case EACI_MSG_CMD_SET_DEVNAME:
            app_eaci_cmd_per_set_devname_hdl(param_len, param);
            break;

        case EACI_MSG_CMD_BOND:
            app_eaci_cmd_bond_hdl(param_len, param);
            break;

        case EACI_MSG_CMD_CEN_UPDATE_PARAM:
            app_eaci_cmd_cen_update_param_hdl(param_len, param);
            break;

        case EACI_MSG_CMD_PER_UPDATE_PARAM:
            app_eaci_cmd_per_update_param_hdl(param_len, param);
            break;

        default:
            break;
    }
}

/**
 ****************************************************************************************
 * @brief EACI Advertising Command handler
 *
 ****************************************************************************************
 */
void app_eaci_cmd_adv_hdl(uint8_t param_len, uint8_t const *param)
{
    if (param[0] == 0x0)
    {
        ///stop Advertising
        app_gap_adv_stop_req();
#if (QN_DEEP_SLEEP_EN)
        // allow entering into deep sleep mode
        sleep_set_pm(PM_DEEP_SLEEP);
#endif
    }
    else
    {
        ///start Advertising
        app_gap_adv_start_req(GAP_GEN_DISCOVERABLE|GAP_UND_CONNECTABLE, 
            app_env.adv_data, app_set_adv_data(GAP_GEN_DISCOVERABLE), 
            app_env.scanrsp_data, app_set_scan_rsp_data(app_get_local_service_flag()),
            (param[2] << 8 | param[1]), (param[4] << 8 | param[3]));
        
#if (QN_DEEP_SLEEP_EN)
        // prevent entering into deep sleep mode
        sleep_set_pm(PM_SLEEP);
#endif
    }
}

/**
 ****************************************************************************************
 * @brief EACI Scan Command handler
 *
 ****************************************************************************************
 */
void app_eaci_cmd_scan_hdl(uint8_t param_len, uint8_t const *param)
{
    if (param[0] == 0x0)
    {
        ///stop Scan
        app_gap_dev_inq_cancel_req();
    }
    else
    {
        ///start Scan
        app_gap_dev_inq_req(GAP_GEN_INQ_TYPE, QN_ADDR_TYPE);
    }
}

/**
 ****************************************************************************************
 * @brief EACI Connect Command handler
 *
 ****************************************************************************************
 */
void app_eaci_cmd_conn_hdl(uint8_t param_len, uint8_t const *param)
{
    if (param_len == BD_ADDR_LEN + 7)
    {
        struct bd_addr addr;
        app_eaci_get_bd_addr_by_param(&addr, param+7);
        if (app_get_link_nb() != BLE_CONNECTION_MAX)
        {
            app_gap_le_create_conn_req(&addr, param[0], QN_ADDR_TYPE,
                                    param[2] << 8 | param[1],
                                    param[4] << 8 | param[3],
                                    param[6] << 8 | param[5]);
        }
#if (QN_DEEP_SLEEP_EN)
        // prevent entering into deep sleep mode
        sleep_set_pm(PM_SLEEP);
#endif
    }
}

/**
 ****************************************************************************************
 * @brief EACI Disconnect Command handler
 *
 ****************************************************************************************
 */
void app_eaci_cmd_disc_hdl(uint8_t param_len, uint8_t const *param)
{
    if (param_len == BD_ADDR_LEN)
    {
        uint8_t idx;
        struct bd_addr addr;
        app_eaci_get_bd_addr_by_param(&addr, param);
        uint16_t conhdl = app_get_conhdl_by_bdaddr(&addr, &idx);
        if (conhdl != 0xFFFF)
            app_gap_discon_req(conhdl);
    }
}

/**
 ****************************************************************************************
 * @brief EACI Peripheral Set Device Name
 *
 ****************************************************************************************
 */
void app_eaci_cmd_per_set_devname_hdl(uint8_t param_len, uint8_t const *param)
{
    app_gap_set_devname_req(param, param_len);
    device_name.namelen = param_len;
    memcpy(device_name.name, param, param_len);
}

/**
 ****************************************************************************************
 * @brief EACI Bond Command handler
 *
 ****************************************************************************************
 */
void app_eaci_cmd_bond_hdl(uint8_t param_len, uint8_t const *param)
{
    if (param_len == BD_ADDR_LEN)
    {
        struct bd_addr addr;
        app_eaci_get_bd_addr_by_param(&addr, param);
        app_gap_security_req(&addr);
    }
}

/**
 ****************************************************************************************
 * @brief EACI Master initiates a change in connection parameter.
 *
 ****************************************************************************************
 */
#if (BLE_CENTRAL)
void app_eaci_cmd_cen_update_param_hdl(uint8_t param_len, uint8_t const *param)
{
    uint16_t conhdl;
    app_eaci_get_conhdl_by_param(&conhdl, param + 10);
    if (conhdl != 0xFFFF)
    {
        uint16_t result = param[1] << 8 | param[0];
        struct gap_conn_param_update conn_par;
        conn_par.intv_min = param[3] << 8 | param[2];
        conn_par.intv_max = param[5] << 8 | param[4];
        conn_par.latency  = param[7] << 8 | param[6];
        conn_par.time_out = param[9] << 8 | param[8];
        app_gap_change_param_req(conhdl, result, &conn_par);
    }
}
#endif

/**
 ****************************************************************************************
 * @brief EACI Slave request for connection parameter change
 *
 ****************************************************************************************
 */
#if (BLE_PERIPHERAL)
void app_eaci_cmd_per_update_param_hdl(uint8_t param_len, uint8_t const *param)
{
    struct gap_conn_param_update conn_par;
    conn_par.intv_min = param[1] << 8 | param[0];
    conn_par.intv_max = param[3] << 8 | param[2];
    conn_par.latency  = param[5] << 8 | param[4];
    conn_par.time_out = param[7] << 8 | param[6];
    app_gap_param_update_req(app_env.dev_rec[0].conhdl, &conn_par);
}
#endif

void gap_app_task_msg_hdl(ke_msg_id_t const msgid, void const *param)
{
    switch(msgid)
    {
        case GAP_SET_DEVNAME_REQ_CMP_EVT: 
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_EVT, 0, 1, 0};
                pdu[1] = EACI_MSG_EVT_SET_DEVNAME;
                pdu[3] = ((struct gap_event_common_cmd_complete *)param)->status;
                eaci_pdu_send(sizeof(pdu), pdu);
            }
            break;

        case GAP_SET_MODE_REQ_CMP_EVT:
            {
                if (ke_state_get(TASK_APP) == APP_IDLE)
                {
                    usr_led1_set(LED_ON_DUR_ADV_FAST, LED_OFF_DUR_ADV_FAST);
                    // Adverstising started
                    uint8_t pdu[] = {EACI_MSG_TYPE_EVT, EACI_MSG_EVT_ADV, 1, 1};
                    eaci_pdu_send(sizeof(pdu), pdu);
                }
            }
            break;

        case GAP_ADV_REQ_CMP_EVT: 
            {
                usr_led1_set(LED_ON_DUR_IDLE, LED_OFF_DUR_IDLE);
                // Adverstising stopped
                uint8_t pdu[] = {EACI_MSG_TYPE_EVT, EACI_MSG_EVT_ADV, 1, 0};
                eaci_pdu_send(sizeof(pdu), pdu);
            }
            break;

        case GAP_DEV_INQ_RESULT_EVT:
            {
                // Scan result
                uint8_t pdu[40] = {EACI_MSG_TYPE_EVT, EACI_MSG_EVT_INQ_RESULT, 7, 0, 0, 0, 0, 0, 0, 0};
                // BD address type
                pdu[3] = ((struct gap_dev_inq_result_evt *)param)->adv_rep.adv_addr_type;
                // BD address
                for (uint8_t i = 0; i < BD_ADDR_LEN; i++)
                    pdu[4+i] = ((struct gap_dev_inq_result_evt *)param)->adv_rep.adv_addr.addr[i];

                // Device name
                struct app_adv_data adv_data;
                if (app_parser_adv_data(((struct gap_dev_inq_result_evt *)param)->adv_rep.data,
                                        ((struct gap_dev_inq_result_evt *)param)->adv_rep.data_len,
                                        &adv_data))
                {
                    // Device name 
                    if (adv_data.flag & AD_TYPE_NAME_BIT)
                    {
                        pdu[2] += strlen((const char *)adv_data.name);
                        memcpy(&pdu[10], adv_data.name, strlen((const char *)adv_data.name));
                    }
                }
                eaci_pdu_send(pdu[2]+3, pdu);
            }
            break;

        case GAP_DEV_INQ_REQ_CMP_EVT:
            {
                // Scan completed
                uint8_t pdu[] = {EACI_MSG_TYPE_EVT, EACI_MSG_EVT_INQ_CMP, 1, 0};
                // Total found devices number
                pdu[3] = app_env.inq_idx;
                eaci_pdu_send(sizeof(pdu), pdu);
                #if (QN_DEEP_SLEEP_EN)
                // allow entering into deep sleep mode
                sleep_set_pm(PM_DEEP_SLEEP);
                #endif
            }
            break;

        case GAP_LE_CREATE_CONN_REQ_CMP_EVT:
            {
                usr_led1_set(LED_ON_DUR_CON, LED_OFF_DUR_CON);
                // Connection completed
                uint8_t pdu[] = {EACI_MSG_TYPE_EVT, EACI_MSG_EVT_CONN, 7, 0, 0, 0, 0, 0, 0, 0};
                // Confirmation status
                pdu[3] = ((struct gap_le_create_conn_req_cmp_evt *)param)->conn_info.status;
                // BD address
                app_eaci_get_bd_addr_by_conhdl(((struct gap_le_create_conn_req_cmp_evt *)param)->conn_info.conhdl, pdu+4);
                eaci_pdu_send(sizeof(pdu), pdu);
            }
            break;

        case GAP_DISCON_CMP_EVT:
            {
                usr_led1_set(LED_ON_DUR_IDLE, LED_OFF_DUR_IDLE);
                // Connection disconnected
                uint8_t pdu[] = {EACI_MSG_TYPE_EVT, EACI_MSG_EVT_DISC, 7, 0, 0, 0, 0, 0, 0, 0};
                // Reason
                pdu[3] = ((struct gap_discon_cmp_evt *)param)->reason;
                // BD address
                app_eaci_get_bd_addr_by_conhdl(((struct gap_discon_cmp_evt *)param)->conhdl, pdu+4);
                eaci_pdu_send(sizeof(pdu), pdu);

                #if (QN_DEEP_SLEEP_EN)
                // allow entering into deep sleep mode
                sleep_set_pm(PM_DEEP_SLEEP);
                #endif
            }
            break;

        case GAP_BOND_REQ_CMP_EVT:
            {
                // Bond completed
                uint8_t pdu[] = {EACI_MSG_TYPE_EVT, EACI_MSG_EVT_BOND, 8, 0, 0, 0, 0, 0, 0, 0, 0};
                // Status
                pdu[3] = ((struct gap_bond_req_cmp_evt *)param)->status;
                // Bonded
                pdu[4] = ((struct gap_bond_req_cmp_evt *)param)->bonded;
                // BD address
                app_eaci_get_bd_addr_by_conhdl(((struct gap_bond_req_cmp_evt *)param)->conhdl, pdu+5);
                eaci_pdu_send(sizeof(pdu), pdu);
            }
            break;

#if (BLE_PERIPHERAL)
        case GAP_PARAM_UPDATE_RESP:
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_EVT, 0, 1, 0};
                pdu[1] = EACI_MSG_EVT_UPDATE_PARAM;
                pdu[3] = ((struct gap_param_update_resp *)param)->status;
                eaci_pdu_send(sizeof(pdu), pdu);
            }
            break;
#endif
#if (BLE_CENTRAL)
        case GAP_CHANGE_PARAM_REQ_CMP_EVT:
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_EVT, 0, 1, 0};
                pdu[1] = EACI_MSG_EVT_CEN_UPDATE_PARAM;
                pdu[3] = ((struct gap_change_param_req_cmp_evt *)param)->status;
                eaci_pdu_send(sizeof(pdu), pdu);
            }
            break;
#endif
#if (QN_SECURITY_ON)
        case SMPC_SEC_STARTED_IND:
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_EVT, 0, 1, 0};
                pdu[1] = EACI_MSG_EVT_SMP_SEC;
                pdu[3] = ((struct smpc_sec_started_ind *)param)->status;
                eaci_pdu_send(sizeof(pdu), pdu);
            }
            break;
#endif
        default:
            break;
    }
}

/// @} APP_EACI_GENERIC_ACI
