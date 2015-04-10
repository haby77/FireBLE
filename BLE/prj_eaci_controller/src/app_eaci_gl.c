/**
 ****************************************************************************************
 *
 * @file app_eaci_gl.c
 *
 * @brief Application GL ACI
 *
 * Copyright (C) Quintic 2012-2013
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup APP_EACI_GL_ACI
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
 
#include "app_eaci.h"
#include "app_eaci_gl.h"

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

#define APP_GLPS_STROED_RECORDS_NUM     10
#define APP_GLPS_MEAS_SEND_TO           1200    // 12s

/**
 ****************************************************************************************
 * @brief GL data request
 *
 ****************************************************************************************
 */
#if BLE_GL_COLLECTOR
void app_eaci_data_glc_hdl(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
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
    uint8_t idx = app_get_client_idx_by_conhdl(conhdl, ATT_SVC_GLUCOSE);
    
    if ((app_glpc_env[idx].enabled == false) && (msg_id != EACI_MSG_DATA_REQ_GL_ENABLE))
        return;

    switch (msg_id)
    {
        case EACI_MSG_DATA_REQ_GL_ENABLE:
            if (param_len == BD_ADDR_LEN)
            {
                if (conhdl != 0xFFFF)
                    app_glpc_enable_req(NULL, conhdl);
            }
            break;
        // Register
        case EACI_MSG_DATA_REQ_GL_REGISTER:
            if (param_len == (BD_ADDR_LEN + 1))
            {
                uint8_t meas_ctx_en = param[0];
                if (conhdl != 0xFFFF)
                    app_glpc_register_req(meas_ctx_en, conhdl);
            }
            break;
        // Read Features
        case EACI_MSG_DATA_REQ_GL_FEATURES_RD:
            if (param_len == BD_ADDR_LEN)
            {
                if (conhdl != 0xFFFF)
                    app_glpc_read_features_req(conhdl);
            }
            break;

        //operation code - Report Stored Records
        case EACI_MSG_DATA_REQ_GL_RSR_RACP:
            if (param_len == BD_ADDR_LEN + 5)
            {
                struct glp_racp_req req;
                req.op_code = GLP_REQ_REP_STRD_RECS;
                req.filter.operator = param[0];
                req.filter.filter_type = GLP_FILTER_SEQ_NUMBER;
                req.filter.val.seq_num.min = param[2] << 8 | param[1];
                req.filter.val.seq_num.max = param[4] << 8 | param[3];
                if (conhdl != 0xFFFF)
                    app_glpc_racp_req(&req, conhdl);
            }
            break;

        //operation code - Delete Stored Records
        case EACI_MSG_DATA_REQ_GL_DSR_RACP:
            if (param_len == BD_ADDR_LEN + 5)
            {
                struct glp_racp_req req;
                req.op_code = GLP_REQ_DEL_STRD_RECS;
                req.filter.operator = param[0];
                req.filter.filter_type = GLP_FILTER_SEQ_NUMBER;
                req.filter.val.seq_num.min = param[2] << 8 | param[1];
                req.filter.val.seq_num.max = param[4] << 8 | param[3];
                if (conhdl != 0xFFFF)
                    app_glpc_racp_req(&req, conhdl);
            }
            break;

        //operation code - Abort Operation
        case EACI_MSG_DATA_REQ_GL_AO_RACP:
            if (param_len == BD_ADDR_LEN + 5)
            {
                struct glp_racp_req req;
                req.op_code = GLP_REQ_ABORT_OP;
                if (conhdl != 0xFFFF)
                    app_glpc_racp_req(&req, conhdl);
            }
            break;

        //operation code - Report Number of Stored Records
        case EACI_MSG_DATA_REQ_GL_RN_RACP:
            if (param_len == BD_ADDR_LEN + 5)
            {
                struct glp_racp_req req;
                req.op_code = GLP_REQ_REP_NUM_OF_STRD_RECS;
                req.filter.operator = param[0];
                req.filter.filter_type = GLP_FILTER_SEQ_NUMBER;
                req.filter.val.seq_num.min = param[2] << 8 | param[1];
                req.filter.val.seq_num.max = param[4] << 8 | param[3];
                
                if (conhdl != 0xFFFF)
                    app_glpc_racp_req(&req, conhdl);
            }
            break;

        default:
            break;
    }
}
#endif

