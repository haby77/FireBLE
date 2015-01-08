/**
 ****************************************************************************************
 *
 * @file app_eaci_csc.c
 *
 * @brief Application CSC ACI
 *
 * Copyright (C) Quintic 2012-2013
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup APP_EACI_CSC_ACI
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
 
#include "app_eaci.h"
#include "app_eaci_csc.h"
/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief CSCPC data request
 *
 ****************************************************************************************
 */
#if BLE_CSC_COLLECTOR
void app_eaci_data_cscpc_hdl(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
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
    uint8_t idx = app_get_client_idx_by_conhdl(conhdl, ATT_SVC_CYCLING_SPEED_CADENCE);
    
    if ((app_cscpc_env[idx].enabled == false) && (msg_id != EACI_MSG_DATA_REQ_CSCPC_ENABLE))
        return;

    switch (msg_id)
    {
         case EACI_MSG_DATA_REQ_CSCPC_ENABLE:
            if (param_len == BD_ADDR_LEN)
            {
                if (conhdl != 0xFFFF)
                    app_cscpc_enable_req(NULL, conhdl);
            }
            break;

        case EACI_MSG_DATA_REQ_CSCPC_RD_CODE:
            if (param_len == BD_ADDR_LEN + 1)
            {
                uint8_t read_code = param[0];
                
                if (conhdl != 0xFFFF)
                    app_cscpc_read_req(read_code, conhdl);
            }
            break;

        case EACI_MSG_DATA_REQ_CSCPC_WR_CFG:
            if (param_len == BD_ADDR_LEN + 3)
            {
                uint8_t desc_code = param[0];
                uint16_t ntfind_cfg = co_read16p(param + 1);

                if (conhdl != 0xFFFF)
                    app_cscpc_cfg_ntfind_req(desc_code, ntfind_cfg, conhdl);
            }
            break;

        case EACI_MSG_DATA_REQ_CSCPC_SEND_CTNL_PT:
            if (param_len == BD_ADDR_LEN + 5)
            {
                struct cscp_sc_ctnl_pt_req sc_ctnl_pt;
                sc_ctnl_pt.op_code = param[0];

                switch (sc_ctnl_pt.op_code)
                {
                    case CSCP_CTNL_PT_OP_UPD_LOC:
                        sc_ctnl_pt.value.sensor_loc = param[1];
                        break;
                    case CSCP_CTNL_PT_OP_SET_CUMUL_VAL:
                        sc_ctnl_pt.value.cumul_val = co_read32p(param + 1);
                        break;
                    case CSCP_CTNL_PT_OP_REQ_SUPP_LOC:
                        break;
                    case CSCP_CTNL_PT_OP_START_CALIB:
                        break;
                    default:
                        break;
                }

                if (conhdl != 0xFFFF)
                    app_cscpc_ctnl_pt_cfg_req(&sc_ctnl_pt, conhdl);
            }
            break;

        default:
            break;
    }
}
#endif


/**
 ****************************************************************************************
 * @brief CSCPS data request
 *
 ****************************************************************************************
 */

