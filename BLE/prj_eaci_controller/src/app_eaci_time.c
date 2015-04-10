/**
 ****************************************************************************************
 *
 * @file app_eaci_time.c
 *
 * @brief Application TIME ACI
 *
 * Copyright (C) Quintic 2012-2013
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */


/**
 ****************************************************************************************
 * @addtogroup APP_EACI_TIME_ACI
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
 
#include "app_eaci.h"
#include "app_eaci_time.h"

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief TIPC data request
 *
 ****************************************************************************************
 */
#if BLE_TIP_CLIENT
void app_eaci_data_tipc_hdl(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    uint16_t conhdl;
    if (param_len == BD_ADDR_LEN)
    {
        app_eaci_get_conhdl_by_param(&conhdl, param);
    }
    else
    {
        app_eaci_get_conhdl_by_param(&conhdl, param + (param_len - BD_ADDR_LEN));
    }
    uint8_t idx = app_get_client_idx_by_conhdl(conhdl, ATT_SVC_CURRENT_TIME);
    
    if ((app_tipc_env[idx].enabled == false) && (msg_id != EACI_MSG_DATA_REQ_TIP_ENABLE))
        return;

    switch (msg_id)
    {
        case EACI_MSG_DATA_REQ_TIP_ENABLE:
            if (param_len == BD_ADDR_LEN)
            {
                if (conhdl != 0xFFFF)
                    app_tipc_enable_req(NULL, NULL, NULL, conhdl);
            }
            break;

        case EACI_MSG_DATA_REQ_TIP_CHAR_VAL_RD:
            if (param_len == (BD_ADDR_LEN + 1))
            {
                uint8_t char_code = param[0];
                if (conhdl != 0xFFFF)
                    app_tipc_rd_char_req(char_code, conhdl);
            }
            break;

        case EACI_MSG_DATA_REQ_TIP_CT_NTF_CFG:
            if (param_len == (BD_ADDR_LEN + 2))
            {
                uint16_t cfg_val = param[1] << 8 | param[0];
                if (conhdl != 0xFFFF)
                    app_tipc_ct_ntf_cfg_req(cfg_val, conhdl);
            }
            break;

        case EACI_MSG_DATA_REQ_TIP_WR_UDP_CTNL_PT:
            if (param_len == (BD_ADDR_LEN + 1))
            {
                uint8_t value = param[0];
                if (conhdl != 0xFFFF)
                    app_tipc_wr_time_udp_ctnl_pt_req(value, conhdl);
            }
            break;

            default:
            break;
    }
}
#endif

/**
 ****************************************************************************************
 * @brief TIPS data request
 *
 ****************************************************************************************
 */
#if BLE_TIP_SERVER
void app_eaci_data_tips_hdl(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    if (app_tips_env->enabled == false)
        return;

    switch (msg_id)
    {
        case EACI_MSG_DATA_TIPS_UPD_CURR_TIME:
        {
            struct tip_curr_time current_time;
            current_time.exact_time_256.day_date_time.date_time.year = param[1] << 8 | param[0];
            current_time.exact_time_256.day_date_time.date_time.month = param[2];
            current_time.exact_time_256.day_date_time.date_time.day = param[3];
            current_time.exact_time_256.day_date_time.date_time.hour = param[4];
            current_time.exact_time_256.day_date_time.date_time.min = param[5];
            current_time.exact_time_256.day_date_time.date_time.sec = param[6];
            current_time.exact_time_256.day_date_time.day_of_week = param[7];

            app_tips_upd_curr_time_req(app_tips_env->conhdl, &current_time, 1);
        }
            break;

        default:
            break;
    }
}
#endif

/**
 ****************************************************************************************
 * @brief EACI application task message handler
 *
 ****************************************************************************************
 */