#if BLE_GL_SENSOR
void app_eaci_data_gls_hdl(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    if (app_glps_env->enabled == false)
        return;

    switch (msg_id)
    {
        case EACI_MSG_DATA_REQ_GL_SEND_MEAS_WITHOUT_CTX:
        {
            struct glp_meas send_meas;
            uint8_t cursor = 10;
            uint8_t Flags = 0;
            uint16_t seq_num = 0;
            
            send_meas.flags = param[0];
            Flags = param[0];
            seq_num = (param[2] << 8) | param[1];
            send_meas.base_time.year = (param[4] << 8) | param[3];
            send_meas.base_time.month = param[5];
            send_meas.base_time.day = param[6];
            send_meas.base_time.hour = param[7];
            send_meas.base_time.min = param[8];
            send_meas.base_time.sec = param[9];

            //Time Offset
            if ((Flags & GLP_MEAS_TIME_OFF_PRES) != 0)
            {
                send_meas.time_offset = (param[cursor + 1] << 8) | param[cursor];
                cursor += 2;
            }

            // Glucose Concentration, type and location
            if ((Flags & GLP_MEAS_GL_CTR_TYPE_AND_SPL_LOC_PRES) != 0)
            {
                send_meas.concentration = (param[cursor + 1] << 8) | param[cursor];
                send_meas.type = param[cursor + 2];
                send_meas.location = param[cursor + 3];
                cursor += 4;
            }

            // Sensor Status Annunciation
            if((Flags & GLP_MEAS_SENS_STAT_ANNUN_PRES) != 0)
            {
                send_meas.status = (param[cursor + 1] << 8) | param[cursor]; 
                cursor += 2;
            }

            app_glps_meas_without_ctx_req_send(app_glps_env->conhdl, seq_num, &send_meas);
        }
            break;
            
        case EACI_MSG_DATA_REQ_GL_SEND_MEAS_WITH_CTX:
        {
            uint16_t seq_num = param[2] << 8 | param[1];
            uint8_t Flags = param[0];
            uint8_t cursor = 10;
            uint8_t Flags_ctx = 0;
            struct glp_meas send_meas;
            
            send_meas.flags = param[0];
            send_meas.base_time.year = param[4] << 8 | param[3];
            send_meas.base_time.month = param[5];
            send_meas.base_time.day = param[6];
            send_meas.base_time.hour = param[7];
            send_meas.base_time.min = param[8];
            send_meas.base_time.sec = param[9];

            //Time Offset
            if ((Flags & GLP_MEAS_TIME_OFF_PRES) != 0)
            {
                send_meas.time_offset = (param[cursor + 1] << 8) | param[cursor];
                cursor += 2;
            }

            // Glucose Concentration, type and location
            if ((Flags & GLP_MEAS_GL_CTR_TYPE_AND_SPL_LOC_PRES) != 0)
            {
                send_meas.concentration = (param[cursor + 1] << 8) | param[cursor];
                send_meas.type = param[cursor + 2];
                send_meas.location = param[cursor + 3];
                cursor += 4;
            }

            // Sensor Status Annunciation
            if((Flags & GLP_MEAS_SENS_STAT_ANNUN_PRES) != 0)
            {
                send_meas.status = (param[cursor + 1] << 8) | param[cursor]; 
                cursor += 2;
            }

            /// Glucose measurement context
            struct glp_meas_ctx send_ctx;
            send_ctx.flags = param[cursor];
            Flags_ctx = param[cursor];

            // Extended Flags
            if((Flags_ctx & GLP_CTX_EXTD_F_PRES) != 0)
            {
                send_ctx.ext_flags = param[cursor + 1];
                cursor += 2;
            }

            // Carbohydrate ID And Carbohydrate Present
            if((Flags_ctx & GLP_CTX_CRBH_ID_AND_CRBH_PRES) != 0)
            {
                send_ctx.carbo_id = param[cursor + 1];
                send_ctx.carbo_val = param[cursor + 3] << 8 | param[cursor + 2];
                cursor += 4;
            }

            // Meal Present
            if((Flags_ctx & GLP_CTX_MEAL_PRES) != 0)
            {
                send_ctx.meal = param[cursor + 1];
                cursor += 2;
            }

            // Tester-Health Present
            if((Flags_ctx & GLP_CTX_TESTER_HEALTH_PRES) != 0)
            {
                send_ctx.tester = param[cursor + 1];
                send_ctx.health = param[cursor + 2];
                cursor += 3;
            }

            // Exercise Duration & Exercise Intensity Present
            if((Flags_ctx & GLP_CTX_EXE_DUR_AND_EXE_INTENS_PRES) != 0)
            {
                send_ctx.exercise_dur = param[cursor + 2] << 8 | param[cursor + 1];
                send_ctx.exercise_intens = param[cursor + 3];
                cursor += 4;
            }

            // Medication ID And Medication Present
            if((Flags_ctx & GLP_CTX_MEDIC_ID_AND_MEDIC_PRES) != 0)
            {
                send_ctx.med_id = param[cursor + 1];
                send_ctx.med_val = param[cursor + 3] << 8 | param[cursor + 2];
                cursor += 4;
            }

            // HbA1c Present
            if((Flags_ctx & GLP_CTX_HBA1C_PRES) != 0)
            {
                send_ctx.hba1c_val =  param[cursor + 2] << 8 | param[cursor + 1];
                cursor += 3;
            }

            app_glps_meas_with_ctx_req_send(app_glps_env->conhdl, seq_num, &send_meas, &send_ctx);
        }
            break;

        default:
            break;
    }
}
#endif
/**
 ****************************************************************************************
 * @brief This function is used for demonstration.
 *
 * @param[in] racp_req The operator of Record Access Control Point(RACP)
 *
 * @return None.
 ****************************************************************************************
 */
