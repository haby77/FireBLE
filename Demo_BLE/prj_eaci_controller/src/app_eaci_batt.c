/**
 ****************************************************************************************
 *
 * @file app_eaci_batt.c
 *
 * @brief Application BATT ACI
 *
 * Copyright (C) Quintic 2012-2013
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup APP_EACI_BATT_ACI
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
 
#include "app_eaci.h"
#include "app_eaci_batt.h"

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief BASC data request
 *
 ****************************************************************************************
 */
#if BLE_BATT_CLIENT
void app_eaci_data_basc_hdl(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
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
    uint8_t idx = app_get_client_idx_by_conhdl(conhdl, ATT_SVC_BATTERY_SERVICE);
    
    if ((app_basc_env[idx].enabled == false) && (msg_id != EACI_MSG_DATA_REQ_BATT_ENABLE))
        return;

    switch (msg_id)
    {
        case EACI_MSG_DATA_REQ_BATT_ENABLE:
            if (param_len == BD_ADDR_LEN)
            {
                if (conhdl != 0xFFFF)
                    app_basc_enable_req(NULL, NULL, conhdl);
            }
            break;

        case EACI_MSG_DATA_REQ_BATT_RD_CHAR_VAL:
            if (param_len == (BD_ADDR_LEN + 1))
            {
                if (conhdl != 0xFFFF)
                    app_basc_rd_char_req(param[0], 0, conhdl);
            }
            break;

        case EACI_MSG_DATA_REQ_BATT_CFG_NOTIFY:
            if (param_len == (BD_ADDR_LEN + 2))
            {
                uint16_t ntf_cfg = param[1] << 8 | param[0];
                app_basc_cfg_indntf_req(ntf_cfg, 0, conhdl);
            }
            break;

        default:
            break;
    }
}
#endif

/**
 ****************************************************************************************
 * @brief BASS data request
 *
 ****************************************************************************************
 */
#if BLE_BATT_SERVER
void app_eaci_data_bass_hdl(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    if (app_bass_env->enabled == false)
        return;

    switch (msg_id)
    {
        case EACI_MSG_DATA_REQ_BASS_BATT_LEVEL_UPD:
            if (param_len == BD_ADDR_LEN + 2)
            {
                uint8_t bas_instance = 0;
                uint8_t batt_level = 0;

                bas_instance = param[0];
                batt_level = param[1];
                app_bass_batt_level_upd_req(app_bass_env->conhdl, bas_instance, batt_level);
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
#if BLE_BATT_CLIENT || BLE_BATT_SERVER
void batt_app_task_msg_hdl(ke_msg_id_t const msgid, void const *param)
{
    switch(msgid)
    {
#if BLE_BATT_CLIENT
        case BASC_ENABLE_CFM:
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 3, 0, 0, 0};
                pdu[1] =  EACI_MSG_DATA_IND_BATT_ENABLE;
                pdu[3] = (uint8_t)(ATT_SVC_BATTERY_SERVICE);
                pdu[4] = (uint8_t)(ATT_SVC_BATTERY_SERVICE >> 8);
                pdu[5] = ((struct basc_enable_cfm *)param)->status;
                eaci_pdu_send(sizeof(pdu), pdu);
            }
            break;

        case BASC_WR_CHAR_RSP:
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 3, 0, 0, 0};
                pdu[1] =  EACI_MSG_DATA_IND_BATT_WR_RSP;
                pdu[3] = (uint8_t)(ATT_SVC_BATTERY_SERVICE);
                pdu[4] = (uint8_t)(ATT_SVC_BATTERY_SERVICE >> 8);
                pdu[5] = (uint8_t)(((struct basc_wr_char_rsp *)param)->status);
                eaci_pdu_send(sizeof(pdu), pdu);
            }
            break;

        case BASC_BATT_LEVEL_IND:
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 3, 0, 0, 0};
                pdu[1] =  EACI_MSG_DATA_IND_BATT_LEVEL;
                pdu[3] = (uint8_t)(ATT_SVC_BATTERY_SERVICE);
                pdu[4] = (uint8_t)(ATT_SVC_BATTERY_SERVICE >> 8);
                pdu[5] = (uint8_t)(((struct basc_batt_level_ind *)param)->batt_level);
                eaci_pdu_send(sizeof(pdu), pdu);
            }
            break;

        case BASC_BATT_LEVEL_NTF_CFG_RD_RSP:
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 4, 0, 0, 0, 0};
                pdu[1] =  EACI_MSG_DATA_IND_BATT_RD_CFG;
                pdu[3] = (uint8_t)(ATT_SVC_BATTERY_SERVICE);
                pdu[4] = (uint8_t)(ATT_SVC_BATTERY_SERVICE >> 8);
                pdu[5] = (uint8_t)(((struct basc_batt_level_ntf_cfg_rd_rsp *)param)->ntf_cfg);
                pdu[6] = (uint8_t)((((struct basc_batt_level_ntf_cfg_rd_rsp *)param)->ntf_cfg) >> 8);
                eaci_pdu_send(sizeof(pdu), pdu);
            }
            break;
#endif

#if BLE_BATT_SERVER
        case BASS_BATT_LEVEL_UPD_CFM:
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 3, 0, 0, 0};
                pdu[1] =  EACI_MSG_DATA_IND_BASS_BATT_UPD_CFM;
                pdu[3] = (uint8_t)(ATT_SVC_BATTERY_SERVICE);
                pdu[4] = (uint8_t)(ATT_SVC_BATTERY_SERVICE >> 8);
                pdu[5] = ((struct bass_batt_level_upd_cfm *)param)->status;
                eaci_pdu_send(sizeof(pdu), pdu);
            }
            break;
#endif
        default:
            break;
    }
}
#endif

/// @} APP_EACI_BATT_ACI
