/**
 ****************************************************************************************
 *
 * @file app_eaci_rsc.c
 *
 * @brief Application RSC ACI
 *
 * Copyright (C) Quintic 2012-2013
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup APP_EACI_RSC_ACI
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
 
#include "app_eaci.h"
#include "app_eaci_rsc.h"
/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief RSCPC data request
 *
 ****************************************************************************************
 */
#if BLE_RSC_COLLECTOR
void app_eaci_data_rscpc_hdl(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
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
    uint8_t idx = app_get_client_idx_by_conhdl(conhdl, ATT_SVC_RUNNING_SPEED_CADENCE);

    if ((app_rscpc_env[idx].enabled == false) && (msg_id != EACI_MSG_DATA_REQ_RSCPC_ENABLE))
        return;

    switch (msg_id)
    {
        case EACI_MSG_DATA_REQ_RSCPC_ENABLE:
        {
            if (param_len == BD_ADDR_LEN)
            {
                if (conhdl != 0xFFFF)
                    app_rscpc_enable_req(NULL, conhdl);
            }
            break;
        }

        case EACI_MSG_DATA_REQ_RSCPC_RD:
        {
            if (param_len == BD_ADDR_LEN + 1)
            {
                uint8_t read_code = param[0];
                
                if (conhdl != 0xFFFF)
                    app_rscpc_read_req(read_code, conhdl);
            }
            break;
        }

        case EACI_MSG_DATA_REQ_RSCPC_CFG_NTFIND:
        {
            if (param_len == BD_ADDR_LEN + 3)
            {
                uint8_t desc_code = param[0];
                uint16_t ntfind_cfg = param[2] << 1 | param[1];
                
                if (conhdl != 0xFFFF)
                    app_rscpc_cfg_ntfind_req(desc_code, ntfind_cfg, conhdl);
            }
            break;
        }

        case EACI_MSG_DATA_REQ_RSCPC_CFG_CTNL_PT:
        {
            if (param_len == BD_ADDR_LEN + 5)
            {
                struct rscp_sc_ctnl_pt_req sc_ctnl_pt;
                sc_ctnl_pt.op_code = param[0];
                
                switch (sc_ctnl_pt.op_code)
                {
                    case RSCP_CTNL_PT_OP_SET_CUMUL_VAL:
                        sc_ctnl_pt.value.cumul_val = param[4] << 24 | param[3] << 16
                                                 | param[2] << 8 | param[1];
                        break;
                    case RSCP_CTNL_PT_OP_START_CALIB:
                        break;
                    case RSCP_CTNL_PT_OP_UPD_LOC:
                        sc_ctnl_pt.value.sensor_loc = param[1];
                        break;
                    case RSCP_CTNL_PT_OP_REQ_SUPP_LOC:
                        break;
                    default:
                        break;
                }

                if (conhdl != 0xFFFF)
                    app_rscpc_ctnl_pt_cfg_req(&sc_ctnl_pt, conhdl);
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
 * @brief RSCPS data request
 *
 ****************************************************************************************
 */

#if BLE_RSC_SENSOR
void app_eaci_data_rscps_hdl(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    if (app_rscps_env->enabled == false)
        return;

    switch (msg_id)
    {
        case EACI_MSG_DATA_REQ_RSCPS_NTF_MEAS:
        {
            uint8_t flags = param[0];
            uint8_t inst_cad = param[1];
            uint16_t inst_speed = param[3] << 8 | param[2];
            uint16_t inst_stride_len = param[5] << 8 | param[4];
            uint32_t total_dist = param[9] << 24 | param[8] << 16 | param[7] << 8 | param[6];
            
            app_rscps_ntf_rsc_meas_req(app_rscps_env->conhdl, flags, inst_cad, inst_speed, 
                                       inst_stride_len, total_dist);
            break;
        }

        case EACI_MSG_DATA_REQ_RSCPS_CTNL_PT:
        {
            union rscps_sc_ctnl_pt_cfm_value value;
            uint8_t op_code = param[0];

            switch (op_code)
            {
                case RSCP_CTNL_PT_OP_SET_CUMUL_VAL:
                    break;
                case RSCP_CTNL_PT_OP_START_CALIB:
                    break;
                case RSCP_CTNL_PT_OP_UPD_LOC:
                    value.sensor_loc = param[1];
                    break;
                case RSCP_CTNL_PT_OP_REQ_SUPP_LOC:
                    value.supp_sensor_loc = param[2] << 8 | param[1];
                    break;
                default:
                    break;
            }

            app_rscps_sc_ctnl_pt_cfm(app_rscps_env->conhdl, PRF_ERR_OK, &value);
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
#if BLE_RSC_COLLECTOR || BLE_RSC_SENSOR
void rsc_app_task_msg_hdl(ke_msg_id_t const msgid, void const *param)
{
    switch(msgid)
    {
#if BLE_RSC_COLLECTOR
        case RSCPC_CMP_EVT:
        {
            uint8_t operation = ((struct rscpc_cmp_evt *)param)->operation;
            switch (operation)
            {
                case RSCPC_ENABLE_OP_CODE:
                {
                    uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 3, 0, 0, 0};
                    pdu[1] =  EACI_MSG_DATA_IND_RSCPC_ENABLE;
                    pdu[3] = (uint8_t)(ATT_SVC_RUNNING_SPEED_CADENCE);
                    pdu[4] = (uint8_t)(ATT_SVC_RUNNING_SPEED_CADENCE >> 8);
                    pdu[5] = ((struct rscpc_cmp_evt *)param)->status;
                    eaci_pdu_send(sizeof(pdu), pdu);
                    break;
                }

                case RSCPC_READ_OP_CODE:
                {
                    uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 3, 0, 0, 0};
                    pdu[1] =  EACI_MSG_DATA_IND_RSCPC_READ;
                    pdu[3] = (uint8_t)(ATT_SVC_RUNNING_SPEED_CADENCE);
                    pdu[4] = (uint8_t)(ATT_SVC_RUNNING_SPEED_CADENCE >> 8);
                    pdu[5] = ((struct rscpc_cmp_evt *)param)->status;
                    eaci_pdu_send(sizeof(pdu), pdu);
                    break;
                }

                case RSCPC_CFG_NTF_IND_OP_CODE:
                {
                    uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 3, 0, 0, 0};
                    pdu[1] =  EACI_MSG_DATA_IND_RSCPC_CFG_NTF_IND;
                    pdu[3] = (uint8_t)(ATT_SVC_RUNNING_SPEED_CADENCE);
                    pdu[4] = (uint8_t)(ATT_SVC_RUNNING_SPEED_CADENCE >> 8);
                    pdu[5] = ((struct rscpc_cmp_evt *)param)->status;
                    eaci_pdu_send(sizeof(pdu), pdu);
                    break;
                }

                case RSCPC_CTNL_PT_CFG_WR_OP_CODE:
                {
                    uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 3, 0, 0, 0};
                    pdu[1] =  EACI_MSG_DATA_IND_RSCPC_CTNL_PT_CFG_WR;
                    pdu[3] = (uint8_t)(ATT_SVC_RUNNING_SPEED_CADENCE);
                    pdu[4] = (uint8_t)(ATT_SVC_RUNNING_SPEED_CADENCE >> 8);
                    pdu[5] = ((struct rscpc_cmp_evt *)param)->status;
                    eaci_pdu_send(sizeof(pdu), pdu);
                    break;
                }

                case RSCPC_CTNL_PT_CFG_IND_OP_CODE:
                {
                    uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 3, 0, 0, 0};
                    pdu[1] =  EACI_MSG_DATA_IND_RSCPC_CTNL_PT_CFG_IND;
                    pdu[3] = (uint8_t)(ATT_SVC_RUNNING_SPEED_CADENCE);
                    pdu[4] = (uint8_t)(ATT_SVC_RUNNING_SPEED_CADENCE >> 8);
                    pdu[5] = ((struct rscpc_cmp_evt *)param)->status;
                    eaci_pdu_send(sizeof(pdu), pdu);
                    break;
                }

                default:
                    break;
            }
            break;
        }

        case RSCPC_VALUE_IND:
        {
            uint8_t att_code = ((struct rscpc_value_ind *)param)->att_code;
            switch (att_code)
            {
                case RSCPC_NTF_RSC_MEAS:
                {
                    uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
                    pdu[1] =  EACI_MSG_DATA_IND_RSCPC_NTF_MEAS;
                    pdu[3] = (uint8_t)(ATT_SVC_RUNNING_SPEED_CADENCE);
                    pdu[4] = (uint8_t)(ATT_SVC_RUNNING_SPEED_CADENCE >> 8);
                    pdu[5] = ((struct rscpc_value_ind *)param)->value.rsc_meas.flags;
                    pdu[6] = ((struct rscpc_value_ind *)param)->value.rsc_meas.inst_cad;
                    pdu[7] = (uint8_t)(((struct rscpc_value_ind *)param)->value.rsc_meas.inst_speed);
                    pdu[8] = (uint8_t)(((struct rscpc_value_ind *)param)->value.rsc_meas.inst_speed >> 8);
                    pdu[9] = (uint8_t)(((struct rscpc_value_ind *)param)->value.rsc_meas.inst_stride_len);
                    pdu[10] = (uint8_t)(((struct rscpc_value_ind *)param)->value.rsc_meas.inst_stride_len >> 8);
                    pdu[11] = (uint8_t)(((struct rscpc_value_ind *)param)->value.rsc_meas.total_dist);
                    pdu[12] = (uint8_t)(((struct rscpc_value_ind *)param)->value.rsc_meas.total_dist >> 8);
                    pdu[13] = (uint8_t)(((struct rscpc_value_ind *)param)->value.rsc_meas.total_dist >> 16);
                    pdu[14] = (uint8_t)(((struct rscpc_value_ind *)param)->value.rsc_meas.total_dist >> 24);

                    eaci_pdu_send(sizeof(pdu), pdu);
                    break;
                }
                case RSCPC_RD_RSC_FEAT:
                {
                    uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 4, 0, 0, 0, 0};
                    pdu[1] =  EACI_MSG_DATA_IND_RSCPC_RSC_FEAT;
                    pdu[3] = (uint8_t)(ATT_SVC_RUNNING_SPEED_CADENCE);
                    pdu[4] = (uint8_t)(ATT_SVC_RUNNING_SPEED_CADENCE >> 8);
                    pdu[5] = (uint8_t)(((struct rscpc_value_ind *)param)->value.sensor_feat);
                    pdu[6] = (uint8_t)(((struct rscpc_value_ind *)param)->value.sensor_feat >> 8);
                    eaci_pdu_send(sizeof(pdu), pdu);
                    break;
                }
                case RSCPC_RD_SENSOR_LOC:
                {
                    uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 3, 0, 0, 0};
                    pdu[1] =  EACI_MSG_DATA_IND_RSCPC_SENSOR_LOC;
                    pdu[3] = (uint8_t)(ATT_SVC_RUNNING_SPEED_CADENCE);
                    pdu[4] = (uint8_t)(ATT_SVC_RUNNING_SPEED_CADENCE >> 8);
                    pdu[5] = ((struct rscpc_value_ind *)param)->value.sensor_loc;
                    eaci_pdu_send(sizeof(pdu), pdu);
                    break;
                }
                case RSCPC_IND_SC_CTNL_PT:
                {
                    uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 6, 0, 0, 0, 0, 0, 0};
                    pdu[1] =  EACI_MSG_DATA_IND_RSCPC_CTNL_PT;
                    pdu[3] = (uint8_t)(ATT_SVC_RUNNING_SPEED_CADENCE);
                    pdu[4] = (uint8_t)(ATT_SVC_RUNNING_SPEED_CADENCE >> 8);
                    pdu[5] = ((struct rscpc_value_ind *)param)->value.ctnl_pt_rsp.req_op_code;
                    pdu[6] = ((struct rscpc_value_ind *)param)->value.ctnl_pt_rsp.resp_value;
                    pdu[7] = (uint8_t)(((struct rscpc_value_ind *)param)->value.ctnl_pt_rsp.supp_loc);
                    pdu[8] = (uint8_t)(((struct rscpc_value_ind *)param)->value.ctnl_pt_rsp.supp_loc >> 8);
                    eaci_pdu_send(sizeof(pdu), pdu);
                    break;
                }
                case RSCPC_RD_WR_RSC_MEAS_CFG:
                {
                    uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 4, 0, 0, 0, 0};
                    pdu[1] =  EACI_MSG_DATA_IND_RSCPC_RSC_MEAS_CFG;
                    pdu[3] = (uint8_t)(ATT_SVC_RUNNING_SPEED_CADENCE);
                    pdu[4] = (uint8_t)(ATT_SVC_RUNNING_SPEED_CADENCE >> 8);
                    pdu[5] = (uint8_t)(((struct rscpc_value_ind *)param)->value.ntf_cfg);
                    pdu[6] = (uint8_t)(((struct rscpc_value_ind *)param)->value.ntf_cfg >> 8);
                    eaci_pdu_send(sizeof(pdu), pdu);
                    break;
                }
                case RSCPC_RD_WR_SC_CTNL_PT_CFG:
                {
                    uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 4, 0, 0, 0, 0};
                    pdu[1] =  EACI_MSG_DATA_IND_RSCPC_RSC_PT_CFG;
                    pdu[3] = (uint8_t)(ATT_SVC_RUNNING_SPEED_CADENCE);
                    pdu[4] = (uint8_t)(ATT_SVC_RUNNING_SPEED_CADENCE >> 8);
                    pdu[5] = (uint8_t)(((struct rscpc_value_ind *)param)->value.ntf_cfg);
                    pdu[6] = (uint8_t)(((struct rscpc_value_ind *)param)->value.ntf_cfg >> 8);
                    eaci_pdu_send(sizeof(pdu), pdu);
                    break;
                }
                default:
                    break;
            }
            break;
        }
