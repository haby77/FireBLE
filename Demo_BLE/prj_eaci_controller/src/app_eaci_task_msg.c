/**
 ****************************************************************************************
 *
 * @file app_eaci_task_msg.c
 *
 * @brief Easy ACI interface module source file.
 *
 * Copyright (C) Quintic 2012-2013
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */


/**
 ****************************************************************************************
 * @addtogroup APP_EACI_TASK_MSG_ACI
 * @{
 ****************************************************************************************
 */


/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "app_eaci.h"
#include "app_eaci_task_msg.h"

/**
 ****************************************************************************************
 * @brief EACI application task message handler
 *
 ****************************************************************************************
 */
void app_task_msg_hdl(ke_msg_id_t const msgid, void const *param)
{
    switch(msgid)
    {
        case GAP_SET_DEVNAME_REQ_CMP_EVT: 
        case GAP_SET_MODE_REQ_CMP_EVT:
        case GAP_ADV_REQ_CMP_EVT: 
        case GAP_DEV_INQ_RESULT_EVT:
        case GAP_DEV_INQ_REQ_CMP_EVT:
        case GAP_LE_CREATE_CONN_REQ_CMP_EVT:
        case GAP_DISCON_CMP_EVT:
        case GAP_BOND_REQ_CMP_EVT:
        case GAP_PARAM_UPDATE_RESP:
        case GAP_CHANGE_PARAM_REQ_CMP_EVT:
        case SMPC_SEC_STARTED_IND:
            gap_app_task_msg_hdl(msgid, param);
            break;

//Alert Notification Status profile
#if BLE_AN_CLIENT || BLE_AN_SERVER
        #if BLE_AN_CLIENT
        case ANPC_CMP_EVT:
        case ANPC_VALUE_IND:
        #endif
        #if BLE_AN_SERVER
        case ANPS_CMP_EVT:
        #endif
            an_app_task_msg_hdl(msgid, param);
            break;
#endif

//Battery Profile
#if BLE_BATT_CLIENT || BLE_BATT_SERVER
        #if BLE_BATT_CLIENT
        case BASC_ENABLE_CFM:
        case BASC_WR_CHAR_RSP:
        case BASC_BATT_LEVEL_IND:
        case BASC_BATT_LEVEL_NTF_CFG_RD_RSP:
        #endif
        #if BLE_BATT_SERVER
        case BASS_BATT_LEVEL_UPD_CFM:
        #endif
            batt_app_task_msg_hdl(msgid, param);
            break;
#endif

//Blood Pressure Profile
#if BLE_BP_COLLECTOR || BLE_BP_SENSOR
        #if BLE_BP_COLLECTOR
        case BLPC_ENABLE_CFM:
        case BLPC_RD_CHAR_RSP:
        case BLPC_WR_CHAR_RSP:
        case BLPC_BP_MEAS_IND:
        #endif
        #if BLE_BP_SENSOR
        case BLPS_MEAS_SEND_CFM:
        case BLPS_CFG_INDNTF_IND:
        #endif
            bp_app_task_msg_hdl(msgid, param);
            break;
#endif

//Cycling Speed and Cadence profile
#if BLE_CSC_COLLECTOR || BLE_CSC_SENSOR
        #if BLE_CSC_COLLECTOR
        case CSCPC_CMP_EVT:
        case CSCPC_VALUE_IND:
        #endif
        #if BLE_CSC_SENSOR
        case CSCPS_SC_CTNL_PT_REQ_IND:
        case CSCPS_CMP_EVT:
        #endif
            csc_app_task_msg_hdl(msgid, param);
            break;
#endif

//Device Information Profile
#if BLE_DIS_CLIENT
        case DISC_ENABLE_CFM:
        case DISC_RD_CHAR_RSP:
            dis_app_task_msg_hdl(msgid, param);
            break;
#endif

//Find Me Profile
#if BLE_FINDME_LOCATOR || BLE_FINDME_TARGET
        #if BLE_FINDME_LOCATOR
        case FINDL_ENABLE_CFM:
        #endif
        #if BLE_FINDME_TARGET
        case FINDT_ALERT_IND:
        #endif
            findme_app_task_msg_hdl(msgid, param);
            break;
#endif

//Glucose Profile
#if BLE_GL_COLLECTOR || BLE_GL_SENSOR
        #if BLE_GL_COLLECTOR
        case GLPC_ENABLE_CFM:
        case GLPC_REGISTER_CFM:
        case GLPC_READ_FEATURES_RSP:
        case GLPC_MEAS_IND:
        case GLPC_MEAS_CTX_IND:
        case GLPC_RACP_RSP:
        #endif
        #if BLE_GL_SENSOR
        case GLPS_CFG_INDNTF_IND:
        case GLPS_RACP_REQ_IND:
        #endif
            gl_app_task_msg_hdl(msgid, param);
            break;
#endif

//Heart Rate Profile
#if BLE_HR_COLLECTOR || BLE_HR_SENSOR
        #if BLE_HR_COLLECTOR
        case HRPC_ENABLE_CFM:
        case HRPC_RD_CHAR_RSP:
        case HRPC_WR_CHAR_RSP:
        case HRPC_HR_MEAS_IND:
        #endif
        #if BLE_HR_SENSOR
        case HRPS_CFG_INDNTF_IND:
        #endif
            hr_app_task_msg_hdl(msgid, param);
            break;
#endif

//Health Thermometer Profile
#if BLE_HT_COLLECTOR || BLE_HT_THERMOM
        #if BLE_HT_COLLECTOR
        case HTPC_ENABLE_CFM:
        case HTPC_RD_CHAR_RSP:
        case HTPC_TEMP_IND:
        case HTPC_MEAS_INTV_IND:
        case HTPC_WR_CHAR_RSP:
        #endif
        #if BLE_HT_THERMOM
        case HTPT_MEAS_INTV_CHG_IND:
        case HTPT_CFG_INDNTF_IND:
        #endif
            ht_app_task_msg_hdl(msgid, param);
            break;
#endif

//Phone Alert Status Profile
#if BLE_PAS_CLIENT || BLE_PAS_SERVER
        #if BLE_PAS_CLIENT
        case PASPC_CMP_EVT:
        case PASPC_VALUE_IND:
        #endif
        #if BLE_PAS_SERVER
        case PASPS_CMP_EVT:
        #endif
            pas_app_task_msg_hdl(msgid, param);
            break;
#endif

//Proximity Profile
#if BLE_PROX_MONITOR || BLE_PROX_REPORTER
        #if BLE_PROX_MONITOR
        case PROXM_ENABLE_CFM:
        case PROXM_RD_CHAR_RSP:
        #endif
        #if BLE_PROX_REPORTER
        case PROXR_ALERT_IND:
        #endif
            prox_app_task_msg_hdl(msgid, param);
            break;
#endif

//Running Speed and Cadence profile
#if BLE_RSC_COLLECTOR || BLE_RSC_SENSOR
        #if BLE_RSC_COLLECTOR
        case RSCPC_CMP_EVT:
        case RSCPC_VALUE_IND:
        #endif
        #if BLE_RSC_SENSOR
        case RSCPS_SC_CTNL_PT_REQ_IND:
        case RSCPS_CMP_EVT:
        #endif
            rsc_app_task_msg_hdl(msgid, param);
            break;
#endif

//Scan Parameters Profile
#if BLE_SP_CLIENT || BLE_SP_SERVER
        #if BLE_SP_CLIENT
        case SCPPC_ENABLE_CFM:
        case SCPPC_WR_CHAR_RSP:
        case SCPPC_SCAN_REFRESH_NTF_CFG_RD_RSP:
        #endif
        #if BLE_SP_SERVER
        case SCPPS_SCAN_INTV_WD_IND:
        case SCPPS_SCAN_REFRESH_NTF_CFG_IND:
        #endif
            sp_app_task_msg_hdl(msgid, param);
            break;
#endif

//Time Profile
#if BLE_TIP_CLIENT || BLE_TIP_SERVER
        #if BLE_TIP_CLIENT
        case TIPC_ENABLE_CFM:
        case TIPC_WR_CHAR_RSP:
        case TIPC_CT_IND:
        case TIPC_CT_NTF_CFG_RD_RSP:
        case TIPC_LTI_RD_RSP:
        case TIPC_RTI_RD_RSP:
        case TIPC_TDST_RD_RSP:
        case TIPC_TUS_RD_RSP:
        #endif
        #if BLE_TIP_SERVER
        case TIPS_CURRENT_TIME_CCC_IND:
        case TIPS_TIME_UPD_CTNL_PT_IND:
        #endif
            tip_app_task_msg_hdl(msgid, param);
            break;
#endif

        default:
            break;
    }
}

/// @} APP_EACI_TASK_MSG_ACI