#if BLE_CSC_SENSOR
void app_eaci_data_cscps_hdl(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    if (app_cscps_env->enabled == false)
        return;

    switch (msg_id)
    {
        case EACI_MSG_DATA_REQ_CSCPS_NTF_MEAS:
        {
            uint8_t flags = param[0];
            uint16_t cumul_crank_rev = param[2] << 8 | param[1];
            uint16_t last_crank_evt_time = param[4] << 8 | param[3];
            uint16_t last_wheel_evt_time = param[6] << 8 | param[5];
            int16_t wheel_rev = param[8] << 8 | param[7];
            
            app_cscps_ntf_csc_meas_req(app_cscps_env->conhdl, flags, cumul_crank_rev, 
                                        last_crank_evt_time, last_wheel_evt_time, wheel_rev);
            break;
        }

        case EACI_MSG_DATA_REQ_CSCPS_CTNL_PT:
        {
            union cscps_sc_ctnl_pt_cfm_value value;
            uint8_t op_code = param[0];
            
            switch (op_code)
            {
                case CSCP_CTNL_PT_OP_SET_CUMUL_VAL:
                {
                    value.cumul_wheel_rev = param[4] << 24 | param[3] << 16
                                        | param[2] << 8 | param[1];
                    app_cscps_sc_ctnl_pt_cfm(app_cscps_env->conhdl, PRF_ERR_OK, &value);
                }
                    break;
                default:
                    break;
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
 * @brief EACI application task message handler
 *
 ****************************************************************************************
 */
#if BLE_CSC_COLLECTOR || BLE_CSC_SENSOR
void csc_app_task_msg_hdl(ke_msg_id_t const msgid, void const *param)
{
    switch(msgid)
    {
#if BLE_CSC_COLLECTOR
        case CSCPC_CMP_EVT:
        {
            uint8_t operation = ((struct cscpc_cmp_evt *)param)->operation;

            switch (operation)
            {
                case CSCPC_ENABLE_OP_CODE:
                {
                    uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 3, 0, 0, 0};
                    pdu[1] =  EACI_MSG_DATA_IND_CSCPC_ENABLE;
                    pdu[3] = (uint8_t)(ATT_SVC_CYCLING_SPEED_CADENCE);
                    pdu[4] = (uint8_t)(ATT_SVC_CYCLING_SPEED_CADENCE >> 8);
                    pdu[5] = ((struct cscpc_cmp_evt *)param)->status;
                    eaci_pdu_send(sizeof(pdu), pdu);
                    break;
                }

                case CSCPC_READ_OP_CODE:
                {
                    uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 3, 0, 0, 0};
                    pdu[1] =  EACI_MSG_DATA_IND_CSCPC_READ;
                    pdu[3] = (uint8_t)(ATT_SVC_CYCLING_SPEED_CADENCE);
                    pdu[4] = (uint8_t)(ATT_SVC_CYCLING_SPEED_CADENCE >> 8);
                    pdu[5] = ((struct cscpc_cmp_evt *)param)->status;
                    eaci_pdu_send(sizeof(pdu), pdu);
                    break;
                }

                case CSCPC_CFG_NTF_IND_OP_CODE:
                {
                    uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 3, 0, 0, 0};
                    pdu[1] =  EACI_MSG_DATA_IND_CSCPC_WR_CFG_STATUS;
                    pdu[3] = (uint8_t)(ATT_SVC_CYCLING_SPEED_CADENCE);
                    pdu[4] = (uint8_t)(ATT_SVC_CYCLING_SPEED_CADENCE >> 8);
                    pdu[5] = ((struct cscpc_cmp_evt *)param)->status;
                    eaci_pdu_send(sizeof(pdu), pdu);
                    break;
                }

                case CSCPC_CTNL_PT_CFG_WR_OP_CODE:
                {
                    uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 3, 0, 0, 0};
                    pdu[1] =  EACI_MSG_DATA_IND_CSCPC_CFG_WR;
                    pdu[3] = (uint8_t)(ATT_SVC_CYCLING_SPEED_CADENCE);
                    pdu[4] = (uint8_t)(ATT_SVC_CYCLING_SPEED_CADENCE >> 8);
                    pdu[5] = ((struct cscpc_cmp_evt *)param)->status;
                    eaci_pdu_send(sizeof(pdu), pdu);
                    break;
                }

                default:
                    break;
            }
            break;
        }
        
        case CSCPC_VALUE_IND:
        {
            uint8_t att_code = ((struct cscpc_value_ind *)param)->att_code;
            
            switch (att_code)
            {
                case CSCPC_RD_CSC_FEAT:
                {
                    uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 4, 0, 0, 0, 0};
                    pdu[1] =  EACI_MSG_DATA_IND_CSCPC_RD_CSC_FEAT;
                    pdu[3] = (uint8_t)(ATT_SVC_CYCLING_SPEED_CADENCE);
                    pdu[4] = (uint8_t)(ATT_SVC_CYCLING_SPEED_CADENCE >> 8);
                    pdu[5] = (uint8_t)(((struct cscpc_value_ind *)param)->value.sensor_feat);
                    pdu[6] = (uint8_t)(((struct cscpc_value_ind *)param)->value.sensor_feat >> 8);
                    eaci_pdu_send(sizeof(pdu), pdu);
                    break;
                }

                case CSCPC_RD_SENSOR_LOC:
                {
                    uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 3, 0, 0, 0};
                    pdu[1] =  EACI_MSG_DATA_IND_CSCPC_RD_SENSOR_LOC;
                    pdu[3] = (uint8_t)(ATT_SVC_CYCLING_SPEED_CADENCE);
                    pdu[4] = (uint8_t)(ATT_SVC_CYCLING_SPEED_CADENCE >> 8);
                    pdu[5] = (uint8_t)(((struct cscpc_value_ind *)param)->value.sensor_loc);
                    eaci_pdu_send(sizeof(pdu), pdu);
                    break;
                }

                case CSCPC_RD_WR_CSC_MEAS_CFG:
                {
                    uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 4, 0, 0, 0, 0};
                    pdu[1] =  EACI_MSG_DATA_IND_CSCPC_READ_MEAS_CFG;
                    pdu[3] = (uint8_t)(ATT_SVC_CYCLING_SPEED_CADENCE);
                    pdu[4] = (uint8_t)(ATT_SVC_CYCLING_SPEED_CADENCE >> 8);
                    pdu[5] = (uint8_t)(((struct cscpc_value_ind *)param)->value.ntf_cfg);
                    pdu[6] = (uint8_t)(((struct cscpc_value_ind *)param)->value.ntf_cfg >> 8);
                    eaci_pdu_send(sizeof(pdu), pdu);
                    break;
                }

                case CSCPC_RD_WR_SC_CTNL_PT_CFG:
                {
                    uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 4, 0, 0, 0, 0};
                    pdu[1] =  EACI_MSG_DATA_IND_CSCPC_READ_CTNL_PT_CFG;
                    pdu[3] = (uint8_t)(ATT_SVC_CYCLING_SPEED_CADENCE);
                    pdu[4] = (uint8_t)(ATT_SVC_CYCLING_SPEED_CADENCE >> 8);
                    pdu[5] = (uint8_t)(((struct cscpc_value_ind *)param)->value.ntf_cfg);
                    pdu[6] = (uint8_t)(((struct cscpc_value_ind *)param)->value.ntf_cfg >> 8);
                    eaci_pdu_send(sizeof(pdu), pdu);
                    break;
                }

                case CSCPC_IND_SC_CTNL_PT:
                {
                    uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 6, 0, 0, 0, 0, 0, 0};
                    pdu[1] =  EACI_MSG_DATA_IND_CSCPC_SC_CTNL_PT;
                    pdu[3] = (uint8_t)(ATT_SVC_CYCLING_SPEED_CADENCE);
                    pdu[4] = (uint8_t)(ATT_SVC_CYCLING_SPEED_CADENCE >> 8);
                    pdu[5] = ((struct cscpc_value_ind *)param)->value.ctnl_pt_rsp.req_op_code;
                    pdu[6] = ((struct cscpc_value_ind *)param)->value.ctnl_pt_rsp.resp_value;
                    pdu[7] = (uint8_t)(((struct cscpc_value_ind *)param)->value.ctnl_pt_rsp.supp_loc);
                    pdu[8] = (uint8_t)(((struct cscpc_value_ind *)param)->value.ctnl_pt_rsp.supp_loc >> 8);
                    eaci_pdu_send(sizeof(pdu), pdu);
                    break;
                }

                case CSCPC_NTF_CSC_MEAS:
                {
                    uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0};
                    pdu[1] =  EACI_MSG_DATA_IND_CSCPC_NTF_MEAS;
                    pdu[3] = (uint8_t)(ATT_SVC_CYCLING_SPEED_CADENCE);
                    pdu[4] = (uint8_t)(ATT_SVC_CYCLING_SPEED_CADENCE >> 8);
                    pdu[5] = ((struct cscpc_value_ind *)param)->value.csc_meas.flags;
                    pdu[6] = (uint8_t)(((struct cscpc_value_ind *)param)->value.csc_meas.cumul_crank_rev);
                    pdu[7] = (uint8_t)(((struct cscpc_value_ind *)param)->value.csc_meas.cumul_crank_rev >> 8);
                    pdu[8] = (uint8_t)(((struct cscpc_value_ind *)param)->value.csc_meas.last_crank_evt_time);
                    pdu[9] = (uint8_t)(((struct cscpc_value_ind *)param)->value.csc_meas.last_crank_evt_time >> 8);
                    pdu[10] = (uint8_t)(((struct cscpc_value_ind *)param)->value.csc_meas.last_wheel_evt_time);
                    pdu[11] = (uint8_t)(((struct cscpc_value_ind *)param)->value.csc_meas.last_wheel_evt_time >> 8);
                    pdu[12] = (uint8_t)(((struct cscpc_value_ind *)param)->value.csc_meas.cumul_wheel_rev);
                    pdu[13] = (uint8_t)(((struct cscpc_value_ind *)param)->value.csc_meas.cumul_wheel_rev >> 8);
                    pdu[14] = (uint8_t)(((struct cscpc_value_ind *)param)->value.csc_meas.cumul_wheel_rev >> 16);
                    pdu[15] = (uint8_t)(((struct cscpc_value_ind *)param)->value.csc_meas.cumul_wheel_rev >> 24);

                    eaci_pdu_send(sizeof(pdu), pdu);
                    break;
                }

                default:
                    break;
            }
            break;
        }
