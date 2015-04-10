/**
 ****************************************************************************************
 *
 * @file app_eaci_pas.c
 *
 * @brief Application PAS ACI
 *
 * Copyright (C) Quintic 2012-2013
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup APP_EACI_PAS_ACI
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
 
#include "app_eaci.h"
#include "app_eaci_pas.h"
/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief PASPC data request
 *
 ****************************************************************************************
 */
#if BLE_PAS_CLIENT
void app_eaci_data_paspc_hdl(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
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
    uint8_t idx = app_get_client_idx_by_conhdl(conhdl, ATT_SVC_PHONE_ALERT_STATUS);
    
    if ((app_paspc_env[idx].enabled == false) && (msg_id != EACI_MSG_DATA_REQ_PASPC_ENABLE))
        return;

    switch (msg_id)
    {
        case EACI_MSG_DATA_REQ_PASPC_ENABLE:
        {
            if (param_len == BD_ADDR_LEN)
            {
                if (conhdl != 0xFFFF)
                    app_paspc_enable_req(NULL, conhdl);
            }
            break;
        }

        case EACI_MSG_DATA_REQ_PASPC_RD:
        {
            if (param_len == BD_ADDR_LEN + 1)
            {
                uint8_t read_code = param[0];
                
                if (conhdl != 0xFFFF)
                    app_paspc_read_req(read_code, conhdl);
            }
            break;
        }

        case EACI_MSG_DATA_REQ_PASPC_WR:
        {
            if (param_len == BD_ADDR_LEN + 3)
            {
                union write_value_tag value;
                uint8_t write_code = param[0];

                switch (write_code)
                {
                    case PASPC_RD_WR_ALERT_STATUS_CFG:
                    {
                        value.alert_status_ntf_cfg = param[2] << 8 | param[1];
                        break;
                    }
                    case PASPC_RD_WR_RINGER_SETTING_CFG:
                    {
                        value.ringer_setting_ntf_cfg = param[2] << 8 | param[1];
                        break;
                    }
                    case PASPC_WR_RINGER_CTNL_PT:
                    {
                        value.ringer_ctnl_pt = param[1];
                        break;
                    }
                    default:
                        break;
                }

                if (conhdl != 0xFFFF)
                    app_paspc_wr_char_req(write_code, &value, conhdl);
            }
            break;
        }

        default:
            break;
    }
}
#endif


/**
 ****************************************************************************************
 * @brief PASPS data request
 *
 ****************************************************************************************
 */

