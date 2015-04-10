/**
 ****************************************************************************************
 *
 * @file app_eaci_blp.c
 *
 * @brief Application BLP ACI
 *
 * Copyright (C) Quintic 2012-2013
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup APP_EACI_BLP_ACI
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
 
#include "app_eaci.h"
#include "app_eaci_blp.h"
/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief BLPC data request
 *
 ****************************************************************************************
 */
#if BLE_BP_COLLECTOR
void app_eaci_data_bpc_hdl(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
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
    uint8_t idx = app_get_client_idx_by_conhdl(conhdl, ATT_SVC_BLOOD_PRESSURE);
    
    if ((app_blpc_env[idx].enabled == false) && (msg_id != EACI_MSG_DATA_REQ_BP_ENABLE))
        return;

    switch (msg_id)
    {
        case EACI_MSG_DATA_REQ_BP_ENABLE:
            if (param_len == BD_ADDR_LEN)
            {
                if (conhdl != 0xFFFF)
                    app_blpc_enable_req(NULL, conhdl);
            }
            break;

        case EACI_MSG_DATA_REQ_BP_RD_CHAR_CODE:
            if (param_len == (BD_ADDR_LEN + 1))
            {
                if (conhdl != 0xFFFF)
                    app_blpc_rd_char_req(param[0], conhdl);
            }
            break;

        case EACI_MSG_DATA_REQ_BP_CFG_INDNTF:
            if (param_len == (BD_ADDR_LEN + 3))
            {
                uint8_t char_code = param[0];
                uint16_t cfg_val = param[2] << 8 | param[1];
                if (conhdl != 0xFFFF)
                    app_blpc_cfg_indntf_req(char_code, cfg_val, conhdl);
            }
            break;

        default:
            break;
    }
}
#endif


/**
 ****************************************************************************************
 * @brief BLPS data request
 *
 ****************************************************************************************
 */

