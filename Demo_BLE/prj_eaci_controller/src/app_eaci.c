/**
 ****************************************************************************************
 *
 * @file app_eaci.c
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
 * @addtogroup EACI
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "app_env.h"
#include "app_eaci_trans.h"
#include "lib.h"
#include "uart.h"
#include "app_eaci_util.h"

/// Data handler
static void app_eaci_data_hdl(uint8_t msg_id, uint8_t param_len, uint8_t const *param);

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */
/// EACI environment context
struct eaci_env_tag eaci_env;

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Initialize EACI interface
 *
 ****************************************************************************************
 */
void app_eaci_init(void)
{
    //initialize tx state
    eaci_env.tx_state = EACI_STATE_TX_IDLE;

    //init TX queue
    co_list_init(&eaci_env.queue_tx);

    reg_eaci_tx_done(eaci_tx_done);

    eaci_env.error = TRUE;
    // start rx
    eaci_trans_init();

    eaci_env.param = NULL;
}

// Push msg into eaci tx queue
static void eaci_push(struct ke_msg *msg)
{
    // Push the message into the list of messages pending for transmission
    co_list_push_back(&eaci_env.queue_tx, &msg->hdr);

    // Check if there is no transmission ongoing
    if (eaci_env.tx_state == EACI_STATE_TX_IDLE)
        // Forward the message to the HCI UART for immediate transmission
        eaci_trans_write(msg);
}

/**
 ****************************************************************************************
 * @brief EACI send PDU
 *
 ****************************************************************************************
 */
void eaci_pdu_send(uint8_t len, uint8_t *par)
{
    // Allocate one msg for EACI tx
    uint8_t *msg_param = (uint8_t*)ke_msg_alloc(0, 0, 0, len);

    // Save the PDU in the MSG
    memcpy(msg_param, par, len);

     //extract the ke_msg pointer from the param passed and push it in HCI queue
    eaci_push(ke_param2msg(msg_param));
}

/**
 ****************************************************************************************
 * @brief After-process when one PDU has been sent.
 *
 ****************************************************************************************
 */
void eaci_tx_done(void)
{
    struct ke_msg * msg;
    // Clear the event
    ke_evt_clear(0x10000000);
    // Go back to IDLE state
    eaci_env.tx_state = EACI_STATE_TX_IDLE;
    //release current message (which was just sent)
    msg = (struct ke_msg *)co_list_pop_front(&eaci_env.queue_tx);
    // Free the kernel message space
    ke_msg_free(msg);
    // Check if there is a new message pending for transmission
    if ((msg = (struct ke_msg *)co_list_pick(&eaci_env.queue_tx)) != NULL)
    {
        // Forward the message to the HCI UART for immediate transmission
        eaci_trans_write(msg);
    }
}

/**
 ****************************************************************************************
 * @brief EACI uart message handler
 *
 ****************************************************************************************
 */
void app_eaci_msg_hdl(uint8_t msg_type, uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    switch (msg_type)
    {
        case EACI_MSG_TYPE_CMD:
            app_eaci_cmd_hdl(msg_id, param_len, param);
            break;

        case EACI_MSG_TYPE_DATA_REQ:
            app_eaci_data_hdl(msg_id, param_len, param);
            break;

        default:
            break;
    }
}

/**
 ****************************************************************************************
 * @brief EACI data from host handler
 *
 ****************************************************************************************
 */

