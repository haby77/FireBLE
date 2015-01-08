/**
 ****************************************************************************************
 *
 * @file app_data.c
 *
 * @brief Application Data request and Data indication functions file
 *
 * Copyright (C) Quintic 2009-2013
 *
 * $Rev: $
 *
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "app_env.h"

// Manufacturing name
#define DIS_MANU_NAME_VAL               "QUINTIC"
//Model number
#define DIS_MODEL_NB_VAL                "QN-BLE-B2"
//Serial number
#define DIS_SERIAL_NB_VAL               "1.9.1"
//Hardware Revision string value
#define DIS_HW_REV_VAL                  "6.6.6"
//Firmware Revision string value
#define DIS_FW_REV_VAL                  "8.8.8"
//Software Revision string value
#define DIS_SW_REV_VAL                  "5.5.5"
//System ID
#define DIS_SYS_ID_VAL                  "\xff\xff\xff\xFE\xFF\xBE\x7C\x08"
//IEEE Certif bytes
#define DIS_IEEE_CERTIF_VAL             "\x01\x02\x03\x04\x05\x06"
//PnP ID bytes
#define DIS_PNP_ID_VAL                  "\x03\x04\x05\x06\x07\x08\x09"

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

/**
*************************************************************************************
*** Alert Notification Profile
*************************************************************************************
 */
#if BLE_AN_CLIENT
void app_eaci_data_req_anpc(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    switch (msg_id)
    {
        case EACI_MSG_DATA_REQ_ANPC_ENABLE:
            if (param_len == BD_ADDR_LEN)
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_REQ, EACI_MSG_DATA_REQ_ANPC_ENABLE, 8, 0, 0, 0, 0, 0, 0, 0, 0};
                pdu[3] = (uint8_t)(ATT_SVC_ALERT_NTF);
                pdu[4] = (uint8_t)(ATT_SVC_ALERT_NTF >> 8);
                app_eaci_set_bd_addr_by_param((pdu + 5), param);
                eaci_uart_write(sizeof(pdu), pdu);
            }
            break;

        case EACI_MSG_DATA_REQ_ANPC_RD_CFG:
            if (param_len == BD_ADDR_LEN + 1)
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_REQ, EACI_MSG_DATA_REQ_ANPC_RD_CFG, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0};
                pdu[3] = (uint8_t)(ATT_SVC_ALERT_NTF);
                pdu[4] = (uint8_t)(ATT_SVC_ALERT_NTF >> 8);
                pdu[5] = param[0];
                app_eaci_set_bd_addr_by_param((pdu + 6), param + 1);
                eaci_uart_write(sizeof(pdu), pdu);
            }
            break;

        case EACI_MSG_DATA_REQ_ANPC_WR_VAL:
            if (param_len == BD_ADDR_LEN + 3)
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_REQ, EACI_MSG_DATA_REQ_ANPC_WR_VAL, 11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
                pdu[3] = (uint8_t)(ATT_SVC_ALERT_NTF);
                pdu[4] = (uint8_t)(ATT_SVC_ALERT_NTF >> 8);
                pdu[5] = param[0];
                pdu[6] = param[1];
                pdu[7] = param[2];
                app_eaci_set_bd_addr_by_param((pdu + 8), param + 3);
                eaci_uart_write(sizeof(pdu), pdu);
            }
            break;

        default:
            break;
    }
}
static void app_eaci_data_ind_anpc(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    switch (msg_id)
    {
        case EACI_MSG_DATA_IND_ANPC_ENABLE:
            if (param_len == 1)
            {
                QPRINTF("ANPC enable confirmation status: 0x%X.\r\n", param[0]);
            }
            break;

        case EACI_MSG_DATA_IND_ANPC_WR_STATUS:
            if (param_len == 1)
            {
                QPRINTF("ANPC Write confirmation status: 0x%X.\r\n", param[0]);
            }
            break;

        case EACI_MSG_DATA_IND_ANPC_NEW_ALERT_CFG:
            if (param_len == 2)
            {
                uint16_t cfg_val = param[1] << 8 | param[0];
                QPRINTF("New Alert Client CFG.: 0x%04X.\r\n", cfg_val);
            }
            break;

        case EACI_MSG_DATA_IND_ANPC_UNREAD_ALERT_CFG:
            if (param_len == 2)
            {
                uint16_t cfg_val = param[1] << 8 | param[0];
                QPRINTF("Unread Alert Status Client CFG.: 0x%04X.\r\n", cfg_val);
            }
            break;

        case EACI_MSG_DATA_IND_ANPC_NEW_ALERT:
            if (param_len == 2)
            {
                uint16_t new_alert = param[1] << 8 | param[0];
                QPRINTF("New Alert Status Value: 0x%04X.\r\n", new_alert);
            }
            break;

        case EACI_MSG_DATA_IND_ANPC_UNREAD_ALERT_STATUS:
            if (param_len == 2)
            {
                uint16_t unread_alert = param[1] << 8 | param[0];
                QPRINTF("Unread Alert Status Value: 0x%04X.\r\n", unread_alert);
            }
            break;

        default:
            break;
    }
}
#endif

#if BLE_AN_SERVER
void app_eaci_data_req_anps(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    switch (msg_id)
    {
        case EACI_MSG_DATA_REQ_ANPS_NTF_ALERT:
        {
            uint8_t *pdu;
            pdu = (uint8_t *)malloc(5 + param_len);
            if (pdu != NULL)
            {
                memset(pdu,0,param_len+5);
                pdu[0] = EACI_MSG_TYPE_DATA_REQ;
                pdu[1] = EACI_MSG_DATA_REQ_ANPS_NTF_ALERT;
                pdu[2] = 2 + param_len;
                pdu[3] = (uint8_t)(ATT_SVC_ALERT_NTF);
                pdu[4] = (uint8_t)(ATT_SVC_ALERT_NTF >> 8);
                memcpy((pdu + 5), param, param_len);
                eaci_uart_write((5 + param_len), pdu);
                free(pdu);
                pdu = NULL;
            }
        }
            break;

        default:
            break;
    }
}
static void app_eaci_data_ind_anps(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    switch (msg_id)
    {
        case EACI_MSG_DATA_IND_ANPS_UPD_NEW_ALERT:
            if (param_len == 1)
            {
                QPRINTF("Update New Alert Status: 0x%02X.\r\n", param[0]);
            }
            break;

        case EACI_MSG_DATA_IND_ANPS_UPD_UNREAD_ALERT:
            if (param_len == 1)
            {
                QPRINTF("Update Unread Alert Status: 0x%02X.\r\n", param[0]);
            }
            break;

        default:
            break;
    }
}
#endif

/**
*************************************************************************************
*** Battery Profile
*************************************************************************************
 */
#if BLE_BATT_CLIENT
uint8_t rd_batc_char = 0;
void app_eaci_data_req_basc(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    switch (msg_id)
    {
        case EACI_MSG_DATA_REQ_BATT_ENABLE:
            if (param_len == BD_ADDR_LEN)
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_REQ, EACI_MSG_DATA_REQ_BATT_ENABLE, 8, 0x0f, 0x18, 0, 0, 0, 0, 0, 0};
                app_eaci_set_bd_addr_by_param((pdu + 5), param);
                eaci_uart_write(sizeof(pdu), pdu);
            }
            break;

        case EACI_MSG_DATA_REQ_BATT_RD_CHAR_VAL:
            if (param_len == (BD_ADDR_LEN + 1))
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_REQ, EACI_MSG_DATA_REQ_BATT_RD_CHAR_VAL, 9, 0x0f, 0x18, 0, 0, 0, 0, 0, 0, 0};
                pdu[5] = param[0];
                rd_batc_char = param[0];
                app_eaci_set_bd_addr_by_param((pdu + 6), (param +1));
                eaci_uart_write(sizeof(pdu), pdu);
            }
            break;

        case EACI_MSG_DATA_REQ_BATT_CFG_NOTIFY:
            if (param_len == (BD_ADDR_LEN + 2))
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_REQ, EACI_MSG_DATA_REQ_BATT_CFG_NOTIFY, 10, 0x0f, 0x18, 0, 0, 0, 0, 0, 0, 0, 0};
                pdu[5] = param[0];
                pdu[6] = param[1];
                app_eaci_set_bd_addr_by_param((pdu + 7), (param + 2));
                eaci_uart_write(sizeof(pdu), pdu);
            }
            break;

        default:
            break;
    }
}
#endif

#if BLE_BATT_SERVER
void app_eaci_data_req_bass(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    switch (msg_id)
    {
        case EACI_MSG_DATA_REQ_BASS_BATT_LEVEL_UPD:
            if (param_len == BD_ADDR_LEN + 2)
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_REQ, EACI_MSG_DATA_REQ_BASS_BATT_LEVEL_UPD, 10, 0x0f, 0x18, 0, 0, 0, 0, 0, 0, 0, 0};
                pdu[5] = param[0];
                pdu[6] = param[1];
                app_eaci_set_bd_addr_by_param((pdu + 7), (param + 2));
                eaci_uart_write(sizeof(pdu), pdu);
            }
            break;

        default:
            break;
    }
}
#endif

#if BLE_BATT_CLIENT
static void app_eaci_data_ind_basc(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    switch (msg_id)
    {
        case EACI_MSG_DATA_IND_BATT_ENABLE:
            if (param_len == 1)
            {
                QPRINTF("Battery enable confirmation status: 0x%X.\r\n", param[0]);
            }
            break;

        case EACI_MSG_DATA_IND_BATT_LEVEL:
            if (param_len == 1)
            {
                QPRINTF("Battery Level: 0x%X.\r\n", param[0]);
            }
            break;

        case EACI_MSG_DATA_IND_BATT_RD_CFG:
            if (param_len == 2)
            {
                QPRINTF("Battery cfg.: 0x%04X.\r\n", param[1] << 8 | param[0]);
            }
            break;

        case EACI_MSG_DATA_IND_BATT_WR_RSP:
            if (param_len == 1)
            {
                QPRINTF("Write Status: 0x%02X.\r\n", param[0]);
            }
            break;

        default:
            break;
    }
}
#endif
#if BLE_BATT_SERVER
static void app_eaci_data_ind_bass(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    switch (msg_id)
    {
        case EACI_MSG_DATA_IND_BASS_BATT_UPD_CFM:
            if (param_len == 1)
            {
                QPRINTF("Battery Update status: 0x%X.\r\n", param[0]);
            }
            break;

        default:
            break;
    }
}
#endif

/**
*************************************************************************************
*** Blood Pressure Profile
*************************************************************************************
 */
