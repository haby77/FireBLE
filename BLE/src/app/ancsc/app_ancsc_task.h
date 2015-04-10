/**
 ****************************************************************************************
 *
 * @file app_ancsc_task.h
 *
 * @brief Application ANCSC task implementation
 *
 * Copyright (C) Quintic 2014-2014
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */

#ifndef APP_ANCSC_TASK_H_
#define APP_ANCSC_TASK_H_

/**
 ****************************************************************************************
 * @addtogroup APP_ANCSC_TASK Apple Notification Center Service NC Role Task API
 * @ingroup APP_ANCSC
 * @brief Apple Notification Center Service NC Role Task API
 *
 * Apple Notification Center Service NC Task APIs are used to handle the message from ANCSC to APP.
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#if BLE_ANCS_NC
#include "app_ancsc.h"
/// @cond

//NVDS tag for nvds serice handle
#define APP_ANCSC_NVDS_TAG  (150)

enum
{
    ANCSC_OP_IDLE,
    ANCSC_OP_SECURITY,
    ANCSC_OP_CFG_NTF_SOURCE,
    ANCSC_OP_CFG_DATA_SOURCE,
    ANCSC_OP_CONTROL_POINT
};

struct ancsc_service_info
{
    struct bd_addr addr;
    struct ancsc_content ancs;
};

/// Apple Notification Center Service NC environment variable
struct app_ancsc_env_tag
{
    /// Profile role state: enabled/disabled
    uint8_t enabled;
    /// Current operation
    uint8_t operation;
    /// Connection handle
    uint16_t conhdl;
    /// Notification UID
    uint32_t ntf_uid;
    struct ancsc_content ancs;
};

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */
extern struct app_ancsc_env_tag *app_ancsc_env;
/// @endcond

/*
 ****************************************************************************************
 * @brief Indicate the content of the peer device ANCS.
 *
 ****************************************************************************************
 */
int app_ancsc_ancs_content_ind_handler(ke_msg_id_t const msgid,
                                       struct ancsc_ancs_content_ind *param,
                                       ke_task_id_t const dest_id,
                                       ke_task_id_t const src_id);

/*
 ****************************************************************************************
 * @brief Accept and process of the Notification Source notification.
 *
 ****************************************************************************************
 */
int app_ancsc_ntf_source_ind_handler(ke_msg_id_t const msgid,
                                     struct ancsc_ntf_source_ind *param,
                                     ke_task_id_t const dest_id,
                                     ke_task_id_t const src_id);
                                     
/*
 ****************************************************************************************
 * @brief Accept and process of the Data Source notification.
 *
 ****************************************************************************************
 */
int app_ancsc_data_source_ind_handler(ke_msg_id_t const msgid,
                                      struct ancsc_data_source_ind *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id);

/*
 ****************************************************************************************
 * @brief Complete Event Information.
 *
 ****************************************************************************************
 */
int app_ancsc_cmp_evt_handler(ke_msg_id_t const msgid,
                              struct ancsc_cmp_evt *param,
                              ke_task_id_t const dest_id,
                              ke_task_id_t const src_id);

/*
 ****************************************************************************************
 * @brief Handles the disable indication to APP.
 *
 ****************************************************************************************
 */
int app_ancsc_disable_ind_handler(ke_msg_id_t const msgid,
                                  struct prf_client_disable_ind *param,
                                  ke_task_id_t const dest_id,
                                  ke_task_id_t const src_id);
                                  
void app_ancsc_sm_entry(uint8_t status);

#endif // BLE_ANCS_NC

/// @} APP_ANCSC_TASK

#endif // APP_ANCSC_TASK_H_
