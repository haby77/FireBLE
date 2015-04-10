/**
 ****************************************************************************************
 *
 * @file app_eaci_an.c
 *
 * @brief Application AN ACI
 *
 * Copyright (C) Quintic 2012-2013
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup APP_EACI_AN_ACI
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
 
#include "app_eaci.h"
#include "app_eaci_an.h"
/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief ANPC data request
 *
 ****************************************************************************************
 */
#if BLE_AN_CLIENT
void app_eaci_data_anpc_hdl(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
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
    uint8_t idx = app_get_client_idx_by_conhdl(conhdl, ATT_SVC_ALERT_NTF);
    
    if ((app_anpc_env[idx].enabled == false) && (msg_id != EACI_MSG_DATA_REQ_ANPC_ENABLE))
        return;

    switch (msg_id)
    {
        case EACI_MSG_DATA_REQ_ANPC_ENABLE:
            if (param_len == BD_ADDR_LEN)
            {
                if (conhdl != 0xFFFF)
                    app_anpc_enable_req(NULL, NULL, NULL, conhdl);
            }
            break;

        case EACI_MSG_DATA_REQ_ANPC_RD_CFG:
            if (param_len == BD_ADDR_LEN + 1)
            {
                uint8_t read_code = param[0];
                
                if (conhdl != 0xFFFF)
                    app_anpc_rd_char_req(read_code, conhdl);
            }
            break;

        case EACI_MSG_DATA_REQ_ANPC_WR_VAL:
            if (param_len == BD_ADDR_LEN + 3)
            {
                union anpc_write_value_tag value;
                uint8_t write_code = param[0];

                switch (write_code)
                {
                    case ANPC_WR_ALERT_NTF_CTNL_PT:
                    {
                        value.ctnl_pt.cmd_id = param[1];
                        value.ctnl_pt.cat_id = param[2];
                    }
                        break;
                    case ANPC_RD_WR_NEW_ALERT_CFG:
                        value.new_alert_ntf_cfg = param[2] << 8 | param[1];
                        break;
                    case ANPC_RD_WR_UNREAD_ALERT_STATUS_CFG:
                        value.unread_alert_status_ntf_cfg = param[2] << 8 | param[1];
                        break;
                    default:
                        break;
                }

                if (conhdl != 0xFFFF)
                    app_anpc_wr_char_req(write_code, &value, conhdl);
            }
            break;

        default:
            break;
    }
}
#endif


/**
 ****************************************************************************************
 * @brief ANPS data request
 *
 ****************************************************************************************
 */