#if BLE_BP_COLLECTOR
uint8_t bp_rd_char = 0;
void app_eaci_data_req_blpc(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    switch (msg_id)
    {
        case EACI_MSG_DATA_REQ_BP_ENABLE:
            if (param_len == BD_ADDR_LEN)
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_REQ, EACI_MSG_DATA_REQ_BP_ENABLE, 8, 0x10, 0x18, 0, 0, 0, 0, 0, 0};
                app_eaci_set_bd_addr_by_param((pdu + 5), param);
                eaci_uart_write(sizeof(pdu), pdu);
            }
            break;

        case EACI_MSG_DATA_REQ_BP_RD_CHAR_CODE:
            if (param_len == (BD_ADDR_LEN + 1))
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_REQ, EACI_MSG_DATA_REQ_BP_RD_CHAR_CODE, 9, 0x10, 0x18, 0, 0, 0, 0, 0, 0, 0};
                pdu[5] = param[0];
                bp_rd_char = param[0];
                app_eaci_set_bd_addr_by_param((pdu + 6), (param + 1));
                eaci_uart_write(sizeof(pdu), pdu);
            }
            break;

        case EACI_MSG_DATA_REQ_BP_CFG_INDNTF:
            if (param_len == (BD_ADDR_LEN + 2))
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_REQ, EACI_MSG_DATA_REQ_BP_CFG_INDNTF, 11, 0x10, 0x18, 0, 0, 0, 0, 0, 0, 0, 0, 0};
                pdu[5] = param[0];
                pdu[6] = param[1];
                pdu[7] = 0;
                app_eaci_set_bd_addr_by_param((pdu + 8), (param + 2));
                eaci_uart_write(sizeof(pdu), pdu);
            }
            break;

        default:
            break;
    }
}
#endif
#if BLE_BP_SENSOR
void app_eaci_data_req_blps(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    switch (msg_id)
    {
        case EACI_MSG_DATA_REQ_BP_SEND_PRES:
            if (param_len == 2)
            {
                uint8_t *pdu;
                uint8_t data_len = 0;
                uint8_t index = 7;
                
                if (param[1] & BPS_FLAG_USER_ID_PRESENT)
                {
                    data_len += 1;
                }
                if (param[1] & BPS_FLAG_KPA)                        // Pressure in pascal
                {
                    data_len += 6;
                }
                else                                            // pressure in millimetre of mercury
                {
                    data_len += 6;
                }
                
                if (param[1] & BPS_FLAG_PULSE_RATE_PRESENT)         // Pulse Rate
                {
                    data_len += 2;
                }
                
                if (param[1] & BPS_FLAG_MEAS_STATUS_PRESENT)        // Measurement Status
                {
                    data_len += 2;
                }
                
                if (param[1] & BPS_FLAG_TIME_STAMP_PRESENT)         // Time Stamp
                {
                    data_len += 7;
                }
                pdu = (uint8_t *)malloc(data_len + 7);
                
                if (pdu != NULL)
                {
                    memset(pdu,0,data_len + 7);
                    pdu[0] = EACI_MSG_TYPE_DATA_REQ;
                    pdu[1] = EACI_MSG_DATA_REQ_BP_SEND_PRES;
                    pdu[2] = data_len + 4;
                    pdu[3] = (uint8_t)(ATT_SVC_BLOOD_PRESSURE);
                    pdu[4] = (uint8_t)(ATT_SVC_BLOOD_PRESSURE >> 8);
                    pdu[5] = param[0];
                    pdu[6] = param[1];
                    
                    if (pdu[6] & BPS_FLAG_USER_ID_PRESENT)   // User ID present
                    {
                        pdu[index] = 0xff;
                        index += 1;
                    }
                    
                    if (pdu[6] & BPS_FLAG_KPA)         // Pressure in pascal
                    {
                        //Systolic (mmHg/kPa)
                        pdu[index] = 0x3f;
                        pdu[index + 1] = 0xe6;
                        //Diastolic (mmHg/kPa)
                        pdu[index + 2] = 0x2a;
                        pdu[index + 3] = 0xe4;
                        //Mean Arterial Pressure (mmHg/kPa)
                        pdu[index + 4] = 0xe7;
                        pdu[index + 5] = 0xe3;
                        index += 6;
                    }
                    else                                            // pressure in millimetre of mercury
                    {
                        //Systolic (mmHg/kPa)
                        pdu[index] = 0x78;
                        pdu[index + 1] = 0x00;
                        //Diastolic (mmHg/kPa)
                        pdu[index + 2] = 0x50;
                        pdu[index + 3] = 0x00;
                        //Mean Arterial Pressure (mmHg/kPa)
                        pdu[index + 4] = 0x4b;
                        pdu[index + 5] = 0x00;
                        index += 6;
                    }
                    
                    if (pdu[6] & BPS_FLAG_PULSE_RATE_PRESENT)         // Pulse Rate
                    {
                        pdu[index] = 0x3c;
                        pdu[index + 1] = 0x00;
                        index += 2;
                    }
                    
                    if (pdu[6] & BPS_FLAG_MEAS_STATUS_PRESENT)        // Measurement Status
                    {
                        pdu[index] = (uint8_t)BPS_STATE_CUFF_TOO_LOOSE;
                        pdu[index + 1] = (uint8_t)((BPS_STATE_CUFF_TOO_LOOSE) >> 8);
                        
                        index += 2;
                    }
                    
                    if (pdu[6] & BPS_FLAG_TIME_STAMP_PRESENT)         // Time Stamp
                    {
                        pdu[index] = 0xDD;
                        pdu[index + 1] = 0x07; // year 2013
                        pdu[index + 2] = 10; //month
                        pdu[index + 3] = 22; //day
                        pdu[index + 4] = 11; //hour
                        pdu[index + 5] = 01; //min
                        pdu[index + 6] = 55; //sec
                        
                        index += 7;
                    }
                    
                    eaci_uart_write(index, pdu);
                    free(pdu);
                    pdu = NULL;
                }
            }
            break;
        
        default:
            break;
    }
}
#endif

#if BLE_BP_COLLECTOR
static void app_eaci_data_ind_blpc(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    switch (msg_id)
    {
        case EACI_MSG_DATA_IND_BP_ENABLE:
            if (param_len == 1)
            {
                QPRINTF("Blood Pressure enable confirmation status: 0x%02X.\r\n", (uint8_t)param[0]);
            }
            break;

        case EACI_MSG_DATA_IND_BP_RD_RSP:
        {
            QPRINTF("Blood Pressure Read char response status: 0x%02X.\r\n", (uint8_t)param[0]);
            switch(bp_rd_char)
            {
                case 2:
                    QPRINTF("Blood Pressure Features :");
                    break;
                case 0x10:
                    QPRINTF("Blood Pressure Measurement Client Cfg. Desc :");
                    break;
                case 0x11:
                    QPRINTF("Intermdiate Cuff Pressure Client Cfg. Desc :");
                    break;
                default:
                    break;
            }
            uint16_t bp_value = param[1] | (param[2] << 8);
            QPRINTF(" 0x%04X \r\n", bp_value);
        }
            break;

        case EACI_MSG_DATA_IND_BP_WR_RSP:
            if (param_len == 1)
            {
                QPRINTF("Blood Pressure Write char response status: 0x%02X.\r\n", (uint8_t)param[0]);
            }
            break;

        case EACI_MSG_DATA_IND_BP_MEAS:
        {
            uint16_t flag_interm_cp = 0;
            uint8_t flags = 0;
            uint8_t index = 3;
            
            flag_interm_cp = param[1] << 8 | param[0];
            if (flag_interm_cp)
            {
                QPRINTF("Intermediary cuff pressure measurement: 0x%04X.\r\n", flag_interm_cp);
            }
            else
            {
                QPRINTF("Stable blood pressure measurement: 0x%04X.\r\n", flag_interm_cp);
            }
            
            flags = param[2];
            if (flags & BPS_FLAG_USER_ID_PRESENT)            // User ID present
            {
                QPRINTF("User ID present: 0x%02X.\r\n", param[index]);
                index += 1;
            }
            
            if (flags & BPS_FLAG_KPA)                        // Pressure in pascal
            {
                QPRINTF("Systolic (mmHg/kPa): 0x%04X.\r\n", (param[index + 1] << 8) | param[index]);
                QPRINTF("Diastolic (mmHg/kPa): 0x%04X.\r\n", (param[index + 3] << 8) | param[index + 2]);
                QPRINTF("Mean Arterial Pressure (mmHg/kPa): 0x%04X.\r\n", (param[index + 5] << 8) | param[index + 4]);
                index += 6;
            }
            else                                            // pressure in millimetre of mercury
            {
                QPRINTF("Systolic (mmHg/kPa): 0x%04X.\r\n", (param[index + 1] << 8) | param[index]);
                QPRINTF("Diastolic (mmHg/kPa): 0x%04X.\r\n", (param[index + 3] << 8) | param[index + 2]);
                QPRINTF("Mean Arterial Pressure (mmHg/kPa): 0x%04X.\r\n", (param[index + 5] << 8) | param[index + 4]);
                index += 6;
            }
            
            if (flags & BPS_FLAG_PULSE_RATE_PRESENT)         // Pulse Rate
            {
                QPRINTF("Pulse Rate: 0x%04X.\r\n", (param[index + 1] << 8) | param[index]);
                index += 2;
            }
            
            if (flags & BPS_FLAG_MEAS_STATUS_PRESENT)        // Measurement Status
            {
                QPRINTF("Measurement Status: 0x%04X.\r\n", (param[index + 1] << 8) | param[index]);
                index += 2;
            }
            
            if (flags & BPS_FLAG_TIME_STAMP_PRESENT)         // Time Stamp
            {
                QPRINTF("Year: %d. ", (param[index + 1] << 8) | param[index]);
                QPRINTF("month: %d. ", param[index + 2]);
                QPRINTF("day: %d. ", param[index + 3]);
                QPRINTF("hour: %d. ", param[index + 4]);
                QPRINTF("min: %d. ", param[index + 5]);
                QPRINTF("sec: %d.\r\n", param[index + 6]);
                index += 7;
            }
        }
           break;

        default:
            break;
    }
}
#endif
#if BLE_BP_SENSOR
static void app_eaci_data_ind_blps(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    switch (msg_id)
    {
        case EACI_MSG_DATA_IND_BPS_CFG:
            if (param_len == 3)
            {
                uint16_t cfg_val = param[2] << 8 | param[1];
                if (param[0] == 0x01)
                {
                    QPRINTF("BLPS configure indication: 0x%04X.\r\n", cfg_val);
                }
                else
                {
                    QPRINTF("BLPS configure notify: 0x%04X.\r\n", cfg_val);
                }
            }
            break;

        case EACI_MSG_DATA_IND_BPS_MEAS_SEND_CFM:
            if (param_len == 1)
            {
                QPRINTF("BLPS Send Measurement status: 0x%02X.\r\n", param[0]);
            }
            break;

        default:
            break;
    }
}
#endif

/**
*************************************************************************************
*** Cycling Speed and Cadence profile
*************************************************************************************
 */
#if BLE_CSC_COLLECTOR
void app_eaci_data_req_cscpc(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    switch (msg_id)
    {
        case EACI_MSG_DATA_REQ_CSCPC_ENABLE:
            if (param_len == BD_ADDR_LEN)
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_REQ, EACI_MSG_DATA_REQ_CSCPC_ENABLE, 8, 0, 0, 0, 0, 0, 0, 0, 0};
                pdu[3] = (uint8_t)(ATT_SVC_CYCLING_SPEED_CADENCE);
                pdu[4] = (uint8_t)(ATT_SVC_CYCLING_SPEED_CADENCE >> 8);
                app_eaci_set_bd_addr_by_param((pdu + 5), param);
                eaci_uart_write(sizeof(pdu), pdu);
            }
            break;

        case EACI_MSG_DATA_REQ_CSCPC_RD_CODE:
            if (param_len == BD_ADDR_LEN + 1)
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_REQ, EACI_MSG_DATA_REQ_CSCPC_RD_CODE, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0};
                pdu[3] = (uint8_t)(ATT_SVC_CYCLING_SPEED_CADENCE);
                pdu[4] = (uint8_t)(ATT_SVC_CYCLING_SPEED_CADENCE >> 8);
                pdu[5] = param[0];
                app_eaci_set_bd_addr_by_param((pdu + 6), param + 1);
                eaci_uart_write(sizeof(pdu), pdu);
            }
            break;

        case EACI_MSG_DATA_REQ_CSCPC_WR_CFG:
            if (param_len == BD_ADDR_LEN + 3)
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_REQ, EACI_MSG_DATA_REQ_CSCPC_WR_CFG, 11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
                pdu[3] = (uint8_t)(ATT_SVC_CYCLING_SPEED_CADENCE);
                pdu[4] = (uint8_t)(ATT_SVC_CYCLING_SPEED_CADENCE >> 8);
                pdu[5] = param[0];
                pdu[6] = param[1];
                pdu[7] = param[2];
                app_eaci_set_bd_addr_by_param((pdu + 8), param + 3);
                eaci_uart_write(sizeof(pdu), pdu);
            }
            break;

        case EACI_MSG_DATA_REQ_CSCPC_SEND_CTNL_PT:
            if (param_len == BD_ADDR_LEN + 5)
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_REQ, EACI_MSG_DATA_REQ_CSCPC_SEND_CTNL_PT, 13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
                pdu[3] = (uint8_t)(ATT_SVC_CYCLING_SPEED_CADENCE);
                pdu[4] = (uint8_t)(ATT_SVC_CYCLING_SPEED_CADENCE >> 8);
                pdu[5] = param[0];
                pdu[6] = param[1];
                pdu[7] = param[2]; 
                pdu[8] = param[3]; 
                pdu[9] = param[4];
                app_eaci_set_bd_addr_by_param((pdu + 10), param + 5);
                eaci_uart_write(sizeof(pdu), pdu);
            }
            break;

        default:
            break;
    }
}
static void app_eaci_data_ind_cscpc(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    switch (msg_id)
    {
        case EACI_MSG_DATA_IND_CSCPC_ENABLE:
            if (param_len == 1)
            {
                QPRINTF("CSCPC enable confirmation status: 0x%02X.\r\n", param[0]);
            }
            break;

        case EACI_MSG_DATA_IND_CSCPC_READ:
            if (param_len == 1)
            {
                QPRINTF("CSCPC Read Request Status: 0x%02X.\r\n", param[0]);
            }
            break;

        case EACI_MSG_DATA_IND_CSCPC_RD_SENSOR_LOC:
            if (param_len == 1)
            {
                QPRINTF("CSCPC Sensor Location: 0x%02X.\r\n", param[0]);
            }
            break;

        case EACI_MSG_DATA_IND_CSCPC_RD_CSC_FEAT:
            if (param_len == 2)
            {
                QPRINTF("CSCPC CSC Feature: 0x%04X.\r\n", param[1] << 8 | param[0]);
            }
            break;

        case EACI_MSG_DATA_IND_CSCPC_READ_CTNL_PT_CFG:
            if (param_len == 2)
            {
                QPRINTF("CSCPC Read SC Control Point CFG.: 0x%04X.\r\n", param[1] << 8 | param[0]);
            }
            break;

        case EACI_MSG_DATA_IND_CSCPC_READ_MEAS_CFG:
            if (param_len == 2)
            {
                QPRINTF("CSCPC Read CSC Measurement CFG.: 0x%04X.\r\n", param[1] << 8 | param[0]);
            }
            break;

        case EACI_MSG_DATA_IND_CSCPC_WR_CFG_STATUS:
            if (param_len == 1)
            {
                QPRINTF("CSCPC Write CFG. Status: 0x%02X.\r\n", param[0]);
            }
            break;

        case EACI_MSG_DATA_IND_CSCPC_CFG_WR:
            if (param_len == 1)
            {
                QPRINTF("CSCPC Write CFG. Status: 0x%02X.\r\n", param[0]);
            }
            break;

        case EACI_MSG_DATA_IND_CSCPC_SC_CTNL_PT:
            if (param_len == 4)
            {
                QPRINTF("CSCPC SC Control Point Requested Operation Code: 0x%02X.\r\n", param[0]);
                QPRINTF("CSCPC SC Control Point Response Value: 0x%02X.\r\n", param[1]);
                QPRINTF("CSCPC SC Control Point List of supported locations: 0x%04X.\r\n", param[3] << 8 | param[2]);
            }
            break;

        case EACI_MSG_DATA_IND_CSCPC_NTF_MEAS:
            if (param_len == 11)
            {
                QPRINTF("CSC Measurement Flags: 0x%02X.\r\n", param[0]);
                QPRINTF("CSC Measurement Cumulative Crank Revolution: 0x%02X.\r\n", param[2] << 8 | param[1]);
                QPRINTF("CSC Measurement Last Crank Event Time: 0x%04X.\r\n", param[4] << 8 | param[3]);
                QPRINTF("CSC Measurement Last Wheel Event Time: 0x%04X.\r\n", param[6] << 8 | param[5]);
                QPRINTF("CSC Measurement Cumulative Wheel Revolution: 0x%08X.\r\n",
                                    param[10] << 24 | param[9] << 16 | param[8] << 8 | param[7]);
            }
            break;

        default:
            break;
    }
}
#endif

