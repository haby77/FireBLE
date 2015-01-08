/**
 ****************************************************************************************
 *
 * @file app_eaci_fm.c
 *
 * @brief Application FM ACI
 *
 * Copyright (C) Quintic 2012-2013
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup APP_EACI_FM_ACI
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
 
#include "app_eaci.h"
#include "app_eaci_fm.h"
/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief FM data request
 *
 ****************************************************************************************
 */
#if BLE_FINDME_LOCATOR
void app_eaci_data_fm_hdl(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
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
    uint8_t idx = app_get_client_idx_by_conhdl(conhdl, ATT_SVC_IMMEDIATE_ALERT);
    
    if ((app_findl_env[idx].enabled == false) && (msg_id != EACI_MSG_DATA_REQ_FM_ENABLE))
        return;

    switch (msg_id)
    {
        case EACI_MSG_DATA_REQ_FM_ENABLE:
            if (param_len == BD_ADDR_LEN)
            {
                if (conhdl != 0xFFFF)
                    app_findl_enable_req(NULL, conhdl);
            }
            break;

        case EACI_MSG_DATA_REQ_FM_WR_ALERT:
            if (param_len == (BD_ADDR_LEN + 1))
            {
                uint8_t alert_lv = param[0];
                if (conhdl != 0xFFFF)
                    app_findl_set_alert_req(alert_lv, conhdl);
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
#if BLE_FINDME_LOCATOR || BLE_FINDME_TARGET
void findme_app_task_msg_hdl(ke_msg_id_t const msgid, void const *param)
{
    switch(msgid)
    {
#if BLE_FINDME_LOCATOR
        case FINDL_ENABLE_CFM:
        {
            uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 3, 0, 0, 0};
            pdu[1] =  EACI_MSG_DATA_IND_FM_ENABLE;
            pdu[3] = (uint8_t)(ATT_SVC_IMMEDIATE_ALERT);
            pdu[4] = (uint8_t)(ATT_SVC_IMMEDIATE_ALERT >> 8);
            pdu[5] = ((struct findl_enable_cfm *)param)->status;
            eaci_pdu_send(sizeof(pdu), pdu);
        }
            break;
#endif

#if BLE_FINDME_TARGET
        case FINDT_ALERT_IND:
        {
            uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 3, 0, 0, 0};
            pdu[1] =  EACI_MSG_DATA_IND_ALERT_LEV;
            pdu[3] = (uint8_t)(ATT_SVC_IMMEDIATE_ALERT);
            pdu[4] = (uint8_t)(ATT_SVC_IMMEDIATE_ALERT >> 8);
            pdu[5] = ((struct findt_alert_ind *)param)->alert_lvl;
            eaci_pdu_send(sizeof(pdu), pdu);
        }
            break;
#endif
        default:
            break;
    }
}
#endif

/// @} APP_EACI_FM_ACI
