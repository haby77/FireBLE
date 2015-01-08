/**
 ****************************************************************************************
 *
 * @file app_eaci_ht.c
 *
 * @brief Application HT ACI
 *
 * Copyright (C) Quintic 2012-2013
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */


/**
 ****************************************************************************************
 * @addtogroup APP_EACI_HT_ACI
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
 
#include "app_eaci.h"
#include "app_eaci_ht.h"

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief HTPC data request
 *
 ****************************************************************************************
 */
#if BLE_HT_COLLECTOR
void app_eaci_data_htpc_hdl(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
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
    uint8_t idx = app_get_client_idx_by_conhdl(conhdl, ATT_SVC_HEALTH_THERMOM);
    
    if ((app_htpc_env[idx].enabled == false) && (msg_id != EACI_MSG_DATA_REQ_HT_ENABLE))
        return;

    switch(msg_id)
    {
        case EACI_MSG_DATA_REQ_HT_ENABLE:
            if (param_len == BD_ADDR_LEN)
            {
                if (conhdl != 0xFFFF)
                    app_htpc_enable_req(NULL, conhdl);
            }
            break;
            
        case EACI_MSG_DATA_REQ_HT_TEMP_MEAS:
            if (param_len == (BD_ADDR_LEN + 2))
            {
                uint16_t cfg_val = param[1] << 8 | param[0];
                if (conhdl != 0xFFFF)
                    app_htpc_cfg_indntf_req(HTPC_CHAR_HTS_TEMP_MEAS, cfg_val,conhdl);
            }
            break;
            
        case EACI_MSG_DATA_REQ_HT_INTM_TEMP:
            if (param_len == (BD_ADDR_LEN + 2))
            {
                uint16_t cfg_val = param[1] << 8 | param[0];
                if (conhdl != 0xFFFF)
                    app_htpc_cfg_indntf_req(HTPC_CHAR_HTS_INTM_TEMP, cfg_val,conhdl);
            }
            break;
        
        case EACI_MSG_DATA_REQ_HT_MEAS_INTV:
            if (param_len == (BD_ADDR_LEN + 2))
            {
                uint16_t cfg_val = param[1] << 8 | param[0];
                if (conhdl != 0xFFFF)
                    app_htpc_cfg_indntf_req(HTPC_CHAR_HTS_MEAS_INTV, cfg_val,conhdl);
            }
            break;

        case EACI_MSG_DATA_REQ_HT_RD_MEAS_INTV:
            if (param_len == BD_ADDR_LEN)
            {
                if (conhdl != 0xFFFF)
                    app_htpc_rd_char_req(HTPC_CHAR_HTS_MEAS_INTV, conhdl);
            }
            break;

        default:
            break;
    }
}
#endif

/**
 ****************************************************************************************
 * @brief HTPT data request
 *
 ****************************************************************************************
 */