#if BLE_CSC_SENSOR
void app_eaci_data_req_cscps(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    switch (msg_id)
    {
        case EACI_MSG_DATA_REQ_CSCPS_NTF_MEAS:
        {
            if (param_len == BD_ADDR_LEN + 9)
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_REQ, EACI_MSG_DATA_REQ_CSCPS_NTF_MEAS, 17, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
                pdu[3] = (uint8_t)(ATT_SVC_CYCLING_SPEED_CADENCE);
                pdu[4] = (uint8_t)(ATT_SVC_CYCLING_SPEED_CADENCE >> 8);
                pdu[5] = param[0];
                pdu[6] = param[1];
                pdu[7] = param[2]; 
                pdu[8] = param[3]; 
                pdu[9] = param[4];
                pdu[10] = param[5];
                pdu[11] = param[6]; 
                pdu[12] = param[7]; 
                pdu[13] = param[8];
                app_eaci_set_bd_addr_by_param((pdu + 14), param + 9);
                eaci_uart_write(sizeof(pdu), pdu);
            }
        }
            break;
        case EACI_MSG_DATA_REQ_CSCPS_CTNL_PT:
        {
            if (param_len == BD_ADDR_LEN + 5)
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_REQ, EACI_MSG_DATA_REQ_CSCPS_CTNL_PT, 13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
                pdu[3] = (uint8_t)(ATT_SVC_CYCLING_SPEED_CADENCE);
                pdu[4] = (uint8_t)(ATT_SVC_CYCLING_SPEED_CADENCE >> 8);
                pdu[5] = param[0];
                pdu[6] = param[1];
                pdu[7] = param[2]; 
                pdu[8] = param[3]; 
                pdu[9] = param[4];
                app_eaci_set_bd_addr_by_param((pdu + 10), param + 5);
                eaci_uart_write(sizeof(pdu), pdu);
            }
        }
            break;
        default:
            break;
    }
}
static void app_eaci_data_ind_cscps(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    switch (msg_id)
    {
        case EACI_MSG_DATA_IND_CSCPS_MEAS:
            if (param_len == 1)
            {
                QPRINTF("CSCPS Send CSC Measurement Status: 0x%02X.\r\n", param[0]);
            }
            break;

        default:
            break;
    }
}
#endif

/**
*************************************************************************************
*** Device Information Service Profile
*************************************************************************************
 */
#if BLE_DIS_CLIENT
uint8_t rd_char = 0;
void app_eaci_data_req_disc(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    switch (msg_id)
    {
        case EACI_MSG_DATA_REQ_DISC_ENABLE:
            if (param_len == BD_ADDR_LEN)
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_REQ, EACI_MSG_DATA_REQ_DISC_ENABLE, 8, 0x0a, 0x18, 0, 0, 0, 0, 0, 0};
                app_eaci_set_bd_addr_by_param((pdu + 5), param);
                eaci_uart_write(sizeof(pdu), pdu);
            }
            break;

        case EACI_MSG_DATA_REQ_DISC_CHAR_RD:
            if (param_len == (BD_ADDR_LEN + 1))
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_REQ, EACI_MSG_DATA_REQ_DISC_CHAR_RD, 9, 0x0a, 0x18, 0, 0, 0, 0, 0, 0, 0};
                pdu[5] = param[0];
                rd_char = param[0];
                app_eaci_set_bd_addr_by_param((pdu + 6), (param + 1));
                eaci_uart_write(sizeof(pdu), pdu);
            }
            break;

        default:
            break;
    }
}
#endif

#if BLE_DIS_SERVER
void app_eaci_data_req_diss(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    switch (msg_id)
    {
        case EACI_MSG_DATA_REQ_DISS_CHAR_WR:
            {
                uint8_t datalen = 0;
                uint8_t *val = NULL;
                switch (param[0])
                {
                    case DIS_MANUFACTURER_NAME_CHAR:
                    {
                        datalen = sizeof(DIS_MANU_NAME_VAL) - 1;
                        val = DIS_MANU_NAME_VAL;
                    }
                        break;
                    case DIS_MODEL_NB_STR_CHAR:
                        datalen = sizeof(DIS_MODEL_NB_VAL) - 1;
                        val = DIS_MODEL_NB_VAL;
                        break;
                    case DIS_SERIAL_NB_STR_CHAR:
                        datalen = sizeof(DIS_SERIAL_NB_VAL) - 1;
                        val = DIS_SERIAL_NB_VAL;
                        break;
                    case DIS_HARD_REV_STR_CHAR:
                        datalen = sizeof(DIS_HW_REV_VAL) - 1;
                        val = DIS_HW_REV_VAL;
                        break;
                    case DIS_FIRM_REV_STR_CHAR:
                        datalen = sizeof(DIS_FW_REV_VAL) - 1;
                        val = DIS_FW_REV_VAL;
                        break;
                    case DIS_SW_REV_STR_CHAR:
                        datalen = sizeof(DIS_SW_REV_VAL) - 1;
                        val = DIS_SW_REV_VAL;
                        break;
                    case DIS_SYSTEM_ID_CHAR:
                        datalen = sizeof(DIS_SYS_ID_VAL) - 1;
                        val = DIS_SYS_ID_VAL;
                        break;
                    case DIS_IEEE_CHAR:
                        datalen = sizeof(DIS_IEEE_CERTIF_VAL) - 1;
                        val = DIS_IEEE_CERTIF_VAL;
                        break;
                    case DIS_PNP_ID_CHAR:
                        datalen = sizeof(DIS_PNP_ID_VAL) - 1;
                        val = DIS_PNP_ID_VAL;
                        break;
                    default:
                        break;
                }
                uint8_t pdu[6 + datalen];
                pdu[0] = EACI_MSG_TYPE_DATA_REQ;
                pdu[1] = EACI_MSG_DATA_REQ_DISS_CHAR_WR;
                pdu[2] = 3 + datalen;
                pdu[3] = 0x0a;
                pdu[4] = 0x18;
                pdu[5] = param[0];
                memcpy((pdu + 6), val, datalen);
                eaci_uart_write(sizeof(pdu), pdu);
            }
            break;

        default:
            break;
    }
}
#endif

#if BLE_DIS_CLIENT
static void app_eaci_data_ind_disc(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    switch (msg_id)
    {
        case EACI_MSG_DATA_IND_DISC_ENABLE:
            if (param_len == 1)
            {
                QPRINTF("Device Information Service enable confirmation status: 0x%02X.\r\n", (uint8_t)param[0]);
            }
            break;

        case EACI_MSG_DATA_IND_DISC_CHAR_RD:
        {
            QPRINTF("Device Information Service Read char status: 0x%02X.\r\n", (uint8_t)param[0]);
            switch(rd_char)
            {
                case 0:
                    QPRINTF("Manufacturer Name:");
                    break;
                case 1:
                    QPRINTF("Model Number:");
                    break;
                case 2:
                    QPRINTF("Serial Number:");
                    break;
                case 3:
                    QPRINTF("Hardware Revision:");
                    break;
                case 4:
                    QPRINTF("Firmware Revision:");
                    break;
                case 5:
                    QPRINTF("TSoftware Revision:");
                    break;
                case 6:
                    QPRINTF("System ID:");
                    break;
                case 7:
                    QPRINTF("IEEE:");
                    break;
                case 8:
                    QPRINTF("PnP ID:");
                    break;
                default:
                    break;
            }
            param += 1;
            for(int i=0;i<(param_len-1);i++,param++)
            {
                QPRINTF(" 0x%02x", (uint8_t)(*param));
            }
            QPRINTF("\r\n");
        }
            break;

        default:
            break;
    }
}
#endif

/**
*************************************************************************************
*** Find Me Profile
*************************************************************************************
 */
#if BLE_FINDME_LOCATOR
void app_eaci_data_req_findl(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    switch (msg_id)
    {
        case EACI_MSG_DATA_REQ_FM_ENABLE:
            if (param_len == BD_ADDR_LEN)
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_REQ, EACI_MSG_DATA_REQ_FM_ENABLE, 8, 0x02, 0x18, 0, 0, 0, 0, 0, 0};
                app_eaci_set_bd_addr_by_param((pdu + 5), param);
                eaci_uart_write(sizeof(pdu), pdu);
            }
            break;

        case EACI_MSG_DATA_REQ_FM_WR_ALERT:
            if (param_len == (BD_ADDR_LEN + 1))
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_REQ, EACI_MSG_DATA_REQ_FM_WR_ALERT, 9, 0x02, 0x18, 0, 0, 0, 0, 0, 0, 0};
                pdu[5] = param[0];
                app_eaci_set_bd_addr_by_param((pdu + 6), (param + 1));
                eaci_uart_write(sizeof(pdu), pdu);
            }
            break;

        default:
            break;
    }
}
#endif

#if BLE_FINDME_LOCATOR
static void app_eaci_data_ind_findl(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    switch (msg_id)
    {
        case EACI_MSG_DATA_IND_FM_ENABLE:
            if (param_len == 1)
            {
                QPRINTF("FINDME enable confirmation status: 0x%02X.\r\n", (uint8_t)param[0]);
            }
            break;

        default:
            break;
    }
}
#endif
#if BLE_FINDME_TARGET
static void app_eaci_data_ind_findt(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    switch (msg_id)
    {
        case EACI_MSG_DATA_IND_ALERT_LEV:
            if (param_len == 1)
            {
                QPRINTF("FIND Me Target Alert Level: 0x%02X.\r\n", (uint8_t)param[0]);
            }
            break;

        default:
            break;
    }
}
#endif

/**
*************************************************************************************
*** Glucose Profile
*************************************************************************************
 */
#if BLE_GL_COLLECTOR
void app_eaci_data_req_glpc(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    switch (msg_id)
    {
        case EACI_MSG_DATA_REQ_GL_ENABLE:
            if (param_len == BD_ADDR_LEN)
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_REQ, EACI_MSG_DATA_REQ_GL_ENABLE, 8, 0x08, 0x18, 0, 0, 0, 0, 0, 0};
                app_eaci_set_bd_addr_by_param((pdu + 5), param);
                eaci_uart_write(sizeof(pdu), pdu);
            }
            break;
        // Register
        case EACI_MSG_DATA_REQ_GL_REGISTER:
            if (param_len == BD_ADDR_LEN)
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_REQ, EACI_MSG_DATA_REQ_GL_REGISTER, 9, 0x08, 0x18, 0, 0, 0, 0, 0, 0, 0};
                pdu[5] = 0x01;
                app_eaci_set_bd_addr_by_param((pdu + 6), param);
                eaci_uart_write(sizeof(pdu), pdu);
            }
            break;

        case EACI_MSG_DATA_REQ_GL_FEATURES_RD:
            if (param_len == BD_ADDR_LEN)
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_REQ, EACI_MSG_DATA_REQ_GL_FEATURES_RD, 8, 0x08, 0x18, 0, 0, 0, 0, 0, 0};
                app_eaci_set_bd_addr_by_param((pdu + 5), param);
                eaci_uart_write(sizeof(pdu), pdu);
            }
            break;
        //operation code - Report Stored Records
        case EACI_MSG_DATA_REQ_GL_RSR_RACP:
            if (param_len == BD_ADDR_LEN + 5)
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_REQ, EACI_MSG_DATA_REQ_GL_RSR_RACP, 13, 0x08, 0x18, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
                pdu[5] = param[0];
                pdu[6] = param[1];
                pdu[7] = param[2];
                pdu[8] = param[3];
                pdu[9] = param[4];
                app_eaci_set_bd_addr_by_param((pdu + 10), param + 5);
                eaci_uart_write(sizeof(pdu), pdu);
            }
            break;
        //operation code - Delete Stored Records
        case EACI_MSG_DATA_REQ_GL_DSR_RACP:
            if (param_len == BD_ADDR_LEN + 5)
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_REQ, EACI_MSG_DATA_REQ_GL_DSR_RACP, 13, 0x08, 0x18, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
                pdu[5] = param[0];
                pdu[6] = param[1];
                pdu[7] = param[2];
                pdu[8] = param[3];
                pdu[9] = param[4];
                app_eaci_set_bd_addr_by_param((pdu + 10), param + 5);
                eaci_uart_write(sizeof(pdu), pdu);
            }
            break;
        //operation code - Abort Operation
        case EACI_MSG_DATA_REQ_GL_AO_RACP:
            if (param_len == BD_ADDR_LEN + 5)
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_REQ, EACI_MSG_DATA_REQ_GL_AO_RACP, 13, 0x08, 0x18, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
                pdu[5] = param[0];
                pdu[6] = param[1];
                pdu[7] = param[2];
                pdu[8] = param[3];
                pdu[9] = param[4];
                app_eaci_set_bd_addr_by_param((pdu + 10), param + 5);
                eaci_uart_write(sizeof(pdu), pdu);
            }
            break;
        //operation code - Report Number of Stored Records
        case EACI_MSG_DATA_REQ_GL_RN_RACP:
            if (param_len == BD_ADDR_LEN + 5)
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_REQ, EACI_MSG_DATA_REQ_GL_RN_RACP, 13, 0x08, 0x18, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
                pdu[5] = param[0];
                pdu[6] = param[1];
                pdu[7] = param[2];
                pdu[8] = param[3];
                pdu[9] = param[4];
                app_eaci_set_bd_addr_by_param((pdu + 10), param + 5);
                eaci_uart_write(sizeof(pdu), pdu);
            }
            break;

        default:
            break;
    }
}
#endif