static void app_eaci_data_hdl(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    if (param_len < 2)
        return;

    uint16_t uuid = param[1] << 8 | param[0];
    switch (uuid)
    {
#if (BLE_HR_COLLECTOR || BLE_HR_SENSOR)
        case ATT_SVC_HEART_RATE:
        {
            if (msg_id < EACI_MSG_DATA_REQ_HRPC_MAX)
            {
            #if BLE_HR_COLLECTOR
                app_eaci_data_hrpc_hdl(msg_id, param_len-2, param+2);
            #endif
            }
            else
            {
            #if BLE_HR_SENSOR
                app_eaci_data_hrps_hdl(msg_id, param_len-2, param+2);
            #endif
            }
            break;
        }
#endif

#if (BLE_HT_COLLECTOR || BLE_HT_THERMOM)
        case ATT_SVC_HEALTH_THERMOM:
        {
            if (msg_id < EACI_MSG_DATA_REQ_HT_MAX)
            {
            #if BLE_HT_COLLECTOR
                app_eaci_data_htpc_hdl(msg_id, param_len-2, param+2);
            #endif
            }
            else
            {
            #if BLE_HT_THERMOM
                app_eaci_data_htps_hdl(msg_id, param_len-2, param+2);
            #endif
            }
            break;
        }
#endif

#if (BLE_BP_COLLECTOR || BLE_BP_SENSOR)
        case ATT_SVC_BLOOD_PRESSURE:
        {
            if (msg_id < EACI_MSG_DATA_REQ_BP_MAX)
            {
            #if BLE_BP_COLLECTOR
                app_eaci_data_bpc_hdl(msg_id, param_len-2, param+2);
            #endif
            }
            else
            {
            #if BLE_BP_SENSOR
                app_eaci_data_bps_hdl(msg_id, param_len-2, param+2);
            #endif
            }
            break;
        }
#endif

#if (BLE_GL_COLLECTOR || BLE_GL_SENSOR)
        case ATT_SVC_GLUCOSE:
        {
            if (msg_id < EACI_MSG_DATA_REQ_GL_MAX)
            {
            #if BLE_GL_COLLECTOR
                app_eaci_data_glc_hdl(msg_id, param_len-2, param+2);
            #endif
            }
            else
            {
            #if BLE_GL_SENSOR
                app_eaci_data_gls_hdl(msg_id, param_len-2, param+2);
            #endif
            }
            break;
        }
#endif

#if (BLE_PROX_MONITOR || BLE_PROX_REPORTER)
        case ATT_SVC_LINK_LOSS:
        case ATT_SVC_TX_POWER:
        {
            if (msg_id < EACI_MSG_DATA_REQ_PROXM_MAX)
            {
            #if BLE_PROX_MONITOR
                app_eaci_data_proxm_hdl(msg_id, param_len-2, param+2);
            #endif
            }
            else
            {
            #if BLE_PROX_REPORTER
            #endif
            }
            break;
        }
#endif

#if (BLE_FINDME_LOCATOR || BLE_FINDME_TARGET)
        case ATT_SVC_IMMEDIATE_ALERT:
        {
            if (msg_id < EACI_MSG_DATA_REQ_FM_MAX)
            {
            #if BLE_FINDME_LOCATOR
                app_eaci_data_fm_hdl(msg_id, param_len-2, param+2);
            #endif
            }
            else
            {
            #if BLE_FINDME_TARGET
            #endif
            }
            break;
        }
#endif

#if (BLE_TIP_CLIENT || BLE_TIP_SERVER)
        case ATT_SVC_CURRENT_TIME:
        case ATT_SVC_REF_TIME_UPDATE:
        case ATT_SVC_NEXT_DST_CHANGE:
        {
            if (msg_id < EACI_MSG_DATA_REQ_TIP_MAX)
            {
            #if BLE_TIP_CLIENT
                app_eaci_data_tipc_hdl(msg_id, param_len-2, param+2);
            #endif
            }
            else
            {
            #if BLE_TIP_SERVER
                app_eaci_data_tips_hdl(msg_id, param_len-2, param+2);
            #endif
            }
            break;
        }
#endif

#if (BLE_SP_CLIENT || BLE_SP_SERVER)
        case ATT_SVC_SCAN_PARAMETERS:
        {
            if (msg_id < EACI_MSG_DATA_REQ_SP_MAX)
            {
            #if BLE_SP_CLIENT
                app_eaci_data_spc_hdl(msg_id, param_len-2, param+2);
            #endif
            }
            else
            {
            #if BLE_SP_SERVER
                app_eaci_data_sps_hdl(msg_id);
            #endif
            }
            break;
        }
#endif

#if (BLE_DIS_CLIENT || BLE_DIS_SERVER)
        case ATT_SVC_DEVICE_INFO:
        {
            if (msg_id < EACI_MSG_DATA_REQ_DISC_MAX)
            {
            #if BLE_DIS_CLIENT
                app_eaci_data_disc_hdl(msg_id, param_len-2, param+2);
            #endif
            }
            else
            {
            #if BLE_DIS_SERVER
                app_eaci_data_diss_hdl(msg_id, param_len-2, param+2);
            #endif
            }
            break;
        }
#endif

#if (BLE_BATT_CLIENT || BLE_BATT_SERVER)
        case ATT_SVC_BATTERY_SERVICE:
        {
            if (msg_id < EACI_MSG_DATA_REQ_BATT_MAX)
            {
            #if BLE_BATT_CLIENT
                app_eaci_data_basc_hdl(msg_id, param_len-2, param+2);
            #endif
            }
            else
            {
            #if BLE_BATT_SERVER
                app_eaci_data_bass_hdl(msg_id, param_len-2, param+2);
            #endif
            }
            break;
        }
#endif

#if (BLE_AN_CLIENT || BLE_AN_SERVER)
        case ATT_SVC_ALERT_NTF:
        {
            if (msg_id < EACI_MSG_DATA_REQ_ANPC_MAX)
            {
            #if BLE_AN_CLIENT
                app_eaci_data_anpc_hdl(msg_id, param_len-2, param+2);
            #endif
            }
            else
            {
            #if BLE_AN_SERVER
                app_eaci_data_anps_hdl(msg_id, param_len-2, param+2);
            #endif
            }
            break;
        }
#endif

#if BLE_CSC_COLLECTOR || BLE_CSC_SENSOR
        case ATT_SVC_CYCLING_SPEED_CADENCE:
        {
            if (msg_id < EACI_MSG_DATA_REQ_CSCPC_MAX)
            {
            #if BLE_CSC_COLLECTOR
                app_eaci_data_cscpc_hdl(msg_id, param_len-2, param+2);
            #endif
            }
            else
            {
            #if BLE_CSC_SENSOR
                app_eaci_data_cscps_hdl(msg_id, param_len-2, param+2);
            #endif
            }
            break;
        }
#endif

#if BLE_PAS_CLIENT || BLE_PAS_SERVER
        case ATT_SVC_PHONE_ALERT_STATUS:
        {
            if (msg_id < EACI_MSG_DATA_REQ_PASPC_MAX)
            {
            #if BLE_PAS_CLIENT
                app_eaci_data_paspc_hdl(msg_id, param_len-2, param+2);
            #endif
            }
            else
            {
            #if BLE_PAS_SERVER
                app_eaci_data_pasps_hdl(msg_id, param_len-2, param+2);
            #endif
            }
            break;
        }
#endif

#if BLE_RSC_COLLECTOR || BLE_RSC_SENSOR
        case ATT_SVC_RUNNING_SPEED_CADENCE:
        {
            if (msg_id < EACI_MSG_DATA_REQ_RSCPC_MAX)
            {
            #if BLE_RSC_COLLECTOR
                app_eaci_data_rscpc_hdl(msg_id, param_len-2, param+2);
            #endif
            }
            else
            {
            #if BLE_RSC_SENSOR
                app_eaci_data_rscps_hdl(msg_id, param_len-2, param+2);
            #endif
            }
            break;
        }
#endif

        default:
            break;
    }
}

/// @} EACI