#if BLE_HT_THERMOM
void app_eaci_data_htps_hdl(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    if (app_htpt_env->enabled == false)
        return;
    
    switch (msg_id)
    {
        case EACI_MSG_DATA_REQ_HT_SEND_INTER_VALUE:
        {
            uint16_t meas_intv = param[1] << 8 | param[0];
            app_htpt_measurement_intv_send(app_htpt_env->conhdl, meas_intv);
        }
            break;

        case EACI_MSG_DATA_REQ_HT_SEND_TEMP_VALUE:
        {
            struct htp_temp_meas temp;
            uint8_t flag_stable_meas = param[0];
            temp.flags = param[1];
            temp.type = param[2];
            temp.temp = (param[6] << 24) | (param[5] << 16) | (param[4] << 8) | param[3];
            temp.time_stamp.year = (param[8] << 8) |  param[7];
            temp.time_stamp.month = param[9];
            temp.time_stamp.day = param[10];
            temp.time_stamp.hour = param[11];
            temp.time_stamp.min = param[12];
            temp.time_stamp.sec = param[13];
            app_htpt_temp_send(app_htpt_env->conhdl, &temp, flag_stable_meas);
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
#if BLE_HT_COLLECTOR || BLE_HT_THERMOM
void ht_app_task_msg_hdl(ke_msg_id_t const msgid, void const *param)
{
    switch(msgid)
    {
#if BLE_HT_COLLECTOR
        case HTPC_ENABLE_CFM:
        {
            // Enable confirmation
            uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, EACI_MSG_DATA_IND_HT_ENABLE, 3, 0x09, 0x18, 0};
            pdu[5] = ((struct htpc_enable_cfm *)param)->status;
            eaci_pdu_send(sizeof(pdu), pdu);
        }
            break;

        case HTPC_RD_CHAR_RSP:
        {
            uint8_t pdu[8];
            pdu[0] = EACI_MSG_TYPE_DATA_IND;
            pdu[1] = EACI_MSG_DATA_IND_HT_RD_CHAR_RSP;
            pdu[2] = 5;
            pdu[3] = (uint8_t)(ATT_SVC_HEALTH_THERMOM);
            pdu[4] = (uint8_t)(ATT_SVC_HEALTH_THERMOM >> 8);
            pdu[5] = ((struct htpc_rd_char_rsp *)param)->status;
            pdu[6] = ((struct htpc_rd_char_rsp *)param)->data.data[0];
            pdu[7] = ((struct htpc_rd_char_rsp *)param)->data.data[1];
            eaci_pdu_send(sizeof(pdu), pdu);
        }
            break;

        case HTPC_WR_CHAR_RSP:
        {
            // Write Status
            uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, EACI_MSG_DATA_IND_HT_WR_RSP, 3, 0x09, 0x18, 0};
            pdu[5] = ((struct htpc_wr_char_rsp *)param)->status;
            eaci_pdu_send(sizeof(pdu), pdu);
        }
            break;

        case HTPC_TEMP_IND:
        {
            uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 0x08, 0x09, 0x18, 0, 0, 0, 0, 0, 0};
            pdu[1] =  EACI_MSG_DATA_IND_HT_TEM_MEA;
            pdu[5] = ((struct htpc_temp_ind *)param)->flag_stable_meas;
            pdu[6] = (((struct htpc_temp_ind *)param)->temp_meas.flags) & 0x01;
            pdu[7] = (uint8_t)((((struct htpc_temp_ind *)param)->temp_meas.temp));
            pdu[8] = (uint8_t)((((struct htpc_temp_ind *)param)->temp_meas.temp) >> 8);
            pdu[9] = (uint8_t)((((struct htpc_temp_ind *)param)->temp_meas.temp) >> 16);
            pdu[10] = (uint8_t)((((struct htpc_temp_ind *)param)->temp_meas.temp) >> 24);
            eaci_pdu_send(sizeof(pdu), pdu);
        }
            break;

        case HTPC_MEAS_INTV_IND:
        {
            uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 4, 0x09, 0x18, 0, 0};
            pdu[1] =  EACI_MSG_DATA_IND_HT_MEAS_INTE;
            pdu[5] = (uint8_t)(((struct htpc_meas_intv_ind *)param)->intv);
            pdu[6] = (uint8_t)((((struct htpc_meas_intv_ind *)param)->intv) >> 8);
            eaci_pdu_send(sizeof(pdu), pdu);
        }
            break;
#endif

#if BLE_HT_THERMOM
        case HTPT_MEAS_INTV_CHG_IND:
        {
            uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 4, 0x09, 0x18, 0, 0};
            pdu[1] =  EACI_MSG_DATA_IND_HT_TH_TEM_MEA;
            pdu[5] = (uint8_t)(((struct htpt_meas_intv_chg_ind *)param)->intv);
            pdu[6] = (uint8_t)((((struct htpt_meas_intv_chg_ind *)param)->intv) >> 8);
            eaci_pdu_send(sizeof(pdu), pdu);
        }
            break;

        case HTPT_CFG_INDNTF_IND:
        {
            uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 4, 0x09, 0x18, 0, 0};
            pdu[1] =  EACI_MSG_DATA_IND_HT_TH_CFG_IND;
            pdu[5] = (uint8_t)(((struct htpt_cfg_indntf_ind *)param)->cfg_val);
            pdu[6] = (uint8_t)((((struct htpt_cfg_indntf_ind *)param)->cfg_val) >> 8);
            eaci_pdu_send(sizeof(pdu), pdu);
        }
            break;
#endif
        default:
            break;
    }
}
#endif

/// @} APP_EACI_HT_ACI