#if BLE_GL_COLLECTOR
static void app_eaci_data_ind_glpc(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    switch (msg_id)
    {
        case EACI_MSG_DATA_IND_GL_ENABLE:
            if (param_len == 1)
            {
                QPRINTF("Glucose enable confirmation status: 0x%02X.\r\n", (uint8_t)param[0]);
            }
            break;

        case EACI_MSG_DATA_IND_GL_REGISTER:
            if (param_len == 1)
            {
                QPRINTF("GLPC register confirmation status: 0x%02X.\r\n", (uint8_t)param[0]);
            }
            break;

        case EACI_MSG_DATA_IND_GL_FEATURES_RD:
            if (param_len == 3)
            {
                QPRINTF("GLPC read features response status: 0x%02X.\r\n", (uint8_t)param[0]);
                uint16_t rd_features = param[2] << 8 | param[1];
                QPRINTF("GLPC Feature : 0x%04X.\r\n", rd_features);
            }
            break;

        case EACI_MSG_DATA_IND_GL_MEAS:
            {
                uint8_t cursor = 10;
                uint8_t Flags = 0;

                Flags = param[0];
                QPRINTF("Flags: %02d.\r\n ", param[0]);
                QPRINTF("Glucose measurement value seq_num: 0x%04X.\r\n ", (uint16_t)(param[2] << 8 | param[1]));
                QPRINTF("Year: %04d. ", (uint16_t)(param[4] << 8 | param[3]));
                QPRINTF("Month: %02d. ", (param[5]));
                QPRINTF("Day: %02d. ", (param[6]));
                QPRINTF("Hour: %02d. ", (param[7]));
                QPRINTF("Min: %02d. ", (param[8]));
                QPRINTF("Sec: %02d.\r\n ", (param[9]));

                //Time Offset
                if ((Flags & GLP_MEAS_TIME_OFF_PRES) != 0)
                {
                    QPRINTF("Time_offset: 0x%04X.\r\n ", (uint16_t)(param[cursor + 1] << 8 | param[cursor]));
                    cursor += 2;
                }

                // Glucose Concentration, type and location
                if ((Flags & GLP_MEAS_GL_CTR_TYPE_AND_SPL_LOC_PRES) != 0)
                {
                    QPRINTF("Concentration: 0x%04X.\r\n ", (uint16_t)(param[cursor + 1] << 8 | param[cursor]));
                    QPRINTF("Type: %02d.\r\n ", (param[cursor + 2]));
                    QPRINTF("Location: %02d.\r\n ", (param[cursor + 3]));
                    cursor += 4;
                }

                // Sensor Status Annunciation
                if((Flags & GLP_MEAS_SENS_STAT_ANNUN_PRES) != 0)
                {
                    QPRINTF("Status: 0x%04X.\r\n ", (uint16_t)(param[cursor + 1] << 8 | param[cursor]));
                    cursor += 2;
                }
            }
            break;

        case EACI_MSG_DATA_IND_GL_MEAS_CTX:
            {
                uint8_t flags = 0;
                uint8_t cursor = 3;
                
                flags = param[0];
                QPRINTF("Flags: 0x%02X.\r\n ", param[0]);
                QPRINTF("Glucose measurement context seq_num: 0x%04X.\r\n ", (uint16_t)(param[2] << 8 | param[1]));
                
                // Extended Flags
                if((flags & GLP_CTX_EXTD_F_PRES) != 0)
                {
                    QPRINTF("Extended Flags Present: 0x%02X.\r\n ", param[cursor]);
                    cursor += 1;
                }

                // Carbohydrate ID And Carbohydrate Present
                if((flags & GLP_CTX_CRBH_ID_AND_CRBH_PRES) != 0)
                {
                    QPRINTF("Carbohydrate ID: 0x%02X.\r\n ", param[cursor]);
                    QPRINTF("Carbohydrate: 0x%04X.\r\n ", (uint16_t)(param[cursor + 2] << 8 | param[cursor + 1]));
                    cursor += 3;
                }

                // Meal Present
                if((flags & GLP_CTX_MEAL_PRES) != 0)
                {
                    QPRINTF("Meal: 0x%02X.\r\n ", param[cursor]);
                    cursor += 1;
                }

                // Tester-Health Present
                if((flags & GLP_CTX_TESTER_HEALTH_PRES) != 0)
                {
                    QPRINTF("Tester: 0x%02X.\r\n ", param[cursor]);
                    QPRINTF("Health: 0x%02X.\r\n ", param[cursor + 1]);
                    cursor += 2;
                }

                // Exercise Duration & Exercise Intensity Present
                if((flags & GLP_CTX_EXE_DUR_AND_EXE_INTENS_PRES) != 0)
                {
                    QPRINTF("Exercise Duration: 0x%04X.\r\n ", (uint16_t)(param[cursor + 1] << 8 | param[cursor]));
                    QPRINTF("Exercise Intensity: 0x%02X.\r\n ", (uint8_t)(param[cursor + 2]));
                    cursor += 3;
                }

                // Medication ID And Medication Present
                if((flags & GLP_CTX_MEDIC_ID_AND_MEDIC_PRES) != 0)
                {
                    QPRINTF("Medication ID: 0x%02X.\r\n ", param[cursor]);
                    QPRINTF("Medication value: 0x%04X.\r\n ", (uint16_t)(param[cursor + 2] << 8 | param[cursor + 1]));
                    cursor += 3;
                }

                // HbA1c Present
                if((flags & GLP_CTX_HBA1C_PRES) != 0)
                {
                    QPRINTF("HbA1c Present: 0x%04X.\r\n ", (uint16_t)(param[cursor + 1] << 8 | param[cursor]));
                    cursor += 2;
                }
            }
            break;

        case EACI_MSG_DATA_IND_GL_RACP_RSP:
            if (param_len == 3)
            {
                uint8_t op_code = 0;
                
                op_code = param[0];
                switch (op_code)
                {
                    case GLP_REQ_NUM_OF_STRD_RECS_RSP:
                    {
                        QPRINTF("Number of stored records is : 0x%04X.\r\n", param[2] << 8 | param[1]);
                    }
                        break;
                    case GLP_REQ_RSP_CODE:
                    {
                        switch (param[1])
                        {
                            case GLP_REQ_REP_STRD_RECS:
                                QPRINTF("RACP Report stored records finished.\r\n");
                                break;
                            case GLP_REQ_ABORT_OP:
                                QPRINTF("RACP Abort operation finished.\r\n");
                                break;
                            default:
                                break;
                        }
                    }
                        break;
                }
            }
            break;

        default:
            break;
    }
}
#endif
#if BLE_GL_SENSOR
uint8_t seq_num = 1;
void app_eaci_data_req_glps(uint8_t msg_id)
{
    switch (msg_id)
    {
        case EACI_MSG_DATA_REQ_GL_SEND_MEAS_WITHOUT_CTX:
            {
                uint8_t cursor = 15;
                uint8_t Flags = 0x0F;

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

                pdu[0] = EACI_MSG_TYPE_DATA_REQ;
                pdu[1] = EACI_MSG_DATA_REQ_GL_SEND_MEAS_WITHOUT_CTX;
                pdu[2] = cursor - 3;
                pdu[3] = (uint8_t)(ATT_SVC_GLUCOSE);
                pdu[4] = (uint8_t)(ATT_SVC_GLUCOSE >> 8);
                pdu[5] = Flags; //Flags
                pdu[6] = seq_num;
                seq_num ++;
                pdu[7] = 0x00; // Sequence Number
                pdu[8] = 0xDD;
                pdu[9] = 0x07; // Year 2013
                pdu[10] = 0x0A; // Month
                pdu[11] = 0x06; // Day
                pdu[12] = 0x05; // Hour
                pdu[13] = 0x04; // Min
                pdu[14] = 0x03; // Sec
                cursor = 15;

                //Time Offset
                if ((Flags & GLP_MEAS_TIME_OFF_PRES) != 0)
                {
                    pdu[cursor] = 0x00;
                    pdu[cursor + 1] = 0x06;
                    cursor += 2;
                }

                // Glucose Concentration, type and location
                if ((Flags & GLP_MEAS_GL_CTR_TYPE_AND_SPL_LOC_PRES) != 0)
                {
                    pdu[cursor] = 0x66;
                    pdu[cursor + 1] = 0x55; // Concentration
                    pdu[cursor + 2] = GLP_TYPE_CAPILLARY_WHOLE_BLOOD; // Type
                    pdu[cursor + 3] = GLP_LOC_FINGER; // Sample Location
                    cursor += 4;
                }

                // Sensor Status Annunciation
                if((Flags & GLP_MEAS_SENS_STAT_ANNUN_PRES) != 0)
                {
                    pdu[cursor] = GLP_MEAS_STATE_DEV_BAT_LOW;
                    pdu[cursor + 1] = (uint8_t)(GLP_MEAS_STATE_GEN_DEV_FLT >> 8); 
                    cursor += 2;
                }

                eaci_uart_write(cursor, pdu);
            }
            break;

        case EACI_MSG_DATA_REQ_GL_SEND_MEAS_WITH_CTX:
            {
                uint8_t cursor = 15;
                uint8_t Flags = 0x0F;
                uint8_t Flags_ctx = 0xFF;

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

                
                // Extended Flags
                if((Flags_ctx & GLP_CTX_EXTD_F_PRES) != 0)
                {
                    cursor += 1;
                }

                // Carbohydrate ID And Carbohydrate Present
                if((Flags_ctx & GLP_CTX_CRBH_ID_AND_CRBH_PRES) != 0)
                {
                    cursor += 3;
                }

                // Meal Present
                if((Flags_ctx & GLP_CTX_MEAL_PRES) != 0)
                {
                    cursor += 1;
                }

                // Tester-Health Present
                if((Flags_ctx & GLP_CTX_TESTER_HEALTH_PRES) != 0)
                {
                    cursor += 1;
                }

                // Exercise Duration & Exercise Intensity Present
                if((Flags_ctx & GLP_CTX_EXE_DUR_AND_EXE_INTENS_PRES) != 0)
                {
                    cursor += 3;
                }

                // Medication ID And Medication Present
                if((Flags_ctx & GLP_CTX_MEDIC_ID_AND_MEDIC_PRES) != 0)
                {
                    cursor += 3;
                }

                // HbA1c Present
                if((Flags_ctx & GLP_CTX_HBA1C_PRES) != 0)
                {
                    cursor += 2;
                }

                uint8_t pdu[cursor];

                pdu[0] = EACI_MSG_TYPE_DATA_REQ;
                pdu[1] = EACI_MSG_DATA_REQ_GL_SEND_MEAS_WITH_CTX;
                pdu[2] = cursor - 3;
                pdu[3] = (uint8_t)(ATT_SVC_GLUCOSE);
                pdu[4] = (uint8_t)(ATT_SVC_GLUCOSE >> 8);
                pdu[5] = Flags; //Flags
                pdu[6] = 0x01;
                pdu[7] = 0x00; // Sequence Number
                pdu[8] = 0xDD;
                pdu[9] = 0x07; // Year 2013
                pdu[10] = 0x0A; // Month
                pdu[11] = 0x06; // Day
                pdu[12] = 0x05; // Hour
                pdu[13] = 0x04; // Min
                pdu[14] = 0x03; // Sec
                cursor = 15;

                //Time Offset
                if ((Flags & GLP_MEAS_TIME_OFF_PRES) != 0)
                {
                    pdu[cursor] = 0x00;
                    pdu[cursor + 1] = 0x06;
                    cursor += 2;
                }

                // Glucose Concentration, type and location
                if ((Flags & GLP_MEAS_GL_CTR_TYPE_AND_SPL_LOC_PRES) != 0)
                {
                    pdu[cursor] = 0x66;
                    pdu[cursor + 1] = 0x55; // Concentration
                    pdu[cursor + 2] = GLP_TYPE_CAPILLARY_WHOLE_BLOOD; // Type
                    pdu[cursor + 3] = GLP_LOC_FINGER; // Sample Location
                    cursor += 4;
                }

                // Sensor Status Annunciation
                if((Flags & GLP_MEAS_SENS_STAT_ANNUN_PRES) != 0)
                {
                    pdu[cursor] = GLP_MEAS_STATE_DEV_BAT_LOW;
                    pdu[cursor + 1] = (uint8_t)(GLP_MEAS_STATE_GEN_DEV_FLT >> 8); 
                    cursor += 2;
                }

                pdu[cursor] = Flags_ctx;
                // Extended Flags
                if((Flags_ctx & GLP_CTX_EXTD_F_PRES) != 0)
                {
                    pdu[cursor + 1] = 0x00;
                    cursor += 2;
                }

                // Carbohydrate ID And Carbohydrate Present
                if((Flags_ctx & GLP_CTX_CRBH_ID_AND_CRBH_PRES) != 0)
                {
                    pdu[cursor + 1] = GLP_CID_BREAKFAST;
                    pdu[cursor + 2] = 0x00;
                    pdu[cursor + 3] = 0x00;
                    cursor += 4;
                }

                // Meal Present
                if((Flags_ctx & GLP_CTX_MEAL_PRES) != 0)
                {
                    pdu[cursor + 1] = GLP_MEAL_FASTING;
                    cursor += 2;
                }

                // Tester-Health Present
                if((Flags_ctx & GLP_CTX_TESTER_HEALTH_PRES) != 0)
                {
                    pdu[cursor + 1] = GLP_TESTER_SELF;
                    pdu[cursor + 2] = GLP_HEALTH_DUR_MENSES;
                    cursor += 3;
                }

                // Exercise Duration & Exercise Intensity Present
                if((Flags_ctx & GLP_CTX_EXE_DUR_AND_EXE_INTENS_PRES) != 0)
                {
                    pdu[cursor + 1] = 0x21;
                    pdu[cursor + 2] = 0x12;
                    pdu[cursor + 3] = 0x00;
                    cursor += 4;
                }

                // Medication ID And Medication Present
                if((Flags_ctx & GLP_CTX_MEDIC_ID_AND_MEDIC_PRES) != 0)
                {
                    pdu[cursor + 1] = GLP_MEDID_SHORT_ACTING_INSULIN;
                    pdu[cursor + 2] = 0x00;
                    pdu[cursor + 3] = 0x00;
                    cursor += 4;
                }

                // HbA1c Present
                if((Flags_ctx & GLP_CTX_HBA1C_PRES) != 0)
                {
                    pdu[cursor + 1] = 0x00;
                    pdu[cursor + 2] = 0x00;
                    cursor += 3;
                }
                
                eaci_uart_write(cursor, pdu);
            }
            break;

        default:
            break;
    }
}
#endif
#if BLE_GL_SENSOR
static void app_eaci_data_ind_glps(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    switch (msg_id)
    {
        case EACI_MSG_DATA_IND_GLS_CFG:
            if (param_len == 1)
            {
                QPRINTF("GLPC indication/notification configuration: 0x%X.\r\n", param[0]);
            }
            break;

        default:
            break;
    }
}
#endif