#if BLE_BP_SENSOR
void app_eaci_data_bps_hdl(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    if (app_blps_env->enabled == false)
        return;

    switch (msg_id)
    {
        case EACI_MSG_DATA_REQ_BP_SEND_PRES:
        {
            
            struct bps_bp_meas meas_val;
            uint8_t flag = 0;
            uint8_t flag_interm = 0;
            uint8_t index = 2;
            
            flag_interm = param[0];
            meas_val.flags = param[1];
            flag = meas_val.flags;
            if (flag & BPS_FLAG_USER_ID_PRESENT)            // User ID present
            {
                meas_val.user_id = param[index];                    // 0xFF represents "unknown user"
                index += 1;
            }
            
            if (flag & BPS_FLAG_KPA)                        // Pressure in pascal
            {
                //        Exponent    Mantissa
                // Size    4 bits      12 bits
                meas_val.systolic = param[index + 1] << 8 | param[index];                 // 15.99 kpa equal to 120 mm Hg
                meas_val.diastolic = param[index + 3] << 8 | param[index + 2];                // 10.66 kpa
                meas_val.mean_arterial_pressure = param[index + 5] << 8 | param[index + 4];   // 9.99
                index += 6;
            }
            else                                            // pressure in millimetre of mercury
            {
                meas_val.systolic = param[index + 1] << 8 | param[index];
                meas_val.diastolic = param[index + 3] << 8 | param[index + 2];
                meas_val.mean_arterial_pressure = param[5] << 8 | param[index + 4];
                index += 6;
            }
            
            if (flag & BPS_FLAG_PULSE_RATE_PRESENT)         // Pulse Rate
            {
                meas_val.pulse_rate = param[index + 1] << 8 | param[index];
                index += 2;
            }
            
            if (flag & BPS_FLAG_MEAS_STATUS_PRESENT)        // Measurement Status
            {
                meas_val.meas_status = param[index + 1] << 8 | param[index];
                index += 2;
            }
            
            if (flag & BPS_FLAG_TIME_STAMP_PRESENT)         // Time Stamp
            {
                meas_val.time_stamp.year = param[index + 1] << 8 | param[index];
                meas_val.time_stamp.month = param[index + 2];
                meas_val.time_stamp.day = param[index + 3];
                meas_val.time_stamp.hour = param[index + 4];
                meas_val.time_stamp.min = param[index + 5];
                meas_val.time_stamp.sec = param[index + 6];
                index += 7;
            }
            app_blps_pressure_send_req(app_blps_env->conhdl, flag_interm, &meas_val);
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
#if BLE_BP_COLLECTOR || BLE_BP_SENSOR
void bp_app_task_msg_hdl(ke_msg_id_t const msgid, void const *param)
{
    switch(msgid)
    {
#if BLE_BP_COLLECTOR
        case BLPC_ENABLE_CFM:
        {
            uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 3, 0, 0, 0};
            pdu[1] = EACI_MSG_DATA_IND_BP_ENABLE;
            pdu[3] = (uint8_t)(ATT_SVC_BLOOD_PRESSURE);
            pdu[4] = (uint8_t)(ATT_SVC_BLOOD_PRESSURE >> 8);
            pdu[5] = ((struct blpc_enable_cfm *)param)->status;
            eaci_pdu_send(sizeof(pdu), pdu);
        }
            break;

        case BLPC_RD_CHAR_RSP:
        {
            uint16_t data_len = ((struct blpc_rd_char_rsp *)param)->data.len;
            uint8_t *pdu;
            pdu = (uint8_t *)malloc(6 + data_len);
            if (pdu != NULL)
            {
                memset(pdu,0,data_len+6);
                pdu[0] = EACI_MSG_TYPE_DATA_IND;
                pdu[1] = EACI_MSG_DATA_IND_BP_RD_RSP;
                pdu[2] = 3 + data_len;
                pdu[3] = (uint8_t)(ATT_SVC_BLOOD_PRESSURE);
                pdu[4] = (uint8_t)(ATT_SVC_BLOOD_PRESSURE >> 8);
                pdu[5] = ((struct blpc_rd_char_rsp *)param)->status;
                for (int i = 0;i < data_len;i++)
                {
                    pdu[6 + i] = (uint8_t)(((struct blpc_rd_char_rsp *)param)->data.data[i]);
                }
                eaci_pdu_send((6 + data_len), pdu);
                free(pdu);
                pdu = NULL;
            }
        }
            break;

        case BLPC_WR_CHAR_RSP:
        {
            uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 3, 0, 0, 0};
            pdu[1] = EACI_MSG_DATA_IND_BP_WR_RSP;
            pdu[3] = (uint8_t)(ATT_SVC_BLOOD_PRESSURE);
            pdu[4] = (uint8_t)(ATT_SVC_BLOOD_PRESSURE >> 8);
            pdu[5] = ((struct blpc_wr_char_rsp *)param)->status;
            eaci_pdu_send(sizeof(pdu), pdu);
        }
            break;

        case BLPC_BP_MEAS_IND:
        {
            uint8_t *pdu;
            uint8_t flag = 0;
            uint8_t index = 8;
            uint8_t data_len = 0;
            
            flag = ((struct blpc_meas_ind *)param)->meas_val.flags;
            
            if (flag & BPS_FLAG_USER_ID_PRESENT)            // User ID present
            {
                data_len += 1;
            }
            
            if (flag & BPS_FLAG_KPA)                        // Pressure in pascal
            {
                data_len += 4;
            }
            else                                            // pressure in millimetre of mercury
            {
                data_len += 4;
            }
            
            if (flag & BPS_FLAG_PULSE_RATE_PRESENT)         // Pulse Rate
            {
                data_len += 2;
            }
            
            if (flag & BPS_FLAG_MEAS_STATUS_PRESENT)        // Measurement Status
            {
                data_len += 2;
            }
            
            if (flag & BPS_FLAG_TIME_STAMP_PRESENT)         // Time Stamp
            {
                data_len += 7;
            }
            
            pdu = (uint8_t *)malloc(data_len + 7);
            if (pdu != NULL)
            {
                memset(pdu,0,data_len+7);
                pdu[0] = EACI_MSG_TYPE_DATA_IND;
                pdu[1] = EACI_MSG_DATA_IND_BP_MEAS;
                //pdu[2] = data_len + 7;
                //uuid 0x1810
                pdu[3] = (uint8_t)(ATT_SVC_BLOOD_PRESSURE);
                pdu[4] = (uint8_t)(ATT_SVC_BLOOD_PRESSURE >> 8);
                //flag = itermediate or stable
                pdu[5] = (uint8_t)(((struct blpc_meas_ind *)param)->flag_interm_cp);
                pdu[6] = (uint8_t)((((struct blpc_meas_ind *)param)->flag_interm_cp) >> 8);
                pdu[7] = ((struct blpc_meas_ind *)param)->meas_val.flags;
                
                if (flag & BPS_FLAG_USER_ID_PRESENT)            // User ID present
                {
                    pdu[index] = ((struct blpc_meas_ind *)param)->meas_val.user_id;
                    index += 1;
                }
                
                if (flag & BPS_FLAG_KPA)                        // Pressure in pascal
                {
                    //Systolic (mmHg/kPa)
                    pdu[index] = (uint8_t)(((struct blpc_meas_ind *)param)->meas_val.systolic);
                    pdu[index + 1] = (uint8_t)((((struct blpc_meas_ind *)param)->meas_val.systolic) >> 8);
                    //Diastolic (mmHg/kPa)
                    pdu[index + 2] = (uint8_t)(((struct blpc_meas_ind *)param)->meas_val.diastolic);
                    pdu[index + 3] = (uint8_t)((((struct blpc_meas_ind *)param)->meas_val.diastolic) >> 8);
                    //Mean Arterial Pressure (mmHg/kPa)
                    pdu[index + 4] = (uint8_t)(((struct blpc_meas_ind *)param)->meas_val.mean_arterial_pressure);
                    pdu[index + 5] = (uint8_t)((((struct blpc_meas_ind *)param)->meas_val.mean_arterial_pressure) >> 8);
    
                    index += 6;
                }
                else                                            // pressure in millimetre of mercury
                {
                    //Systolic (mmHg/kPa)
                    pdu[index] = (uint8_t)(((struct blpc_meas_ind *)param)->meas_val.systolic);
                    pdu[index + 1] = (uint8_t)((((struct blpc_meas_ind *)param)->meas_val.systolic) >> 8);
                    //Diastolic (mmHg/kPa)
                    pdu[index + 2] = (uint8_t)(((struct blpc_meas_ind *)param)->meas_val.diastolic);
                    pdu[index + 3] = (uint8_t)((((struct blpc_meas_ind *)param)->meas_val.diastolic) >> 8);
                    //Mean Arterial Pressure (mmHg/kPa)
                    pdu[index + 4] = (uint8_t)(((struct blpc_meas_ind *)param)->meas_val.mean_arterial_pressure);
                    pdu[index + 5] = (uint8_t)((((struct blpc_meas_ind *)param)->meas_val.mean_arterial_pressure) >> 8);
    
                    index += 6;
                }
                
                if (flag & BPS_FLAG_PULSE_RATE_PRESENT)         // Pulse Rate
                {
                    pdu[index] = (uint8_t)(((struct blpc_meas_ind *)param)->meas_val.pulse_rate);
                    pdu[index + 1] = (uint8_t)((((struct blpc_meas_ind *)param)->meas_val.pulse_rate) >> 8);
                    index += 2;
                }
                
                if (flag & BPS_FLAG_MEAS_STATUS_PRESENT)        // Measurement Status
                {
                    pdu[index] = (uint8_t)(((struct blpc_meas_ind *)param)->meas_val.meas_status);
                    pdu[index + 1] = (uint8_t)((((struct blpc_meas_ind *)param)->meas_val.meas_status) >> 8);
                    
                    index += 2;
                }
                
                if (flag & BPS_FLAG_TIME_STAMP_PRESENT)         // Time Stamp
                {
                    pdu[index] = (uint8_t)(((struct blpc_meas_ind *)param)->meas_val.time_stamp.year);
                    pdu[index + 1] = (uint8_t)((((struct blpc_meas_ind *)param)->meas_val.time_stamp.year) >> 8);
                    pdu[index + 2] = ((struct blpc_meas_ind *)param)->meas_val.time_stamp.month;
                    pdu[index + 3] = ((struct blpc_meas_ind *)param)->meas_val.time_stamp.day;
                    pdu[index + 4] = ((struct blpc_meas_ind *)param)->meas_val.time_stamp.hour;
                    pdu[index + 5] = ((struct blpc_meas_ind *)param)->meas_val.time_stamp.min;
                    pdu[index + 6] = ((struct blpc_meas_ind *)param)->meas_val.time_stamp.sec;
                    index += 7;
                }
                pdu[2] = index - 3;
                eaci_pdu_send(index , pdu);
                free(pdu);
            }
        }
            break;
#endif

#if BLE_BP_SENSOR
        case BLPS_CFG_INDNTF_IND:
        {
            uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 5, 0, 0, 0, 0, 0};
            pdu[1] = EACI_MSG_DATA_IND_BPS_CFG;
            pdu[3] = (uint8_t)(ATT_SVC_BLOOD_PRESSURE);
            pdu[4] = (uint8_t)(ATT_SVC_BLOOD_PRESSURE >> 8);
            pdu[5] = ((struct blps_cfg_indntf_ind *)param)->char_code;
            pdu[6] = (uint8_t)(((struct blps_cfg_indntf_ind *)param)->cfg_val);
            pdu[7] = (uint8_t)((((struct blps_cfg_indntf_ind *)param)->cfg_val) >> 8);
            eaci_pdu_send(sizeof(pdu), pdu);
        }
            break;

        case BLPS_MEAS_SEND_CFM:
        {
            uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 3, 0, 0, 0};
            pdu[1] = EACI_MSG_DATA_IND_BPS_MEAS_SEND_CFM;
            pdu[3] = (uint8_t)(ATT_SVC_BLOOD_PRESSURE);
            pdu[4] = (uint8_t)(ATT_SVC_BLOOD_PRESSURE >> 8);
            pdu[5] = ((struct blps_meas_send_cfm *)param)->status;
            eaci_pdu_send(sizeof(pdu), pdu);
        }
            break;
#endif
        default:
            break;
    }
}
#endif

/// @} APP_EACI_BLP_ACI
