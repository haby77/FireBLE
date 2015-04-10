/**
 ****************************************************************************************
 *
 * @file app_eaci_prx.c
 *
 * @brief Application HRP ACI
 *
 * Copyright (C) Quintic 2012-2013
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup APP_EACI_PRX_ACI
 * @{
 ****************************************************************************************
 */


/*
 * INCLUDE FILES
 ****************************************************************************************
 */
 
#include "app_eaci.h"
#include "app_eaci_prx.h"
/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief PROX data request
 *
 ****************************************************************************************
 */

#if BLE_PROX_MONITOR
void app_eaci_data_proxm_hdl(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
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
    uint8_t idx = app_get_client_idx_by_conhdl(conhdl, ATT_SVC_LINK_LOSS);
    
    if ((app_proxm_env[idx].enabled == false) && (msg_id != EACI_MSG_DATA_REQ_PROXM_ENABLE))
        return;

    switch (msg_id)
    {
        case EACI_MSG_DATA_REQ_PROXM_ENABLE:
            if (param_len == BD_ADDR_LEN)
            {
                if (conhdl != 0xFFFF)
                    app_proxm_enable_req(NULL, NULL, NULL, conhdl);
            }
            break;

        case EACI_MSG_DATA_READ_PROXM_TX_POWER:
            if (param_len == BD_ADDR_LEN)
            {
                if (conhdl != 0xFFFF)
                    app_proxm_rd_txpw_lvl_req(conhdl);
            }

        case EACI_MSG_DATA_WRITE_PROXM_IAS:
            if (param_len == (BD_ADDR_LEN + 1))
            {
                uint8_t alert_lvl = param[0];
                if (conhdl != 0xFFFF)
                    app_proxm_wr_alert_lvl_req(1, alert_lvl, conhdl);
            }
            break;

        case EACI_MSG_DATA_WRITE_PROXM_LLS:
            if (param_len == (BD_ADDR_LEN + 1))
            {
                uint8_t alert_lvl = param[0];
                if (conhdl != 0xFFFF)
                    app_proxm_wr_alert_lvl_req(0, alert_lvl, conhdl);
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
#if BLE_PROX_MONITOR || BLE_PROX_REPORTER
void prox_app_task_msg_hdl(ke_msg_id_t const msgid, void const *param)
{
    switch(msgid)
    {
#if BLE_PROX_MONITOR
        case PROXM_ENABLE_CFM:
        {
            // Enable confirmation
            uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, EACI_MSG_DATA_IND_PROXM_ENABLE, 3, 0x03, 0x18, 0};
            pdu[5] = ((struct proxm_enable_cfm *)param)->status;
            eaci_pdu_send(sizeof(pdu), pdu);
        }
            break;

        case PROXM_RD_CHAR_RSP:
        {
            /// read data rsp
            uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 3, 0x04, 0x18, 0};
            pdu[1] =  EACI_MSG_DATA_IND_PROXM_TX_POWER;
            pdu[5] = ((struct proxm_rd_char_rsp *)param)->val;
            eaci_pdu_send(sizeof(pdu), pdu);
        }
            break;
#endif

#if BLE_PROX_REPORTER
        case PROXR_ALERT_IND:
        {
            // Alert Indication
            uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 3, 0, 0, 0};
            if (((struct proxr_alert_ind *)param)->char_code == PROXR_IAS_CHAR)
            {
                pdu[1] =  EACI_MSG_DATA_IND_PROXR_ALERT;
                pdu[3] = (uint8_t)(ATT_SVC_TX_POWER & 0x00FF);
                pdu[4] = (uint8_t)(ATT_SVC_TX_POWER >> 8);
            }
            else
            {
                pdu[1] =  EACI_MSG_DATA_IND_PROXR_LINK_LOST;
                pdu[3] = (uint8_t)(ATT_SVC_LINK_LOSS & 0x00FF);
                pdu[4] = (uint8_t)(ATT_SVC_LINK_LOSS >> 8);
            }
            // Alter Level
            pdu[5] = ((struct proxr_alert_ind *)param)->alert_lvl;
            eaci_pdu_send(sizeof(pdu), pdu);
        }
            break;
#endif
        default:
            break;
    }
}
#endif

/// @} APP_EACI_PRX_ACI