#if BLE_GL_SENSOR
static uint8_t app_glps_racp_procedure_handle(struct glp_racp_req *racp_req)
{
    uint8_t res = GLP_RSP_SUCCESS;

    switch (racp_req->filter.operator)
    {
        case GLP_OP_LT_OR_EQ:
        case GLP_OP_GT_OR_EQ:
        case GLP_OP_WITHIN_RANGE_OF:
            res = GLP_RSP_OPERATOR_NOT_SUP;
            break;
        case GLP_OP_ALL_RECS:
        case GLP_OP_FIRST_REC:
            app_glps_env->records_idx = 0;
            break;
        case GLP_OP_LAST_REC:
            app_glps_env->records_idx = APP_GLPS_STROED_RECORDS_NUM;
            break;
        default:
            res = GLP_RSP_INVALID_OPERATOR;
            break;
    }
    return res;
}
#endif
/**
 ****************************************************************************************
 * @brief EACI application task message handler
 *
 ****************************************************************************************
 */
#if BLE_GL_COLLECTOR || BLE_GL_SENSOR
void gl_app_task_msg_hdl(ke_msg_id_t const msgid, void const *param)
{
    switch(msgid)
    {
#if BLE_GL_COLLECTOR
        case GLPC_ENABLE_CFM:
        {
            uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 3, 0, 0, 0};
            pdu[1] = EACI_MSG_DATA_IND_GL_ENABLE;
            pdu[3] = (uint8_t)(ATT_SVC_GLUCOSE);
            pdu[4] = (uint8_t)(ATT_SVC_GLUCOSE >> 8);
            pdu[5] = ((struct glpc_enable_cfm *)param)->status;
            eaci_pdu_send(sizeof(pdu), pdu);
        }
            break;

        case GLPC_REGISTER_CFM:
        {
            uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 3, 0, 0, 0};
            pdu[1] = EACI_MSG_DATA_IND_GL_REGISTER;
            pdu[3] = (uint8_t)(ATT_SVC_GLUCOSE);
            pdu[4] = (uint8_t)(ATT_SVC_GLUCOSE >> 8);
            pdu[5] = ((struct glpc_register_cfm *)param)->status;
            eaci_pdu_send(sizeof(pdu), pdu);
        }
            break;

        case GLPC_READ_FEATURES_RSP:
        {
            uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 5, 0, 0, 0, 0, 0};
            pdu[1] = EACI_MSG_DATA_IND_GL_FEATURES_RD;
            pdu[3] = (uint8_t)(ATT_SVC_GLUCOSE);
            pdu[4] = (uint8_t)(ATT_SVC_GLUCOSE >> 8);
            pdu[5] = ((struct glpc_read_features_rsp *)param)->status;
            pdu[6] = (uint8_t)(((struct glpc_read_features_rsp *)param)->features);
            pdu[7] = (uint8_t)((((struct glpc_read_features_rsp *)param)->features) >> 8);
            eaci_pdu_send(sizeof(pdu), pdu);
        }
            break;

        case GLPC_MEAS_IND:
        {
            uint8_t Flags = 0;
            uint8_t cursor = 15;

            Flags = ((struct glpc_meas_ind *)param)->meas_val.flags;

            if ((Flags & GLP_MEAS_TIME_OFF_PRES) != 0)
            {
                cursor += 2;
            }

            if ((Flags & GLP_MEAS_GL_CTR_TYPE_AND_SPL_LOC_PRES) != 0)
            {
                cursor += 4;
            }

            // Sensor Status Annunciation
            if((Flags & GLP_MEAS_SENS_STAT_ANNUN_PRES) != 0)
            {
                cursor += 2;
            }

            uint8_t pdu[cursor];

            pdu[0] = EACI_MSG_TYPE_DATA_IND;
            pdu[1] = EACI_MSG_DATA_IND_GL_MEAS;
            pdu[2] = cursor - 3;
            pdu[3] = (uint8_t)(ATT_SVC_GLUCOSE);
            pdu[4] = (uint8_t)(ATT_SVC_GLUCOSE >> 8);
            pdu[5] = ((struct glpc_meas_ind *)param)->meas_val.flags;
            pdu[6] = (uint8_t)(((struct glpc_meas_ind *)param)->seq_num);
            pdu[7] = (uint8_t)((((struct glpc_meas_ind *)param)->seq_num) >> 8);
            pdu[8] = (uint8_t)(((struct glpc_meas_ind *)param)->meas_val.base_time.year);
            pdu[9] = (uint8_t)((((struct glpc_meas_ind *)param)->meas_val.base_time.year) >> 8);
            pdu[10] = ((struct glpc_meas_ind *)param)->meas_val.base_time.month;
            pdu[11] = ((struct glpc_meas_ind *)param)->meas_val.base_time.day;
            pdu[12] = ((struct glpc_meas_ind *)param)->meas_val.base_time.hour;
            pdu[13] = ((struct glpc_meas_ind *)param)->meas_val.base_time.min;
            pdu[14] = ((struct glpc_meas_ind *)param)->meas_val.base_time.sec;
            cursor = 15;

            //Time Offset
            if ((Flags & GLP_MEAS_TIME_OFF_PRES) != 0)
            {
                pdu[cursor] = (uint8_t)(((struct glpc_meas_ind *)param)->meas_val.time_offset);
                pdu[cursor + 1] =(uint8_t)((((struct glpc_meas_ind *)param)->meas_val.time_offset) >> 8);
                cursor += 2;
            }

            // Glucose Concentration, type and location
            if ((Flags & GLP_MEAS_GL_CTR_TYPE_AND_SPL_LOC_PRES) != 0)
            {
                pdu[cursor] = (uint8_t)(((struct glpc_meas_ind *)param)->meas_val.concentration);
                pdu[cursor + 1] = (uint8_t)((((struct glpc_meas_ind *)param)->meas_val.concentration) >> 8);
                pdu[cursor + 2] = ((struct glpc_meas_ind *)param)->meas_val.type;
                pdu[cursor + 3] = ((struct glpc_meas_ind *)param)->meas_val.location;
                cursor += 4;
            }

            // Sensor Status Annunciation
            if((Flags & GLP_MEAS_SENS_STAT_ANNUN_PRES) != 0)
            {
                pdu[cursor] = (uint8_t)(((struct glpc_meas_ind *)param)->meas_val.status);
                pdu[cursor + 1] = (uint8_t)((((struct glpc_meas_ind *)param)->meas_val.status) >> 8); 
                cursor += 2;
            }

            eaci_pdu_send(cursor, pdu);
        }
            break;

        case GLPC_MEAS_CTX_IND:
        {
            uint8_t flags = 0;
            uint8_t cursor = 8;

            flags = ((struct glpc_meas_ctx_ind *)param)->ctx.flags;

            // Extended Flags
            if((flags & GLP_CTX_EXTD_F_PRES) != 0)
            {
                cursor += 1;
            }

            // Carbohydrate ID And Carbohydrate Present
            if((flags & GLP_CTX_CRBH_ID_AND_CRBH_PRES) != 0)
            {
                cursor += 3;
            }

            // Meal Present
            if((flags & GLP_CTX_MEAL_PRES) != 0)
            {
                cursor += 1;
            }

            // Tester-Health Present
            if((flags & GLP_CTX_TESTER_HEALTH_PRES) != 0)
            {
                cursor += 2;
            }

            // Exercise Duration & Exercise Intensity Present
            if((flags & GLP_CTX_EXE_DUR_AND_EXE_INTENS_PRES) != 0)
            {
                cursor += 3;
            }

            // Medication ID And Medication Present
            if((flags & GLP_CTX_MEDIC_ID_AND_MEDIC_PRES) != 0)
            {
                cursor += 3;
            }

            // HbA1c Present
            if((flags & GLP_CTX_HBA1C_PRES) != 0)
            {
                cursor += 2;
            }

            uint8_t pdu[cursor];
            pdu[0] = EACI_MSG_TYPE_DATA_IND;
            pdu[1] = EACI_MSG_DATA_IND_GL_MEAS_CTX;
            pdu[2] = cursor - 3;
            pdu[3] = (uint8_t)(ATT_SVC_GLUCOSE);
            pdu[4] = (uint8_t)(ATT_SVC_GLUCOSE >> 8);
            pdu[5] = flags;
            pdu[6] = (uint8_t)(((struct glpc_meas_ctx_ind *)param)->seq_num);
            pdu[7] = (uint8_t)((((struct glpc_meas_ctx_ind *)param)->seq_num) >> 8);
            cursor = 8;
            // Extended Flags
            if((flags & GLP_CTX_EXTD_F_PRES) != 0)
            {
                pdu[cursor] = (uint8_t)(((struct glpc_meas_ctx_ind *)param)->ctx.ext_flags);
                cursor += 1;
            }

            // Carbohydrate ID And Carbohydrate Present
            if((flags & GLP_CTX_CRBH_ID_AND_CRBH_PRES) != 0)
            {
                pdu[cursor] = ((struct glpc_meas_ctx_ind *)param)->ctx.carbo_id;
                pdu[cursor + 1] = (uint8_t)(((struct glpc_meas_ctx_ind *)param)->ctx.carbo_val);
                pdu[cursor + 2] = (uint8_t)((((struct glpc_meas_ctx_ind *)param)->ctx.carbo_val) >> 8);
                cursor += 3;
            }

            // Meal Present
            if((flags & GLP_CTX_MEAL_PRES) != 0)
            {
                pdu[cursor] = ((struct glpc_meas_ctx_ind *)param)->ctx.meal;
                cursor += 1;
            }

            // Tester-Health Present
            if((flags & GLP_CTX_TESTER_HEALTH_PRES) != 0)
            {
                pdu[cursor] = ((struct glpc_meas_ctx_ind *)param)->ctx.tester;
                pdu[cursor + 1] = ((struct glpc_meas_ctx_ind *)param)->ctx.health;
                cursor += 2;
            }

            // Exercise Duration & Exercise Intensity Present
            if((flags & GLP_CTX_EXE_DUR_AND_EXE_INTENS_PRES) != 0)
            {
                pdu[cursor] = (uint8_t)(((struct glpc_meas_ctx_ind *)param)->ctx.exercise_dur);
                pdu[cursor + 1] = (uint8_t)((((struct glpc_meas_ctx_ind *)param)->ctx.exercise_dur) >> 8);
                pdu[cursor + 2] = ((struct glpc_meas_ctx_ind *)param)->ctx.exercise_intens;
                cursor += 3;
            }

            // Medication ID And Medication Present
            if((flags & GLP_CTX_MEDIC_ID_AND_MEDIC_PRES) != 0)
            {
                pdu[cursor] = (uint8_t)(((struct glpc_meas_ctx_ind *)param)->ctx.med_id);
                pdu[cursor + 1] = (uint8_t)(((struct glpc_meas_ctx_ind *)param)->ctx.med_val);
                pdu[cursor + 2] = (uint8_t)((((struct glpc_meas_ctx_ind *)param)->ctx.med_val) >> 8);
                cursor += 3;
            }

            // HbA1c Present
            if((flags & GLP_CTX_HBA1C_PRES) != 0)
            {
                pdu[cursor] = (uint8_t)(((struct glpc_meas_ctx_ind *)param)->ctx.hba1c_val);
                pdu[cursor + 1] = (uint8_t)((((struct glpc_meas_ctx_ind *)param)->ctx.hba1c_val) >> 8);
                cursor += 2;
            }

            eaci_pdu_send(cursor, pdu);
        }
            break;

        case GLPC_RACP_RSP:
        {
            uint8_t op_code = 0;
            uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 5, 0, 0, 0, 0, 0};
            pdu[1] = EACI_MSG_DATA_IND_GL_RACP_RSP;
            pdu[3] = (uint8_t)(ATT_SVC_GLUCOSE);
            pdu[4] = (uint8_t)(ATT_SVC_GLUCOSE >> 8);
            pdu[5] = ((struct glpc_racp_rsp *)param)->racp_rsp.op_code;

            op_code = ((struct glpc_racp_rsp *)param)->racp_rsp.op_code;
            switch (op_code)
            {
                case GLP_REQ_NUM_OF_STRD_RECS_RSP:
                {
                    pdu[6] = (uint8_t)(((struct glpc_racp_rsp *)param)->racp_rsp.operand.num_of_record);
                    pdu[7] = (uint8_t)((((struct glpc_racp_rsp *)param)->racp_rsp.operand.num_of_record) >> 8);
                }
                    break;
                case GLP_REQ_RSP_CODE:
                {
                    pdu[6] = (((struct glpc_racp_rsp *)param)->racp_rsp.operand.rsp.op_code_req);
                    pdu[7] = 0;
                }
                    break;
                default:
                    break;
            }

            eaci_pdu_send(sizeof(pdu), pdu);
        }
            break;