/**
*************************************************************************************
*** Heart Rate Profile
*************************************************************************************
 */
#if BLE_HR_COLLECTOR
/**
 ****************************************************************************************
 * @brief HRPC data request API
 *
 ****************************************************************************************
 */
void app_eaci_data_req_hrpc(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{  
    switch (msg_id)
    {
        case EACI_MSG_DATA_REQ_HRPC_ENABLE:
            if (param_len == BD_ADDR_LEN)
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_REQ, EACI_MSG_DATA_REQ_HRPC_ENABLE, 8, 0x0D, 0x18, 0, 0, 0, 0, 0, 0};
                app_eaci_set_bd_addr_by_param(pdu+5, param);
                eaci_uart_write(sizeof(pdu), pdu);
            }
            break;
        
        case EACI_MSG_DATA_READ_HRPC_BODY_SENSOR:
            if (param_len == BD_ADDR_LEN)
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_REQ, EACI_MSG_DATA_READ_HRPC_BODY_SENSOR, 8, 0x0D, 0x18, 0, 0, 0, 0, 0, 0};
                app_eaci_set_bd_addr_by_param(pdu+5, param);
                eaci_uart_write(sizeof(pdu), pdu);
            }
            break;

        case EACI_MSG_DATA_READ_HRPC_CFG_INDNTF:
            if (param_len == (BD_ADDR_LEN + 1))
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_REQ, EACI_MSG_DATA_READ_HRPC_CFG_INDNTF, 10, 0x0D, 0x18, 0, 0, 0, 0, 0, 0, 0, 0};
                pdu[5] = param[0];
                pdu[6] = 0;
                app_eaci_set_bd_addr_by_param((pdu + 7), (param + 1));
                eaci_uart_write(sizeof(pdu), pdu);
            }
            break;
    
        default:
            break;
    }
}
#endif

#if BLE_HR_SENSOR
/**
 ****************************************************************************************
 * @brief HRPS data request API
 *
 ****************************************************************************************
 */
void app_eaci_data_req_hrps(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{  
    switch (msg_id)
    {
        case EACI_MSG_DATA_REQ_HRPS_MEAS_VALUE:
            if (param_len == 2)
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_REQ, EACI_MSG_DATA_REQ_HRPS_MEAS_VALUE, 4, 0x0D, 0x18, 0, 0};
                pdu[5] = param[0];
                pdu[6] = param[1];
                eaci_uart_write(sizeof(pdu), pdu);
            }
            break;
    
        default:
            break;
    }
}
#endif

#if BLE_HR_COLLECTOR
static void app_eaci_data_ind_hrpc(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    switch (msg_id)
    {
        case EACI_MSG_DATA_IND_HRPC_ENABLE:
            if (param_len == 1)
            {
                QPRINTF("HRPC enable confirmation status: 0x%02X.\r\n", param[0]);
            }
            break;

        case EACI_MSG_DATA_IND_HRPC_READ_BSL_RSP:
            if (param_len == 2)
            {
                QPRINTF("Read status: 0x%02X.\r\n", param[0]);
                QPRINTF("Body Sensor Location is: 0x%02X.\r\n", param[1]);
            }
            break;

        case EACI_MSG_DATA_IND_HRPC_WR_RSP:
            if (param_len == 1)
            {
                QPRINTF("Write RSP.: 0x%02X.\r\n", param[0]);
            }
            break;

        case EACI_MSG_DATA_IND_HRPC_MEAS:
            if (param_len == 2)
            {
                uint16_t hr_val = param[1] << 8 | param[0];
                QPRINTF("Heart Rate value: 0x%04X.\r\n", hr_val);
            }
            break;

        default:
            break;
    }
}
#endif

#if BLE_HR_SENSOR
static void app_eaci_data_ind_hrps(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    switch (msg_id)
    {
        case EACI_MSG_DATA_IND_HRPS_CFG:
            if (param_len == 2)
            {
                uint16_t cfg_val = param[1] << 8 | param[0];
                QPRINTF("HRPS confirmation status: 0x%04X.\r\n", cfg_val);
            }
            break;

        default:
            break;
    }
}
#endif
 
/**
*************************************************************************************
*** Health Thermometer Profile
*************************************************************************************
 */
#if BLE_HT_COLLECTOR
void app_eaci_data_req_htpc(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    switch (msg_id)
    {
        case EACI_MSG_DATA_REQ_HT_ENABLE:
            if (param_len == BD_ADDR_LEN)
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_REQ, EACI_MSG_DATA_REQ_HT_ENABLE, 8, 0x09, 0x18, 0, 0, 0, 0, 0, 0};
                app_eaci_set_bd_addr_by_param((pdu + 5), param);
                eaci_uart_write(sizeof(pdu), pdu);
            }
            break;

        case EACI_MSG_DATA_REQ_HT_TEMP_MEAS:
            if (param_len == (BD_ADDR_LEN + 1))
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_REQ, EACI_MSG_DATA_REQ_HT_TEMP_MEAS, 10, 0x09, 0x18, 0, 0, 0, 0, 0, 0, 0, 0};
                pdu[5] = param[0];
                pdu[6] = 0x00;
                app_eaci_set_bd_addr_by_param((pdu + 7), (param + 1));
                eaci_uart_write(sizeof(pdu), pdu);
            }
            break;

        case EACI_MSG_DATA_REQ_HT_INTM_TEMP:
            if (param_len == (BD_ADDR_LEN + 1))
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_REQ, EACI_MSG_DATA_REQ_HT_INTM_TEMP, 10, 0x09, 0x18, 0, 0, 0, 0, 0, 0, 0, 0};
                pdu[5] = param[0];
                pdu[6] = 0x00;
                app_eaci_set_bd_addr_by_param((pdu + 7), (param + 1));
                eaci_uart_write(sizeof(pdu), pdu);
            }
            break;

        case EACI_MSG_DATA_REQ_HT_MEAS_INTV:
            if (param_len == (BD_ADDR_LEN + 1))
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_REQ, EACI_MSG_DATA_REQ_HT_MEAS_INTV, 10, 0x09, 0x18, 0, 0, 0, 0, 0, 0, 0, 0};
                pdu[5] = param[0];
                pdu[6] = 0x00;
                app_eaci_set_bd_addr_by_param((pdu + 7), (param + 1));
                eaci_uart_write(sizeof(pdu), pdu);
            }
            break;

        case EACI_MSG_DATA_REQ_HT_RD_MEAS_INTV:
            if (param_len == BD_ADDR_LEN)
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_REQ, EACI_MSG_DATA_REQ_HT_RD_MEAS_INTV, 8, 0x09, 0x18, 0, 0, 0, 0, 0, 0};
                app_eaci_set_bd_addr_by_param((pdu + 5), param);
                eaci_uart_write(sizeof(pdu), pdu);
            }
            break;

        default:
            break;
    }
}
#endif
#if BLE_HT_THERMOM
void app_eaci_data_req_htpt(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    switch (msg_id)
    {
        case EACI_MSG_DATA_REQ_HT_SEND_INTER_VALUE:
            if (param_len == 2)
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_REQ, EACI_MSG_DATA_REQ_HT_SEND_INTER_VALUE, 4, 0x09, 0x18, 0, 0};
                pdu[5] = param[0];
                pdu[6] = param[1];
                eaci_uart_write(sizeof(pdu), pdu);
            }
            break;

        case EACI_MSG_DATA_REQ_HT_SEND_TEMP_VALUE:
            if (param_len == 2)
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_REQ, EACI_MSG_DATA_REQ_HT_SEND_TEMP_VALUE, 0x0F, 0x09, 0x18, 
                                 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
                pdu[5] = param[0]; //Stable(0x01) intermediary(0x00)
                pdu[6] = param[1]; //Flags 0x06(celsius) 0x07(fahrenheit)
                pdu[7] = 0x03; // Type (Ear)
                pdu[8] = 0x01;
                pdu[9] = 0x02;
                pdu[10] = 0x03;
                pdu[11] = 0x04; //Temperature Measurement Value
                pdu[12] = 0xDD; 
                pdu[13] = 0x07; //Year
                pdu[14] = 0x07; //Month
                pdu[15] = 0x0a; //Day
                pdu[16] = 0x06; //Hour
                pdu[17] = 0x06; //Min
                pdu[18] = 0x06; //Sec
                eaci_uart_write(sizeof(pdu), pdu);
            }
            break;

        default:
            break;
    }
}
#endif