#if BLE_PAS_SERVER
void app_eaci_data_pasps_hdl(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    if (app_pasps_env->enabled == false)
        return;

    switch (msg_id)
    {
        case EACI_MSG_DATA_REQ_PASPS_UPDATE_VAL:
        {
            uint8_t operation = param[0];
            uint8_t value = param[1];
            app_pasps_update_char_val_req(app_pasps_env->conhdl, operation, value);
            break;
        }

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
#if BLE_PAS_CLIENT || BLE_PAS_SERVER
void pas_app_task_msg_hdl(ke_msg_id_t const msgid, void const *param)
{
    switch(msgid)
    {
#if BLE_PAS_CLIENT
        case PASPC_CMP_EVT:
        {
            uint8_t operation = ((struct paspc_cmp_evt *)param)->operation;
            switch (operation)
            {
                case PASPC_ENABLE_OP_CODE:
                {
                    uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 3, 0, 0, 0};
                    pdu[1] =  EACI_MSG_DATA_IND_PASPC_ENABLE;
                    pdu[3] = (uint8_t)(ATT_SVC_PHONE_ALERT_STATUS);
                    pdu[4] = (uint8_t)(ATT_SVC_PHONE_ALERT_STATUS >> 8);
                    pdu[5] = ((struct paspc_cmp_evt *)param)->status;
                    eaci_pdu_send(sizeof(pdu), pdu);
                    break;
                }

                case PASPC_READ_OP_CODE:
                {
                    uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 3, 0, 0, 0};
                    pdu[1] =  EACI_MSG_DATA_IND_PASPC_READ;
                    pdu[3] = (uint8_t)(ATT_SVC_PHONE_ALERT_STATUS);
                    pdu[4] = (uint8_t)(ATT_SVC_PHONE_ALERT_STATUS >> 8);
                    pdu[5] = ((struct paspc_cmp_evt *)param)->status;
                    eaci_pdu_send(sizeof(pdu), pdu);
                    break;
                }

                case PASPC_WRITE_OP_CODE:
                {
                    uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 3, 0, 0, 0};
                    pdu[1] =  EACI_MSG_DATA_IND_PASPC_WRITE;
                    pdu[3] = (uint8_t)(ATT_SVC_PHONE_ALERT_STATUS);
                    pdu[4] = (uint8_t)(ATT_SVC_PHONE_ALERT_STATUS >> 8);
                    pdu[5] = ((struct paspc_cmp_evt *)param)->status;
                    eaci_pdu_send(sizeof(pdu), pdu);
                    break;
                }

                default:
                    break;
            }
            break;
        }

        case PASPC_VALUE_IND:
        {
            uint8_t att_code = ((struct paspc_value_ind *)param)->att_code;
            switch (att_code)
            {
                case PASPC_RD_ALERT_STATUS:
                {
                    uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 3, 0, 0, 0};
                    pdu[1] =  EACI_MSG_DATA_IND_PASPC_ALERT_STATUS;
                    pdu[3] = (uint8_t)(ATT_SVC_PHONE_ALERT_STATUS);
                    pdu[4] = (uint8_t)(ATT_SVC_PHONE_ALERT_STATUS >> 8);
                    pdu[5] = ((struct paspc_value_ind *)param)->value.alert_status;
                    eaci_pdu_send(sizeof(pdu), pdu);
                    break;
                }

                case PASPC_RD_RINGER_SETTING:
                {
                    uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 4, 0, 0, 0, 0};
                    pdu[1] =  EACI_MSG_DATA_IND_PASPC_RINGER_SETTING;
                    pdu[3] = (uint8_t)(ATT_SVC_PHONE_ALERT_STATUS);
                    pdu[4] = (uint8_t)(ATT_SVC_PHONE_ALERT_STATUS >> 8);
                    pdu[5] = (uint8_t)(((struct paspc_value_ind *)param)->value.ringer_setting);
                    pdu[6] = (uint8_t)(((struct paspc_value_ind *)param)->value.ringer_setting >> 8);
                    eaci_pdu_send(sizeof(pdu), pdu);
                    break;
                }

                case PASPC_RD_WR_ALERT_STATUS_CFG:
                {
                    uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 4, 0, 0, 0, 0};
                    pdu[1] =  EACI_MSG_DATA_IND_PASPC_ALERT_STATUS_CFG;
                    pdu[3] = (uint8_t)(ATT_SVC_PHONE_ALERT_STATUS);
                    pdu[4] = (uint8_t)(ATT_SVC_PHONE_ALERT_STATUS >> 8);
                    pdu[5] = (uint8_t)(((struct paspc_value_ind *)param)->value.alert_status_ntf_cfg);
                    pdu[6] = (uint8_t)(((struct paspc_value_ind *)param)->value.alert_status_ntf_cfg >> 8);
                    eaci_pdu_send(sizeof(pdu), pdu);
                    break;
                }

                case PASPC_RD_WR_RINGER_SETTING_CFG:
                {
                    uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 4, 0, 0, 0, 0};
                    pdu[1] =  EACI_MSG_DATA_IND_PASPC_RINGER_SETTING_CFG;
                    pdu[3] = (uint8_t)(ATT_SVC_PHONE_ALERT_STATUS);
                    pdu[4] = (uint8_t)(ATT_SVC_PHONE_ALERT_STATUS >> 8);
                    pdu[5] = (uint8_t)(((struct paspc_value_ind *)param)->value.ringer_setting_ntf_cfg);
                    pdu[6] = (uint8_t)(((struct paspc_value_ind *)param)->value.ringer_setting_ntf_cfg >> 8);
                    eaci_pdu_send(sizeof(pdu), pdu);
                    break;
                }

                default:
                    break;
            }
            break;
        }
#endif
#if BLE_PAS_SERVER
        case PASPS_CMP_EVT:
        {
            uint8_t operation = ((struct pasps_cmp_evt *)param)->operation;
            
            switch (operation)
            {
                case PASPS_UPD_ALERT_STATUS_OP_CODE:
                {
                    uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 3, 0, 0, 0};
                    pdu[1] =  EACI_MSG_DATA_IND_PASPC_UPD_ALERT_STATUS;
                    pdu[3] = (uint8_t)(ATT_SVC_PHONE_ALERT_STATUS);
                    pdu[4] = (uint8_t)(ATT_SVC_PHONE_ALERT_STATUS >> 8);
                    pdu[5] = ((struct pasps_cmp_evt *)param)->status;
                    eaci_pdu_send(sizeof(pdu), pdu);
                    break;
                }

                case PASPS_UPD_RINGER_SETTING_OP_CODE:
                {
                    uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 3, 0, 0, 0};
                    pdu[1] =  EACI_MSG_DATA_IND_PASPC_UPD_RINGER_SETTING;
                    pdu[3] = (uint8_t)(ATT_SVC_PHONE_ALERT_STATUS);
                    pdu[4] = (uint8_t)(ATT_SVC_PHONE_ALERT_STATUS >> 8);
                    pdu[5] = ((struct pasps_cmp_evt *)param)->status;
                    eaci_pdu_send(sizeof(pdu), pdu);
                    break;
                }
            }
            break;
        }
#endif
        default:
            break;
    }
}
#endif

/// @} APP_EACI_PAS_ACI
