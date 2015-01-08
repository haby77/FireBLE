/**
 ****************************************************************************************
 *
 * @file app_ancsc_task.c
 *
 * @brief Application ANCSC task implementation
 *
 * Copyright (C) Quintic 2014-2014
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup APP_ANCSC_TASK
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "app_env.h"

#if BLE_ANCS_NC
#include "app_ancsc.h"

/// @cond
/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */
struct app_ancsc_env_tag *app_ancsc_env = &app_env.ancsc_ev;
/// @endcond

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

void app_ancsc_sm_entry(uint8_t status)
{
    switch (app_ancsc_env->operation)
    {
        case ANCSC_OP_IDLE:
        case ANCSC_OP_SECURITY:
            if (status == CO_ERROR_NO_ERROR)
            {
                //Configure data source characteristic first
                app_ancsc_env->operation = ANCSC_OP_CFG_DATA_SOURCE;
                app_ancsc_cfg_indntf_req(ANCSC_DESC_DATA_SOURCE_CL_CFG, PRF_CLI_START_NTF, app_ancsc_env->conhdl);
            }
            break;
        case ANCSC_OP_CFG_DATA_SOURCE:
            if (status == PRF_ERR_OK || status == PRF_ERR_INEXISTENT_HDL)
            {
                //Configure notify source characteristic
                app_ancsc_env->operation = ANCSC_OP_CFG_NTF_SOURCE;
                app_ancsc_cfg_indntf_req(ANCSC_DESC_NTF_SOURCE_CL_CFG, PRF_CLI_START_NTF, app_ancsc_env->conhdl);
            }
            else if (status == ATT_ERR_INSUFF_AUTHEN || 
                     status == ATT_ERR_INSUFF_ENC)
            {
                //It should configure data source characteristic error, reconfigure it after pair or encrypt success
                app_ancsc_env->operation = ANCSC_OP_SECURITY;
                //In this demo, app only exists one link because app acts as salve, so master address is in dev_rec[0].
                app_gap_bond_req(&app_env.dev_rec[0].bonded_info.peer_addr, SMP_OOB_AUTH_DATA_NOT_PRESENT,
                                 SMP_AUTH_REQ_NO_MITM_BOND, SMP_IO_CAP_NO_INPUT_NO_OUTPUT);
            }
            else
            {
                ;
            }
            break;
        case ANCSC_OP_CFG_NTF_SOURCE:
            app_ancsc_env->operation = ANCSC_OP_IDLE;
            break;
        default:
            break;
    }
}

/*
 ****************************************************************************************
 * @brief Indicate the content of the peer device ANCS. *//**
 *
 * @param[in] msgid     ANCSC_ANCS_CONTENT_IND
 * @param[in] param     Pointer to struct ancsc_ancs_content_ind
 * @param[in] dest_id   TASK_APP
 * @param[in] src_id    TASK_ANCSC
 * @return If the message was consumed or not.
 * @description
 *
 * This handler is called at the end of the discovery procedure performed after the
 * connection establishment. It contains the structure of the peer device ANCS.
 *
 ****************************************************************************************
 */
int app_ancsc_ancs_content_ind_handler(ke_msg_id_t const msgid,
                                       struct ancsc_ancs_content_ind *param,
                                       ke_task_id_t const dest_id,
                                       ke_task_id_t const src_id)
{
    app_ancsc_env->ancs = param->ancs;

    return (KE_MSG_CONSUMED);
}

/*
 ****************************************************************************************
 * @brief Accept and process of the Notification Source notification. *//**
 *
 * @param[in] msgid     ANCSC_NTF_SOURCE_IND
 * @param[in] param     Pointer to struct ancsc_ntf_source_ind
 * @param[in] dest_id   TASK_APP
 * @param[in] src_id    TASK_ANCSC
 * @return If the message was consumed or not.
 * @description
 *
 * This handler is called once a Notification Source value has been received from the peer device 
 * upon a notification operation.
 *
 ****************************************************************************************
 */
