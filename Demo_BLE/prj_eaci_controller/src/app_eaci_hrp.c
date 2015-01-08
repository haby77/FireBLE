/**
 ****************************************************************************************
 *
 * @file app_eaci_hrp.c
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
 * @addtogroup APP_EACI_HRP_ACI
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
 
#include "app_eaci.h"
#include "app_eaci_hrp.h"
/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief HRPC data request
 *
 ****************************************************************************************
 */
#if BLE_HR_COLLECTOR
void app_eaci_data_hrpc_hdl(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
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
    uint8_t idx = app_get_client_idx_by_conhdl(conhdl, ATT_SVC_HEART_RATE);
    
    if ((app_hrpc_env[idx].enabled == false) && (msg_id != EACI_MSG_DATA_REQ_HRPC_ENABLE))
        return;

    switch (msg_id)
    {
        case EACI_MSG_DATA_REQ_HRPC_ENABLE:
        {
            if (param_len == BD_ADDR_LEN)
            {
                if (conhdl != 0xFFFF)
                    app_hrpc_enable_req(NULL, conhdl);
            }
        }
            break;

        case EACI_MSG_DATA_READ_HRPC_BODY_SENSOR:
        {
            if (param_len == BD_ADDR_LEN)
            {
                if (conhdl != 0xFFFF)
                    app_hrpc_rd_char_req(HRPC_RD_HRS_BODY_SENSOR_LOC, conhdl);
            }
        }
            break;

        case EACI_MSG_DATA_READ_HRPC_CFG_INDNTF:
        {
            if (param_len == (BD_ADDR_LEN + 2))
            {
                uint16_t cfg_val = param[1] << 8 | param[0];
                if (conhdl != 0xFFFF)
                    app_hrpc_cfg_indntf_req(cfg_val, conhdl);
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
 * @brief HRPS data request
 *
 ****************************************************************************************
 */
#if BLE_HR_SENSOR
void app_eaci_data_hrps_hdl(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    if (app_hrps_env->enabled == false)
        return;
    
    switch (msg_id)
    {
        case EACI_MSG_DATA_REQ_HRPS_MEAS_VALUE:
        {
            struct hrs_hr_meas meas_val;
            meas_val.flags = HRS_FLAG_HR_16BITS_VALUE | HRS_FLAG_SENSOR_CCT_FET_NOT_SUPPORTED;
            // the real measured hear rate value set here
            meas_val.heart_rate = param[1] << 8 | param[0];

            app_hrps_measurement_send(app_hrps_env->conhdl, &meas_val);
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
#if BLE_HR_COLLECTOR || BLE_HR_SENSOR
void hr_app_task_msg_hdl(ke_msg_id_t const msgid, void const *param)
{
    switch(msgid)
    {
#if BLE_HR_COLLECTOR
        case HRPC_ENABLE_CFM:
        {
            // Enable confirmation
            uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, EACI_MSG_DATA_IND_HRPC_ENABLE, 3, 0x0D, 0x18, 0};
            pdu[5] = ((struct hrpc_enable_cfm *)param)->status;
            eaci_pdu_send(sizeof(pdu), pdu);
        }
            break;

        case HRPC_RD_CHAR_RSP:
        {
            uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 4, 0, 0, 0, 0};
            pdu[1] =  EACI_MSG_DATA_IND_HRPC_READ_BSL_RSP;
            pdu[3] = (uint8_t)(ATT_SVC_HEART_RATE & 0x00FF);
            pdu[4] = (uint8_t)(ATT_SVC_HEART_RATE >> 8);
            pdu[5] = ((struct hrpc_rd_char_rsp *)param)->status;
            pdu[6] = ((struct hrpc_rd_char_rsp *)param)->data.data[0];
            eaci_pdu_send(sizeof(pdu), pdu);
        }
            break;

        case HRPC_WR_CHAR_RSP:
        {
            uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 3, 0, 0, 0};
            pdu[1] =  EACI_MSG_DATA_IND_HRPC_WR_RSP;
            pdu[3] = (uint8_t)(ATT_SVC_HEART_RATE & 0x00FF);
            pdu[4] = (uint8_t)(ATT_SVC_HEART_RATE >> 8);
            pdu[5] = ((struct hrpc_wr_char_rsp *)param)->status;
            eaci_pdu_send(sizeof(pdu), pdu);
        }
            break;

        case HRPC_HR_MEAS_IND:
        {
            uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 4, 0, 0, 0, 0};
            pdu[1] =  EACI_MSG_DATA_IND_HRPC_MEAS;
            pdu[3] = (uint8_t)(ATT_SVC_HEART_RATE & 0x00FF);
            pdu[4] = (uint8_t)(ATT_SVC_HEART_RATE >> 8);
            pdu[5] = ((((struct hrpc_meas_ind *)param)->meas_val.heart_rate) & 0x00FF);
            pdu[6] = ((((struct hrpc_meas_ind *)param)->meas_val.heart_rate) >> 8);
            eaci_pdu_send(sizeof(pdu), pdu);
        }
            break;
#endif

#if BLE_HR_SENSOR
        // No data indication necessary here
        case HRPS_CFG_INDNTF_IND:
        {
            uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 4, 0, 0, 0, 0};
            pdu[1] =  EACI_MSG_DATA_IND_HRPS_CFG;
            pdu[3] = (uint8_t)(ATT_SVC_HEART_RATE & 0x00FF);
            pdu[4] = (uint8_t)(ATT_SVC_HEART_RATE >> 8);
            pdu[5] = ((((struct hrps_cfg_indntf_ind *)param)->cfg_val) & 0x00FF);
            pdu[6] = ((((struct hrps_cfg_indntf_ind *)param)->cfg_val) >> 8);
            eaci_pdu_send(sizeof(pdu), pdu);
        }
            break;
#endif
        default:
            break;
    }
}
#endif

/// @} APP_EACI_HRP_ACI