#if BLE_TIP_CLIENT || BLE_TIP_SERVER
void tip_app_task_msg_hdl(ke_msg_id_t const msgid, void const *param)
{
    switch(msgid)
    {
#if BLE_TIP_CLIENT
        case TIPC_ENABLE_CFM:
        {
            uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 3, 0, 0, 0};
            pdu[1] =  EACI_MSG_DATA_IND_TIP_ENABLE;
            pdu[3] = (uint8_t)(ATT_SVC_CURRENT_TIME);
            pdu[4] = (uint8_t)(ATT_SVC_CURRENT_TIME >> 8);
            pdu[5] = ((struct tipc_enable_cfm *)param)->status;
            eaci_pdu_send(sizeof(pdu), pdu);
        }
            break;

        case TIPC_WR_CHAR_RSP:
        {
            uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 3, 0, 0, 0};
            pdu[1] =  EACI_MSG_DATA_IND_TIP_WR_RSP;
            pdu[3] = (uint8_t)(ATT_SVC_CURRENT_TIME);
            pdu[4] = (uint8_t)(ATT_SVC_CURRENT_TIME >> 8);
            pdu[5] = ((struct tipc_wr_char_rsp *)param)->status;
            eaci_pdu_send(sizeof(pdu), pdu);
        }
            break;

        case TIPC_CT_IND:
        {
            uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
            pdu[1] =  EACI_MSG_DATA_IND_TIP_CT;
            pdu[3] = (uint8_t)(ATT_SVC_CURRENT_TIME);
            pdu[4] = (uint8_t)(ATT_SVC_CURRENT_TIME >> 8);
            pdu[5] = (uint8_t)(((struct tipc_ct_ind *)param)->ct_val.exact_time_256.day_date_time.date_time.year);
            pdu[6] = (uint8_t)((((struct tipc_ct_ind *)param)->ct_val.exact_time_256.day_date_time.date_time.year) >> 8);
            pdu[7] = ((struct tipc_ct_ind *)param)->ct_val.exact_time_256.day_date_time.date_time.month;
            pdu[8] = ((struct tipc_ct_ind *)param)->ct_val.exact_time_256.day_date_time.date_time.day;
            pdu[9] = ((struct tipc_ct_ind *)param)->ct_val.exact_time_256.day_date_time.date_time.hour;
            pdu[10] = ((struct tipc_ct_ind *)param)->ct_val.exact_time_256.day_date_time.date_time.min;
            pdu[11] = ((struct tipc_ct_ind *)param)->ct_val.exact_time_256.day_date_time.date_time.sec;
            pdu[12] = ((struct tipc_ct_ind *)param)->ct_val.exact_time_256.day_date_time.day_of_week;
            eaci_pdu_send(sizeof(pdu), pdu);
        }
            break;

        case TIPC_CT_NTF_CFG_RD_RSP:
        {
            uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 4, 0, 0, 0, 0};
            pdu[1] =  EACI_MSG_DATA_IND_TIP_NTF_CFG_RD;
            pdu[3] = (uint8_t)(ATT_SVC_CURRENT_TIME);
            pdu[4] = (uint8_t)(ATT_SVC_CURRENT_TIME >> 8);
            pdu[5] = (uint8_t)(((struct tipc_ct_ntf_cfg_rd_rsp *)param)->ntf_cfg);
            pdu[6] = (uint8_t)((((struct tipc_ct_ntf_cfg_rd_rsp *)param)->ntf_cfg) >> 8);
            eaci_pdu_send(sizeof(pdu), pdu);
        }
            break;

        case TIPC_LTI_RD_RSP:
        {
            uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 4, 0, 0, 0, 0};
            pdu[1] =  EACI_MSG_DATA_IND_TIP_LTI_RD;
            pdu[3] = (uint8_t)(ATT_SVC_CURRENT_TIME);
            pdu[4] = (uint8_t)(ATT_SVC_CURRENT_TIME >> 8);
            pdu[5] = ((struct tipc_lti_rd_rsp *)param)->lti_val.time_zone;
            pdu[6] = ((struct tipc_lti_rd_rsp *)param)->lti_val.dst_offset;
            eaci_pdu_send(sizeof(pdu), pdu);
        }
            break;

        case TIPC_RTI_RD_RSP:
        {
            uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 6, 0, 0, 0, 0, 0, 0};
            pdu[1] =  EACI_MSG_DATA_IND_TIP_RTI_RD;
            pdu[3] = (uint8_t)(ATT_SVC_CURRENT_TIME);
            pdu[4] = (uint8_t)(ATT_SVC_CURRENT_TIME >> 8);
            pdu[5] = ((struct tipc_rti_rd_rsp *)param)->rti_val.time_source;
            pdu[6] = ((struct tipc_rti_rd_rsp *)param)->rti_val.time_accuracy;
            pdu[7] = ((struct tipc_rti_rd_rsp *)param)->rti_val.days_update;
            pdu[8] = ((struct tipc_rti_rd_rsp *)param)->rti_val.hours_update;
            eaci_pdu_send(sizeof(pdu), pdu);
        }
            break;

        case TIPC_TDST_RD_RSP:
        {
            uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 3, 0, 0, 0};
            pdu[1] =  EACI_MSG_DATA_IND_TIP_TDST_RD;
            pdu[3] = (uint8_t)(ATT_SVC_CURRENT_TIME);
            pdu[4] = (uint8_t)(ATT_SVC_CURRENT_TIME >> 8);
            pdu[5] = ((struct tipc_tdst_rd_rsp *)param)->tdst_val.dst_offset;
            eaci_pdu_send(sizeof(pdu), pdu);
        }
            break;

        case TIPC_TUS_RD_RSP:
        {
            uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 4, 0, 0, 0, 0};
            pdu[1] =  EACI_MSG_DATA_IND_TIP_TUS_RD;
            pdu[3] = (uint8_t)(ATT_SVC_CURRENT_TIME);
            pdu[4] = (uint8_t)(ATT_SVC_CURRENT_TIME >> 8);
            pdu[5] = ((struct tipc_tus_rd_rsp *)param)->tus_val.current_state;
            pdu[6] = ((struct tipc_tus_rd_rsp *)param)->tus_val.result;
            eaci_pdu_send(sizeof(pdu), pdu);
        }
            break;
#endif

#if BLE_TIP_SERVER
        case TIPS_CURRENT_TIME_CCC_IND:
        {
            uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 4, 0, 0, 0, 0};
            pdu[1] =  EACI_MSG_DATA_IND_TIP_CURRENT_CCC;
            pdu[3] = (uint8_t)(ATT_SVC_CURRENT_TIME);
            pdu[4] = (uint8_t)(ATT_SVC_CURRENT_TIME >> 8);
            pdu[5] = (uint8_t)(((struct tips_current_time_ccc_ind *)param)->cfg_val);
            pdu[6] = (uint8_t)((((struct tips_current_time_ccc_ind *)param)->cfg_val) >> 8);
            eaci_pdu_send(sizeof(pdu), pdu);
        }
            break;

        case TIPS_TIME_UPD_CTNL_PT_IND:
        {
            uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 3, 0, 0, 0};
            pdu[1] =  EACI_MSG_DATA_IND_TIP_UPD_CTNL_PT;
            pdu[3] = (uint8_t)(ATT_SVC_CURRENT_TIME);
            pdu[4] = (uint8_t)(ATT_SVC_CURRENT_TIME >> 8);
            pdu[5] = ((struct tips_time_upd_ctnl_pt_ind *)param)->value;
            eaci_pdu_send(sizeof(pdu), pdu);
        }
            break;
#endif
        default:
            break;
    }
}
#endif

/// @} APP_EACI_TIME_ACI