int app_ancsc_ntf_source_ind_handler(ke_msg_id_t const msgid,
                                     struct ancsc_ntf_source_ind *param,
                                     ke_task_id_t const dest_id,
                                     ke_task_id_t const src_id)
{
    //uint8_t idx = KE_IDX_GET(src_id);

    QPRINTF("EventID: %d, EventFlags: %d, CategoryID: %d, count: %d, NotificationUID: %d\r\n", 
                    param->ntf_source.event_id, 
                    param->ntf_source.event_flags,
                    param->ntf_source.category_id, 
                    param->ntf_source.category_count,
                    param->ntf_source.ntf_uid);

    if (param->ntf_source.event_id == NOTIFICATION_ADDED)
    {
        app_ancsc_env->ntf_uid = param->ntf_source.ntf_uid;

        uint8_t cmd[] = {0x00/* AttributeIDAppIdentifier */,
                         0x01/* AttributeIDTitle */, 0x20, 0x00,
                         0x03/* AttributeIDMessage */, 0x20, 0x00};
        app_ancsc_get_ntf_attribute_req(param->ntf_source.ntf_uid, sizeof(cmd), cmd, param->conhdl);
    }

    return (KE_MSG_CONSUMED);
}

/*
 ****************************************************************************************
 * @brief Accept and process of the Data Source notification. *//**
 *
 * @param[in] msgid     ANCSC_DATA_SOURCE_IND
 * @param[in] param     Pointer to struct ancsc_data_source_ind
 * @param[in] dest_id   TASK_APP
 * @param[in] src_id    TASK_ANCSC
 * @return If the message was consumed or not.
 * @description
 *
 * This handler is called once a Data Source value has been received from the peer device 
 * upon a notification operation. If the value is larger than the 20 bytes, it is split into 
 * multiple fragments by the NP. The NC must recompose the value by splicing each fragment.
 * The value is complete when the complete tuples for each requested attribute has been received.
 *
 ****************************************************************************************
 */
int app_ancsc_data_source_ind_handler(ke_msg_id_t const msgid,
                                      struct ancsc_data_source_ind *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id)
{
    QPRINTF("Data Size: %d\r\n", param->data_size);

    for (uint8_t i = 0; i < param->data_size; i++)
        QPRINTF("%02x", param->data_source[i]);
    QPRINTF("\r\n");

    return (KE_MSG_CONSUMED);
}

/*
 ****************************************************************************************
 * @brief Complete Event Information.
 *
 ****************************************************************************************
 */
int app_ancsc_cmp_evt_handler(ke_msg_id_t const msgid,
                              struct ancsc_cmp_evt *param,
                              ke_task_id_t const dest_id,
                              ke_task_id_t const src_id)
{
    //uint8_t idx = KE_IDX_GET(src_id);

    //QPRINTF("ANCSC Complete Status Value: %d. idx = %d, \r\n", param->status, idx);
    switch (param->operation)
    {
        case ANCSC_ENABLE_OP_CODE:
            if (param->status == PRF_ERR_OK)
            {
                app_ancsc_env->conhdl = param->conhdl;
                app_ancsc_env->enabled = true;
                app_ancsc_env->operation = ANCSC_OP_IDLE;
                app_ancsc_sm_entry(param->status);
            }
            QPRINTF("ANCSC enable confirmation status: 0x%X.\r\n", param->status);
            break;
        case ANCSC_CFG_OP_CODE:
            app_ancsc_sm_entry(param->status);
            QPRINTF("ANCSC configure status: 0x%X.\r\n", param->status);
            break;
        case ANCSC_GET_NTF_ATTRIBUTE_OP_CODE:
            QPRINTF("ANCSC_GET_NTF_ATTRIBUTE_OP_CODE\r\n");
            break;
        case ANCSC_GET_APP_ATTRIBUTE_OP_CODE:
            QPRINTF("ANCSC_GET_APP_ATTRIBUTE_OP_CODE\r\n");
            break;
        default:
            break;
    }

    return (KE_MSG_CONSUMED);
}

/*
 ****************************************************************************************
 * @brief Handles the disable indication to APP. *//**
 *
 * @param[in] msgid     ANCSC_DISABLE_IND
 * @param[in] param     Pointer to struct prf_client_disable_ind
 * @param[in] dest_id   TASK_APP
 * @param[in] src_id    TASK_ANCSC
 * @return If the message was consumed or not.
 * @description
 *
 * This handler is used to inform the application that the Alert Notification Client Role
 * task has been correctly disabled or if an error has occurred during this process.
 *
 ****************************************************************************************
 */
int app_ancsc_disable_ind_handler(ke_msg_id_t const msgid,
                                 struct prf_client_disable_ind *param,
                                 ke_task_id_t const dest_id,
                                 ke_task_id_t const src_id)
{
    app_ancsc_env->conhdl = 0xFF;
    app_ancsc_env->enabled = false;
    app_ancsc_env->operation = ANCSC_OP_IDLE;
    
    QPRINTF("ANCSC disable indication\r\n");

    return (KE_MSG_CONSUMED);
}

#endif

/// @} APP_ANCSC_TASK
