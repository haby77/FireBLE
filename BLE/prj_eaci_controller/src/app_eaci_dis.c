/**
 ****************************************************************************************
 *
 * @file app_eaci_dis.c
 *
 * @brief Application DIS ACI
 *
 * Copyright (C) Quintic 2012-2013
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup APP_EACI_DIS_ACI
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
 
#include "app_eaci.h"
#include "app_eaci_dis.h"

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief DISC data request
 *
 ****************************************************************************************
 */
#if BLE_DIS_CLIENT
void app_eaci_data_disc_hdl(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
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
    uint8_t idx = app_get_client_idx_by_conhdl(conhdl, ATT_SVC_DEVICE_INFO);
    
    if ((app_disc_env[idx].enabled == false) && (msg_id != EACI_MSG_DATA_REQ_DISC_ENABLE))
        return;

    switch (msg_id)
    {
        case EACI_MSG_DATA_REQ_DISC_ENABLE:
            if (param_len == BD_ADDR_LEN)
            {
                if (conhdl != 0xFFFF)
                    app_disc_enable_req(NULL, conhdl);
            }
            break;

        case EACI_MSG_DATA_REQ_DISC_CHAR_RD:
            if (param_len == (BD_ADDR_LEN + 1))
            {
                uint8_t char_code = 0;
                uint8_t flg = param[0];
                switch(flg)
                {
                    case 0:
                        char_code = DISC_MANUFACTURER_NAME_CHAR;
                        break;
                    case 1:
                        char_code = DISC_MODEL_NB_STR_CHAR;
                        break;
                    case 2:
                        char_code = DISC_SERIAL_NB_STR_CHAR;
                        break;
                    case 3:
                        char_code = DISC_HARD_REV_STR_CHAR;
                        break;
                    case 4:
                        char_code = DISC_FIRM_REV_STR_CHAR;
                        break;
                    case 5:
                        char_code = DISC_SW_REV_STR_CHAR;
                        break;
                    case 6:
                        char_code = DISC_SYSTEM_ID_CHAR;
                        break;
                    case 7:
                        char_code = DISC_IEEE_CHAR;
                        break;
                    case 8:
                        char_code = DISC_PNP_ID_CHAR;
                        break;
                    default:
                        break;
                }
                if (conhdl != 0xFFFF)
                    app_disc_rd_char_req(char_code, conhdl);

            }
            break;

        default:
            break;
    }
}
#endif

/**
 ****************************************************************************************
 * @brief DISS data request
 *
 ****************************************************************************************
 */
#if BLE_DIS_SERVER
void app_eaci_data_diss_hdl(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    if (app_diss_env->enabled == true)
        return;

    switch (msg_id)
    {
        case EACI_MSG_DATA_REQ_DISS_CHAR_WR:
            {
                uint8_t char_code = 0;
                uint8_t flg = param[0];
                switch(flg)
                {
                    case 0:
                        char_code = DIS_MANUFACTURER_NAME_CHAR;
                        break;
                    case 1:
                        char_code = DIS_MODEL_NB_STR_CHAR;
                        break;
                    case 2:
                        char_code = DIS_SERIAL_NB_STR_CHAR;
                        break;
                    case 3:
                        char_code = DIS_HARD_REV_STR_CHAR;
                        break;
                    case 4:
                        char_code = DIS_FIRM_REV_STR_CHAR;
                        break;
                    case 5:
                        char_code = DIS_SW_REV_STR_CHAR;
                        break;
                    case 6:
                        char_code = DIS_SYSTEM_ID_CHAR;
                        break;
                    case 7:
                        char_code = DIS_IEEE_CHAR;
                        break;
                    case 8:
                        char_code = DIS_PNP_ID_CHAR;
                        break;
                    default:
                        break;
                }

                app_set_char_val_req(char_code, (param_len - 1), (param + 1));
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
#if BLE_DIS_CLIENT
void dis_app_task_msg_hdl(ke_msg_id_t const msgid, void const *param)
{
    switch(msgid)
    {
#if BLE_DIS_CLIENT
        case DISC_ENABLE_CFM:
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_IND, 0, 3, 0, 0, 0};
                pdu[1] =  EACI_MSG_DATA_IND_DISC_ENABLE;
                pdu[3] = (uint8_t)(ATT_SVC_DEVICE_INFO);
                pdu[4] = (uint8_t)(ATT_SVC_DEVICE_INFO >> 8);
                pdu[5] = ((struct disc_enable_cfm *)param)->status;
                eaci_pdu_send(sizeof(pdu), pdu);
            }
            break;

        case DISC_RD_CHAR_RSP:
            {
                uint8_t datalen = ((struct disc_rd_char_rsp *)param)->val_length;
                uint8_t *pdu;
                pdu = (uint8_t *)malloc(6 + datalen);
                if (pdu != NULL)
                {
                    memset(pdu,0,datalen+6);
                    pdu[0] = EACI_MSG_TYPE_DATA_IND;
                    pdu[1] = EACI_MSG_DATA_IND_DISC_CHAR_RD;
                    pdu[2] = 3 + datalen;
                    pdu[3] = (uint8_t)(ATT_SVC_DEVICE_INFO);
                    pdu[4] = (uint8_t)(ATT_SVC_DEVICE_INFO >> 8);
                    pdu[5] = ((struct disc_rd_char_rsp *)param)->status;
                    memcpy((pdu + 6), ((struct disc_rd_char_rsp *)param)->val, datalen);
                    eaci_pdu_send((6 + datalen), pdu);
                    free(pdu);
                    pdu = NULL;
                }
            }
            break;
#endif
        default:
            break;
    }
}
#endif

/// @} APP_EACI_DIS_ACI