#if BLE_HT_COLLECTOR
static void app_eaci_data_ind_htpc(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    switch (msg_id)
    {
        case EACI_MSG_DATA_IND_HT_ENABLE:
            if (param_len == 1)
            {
                QPRINTF("Health Thermometer enable confirmation status: 0x%02X.\r\n", (uint8_t)param[0]);
            }
            break;

        case EACI_MSG_DATA_IND_HT_RD_CHAR_RSP:
            {
                QPRINTF("Read Responses status: 0x%02X.\r\n", (uint8_t)param[0]);
                uint16_t intv = param[2] << 8 | param[1];
                QPRINTF("Read HTS Measurement Interval: 0x%04X.\r\n", intv);
            }
            break;

        case EACI_MSG_DATA_IND_HT_WR_RSP:
            if (param_len == 1)
            {
                QPRINTF("Write status: 0x%02X.\r\n", param[0]);
            }
            break;

        case EACI_MSG_DATA_IND_HT_TEM_MEA:
            if (param_len == 6)
            {
                uint8_t flag_stable_meas = param[0];
                if (flag_stable_meas)
                {
                    QPRINTF("Stable Blood Pressure Measurement value: 0x%02X.\r\n", (uint8_t)param[0]);
                }
                else
                {
                    QPRINTF("Intermediate Cuff Pressure Measurement value: 0x%02X.\r\n", (uint8_t)param[0]);
                }
                uint8_t flags = param[1];
                if (flags)
                {
                    QPRINTF("Temperature Measurement Value (Fahrenheit): 0x%02X.\r\n", (uint8_t)param[1]);
                }
                else
                {
                    QPRINTF("Temperature Measurement Value (Celsius): 0x%02X.\r\n", (uint8_t)param[1]);
                }
                uint32_t temp = param[5] << 24 | param[4] << 16 | param[3] << 8 | param[2];
                QPRINTF("Temperature Measurement Value: 0x%08X.\r\n", temp);
            }
            break;

        case EACI_MSG_DATA_IND_HT_MEAS_INTE:
            if (param_len == 2)
            {
                uint16_t temp = param[1] << 8 | param[0];
                QPRINTF("measurement interval value indication interval: 0x%02X.\r\n", temp);
            }
            break;

        default:
            break;
    }
}
#endif
#if BLE_HT_THERMOM
static void app_eaci_data_ind_htpt(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    switch (msg_id)
    {        
        case EACI_MSG_DATA_IND_HT_TH_TEM_MEA:
            if (param_len == 2)
            {
                uint16_t inter_ind = param[1] << 8 | param[0];
                QPRINTF("Measurement interval indication: 0x%02X.\r\n", inter_ind);
            }
            break;

        case EACI_MSG_DATA_IND_HT_TH_CFG_IND:
            if (param_len == 2)
            {
                uint16_t cfg_val = param[1] << 8 | param[0];
                QPRINTF("CFG> value: 0x%02X.\r\n", cfg_val);
            }
            break;

        default:
            break;
    }
}
#endif

/**
*************************************************************************************
*** Phone Alert Status Profile
*************************************************************************************
 */
#if BLE_PAS_CLIENT
void app_eaci_data_req_paspc(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    switch (msg_id)
    {
        case EACI_MSG_DATA_REQ_PASPC_ENABLE:
        {
            if (param_len == BD_ADDR_LEN)
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_REQ, EACI_MSG_DATA_REQ_PASPC_ENABLE, 8, 0, 0, 0, 0, 0, 0, 0, 0};
                pdu[3] = (uint8_t)(ATT_SVC_PHONE_ALERT_STATUS);
                pdu[4] = (uint8_t)(ATT_SVC_PHONE_ALERT_STATUS >> 8);
                app_eaci_set_bd_addr_by_param((pdu + 5), param);
                eaci_uart_write(sizeof(pdu), pdu);
            }
        }
            break;

        case EACI_MSG_DATA_REQ_PASPC_RD:
        {
            if (param_len == BD_ADDR_LEN + 1)
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_REQ, EACI_MSG_DATA_REQ_PASPC_RD, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0};
                pdu[3] = (uint8_t)(ATT_SVC_PHONE_ALERT_STATUS);
                pdu[4] = (uint8_t)(ATT_SVC_PHONE_ALERT_STATUS >> 8);
                pdu[5] = param[0];
                app_eaci_set_bd_addr_by_param((pdu + 6), (param + 1));
                eaci_uart_write(sizeof(pdu), pdu);
            }
        }
            break;

        case EACI_MSG_DATA_REQ_PASPC_WR:
        {
            if (param_len == BD_ADDR_LEN + 3)
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_REQ, EACI_MSG_DATA_REQ_PASPC_WR, 11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
                pdu[3] = (uint8_t)(ATT_SVC_PHONE_ALERT_STATUS);
                pdu[4] = (uint8_t)(ATT_SVC_PHONE_ALERT_STATUS >> 8);
                pdu[5] = param[0];
                pdu[6] = param[1];
                pdu[7] = param[2];
                app_eaci_set_bd_addr_by_param((pdu + 8), (param + 3));
                eaci_uart_write(sizeof(pdu), pdu);
            }
        }
            break;

        default:
            break;
    }
}

static void app_eaci_data_ind_paspc(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    switch (msg_id)
    {
        case EACI_MSG_DATA_IND_PASPC_ENABLE:
            if (param_len == 1)
            {
                QPRINTF("PASPC enable confirmation status: 0x%02X.\r\n", param[0]);
            }
            break;

        case EACI_MSG_DATA_IND_PASPC_READ:
            if (param_len == 1)
            {
                QPRINTF("PASPC read status: 0x%02X.\r\n", param[0]);
            }
            break;

        case EACI_MSG_DATA_IND_PASPC_WRITE:
            if (param_len == 1)
            {
                QPRINTF("PASPC write status: 0x%02X.\r\n", param[0]);
            }
            break;

        case EACI_MSG_DATA_IND_PASPC_ALERT_STATUS:
            if (param_len == 1)
            {
                QPRINTF("PASPC Alert Status: 0x%02X.\r\n", param[0]);
            }
            break;

        case EACI_MSG_DATA_IND_PASPC_RINGER_SETTING:
            if (param_len == 2)
            {
                QPRINTF("PASPC Ringer Setting: 0x%02X.\r\n", param[1] << 8 | param[0]);
            }
            break;

        case EACI_MSG_DATA_IND_PASPC_ALERT_STATUS_CFG:
            if (param_len == 2)
            {
                QPRINTF("PASPC Alert Status CFG.: 0x%04X.\r\n", param[1] << 8 | param[0]);
            }
            break;

        case EACI_MSG_DATA_IND_PASPC_RINGER_SETTING_CFG:
            if (param_len == 2)
            {
                QPRINTF("PASPC Ringer Setting CFG.: 0x%04X.\r\n", param[1] << 8 | param[0]);
            }
            break;

        default:
            break;
    }
}
#endif

#if BLE_PAS_SERVER
void app_eaci_data_req_pasps(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    switch (msg_id)
    {
        case EACI_MSG_DATA_REQ_PASPS_UPDATE_VAL:
        {
            if (param_len == BD_ADDR_LEN + 2)
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_REQ, EACI_MSG_DATA_REQ_PASPS_UPDATE_VAL, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0};
                pdu[3] = (uint8_t)(ATT_SVC_PHONE_ALERT_STATUS);
                pdu[4] = (uint8_t)(ATT_SVC_PHONE_ALERT_STATUS >> 8);
                pdu[5] = param[0];
                pdu[6] = param[1];
                app_eaci_set_bd_addr_by_param((pdu + 7), (param + 2));
                eaci_uart_write(sizeof(pdu), pdu);
            }
        }
            break;

        default:
            break;
    }
}

static void app_eaci_data_ind_pasps(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    switch (msg_id)
    {
        case EACI_MSG_DATA_IND_PASPC_UPD_ALERT_STATUS:
            if (param_len == 1)
            {
                QPRINTF("PASPS Update Alert Status Char. value status: 0x%02X.\r\n", param[0]);
            }
            break;

        case EACI_MSG_DATA_IND_PASPC_UPD_RINGER_SETTING:
            if (param_len == 1)
            {
                QPRINTF("PASPS Update Ringer Setting Char. value status: 0x%02X.\r\n", param[0]);
            }
            break;

        default:
            break;
    }
}
#endif

/**
*************************************************************************************
*** Proximity Profile
*************************************************************************************
 */

#if BLE_PROX_MONITOR
/**
 ****************************************************************************************
 * @brief PROXM data request API
 *
 ****************************************************************************************
 */
void app_eaci_data_req_proxm(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    switch (msg_id)
    {
        case EACI_MSG_DATA_REQ_PROXM_ENABLE:
            if (param_len == BD_ADDR_LEN)
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_REQ, EACI_MSG_DATA_REQ_PROXM_ENABLE, 8, 0x03, 0x18, 0, 0, 0, 0, 0, 0};
                app_eaci_set_bd_addr_by_param((pdu + 5), param);
                eaci_uart_write(sizeof(pdu), pdu);
            }
            break;

        case EACI_MSG_DATA_READ_PROXM_TX_POWER:
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_REQ, EACI_MSG_DATA_READ_PROXM_TX_POWER, 8, 0x04, 0x18, 0, 0, 0, 0, 0, 0};
                app_eaci_set_bd_addr_by_param((pdu + 5), param);
                eaci_uart_write(sizeof(pdu), pdu);
            }
            break;

        case EACI_MSG_DATA_WRITE_PROXM_IAS:
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_REQ, EACI_MSG_DATA_WRITE_PROXM_IAS, 9, 0x03, 0x18, 0, 0, 0, 0, 0, 0, 0};
                pdu[5] = param[0];
                app_eaci_set_bd_addr_by_param((pdu + 6), (param + 1));
                eaci_uart_write(sizeof(pdu), pdu);
            }
            break;

        case EACI_MSG_DATA_WRITE_PROXM_LLS:
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_REQ, EACI_MSG_DATA_WRITE_PROXM_LLS, 9, 0x03, 0x18, 0, 0, 0, 0, 0, 0, 0};
                pdu[5] = param[0];
                app_eaci_set_bd_addr_by_param((pdu + 6), (param + 1));
                eaci_uart_write(sizeof(pdu), pdu);
            }
            break;

        default:
            break;
    }
}
#endif

#if BLE_PROX_REPORTER
#endif

#if BLE_PROX_MONITOR
static void app_eaci_data_ind_proxm(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    switch (msg_id)
    {
        case EACI_MSG_DATA_IND_PROXM_ENABLE:
            if (param_len == 1)
            {
                QPRINTF("PROXM enable confirmation status: 0x%02X.\r\n", (uint8_t)param[0]);
            }
            break;

        case EACI_MSG_DATA_IND_PROXM_TX_POWER:
            if (param_len == 1)
            {
                QPRINTF("TX Power Level: 0x%02X.\r\n", (uint8_t)param[0]);
            }
            break;

        default:
            break;
    }
}
#endif

#if BLE_PROX_REPORTER
static void app_eaci_data_ind_proxr(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    switch (msg_id)
    {
        case EACI_MSG_DATA_IND_PROXR_ALERT:
            if (param_len == 1)
            {
                QPRINTF("RPOXR IAS Alert Level: 0x%02X.\r\n", (uint8_t)param[0]);
            }
            break;

        case EACI_MSG_DATA_IND_PROXR_LINK_LOST:
            if (param_len == 1)
            {
                QPRINTF("RPOXR LLS Alert Level: 0x%02X.\r\n", (uint8_t)param[0]);
            }
            break;

        default:
            break;
    }
}
#endif

/**
*************************************************************************************
*** Running Speed and Cadence profile
*************************************************************************************
 */
#if BLE_RSC_COLLECTOR
void app_eaci_data_req_rscpc(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    switch (msg_id)
    {
        case EACI_MSG_DATA_REQ_RSCPC_ENABLE:
        {
            if (param_len == BD_ADDR_LEN)
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_REQ, EACI_MSG_DATA_REQ_RSCPC_ENABLE, 8, 0, 0, 0, 0, 0, 0, 0, 0};
                pdu[3] = (uint8_t)(ATT_SVC_RUNNING_SPEED_CADENCE);
                pdu[4] = (uint8_t)(ATT_SVC_RUNNING_SPEED_CADENCE >> 8);
                app_eaci_set_bd_addr_by_param((pdu + 5), param);
                eaci_uart_write(sizeof(pdu), pdu);
            }
        }
            break;

        case EACI_MSG_DATA_REQ_RSCPC_RD:
        {
            if (param_len == BD_ADDR_LEN + 1)
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_REQ, EACI_MSG_DATA_REQ_RSCPC_RD, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0};
                pdu[3] = (uint8_t)(ATT_SVC_RUNNING_SPEED_CADENCE);
                pdu[4] = (uint8_t)(ATT_SVC_RUNNING_SPEED_CADENCE >> 8);
                pdu[5] = param[0];
                app_eaci_set_bd_addr_by_param((pdu + 6), (param + 1));
                eaci_uart_write(sizeof(pdu), pdu);
            }
        }
            break;

        case EACI_MSG_DATA_REQ_RSCPC_CFG_NTFIND:
        {
            if (param_len == BD_ADDR_LEN + 3)
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_REQ, EACI_MSG_DATA_REQ_RSCPC_CFG_NTFIND, 11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
                pdu[3] = (uint8_t)(ATT_SVC_RUNNING_SPEED_CADENCE);
                pdu[4] = (uint8_t)(ATT_SVC_RUNNING_SPEED_CADENCE >> 8);
                pdu[5] = param[0];
                pdu[6] = param[1];
                pdu[7] = param[2];
                app_eaci_set_bd_addr_by_param((pdu + 8), (param + 3));
                eaci_uart_write(sizeof(pdu), pdu);
            }
        }
            break;

        case EACI_MSG_DATA_REQ_RSCPC_CFG_CTNL_PT:
        {
            if (param_len == BD_ADDR_LEN + 5)
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_REQ, EACI_MSG_DATA_REQ_RSCPC_CFG_CTNL_PT, 13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
                pdu[3] = (uint8_t)(ATT_SVC_RUNNING_SPEED_CADENCE);
                pdu[4] = (uint8_t)(ATT_SVC_RUNNING_SPEED_CADENCE >> 8);
                pdu[5] = param[0];
                pdu[6] = param[1];
                pdu[7] = param[2];
                pdu[8] = param[3];
                pdu[9] = param[4];
                app_eaci_set_bd_addr_by_param((pdu + 10), (param + 5));
                eaci_uart_write(sizeof(pdu), pdu);
            }
        }
            break;

        default:
            break;
    }
}