#endif

#if BLE_GL_SENSOR
        case GLPS_CFG_INDNTF_IND:
        {
            uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 3, 0, 0, 0};
            pdu[1] = EACI_MSG_DATA_IND_GLS_CFG;
            pdu[3] = (uint8_t)(ATT_SVC_GLUCOSE);
            pdu[4] = (uint8_t)(ATT_SVC_GLUCOSE >> 8);
            pdu[5] = (uint8_t)(((struct glps_cfg_indntf_ind *)param)->evt_cfg);
            eaci_pdu_send(sizeof(pdu), pdu);
        }
            break;
        
        case GLPS_RACP_REQ_IND:
        {
            uint8_t res;
            switch (((struct glps_racp_req_ind *)param)->racp_req.op_code)
            {
                case GLP_REQ_DEL_STRD_RECS:         // Delete stored records (Operator: Value from Operator Table)
                    app_glps_racp_rsp_req_send(app_glps_env->conhdl, 0, GLP_REQ_DEL_STRD_RECS, GLP_RSP_SUCCESS);
                    break;

                case GLP_REQ_ABORT_OP:              // Abort operation (Operator: Null 'value of 0x00 from Operator Table')
                    ke_timer_clear(APP_GLPS_MEAS_SEND_TIMER, TASK_APP);
                    app_glps_racp_rsp_req_send(app_glps_env->conhdl, 0, GLP_REQ_ABORT_OP, GLP_RSP_SUCCESS);
                    break;

                case GLP_REQ_REP_NUM_OF_STRD_RECS:  // Report number of stored records (Operator: Value from Operator Table)
                    res = app_glps_racp_procedure_handle(&app_glps_env->racp_req);
                    app_glps_racp_rsp_req_send(app_glps_env->conhdl, APP_GLPS_STROED_RECORDS_NUM, GLP_REQ_REP_NUM_OF_STRD_RECS, res);
                    break;

                default:
                    app_glps_racp_rsp_req_send(app_glps_env->conhdl, 0, ((struct glps_racp_req_ind *)param)->racp_req.op_code, GLP_RSP_OP_CODE_NOT_SUP);
                    break;
            }
        }
            break;
#endif
        default:
            break;
    }
}
#endif

/// @} APP_EACI_GL_ACI