#if BLE_AN_SERVER
void app_eaci_data_anps_hdl(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    if (app_anps_env->enabled == false)
        return;

    switch (msg_id)
    {
        case EACI_MSG_DATA_REQ_ANPS_NTF_NEW_ALERT:
        {
            union anps_value_tag value;
            uint8_t operation = param[0];

            if (operation == ANPS_UPD_NEW_ALERT_OP_CODE)
            {
                value.new_alert.info_str_len = param[1];
                value.new_alert.cat_id = param[2];
                value.new_alert.nb_new_alert = param[3];
                memcpy(value.new_alert.str_info, param + 4, param[1]);
                app_anps_ntf_alert_cmd(app_anps_env->conhdl, operation, &value);
            }
            else if (operation == ANPS_UPD_UNREAD_ALERT_STATUS_OP_CODE)
            {
                value.unread_alert_status.cat_id = param[1];
                value.unread_alert_status.nb_unread_alert = param[2];
                app_anps_ntf_alert_cmd(app_anps_env->conhdl, operation, &value);
            }
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
#if BLE_AN_CLIENT || BLE_AN_SERVER
void an_app_task_msg_hdl(ke_msg_id_t const msgid, void const *param)
{
    switch(msgid)
    {
#if BLE_AN_CLIENT
        case ANPC_CMP_EVT:
        {
            uint8_t operation = ((struct anpc_cmp_evt *)param)->operation;
            
            switch (operation)
            {
                case ANPC_ENABLE_OP_CODE:
                {
                    uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 3, 0, 0, 0};
                    pdu[1] =  EACI_MSG_DATA_IND_ANPC_ENABLE;
                    pdu[3] = (uint8_t)(ATT_SVC_ALERT_NTF);
                    pdu[4] = (uint8_t)(ATT_SVC_ALERT_NTF >> 8);
                    pdu[5] = ((struct anpc_cmp_evt *)param)->status;
                    eaci_pdu_send(sizeof(pdu), pdu);
                }
                    break;
                case ANPC_WRITE_OP_CODE:
                {
                    uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 3, 0, 0, 0};
                    pdu[1] =  EACI_MSG_DATA_IND_ANPC_WR_STATUS;
                    pdu[3] = (uint8_t)(ATT_SVC_ALERT_NTF);
                    pdu[4] = (uint8_t)(ATT_SVC_ALERT_NTF >> 8);
                    pdu[5] = ((struct anpc_cmp_evt *)param)->status;
                    eaci_pdu_send(sizeof(pdu), pdu);
                }
                    break;
                default:
                    break;
            }
        }
            break;

        case ANPC_VALUE_IND:
        {
            uint8_t att_code = ((struct anpc_value_ind *)param)->att_code;
            switch (att_code)
            {
                case ANPC_CHAR_NEW_ALERT:
                {
                    uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 4, 0, 0, 0, 0};
                    pdu[1] =  EACI_MSG_DATA_IND_ANPC_NEW_ALERT;
                    pdu[3] = (uint8_t)(ATT_SVC_ALERT_NTF);
                    pdu[4] = (uint8_t)(ATT_SVC_ALERT_NTF >> 8);
                    pdu[5] = ((struct anpc_value_ind *)param)->value.new_alert.cat_id;
                    pdu[6] = ((struct anpc_value_ind *)param)->value.new_alert.nb_new_alert;
                    eaci_pdu_send(sizeof(pdu), pdu);
                }
                    break;
                case ANPC_CHAR_UNREAD_ALERT_STATUS:
                {
                    uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 4, 0, 0, 0, 0};
                    pdu[1] =  EACI_MSG_DATA_IND_ANPC_UNREAD_ALERT_STATUS;
                    pdu[3] = (uint8_t)(ATT_SVC_ALERT_NTF);
                    pdu[4] = (uint8_t)(ATT_SVC_ALERT_NTF >> 8);
                    pdu[5] = ((struct anpc_value_ind *)param)->value.unread_alert.cat_id;
                    pdu[6] = ((struct anpc_value_ind *)param)->value.unread_alert.nb_unread_alert;
                    eaci_pdu_send(sizeof(pdu), pdu);
                }
                    break;
                case ANPC_RD_WR_NEW_ALERT_CFG:
                {
                    uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 4, 0, 0, 0, 0};
                    pdu[1] =  EACI_MSG_DATA_IND_ANPC_NEW_ALERT_CFG;
                    pdu[3] = (uint8_t)(ATT_SVC_ALERT_NTF);
                    pdu[4] = (uint8_t)(ATT_SVC_ALERT_NTF >> 8);
                    pdu[5] = (uint8_t)(((struct anpc_value_ind *)param)->value.ntf_cfg);
                    pdu[6] = (uint8_t)(((struct anpc_value_ind *)param)->value.ntf_cfg >> 8);
                    eaci_pdu_send(sizeof(pdu), pdu);
                }
                    break;
                case ANPC_RD_WR_UNREAD_ALERT_STATUS_CFG:
                {
                    uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 4, 0, 0, 0, 0};
                    pdu[1] =  EACI_MSG_DATA_IND_ANPC_UNREAD_ALERT_CFG;
                    pdu[3] = (uint8_t)(ATT_SVC_ALERT_NTF);
                    pdu[4] = (uint8_t)(ATT_SVC_ALERT_NTF >> 8);
                    pdu[5] = (uint8_t)(((struct anpc_value_ind *)param)->value.ntf_cfg);
                    pdu[6] = (uint8_t)(((struct anpc_value_ind *)param)->value.ntf_cfg >> 8);
                    eaci_pdu_send(sizeof(pdu), pdu);
                }
                    break;

                default:
                    break;
            }
        }
            break;
#endif
#if BLE_AN_SERVER
        case ANPS_CMP_EVT:
        {
            uint8_t operation = ((struct anps_cmp_evt *)param)->operation;
            switch(operation)
            {
                case ANPS_UPD_NEW_ALERT_OP_CODE:
                {
                    uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 3, 0, 0, 0};
                    pdu[1] =  EACI_MSG_DATA_IND_ANPS_UPD_NEW_ALERT;
                    pdu[3] = (uint8_t)(ATT_SVC_ALERT_NTF);
                    pdu[4] = (uint8_t)(ATT_SVC_ALERT_NTF >> 8);
                    pdu[5] = ((struct anps_cmp_evt *)param)->status;
                    eaci_pdu_send(sizeof(pdu), pdu);
                    break;
                }

                case ANPS_UPD_UNREAD_ALERT_STATUS_OP_CODE:
                {
                    uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 3, 0, 0, 0};
                    pdu[1] =  EACI_MSG_DATA_IND_ANPS_UPD_UNREAD_ALERT;
                    pdu[3] = (uint8_t)(ATT_SVC_ALERT_NTF);
                    pdu[4] = (uint8_t)(ATT_SVC_ALERT_NTF >> 8);
                    pdu[5] = ((struct anps_cmp_evt *)param)->status;
                    eaci_pdu_send(sizeof(pdu), pdu);
                    break;
                }

                default:
                    break;
            }
            break;
        }
#endif

        default:
            break;
    }
}
#endif

/// @} APP_EACI_AN_ACI