static void app_eaci_data_ind_rscpc(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    switch (msg_id)
    {
        case EACI_MSG_DATA_IND_RSCPC_ENABLE:
            if (param_len == 1)
            {
                QPRINTF("RSCPC enable confirmation status: 0x%02X.\r\n", param[0]);
            }
            break;

        case EACI_MSG_DATA_IND_RSCPC_READ:
            if (param_len == 1)
            {
                QPRINTF("RSCPC read status: 0x%02X.\r\n", param[0]);
            }
            break;

        case EACI_MSG_DATA_IND_RSCPC_CFG_NTF_IND:
            if (param_len == 1)
            {
                QPRINTF("RSCPC write char CFG. status: 0x%02X.\r\n", param[0]);
            }
            break;

        case EACI_MSG_DATA_IND_RSCPC_CTNL_PT_CFG_WR:
            if (param_len == 1)
            {
                QPRINTF("RSCPC write SC Control Point status: 0x%02X.\r\n", param[0]);
            }
            break;

        case EACI_MSG_DATA_IND_RSCPC_CTNL_PT_CFG_IND:
            if (param_len == 1)
            {
                QPRINTF("RSCPC write SC Control Point Indication status: 0x%02X.\r\n", param[0]);
            }
            break;

        case EACI_MSG_DATA_IND_RSCPC_NTF_MEAS:
            if (param_len == 10)
            {
                QPRINTF("RSCPC Notified RSC Measurement flags: 0x%02X.\r\n",
                            param[0]);
                QPRINTF("RSCPC Notified RSC Measurement Instantaneous Cadence: 0x%02X.\r\n",
                            param[1]);
                QPRINTF("RSCPC Notified RSC Measurement Instantaneous Speed: 0x%04X.\r\n",
                            param[3] << 8 | param[2]);
                QPRINTF("RSCPC Notified RSC Measurement Instantaneous Stride Length: 0x%04X.\r\n",
                            param[5] << 8 | param[4]);
                QPRINTF("RSCPC Notified RSC Measurement Total Distance: 0x%08X.\r\n",
                            param[9] << 24 | param[8] << 16 | param[7] << 8 | param[6]);
            }
            break;

        case EACI_MSG_DATA_IND_RSCPC_RSC_FEAT:
            if (param_len == 2)
            {
                QPRINTF("RSCPC Read RSC Feature: 0x%04X.\r\n", param[1] << 8 | param[0]);
            }
            break;

        case EACI_MSG_DATA_IND_RSCPC_SENSOR_LOC:
            if (param_len == 1)
            {
                QPRINTF("RSCPC Read Sensor Location: 0x%02X.\r\n", param[0]);
            }
            break;

        case EACI_MSG_DATA_IND_RSCPC_CTNL_PT:
            if (param_len == 4)
            {
                QPRINTF("RSCPC Indicated SC Control Point Operation Code: 0x%02X.\r\n", param[0]);
                QPRINTF("RSCPC Indicated SC Control Point Response Value: 0x%02X.\r\n", param[1]);
                QPRINTF("RSCPC Indicated SC Control Point List of supported locations: 0x%04X.\r\n", param[3] << 8 | param[2]);
            }
            break;

        case EACI_MSG_DATA_IND_RSCPC_RSC_MEAS_CFG:
            if (param_len == 2)
            {
                QPRINTF("RSCPC Measurement cfg.: 0x%04X.\r\n", param[1] << 8 | param[0]);
            }
            break;

        case EACI_MSG_DATA_IND_RSCPC_RSC_PT_CFG:
            if (param_len == 2)
            {
                QPRINTF("RSCPC SC Control Point cfg.: 0x%04X.\r\n", param[1] << 8 | param[0]);
            }
            break;

        default:
            break;
    }
}
#endif

#if BLE_RSC_SENSOR
void app_eaci_data_req_rscps(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    switch (msg_id)
    {
        case EACI_MSG_DATA_REQ_RSCPS_NTF_MEAS:
        {
            if (param_len == BD_ADDR_LEN + 10)
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_REQ, EACI_MSG_DATA_REQ_RSCPS_NTF_MEAS, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
                pdu[3] = (uint8_t)(ATT_SVC_RUNNING_SPEED_CADENCE);
                pdu[4] = (uint8_t)(ATT_SVC_RUNNING_SPEED_CADENCE >> 8);
                pdu[5] = param[0];
                pdu[6] = param[1];
                pdu[7] = param[2];
                pdu[8] = param[3];
                pdu[9] = param[4];
                pdu[10] = param[5];
                pdu[11] = param[6];
                pdu[12] = param[7];
                pdu[13] = param[8];
                pdu[14] = param[9];
                app_eaci_set_bd_addr_by_param((pdu + 15), (param + 10));
                eaci_uart_write(sizeof(pdu), pdu);
            }
        }
            break;

        case EACI_MSG_DATA_REQ_RSCPS_CTNL_PT:
        {
            if (param_len == BD_ADDR_LEN + 3)
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_REQ, EACI_MSG_DATA_REQ_RSCPS_CTNL_PT, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0};
                pdu[3] = (uint8_t)(ATT_SVC_RUNNING_SPEED_CADENCE);
                pdu[4] = (uint8_t)(ATT_SVC_RUNNING_SPEED_CADENCE >> 8);
                pdu[5] = param[0];
                pdu[6] = param[1];
                pdu[7] = param[2];
                app_eaci_set_bd_addr_by_param((pdu + 8), (param + 3));
                eaci_uart_write(sizeof(pdu), pdu);
            }
        }
            break;

        default:
            break;
    }
}

static void app_eaci_data_ind_rscps(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    switch (msg_id)
    {

        case EACI_MSG_DATA_IND_PASPS_SEND_MEAS:
            if (param_len == 1)
            {
                QPRINTF(" Send RSC Measurement Status: 0x%02X.\r\n", param[0]);
            }
            break;

        case EACI_MSG_DATA_IND_PASPS_CTNL_CUMUL:
            if (param_len == 1)
            {
                QPRINTF("Set Cumulative Value Status: 0x%02X.\r\n", param[0]);
            }
            break;                            

        case EACI_MSG_DATA_IND_PASPS_CTNL_UPD_LOC:
            if (param_len == 1)
            {
                QPRINTF("Update Sensor Location Status: 0x%02X.\r\n", param[0]);
            }
            break;

        case EACI_MSG_DATA_IND_PASPS_CTNL_SUPP_LOC:
            if (param_len == 1)
            {
                QPRINTF("Request Supported Sensor Locations Status: 0x%02X.\r\n", param[0]);
            }
            break;

        default:
            break;
    }
}
#endif

/**
*************************************************************************************
*** Scan Parameter Profile
*************************************************************************************
 */
#if BLE_SP_CLIENT
void app_eaci_data_req_scppc(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    switch (msg_id)
    {
        case EACI_MSG_DATA_REQ_SP_ENABLE:
            if (param_len == BD_ADDR_LEN)
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_REQ, EACI_MSG_DATA_REQ_SP_ENABLE, 8, 0x13, 0x18, 0, 0, 0, 0, 0, 0};
                app_eaci_set_bd_addr_by_param((pdu + 5), param);
                eaci_uart_write(sizeof(pdu), pdu);
            }
            break;

        case EACI_MSG_DATA_REQ_SP_RD_CFG:
            if (param_len == BD_ADDR_LEN)
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_REQ, EACI_MSG_DATA_REQ_SP_RD_CFG, 8, 0x13, 0x18, 0, 0, 0, 0, 0, 0};
                app_eaci_set_bd_addr_by_param((pdu + 5), param);
                eaci_uart_write(sizeof(pdu), pdu);
            }
            break;

        case EACI_MSG_DATA_REQ_SP_WR_WD:
            if (param_len == BD_ADDR_LEN)
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_REQ, EACI_MSG_DATA_REQ_SP_WR_WD, 12, 0x13, 0x18, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
                //Scan Interval value
                pdu[5] = 0x00;
                pdu[6] = 0x30;
                //Scan Window value
                pdu[7] = 0x00;
                pdu[8] = 0x30;
                app_eaci_set_bd_addr_by_param((pdu + 9), param);
                eaci_uart_write(sizeof(pdu), pdu);
            }
            break;

        case EACI_MSG_DATA_REQ_SP_WR_MEAS:
            if (param_len == (BD_ADDR_LEN + 1))
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_REQ, EACI_MSG_DATA_REQ_SP_WR_MEAS, 10, 0x13, 0x18, 0, 0, 0, 0, 0, 0, 0, 0};
                //Scan Interval value
                pdu[5] = param[0];
                pdu[6] = 0x00;

                app_eaci_set_bd_addr_by_param((pdu + 7), (param + 1));
                eaci_uart_write(sizeof(pdu), pdu);
            }
            break;

        default:
            break;
    }
}
#endif
#if BLE_SP_SERVER
void app_eaci_data_req_scpps(uint8_t msg_id)
{
    switch (msg_id)
    {
        case EACI_MSG_DATA_REQ_SP_SCAN_REFRESH_REQ:
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_REQ, EACI_MSG_DATA_REQ_SP_SCAN_REFRESH_REQ, 2, 0x13, 0x18};
                eaci_uart_write(sizeof(pdu), pdu);
            }
            break;

        default:
            break;
    }
}
#endif

#if BLE_SP_CLIENT
static void app_eaci_data_ind_scppc(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    switch (msg_id)
    {
        case EACI_MSG_DATA_IND_SP_ENABLE:
            if (param_len == 1)
            {
                QPRINTF("Scan Parameters enable confirmation status: 0x%02X.\r\n", param[0]);
            }
            break;

        case EACI_MSG_DATA_IND_SP_WR_RSP:
            if (param_len == 1)
            {
                QPRINTF("Write status: 0x%02X.\r\n", param[0]);
            }
            break;

        case EACI_MSG_DATA_IND_SP_RD_CFG:
            if (param_len == 2)
            {
                uint16_t sp_cfg = param[1] << 8 | param[0];
                QPRINTF("Scan Notification Configuration: 0x%04X.\r\n", sp_cfg);
            }
            break;

        default:
            break;
    }
}
#endif
#if BLE_SP_SERVER
static void app_eaci_data_ind_scpps(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    switch (msg_id)
    {
        case EACI_MSG_DATA_IND_SP_INTV_WD:
            if (param_len == 4)
            {
                uint16_t scan_intv = param[1] << 8 | param[0];
                uint16_t scan_window = param[3] <<8 | param[2];
                QPRINTF("Scan Interval and Win: 0x%04X  0x%04X\r\n", scan_intv, scan_window);
            }
            break;

        case EACI_MSG_DATA_IND_SP_REFRESH_NTF_CFG:
            if (param_len == 2)
            {
                uint16_t sp_cfg = param[1] << 8 | param[0];
                QPRINTF("Configure Scan Refresh: 0x%04X.\r\n", sp_cfg);
            }
            break;

        default:
            break;
    }
}
#endif

/**
*************************************************************************************
*** Time Profile
*************************************************************************************
 */
#if BLE_TIP_CLIENT
void app_eaci_data_req_tipc(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    switch (msg_id)
    {
        case EACI_MSG_DATA_REQ_TIP_ENABLE:
            if (param_len == BD_ADDR_LEN)
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_REQ, EACI_MSG_DATA_REQ_TIP_ENABLE, 8, 0x05, 0x18, 0, 0, 0, 0, 0, 0};
                app_eaci_set_bd_addr_by_param((pdu + 5), param);
                eaci_uart_write(sizeof(pdu), pdu);
            }
            break;

        case EACI_MSG_DATA_REQ_TIP_CHAR_VAL_RD:
            if (param_len == (BD_ADDR_LEN + 1))
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_REQ, EACI_MSG_DATA_REQ_TIP_CHAR_VAL_RD, 9, 0x05, 0x18, 0, 0, 0, 0, 0, 0, 0};
                pdu[5] = param[0];
                app_eaci_set_bd_addr_by_param((pdu + 6), (param + 1));
                eaci_uart_write(sizeof(pdu), pdu);
            }
            break;

        case EACI_MSG_DATA_REQ_TIP_CT_NTF_CFG:
            if (param_len == (BD_ADDR_LEN + 1))
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_REQ, EACI_MSG_DATA_REQ_TIP_CT_NTF_CFG, 10, 0x05, 0x18, 0, 0, 0, 0, 0, 0, 0, 0};
                //Start Notify(uint16_t)
                pdu[5] = param[0];
                pdu[6] = 0x00;
                app_eaci_set_bd_addr_by_param((pdu + 7), (param + 1));
                eaci_uart_write(sizeof(pdu), pdu);
            }
            break;

        case EACI_MSG_DATA_REQ_TIP_WR_UDP_CTNL_PT:
            if (param_len == (BD_ADDR_LEN + 1))
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_REQ, EACI_MSG_DATA_REQ_TIP_WR_UDP_CTNL_PT, 9, 0x05, 0x18, 0, 0, 0, 0, 0, 0, 0};
                //Writing the Time Update Control Point on the Server
                pdu[5] = param[0];
                app_eaci_set_bd_addr_by_param((pdu + 6), (param + 1));
                eaci_uart_write(sizeof(pdu), pdu);
            }
            break;

        default:
            break;
    }
}
#endif
#if BLE_TIP_SERVER
void app_eaci_data_req_tips(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    switch (msg_id)
    {
        case EACI_MSG_DATA_TIPS_UPD_CURR_TIME:
            if (param_len == 8)
            {
                uint8_t pdu[] = {EACI_MSG_TYPE_DATA_REQ, EACI_MSG_DATA_TIPS_UPD_CURR_TIME, 10, 0x05, 0x18, 0, 0, 0, 0, 0, 0, 0, 0};
                pdu[5] = param[0];
                pdu[6] = param[1];
                pdu[7] = param[2];
                pdu[8] = param[3];
                pdu[9] = param[4];
                pdu[10] = param[5];
                pdu[11] = param[6];
                pdu[12] = param[7];
                eaci_uart_write(sizeof(pdu), pdu);
            }
            break;

        default:
            break;
    }
}
#endif