#endif
#if BLE_RSC_SENSOR
        case RSCPS_SC_CTNL_PT_REQ_IND:
        {
            struct rscps_sc_ctnl_pt_req_ind *cfg = (struct rscps_sc_ctnl_pt_req_ind *)param;
            union rscps_sc_ctnl_pt_cfm_value value;

            switch (cfg->op_code)
            {
                case RSCP_CTNL_PT_OP_SET_CUMUL_VAL:
                    app_rscps_sc_ctnl_pt_cfm(app_rscps_env->conhdl, PRF_ERR_OK, &value);
                    break;
                case RSCP_CTNL_PT_OP_START_CALIB:
                    app_rscps_sc_ctnl_pt_cfm(app_rscps_env->conhdl, PRF_ERR_OK, &value);
                    break;
                case RSCP_CTNL_PT_OP_UPD_LOC:
                    value.sensor_loc = cfg->value.sensor_loc;
                    app_rscps_sc_ctnl_pt_cfm(app_rscps_env->conhdl, PRF_ERR_OK, &value);
                    break;
                case RSCP_CTNL_PT_OP_REQ_SUPP_LOC:
                    value.supp_sensor_loc = 0x001f;
                    app_rscps_sc_ctnl_pt_cfm(app_rscps_env->conhdl, PRF_ERR_OK, &value);
                    break;
            }
            break;
        }

        case RSCPS_CMP_EVT:
        {
            uint8_t operation = ((struct rscps_cmp_evt *)param)->operation;
            
            switch (operation)
            {
                case RSCPS_SEND_RSC_MEAS_OP_CODE:
                {
                    uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 3, 0, 0, 0};
                    pdu[1] =  EACI_MSG_DATA_IND_PASPS_SEND_MEAS;
                    pdu[3] = (uint8_t)(ATT_SVC_RUNNING_SPEED_CADENCE);
                    pdu[4] = (uint8_t)(ATT_SVC_RUNNING_SPEED_CADENCE >> 8);
                    pdu[5] = ((struct rscps_cmp_evt *)param)->status;
                    eaci_pdu_send(sizeof(pdu), pdu);
                    break;
                }

                case RSCPS_CTNL_PT_CUMUL_VAL_OP_CODE:
                {
                    uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 3, 0, 0, 0};
                    pdu[1] =  EACI_MSG_DATA_IND_PASPS_CTNL_CUMUL;
                    pdu[3] = (uint8_t)(ATT_SVC_RUNNING_SPEED_CADENCE);
                    pdu[4] = (uint8_t)(ATT_SVC_RUNNING_SPEED_CADENCE >> 8);
                    pdu[5] = ((struct rscps_cmp_evt *)param)->status;
                    eaci_pdu_send(sizeof(pdu), pdu);
                    break;
                }

                case RSCPS_CTNL_PT_UPD_LOC_OP_CODE:
                {
                    uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 3, 0, 0, 0};
                    pdu[1] =  EACI_MSG_DATA_IND_PASPS_CTNL_UPD_LOC;
                    pdu[3] = (uint8_t)(ATT_SVC_RUNNING_SPEED_CADENCE);
                    pdu[4] = (uint8_t)(ATT_SVC_RUNNING_SPEED_CADENCE >> 8);
                    pdu[5] = ((struct rscps_cmp_evt *)param)->status;
                    eaci_pdu_send(sizeof(pdu), pdu);
                    break;
                }

                case RSCPS_CTNL_PT_SUPP_LOC_OP_CODE:
                {
                    uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 3, 0, 0, 0};
                    pdu[1] =  EACI_MSG_DATA_IND_PASPS_CTNL_SUPP_LOC;
                    pdu[3] = (uint8_t)(ATT_SVC_RUNNING_SPEED_CADENCE);
                    pdu[4] = (uint8_t)(ATT_SVC_RUNNING_SPEED_CADENCE >> 8);
                    pdu[5] = ((struct rscps_cmp_evt *)param)->status;
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

/// @} APP_EACI_RSC_ACI
