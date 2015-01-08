/**
 ****************************************************************************************
 *
 * @file app_eaci_sp.c
 *
 * @brief Application SP ACI
 *
 * Copyright (C) Quintic 2012-2013
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */


/**
 ****************************************************************************************
 * @addtogroup APP_EACI_SP_ACI
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
 
#include "app_eaci.h"
#include "app_eaci_sp.h"

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief SPC data request
 *
 ****************************************************************************************
 */
#if BLE_SP_CLIENT
void app_eaci_data_spc_hdl(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
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
    uint8_t idx = app_get_client_idx_by_conhdl(conhdl, ATT_SVC_SCAN_PARAMETERS);
    
    if ((app_scppc_env[idx].enabled == false) && (msg_id != EACI_MSG_DATA_REQ_SP_ENABLE))
        return;

    switch (msg_id)
    {
        case EACI_MSG_DATA_REQ_SP_ENABLE:
            if (param_len == BD_ADDR_LEN)
            {
                if (conhdl != 0xFFFF)
                    app_scppc_enable_req(NULL, NULL, NULL, conhdl);
            }
            break;
        
        case EACI_MSG_DATA_REQ_SP_RD_CFG:
            if (param_len == BD_ADDR_LEN)
            {
                if (conhdl != 0xFFFF)
                    app_scppc_scan_refresh_ntf_cfg_rd_req(conhdl);
            }
            break;

        case EACI_MSG_DATA_REQ_SP_WR_WD:
            if (param_len == (BD_ADDR_LEN + 4))
            {
                uint16_t scan_intv = param[1] << 8 | param[0];
                uint16_t scan_window = param[3] << 8 | param[2];
                if (conhdl != 0xFFFF)
                    app_scppc_scan_intv_wd_wr_req(scan_intv, scan_window, conhdl);
            }
            break;
            
        case EACI_MSG_DATA_REQ_SP_WR_MEAS:
            if (param_len == (BD_ADDR_LEN + 2))
            {
                uint16_t ntf_cfg = param[1] << 8 | param[0];
                if (conhdl != 0xFFFF)
                    app_scppc_wr_meas_intv_req(ntf_cfg, conhdl);
            }
            break;

        default:
            break;
    }
}
#endif

/**
 ****************************************************************************************
 * @brief SPS data request
 *
 ****************************************************************************************
 */
#if BLE_SP_SERVER
void app_eaci_data_sps_hdl(uint8_t msg_id)
{
    if (app_scpps_env->enabled == false)
        return;

    switch (msg_id)
    {
        case EACI_MSG_DATA_REQ_SP_SCAN_REFRESH_REQ:
            {
                app_scpps_scan_refresh_req(0, 0);
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
#if BLE_SP_CLIENT || BLE_SP_SERVER
void sp_app_task_msg_hdl(ke_msg_id_t const msgid, void const *param)
{
    switch(msgid)
    {
#if BLE_SP_CLIENT
        case SCPPC_ENABLE_CFM:
        {
            uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 3, 0, 0, 0};
            pdu[1] =  EACI_MSG_DATA_IND_SP_ENABLE;
            pdu[3] = (uint8_t)(ATT_SVC_SCAN_PARAMETERS);
            pdu[4] = (uint8_t)(ATT_SVC_SCAN_PARAMETERS >> 8);
            pdu[5] = ((struct scppc_enable_cfm *)param)->status;
            eaci_pdu_send(sizeof(pdu), pdu);
        }
            break;

        case SCPPC_WR_CHAR_RSP:
        {
            uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 3, 0, 0, 0};
            pdu[1] =  EACI_MSG_DATA_IND_SP_WR_RSP;
            pdu[3] = (uint8_t)(ATT_SVC_SCAN_PARAMETERS);
            pdu[4] = (uint8_t)(ATT_SVC_SCAN_PARAMETERS >> 8);
            pdu[5] = ((struct scppc_wr_char_rsp *)param)->status;
            eaci_pdu_send(sizeof(pdu), pdu);
        }
            break;

        case SCPPC_SCAN_REFRESH_NTF_CFG_RD_RSP:
        {
            uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 4, 0, 0, 0, 0};
            pdu[1] =  EACI_MSG_DATA_IND_SP_RD_CFG;
            pdu[3] = (uint8_t)(ATT_SVC_SCAN_PARAMETERS);
            pdu[4] = (uint8_t)(ATT_SVC_SCAN_PARAMETERS >> 8);
            pdu[5] = (uint8_t)(((struct scppc_scan_refresh_ntf_cfg_rd_rsp *)param)->ntf_cfg);
            pdu[6] = (uint8_t)((((struct scppc_scan_refresh_ntf_cfg_rd_rsp *)param)->ntf_cfg) >> 8);
            eaci_pdu_send(sizeof(pdu), pdu);
        }
            break;
#endif

#if BLE_SP_SERVER
        case SCPPS_SCAN_INTV_WD_IND:
        {
            uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 6, 0, 0, 0, 0, 0, 0};
            pdu[1] =  EACI_MSG_DATA_IND_SP_INTV_WD;
            pdu[3] = (uint8_t)(ATT_SVC_SCAN_PARAMETERS);
            pdu[4] = (uint8_t)(ATT_SVC_SCAN_PARAMETERS >> 8);
            pdu[5] = (uint8_t)(((struct scpps_scan_intv_wd_ind *)param)->scan_intv_wd.le_scan_intv);
            pdu[6] = (uint8_t)((((struct scpps_scan_intv_wd_ind *)param)->scan_intv_wd.le_scan_intv) >> 8);
            pdu[7] = (uint8_t)(((struct scpps_scan_intv_wd_ind *)param)->scan_intv_wd.le_scan_window);
            pdu[8] = (uint8_t)((((struct scpps_scan_intv_wd_ind *)param)->scan_intv_wd.le_scan_window) >> 8);
            eaci_pdu_send(sizeof(pdu), pdu);
        }
            break;

        case SCPPS_SCAN_REFRESH_NTF_CFG_IND:
        {
            uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 4, 0, 0, 0, 0};
            pdu[1] =  EACI_MSG_DATA_IND_SP_REFRESH_NTF_CFG;
            pdu[3] = (uint8_t)(ATT_SVC_SCAN_PARAMETERS);
            pdu[4] = (uint8_t)(ATT_SVC_SCAN_PARAMETERS >> 8);
            pdu[5] = (uint8_t)(((struct scpps_scan_refresh_ntf_cfg_ind *)param)->scan_refresh_ntf_en);
            pdu[6] = (uint8_t)((((struct scpps_scan_refresh_ntf_cfg_ind *)param)->scan_refresh_ntf_en) >> 8);
            eaci_pdu_send(sizeof(pdu), pdu);
        }
            break;
#endif
        default:
            break;
    }
}
#endif

/// @} APP_EACI_SP_ACI