#endif
#if BLE_CSC_SENSOR
        case CSCPS_SC_CTNL_PT_REQ_IND:
        {
            struct cscps_sc_ctnl_pt_req_ind *cfg = (struct cscps_sc_ctnl_pt_req_ind *)param;
            union cscps_sc_ctnl_pt_cfm_value value;

            switch (cfg->op_code)
            {
                case CSCP_CTNL_PT_OP_UPD_LOC:
                    value.sensor_loc = cfg->value.sensor_loc;
                    app_cscps_sc_ctnl_pt_cfm(cfg->conhdl, PRF_ERR_OK, &value);
                    break;
                case CSCP_CTNL_PT_OP_REQ_SUPP_LOC:
                    value.supp_sensor_loc = 0x003F;
                    app_cscps_sc_ctnl_pt_cfm(cfg->conhdl, PRF_ERR_OK, &value);
                    break;
            }
        }
            break;

        case CSCPS_CMP_EVT:
        {
            uint8_t operation = ((struct cscps_cmp_evt *)param)->operation;
            switch(operation)
            {
                case CSCPS_SEND_CSC_MEAS_OP_CODE:
                {
                    uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 3, 0, 0, 0};
                    pdu[1] =  EACI_MSG_DATA_IND_CSCPS_MEAS;
                    pdu[3] = (uint8_t)(ATT_SVC_CYCLING_SPEED_CADENCE);
                    pdu[4] = (uint8_t)(ATT_SVC_CYCLING_SPEED_CADENCE >> 8);
                    pdu[5] = ((struct cscps_cmp_evt *)param)->status;
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

/// @} APP_EACI_CSC_ACI