#if BLE_TIP_CLIENT
static void app_eaci_data_ind_tipc(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    switch (msg_id)
    {
        case EACI_MSG_DATA_IND_TIP_ENABLE:
            if (param_len == 1)
            {
                QPRINTF("Time enable confirmation status: 0x%02X.\r\n", param[0]);
            }
            break;

        case EACI_MSG_DATA_IND_TIP_WR_RSP:
            if (param_len == 1)
            {
                QPRINTF("Write status: 0x%02X.\r\n", param[0]);
            }
            break;

        case EACI_MSG_DATA_IND_TIP_CT:
            if (param_len == 8)
            {
                QPRINTF("Date: ");
                uint16_t Year = param[1] << 8 | param[0];
                QPRINTF("%04d.", Year);
                QPRINTF("%02d.", param[2]);
                QPRINTF("%02d. ", param[3]);
                QPRINTF("Time: ");
                QPRINTF("%02d.", param[4]);
                QPRINTF("%02d.", param[5]);
                QPRINTF("%02d. ", param[6]);
                QPRINTF("Week: ");
                QPRINTF("%02d.\r\n", param[7]);
            }
            break;

        case EACI_MSG_DATA_IND_TIP_NTF_CFG_RD:
            if (param_len == 2)
            {
                QPRINTF("Read Current Time Notification Configuration: 0x%04X.\r\n", param[1] <<8 | param[0]);
            }
            break;

        case EACI_MSG_DATA_IND_TIP_LTI_RD:
            if (param_len == 2)
            {
                QPRINTF("TIPC Read Local Time Info, Time Zone(%+d): DST Offset: %d.\r\n",param[0],param[1]);
            }
            break;

        case EACI_MSG_DATA_IND_TIP_RTI_RD:
            if (param_len == 4)
            {
                QPRINTF("TIPC Read Reference Time Info, Source(%d): .\r\n",param[0]);
                QPRINTF("Accuracy: %d, Days Update: %d, Hours Update: %d.\r\n",
                        param[1],
                        param[2],
                        param[3]
                        );
            }
            break;

        case EACI_MSG_DATA_IND_TIP_TDST_RD:
            if (param_len == 1)
            {
                QPRINTF("TIPC Read Time With DST value, DST Offset: %d.\r\n", param[0]);
            }
            break;

        case EACI_MSG_DATA_IND_TIP_TUS_RD:
            if (param_len == 2)
            {
                QPRINTF("TIPC Read Time Update State value, State(%d): , Result(%d).\r\n", 
                        param[0],
                        param[1]
                        );
            }
            break;

        default:
            break;
    }
}
#endif
#if BLE_TIP_SERVER
static void app_eaci_data_ind_tips(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    switch (msg_id)
    {
        case EACI_MSG_DATA_IND_TIP_CURRENT_CCC:
            if (param_len == 2)
            {
                QPRINTF("Current time CCC Cfg. value: %d.\r\n", param[1] << 1 | param[0]);
            }
            break;

        case EACI_MSG_DATA_IND_TIP_UPD_CTNL_PT:
            if (param_len == 1)
            {
                QPRINTF("Time Update Control Point value: %d.\r\n", param[0]);
            }
            break;

        default:
            break;
    }
}
#endif

/**
 ****************************************************************************************
 * @brief EACI data indication message handler
 *
 ****************************************************************************************
 */
void app_eaci_data_ind(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    if (param_len < 2)
    {
        app_eaci_data_error_rsp(msg_id, param_len, param);
        return;
    }

    uint16_t uuid = param[1] << 8 | param[0];
    switch (uuid)
    {
#if (BLE_AN_CLIENT || BLE_AN_SERVER)
        case ATT_SVC_ALERT_NTF:
            if (msg_id < EACI_MSG_DATA_IND_ANPC_MAX || msg_id == EACI_MSG_TYPE_DATA_ERROR)
            {
            #if BLE_AN_CLIENT
                app_eaci_data_ind_anpc(msg_id, param_len-2, param+2);
            #endif
            }
            else
            {
            #if BLE_AN_SERVER
                app_eaci_data_ind_anps(msg_id, param_len-2, param+2);
            #endif
            }
            break;
#endif

#if (BLE_BATT_CLIENT || BLE_BATT_SERVER)
        case ATT_SVC_BATTERY_SERVICE:
            if (msg_id < EACI_MSG_DATA_IND_BATT_MAX)
            {
            #if BLE_BATT_CLIENT
                app_eaci_data_ind_basc(msg_id, param_len-2, param+2);
            #endif
            }
            else
            {
            #if BLE_BATT_SERVER
                app_eaci_data_ind_bass(msg_id, param_len-2, param+2);
            #endif
            }
            break;
#endif

#if (BLE_BP_COLLECTOR || BLE_BP_SENSOR)
        case ATT_SVC_BLOOD_PRESSURE:
            if (msg_id < EACI_MSG_DATA_IND_BP_MAX)
            {
            #if BLE_BP_COLLECTOR
                app_eaci_data_ind_blpc(msg_id, param_len-2, param+2);
            #endif
            }
            else
            {
            #if BLE_BP_SENSOR
                app_eaci_data_ind_blps(msg_id, param_len-2, param+2);
            #endif
            }
            break;
#endif

#if (BLE_CSC_COLLECTOR || BLE_CSC_SENSOR)
        case ATT_SVC_CYCLING_SPEED_CADENCE:
            if (msg_id < EACI_MSG_DATA_IND_CSCPC_MAX)
            {
            #if BLE_CSC_COLLECTOR
                app_eaci_data_ind_cscpc(msg_id, param_len-2, param+2);
            #endif
            }
            else
            {
            #if BLE_CSC_SENSOR
                app_eaci_data_ind_cscps(msg_id, param_len-2, param+2);
            #endif
            }
            break;
#endif

#if (BLE_DIS_CLIENT || BLE_DIS_SERVER)
        case ATT_SVC_DEVICE_INFO:
            if (msg_id < EACI_MSG_DATA_REQ_DISC_MAX)
            {
            #if BLE_DIS_CLIENT
                app_eaci_data_ind_disc(msg_id, param_len-2, param+2);
            #endif
            }
            break;
#endif

#if (BLE_FINDME_LOCATOR || BLE_FINDME_TARGET)
        case ATT_SVC_IMMEDIATE_ALERT:
            if (msg_id < EACI_MSG_DATA_IND_FM_MAX)
            {
            #if BLE_FINDME_LOCATOR
                app_eaci_data_ind_findl(msg_id, param_len-2, param+2);
            #endif
            }
            else
            {
            #if BLE_FINDME_TARGET
                app_eaci_data_ind_findt(msg_id, param_len-2, param+2);
            #endif
            }
            break;
#endif

#if (BLE_GL_COLLECTOR || BLE_GL_SENSOR)
        case ATT_SVC_GLUCOSE:
            if (msg_id < EACI_MSG_DATA_IND_GL_MAX)
            {
            #if BLE_GL_COLLECTOR
                app_eaci_data_ind_glpc(msg_id, param_len-2, param+2);
            #endif
            }
            else
            {
            #if BLE_GL_SENSOR
                app_eaci_data_ind_glps(msg_id, param_len-2, param+2);
            #endif
            }
            break;
#endif

#if (BLE_HR_COLLECTOR || BLE_HR_SENSOR)
        case ATT_SVC_HEART_RATE:
            if (msg_id < EACI_MSG_DATA_IND_HRPC_MAX)
            {
            #if BLE_HR_COLLECTOR
                app_eaci_data_ind_hrpc(msg_id, param_len-2, param+2);
            #endif
            }
            else
            {
            #if BLE_HR_SENSOR
                app_eaci_data_ind_hrps(msg_id, param_len-2, param+2);
            #endif
            }
            break;
#endif

#if (BLE_HT_COLLECTOR || BLE_HT_THERMOM)
        case ATT_SVC_HEALTH_THERMOM:
            if (msg_id < EACI_MSG_DATA_IND_HT_MAX)
            {
            #if BLE_HT_COLLECTOR
                app_eaci_data_ind_htpc(msg_id, param_len-2, param+2);
            #endif
            }
            else
            {
            #if BLE_HT_THERMOM
                app_eaci_data_ind_htpt(msg_id, param_len-2, param+2);
            #endif
            }
            break;
#endif

#if (BLE_PAS_CLIENT || BLE_PAS_SERVER)
        case ATT_SVC_PHONE_ALERT_STATUS:
            if (msg_id < EACI_MSG_DATA_IND_PASPC_MAX)
            {
            #if BLE_PAS_CLIENT
                app_eaci_data_ind_paspc(msg_id, param_len-2, param+2);
            #endif
            }
            else
            {
            #if BLE_PAS_SERVER
                app_eaci_data_ind_pasps(msg_id, param_len-2, param+2);
            #endif
            }
            break;
#endif

#if (BLE_PROX_MONITOR || BLE_PROX_REPORTER)
    //    case ATT_SVC_IMMEDIATE_ALERT:   // TODO: confilict with FMP
        case ATT_SVC_LINK_LOSS:
        case ATT_SVC_TX_POWER:
            if (msg_id < EACI_MSG_DATA_IND_PROM_MAX)
            {
            #if BLE_PROX_MONITOR
                app_eaci_data_ind_proxm(msg_id, param_len-2, param+2);
            #endif
            }
            else
            {
            #if BLE_PROX_REPORTER
                app_eaci_data_ind_proxr(msg_id, param_len-2, param+2);
            #endif
            }
            break;
#endif

#if (BLE_RSC_COLLECTOR || BLE_RSC_SENSOR)
        case ATT_SVC_RUNNING_SPEED_CADENCE:
            if (msg_id < EACI_MSG_DATA_IND_RSCPC_MAX)
            {
            #if BLE_RSC_COLLECTOR
                app_eaci_data_ind_rscpc(msg_id, param_len-2, param+2);
            #endif
            }
            else
            {
            #if BLE_RSC_SENSOR
                app_eaci_data_ind_rscps(msg_id, param_len-2, param+2);
            #endif
            }
            break;
#endif

#if (BLE_SP_CLIENT || BLE_SP_SERVER)
        case ATT_SVC_SCAN_PARAMETERS:
            if (msg_id < EACI_MSG_DATA_IND_SP_MAX)
            {
            #if BLE_SP_CLIENT
                app_eaci_data_ind_scppc(msg_id, param_len-2, param+2);
            #endif
            }
            else
            {
            #if BLE_SP_SERVER
                app_eaci_data_ind_scpps(msg_id, param_len-2, param+2);
            #endif
            }
            break;
#endif

#if (BLE_TIP_CLIENT || BLE_TIP_SERVER)
        case ATT_SVC_CURRENT_TIME:
        case ATT_SVC_REF_TIME_UPDATE:
        case ATT_SVC_NEXT_DST_CHANGE:
            if (msg_id < EACI_MSG_DATA_IND_TIP_MAX)
            {
            #if BLE_TIP_CLIENT
                app_eaci_data_ind_tipc(msg_id, param_len-2, param+2);
            #endif
            }
            else
            {
            #if BLE_TIP_SERVER
                app_eaci_data_ind_tips(msg_id, param_len-2, param+2);
            #endif
            }
            break;
#endif

        default:
            break;
    }
}

/**
 ****************************************************************************************
 * @brief EACI data error indication
 *
 ****************************************************************************************
 */
void app_eaci_data_error_rsp(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    switch (msg_id)
    {
        case EACI_MSG_TYPE_DATA_ERROR:
            if (param[0] == 1)
            {
                QPRINTF("EACI Command Error.\r\n");
            }
            break;

        default:
            break;
    }
}




