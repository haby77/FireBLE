/**
 ****************************************************************************************
 *
 * @file app_data.h
 *
 * @brief Application Data request and Data indication functions file
 *
 * Copyright (C) Quintic 2009-2012
 *
 * $Rev: $
 *
 ****************************************************************************************
 */

#ifndef _APP_DATA_H_
#define _APP_DATA_H_

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

#if BLE_RSC_COLLECTOR || BLE_RSC_SENSOR
/**
*************************************************************************************
*** Running Speed and Cadence profile
*************************************************************************************
*/
///EACI Data Request - RSC
enum
{
    ///Reserved
    EACI_MSG_DATA_REQ_RSCPC_RSV = 0x0,
    ///Enable Request
    EACI_MSG_DATA_REQ_RSCPC_ENABLE,
    ///Read Request
    EACI_MSG_DATA_REQ_RSCPC_RD,
    ///Configure sending of notification/indication
    EACI_MSG_DATA_REQ_RSCPC_CFG_NTFIND,
    ///Send the value of the SC Control Point
    EACI_MSG_DATA_REQ_RSCPC_CFG_CTNL_PT,
    ///Number of Data Request of RSCPC
    EACI_MSG_DATA_REQ_RSCPC_MAX,
    ///Send the RSC Sensor Measurement value
    EACI_MSG_DATA_REQ_RSCPS_NTF_MEAS = EACI_MSG_DATA_REQ_RSCPC_MAX,
    ///response for the RSCPS_SC_CTNL_PT_REQ_IND message
    EACI_MSG_DATA_REQ_RSCPS_CTNL_PT
};
///EACI Data Indication - RSC
enum
{
    ///Reserved
    EACI_MSG_DATA_IND_RSCPC_RSV = 0x0,
    ///Enable Confirmation
    EACI_MSG_DATA_IND_RSCPC_ENABLE,
    ///Read attribute value Procedure
    EACI_MSG_DATA_IND_RSCPC_READ,
    ///Wait for the Write Response after having written a Client Char. Cfg. Descriptor.
    EACI_MSG_DATA_IND_RSCPC_CFG_NTF_IND,
    ///Wait for the Write Response after having written the SC Control Point Char.
    EACI_MSG_DATA_IND_RSCPC_CTNL_PT_CFG_WR,
    ///Wait for the Indication Response after having written the SC Control Point Char.
    EACI_MSG_DATA_IND_RSCPC_CTNL_PT_CFG_IND,
    ///Notified RSC Measurement
    EACI_MSG_DATA_IND_RSCPC_NTF_MEAS,
    ///RSC Feature
    EACI_MSG_DATA_IND_RSCPC_RSC_FEAT,
    ///Sensor Location
    EACI_MSG_DATA_IND_RSCPC_SENSOR_LOC,
    ///Indicated SC Control Point
    EACI_MSG_DATA_IND_RSCPC_CTNL_PT,
    ///RSC Measurement cfg.
    EACI_MSG_DATA_IND_RSCPC_RSC_MEAS_CFG,
    ///SC Control Point cfg.
    EACI_MSG_DATA_IND_RSCPC_RSC_PT_CFG,
    ///Number of Data Indication of RSC
    EACI_MSG_DATA_IND_RSCPC_MAX,
    ///RSC Measurement
    EACI_MSG_DATA_IND_PASPS_SEND_MEAS = EACI_MSG_DATA_IND_RSCPC_MAX,
    ///Set Cumulative Value
    EACI_MSG_DATA_IND_PASPS_CTNL_CUMUL,
    ///Update Sensor Location
    EACI_MSG_DATA_IND_PASPS_CTNL_UPD_LOC,
    ///Supported Sensor Locations
    EACI_MSG_DATA_IND_PASPS_CTNL_SUPP_LOC
};
#endif

#if BLE_PAS_CLIENT || BLE_PAS_SERVER
/**
*************************************************************************************
*** Phone Alert Status Profile
*************************************************************************************
*/
///EACI Data Request - PAS
enum
{
    ///Reserved
    EACI_MSG_DATA_REQ_PASPC_RSV = 0x0,
    ///Enable Request
    EACI_MSG_DATA_REQ_PASPC_ENABLE,
    ///Read Request
    EACI_MSG_DATA_REQ_PASPC_RD,
    ///Write Code
    EACI_MSG_DATA_REQ_PASPC_WR,
    ///Number of Data Request of PASPC
    EACI_MSG_DATA_REQ_PASPC_MAX,
    ///Update the value of the Alert Status or the Ringer Setting
    EACI_MSG_DATA_REQ_PASPS_UPDATE_VAL = EACI_MSG_DATA_REQ_PASPC_MAX
};
///EACI Data Indication - PAS
enum
{
    ///Reserved
    EACI_MSG_DATA_IND_PASPC_RSV = 0x0,
    ///Enable Confirmation
    EACI_MSG_DATA_IND_PASPC_ENABLE,
    ///Read attribute value Procedure
    EACI_MSG_DATA_IND_PASPC_READ,
    ///Write attribute value Procedure
    EACI_MSG_DATA_IND_PASPC_WRITE,
    ///PAS Alert Status
    EACI_MSG_DATA_IND_PASPC_ALERT_STATUS,
    ///PAS Ringer Setting
    EACI_MSG_DATA_IND_PASPC_RINGER_SETTING,
    ///PAS Alert Status Client Characteristic Configuration Descriptor
    EACI_MSG_DATA_IND_PASPC_ALERT_STATUS_CFG,
    ///PAS Ringer Setting Client Characteristic Configuration Descriptor
    EACI_MSG_DATA_IND_PASPC_RINGER_SETTING_CFG,
    ///Number of Data Indication of PAS
    EACI_MSG_DATA_IND_PASPC_MAX,
    ///Update Alert Status Char. value
    EACI_MSG_DATA_IND_PASPC_UPD_ALERT_STATUS = EACI_MSG_DATA_IND_PASPC_MAX,
    ///Update Ringer Setting Char. value
    EACI_MSG_DATA_IND_PASPC_UPD_RINGER_SETTING
};
#endif

#if BLE_CSC_COLLECTOR || BLE_CSC_SENSOR
/**
*************************************************************************************
*** Cycling Speed and Cadence Profile
*************************************************************************************
*/
///EACI Data Request - CSC
enum
{
    ///Reserved
    EACI_MSG_DATA_REQ_CSCPC_RSV = 0x0,
    ///Enable Request
    EACI_MSG_DATA_REQ_CSCPC_ENABLE,
    ///Read the value of attribute
    EACI_MSG_DATA_REQ_CSCPC_RD_CODE,
    ///Configure sending of notification/indication
    EACI_MSG_DATA_REQ_CSCPC_WR_CFG,
    ///Send the value of the SC Control Point
    EACI_MSG_DATA_REQ_CSCPC_SEND_CTNL_PT,
    ///Number of Data Request of CSCPC
    EACI_MSG_DATA_REQ_CSCPC_MAX,
    ///Send the CSCP Sensor Measurement value
    EACI_MSG_DATA_REQ_CSCPS_NTF_MEAS = EACI_MSG_DATA_REQ_CSCPC_MAX,
    ///response for the CSCPS_SC_CTNL_PT_REQ_IND message
    EACI_MSG_DATA_REQ_CSCPS_CTNL_PT
};
///EACI Data Indication - CSC
enum
{
    ///Reserved
    EACI_MSG_DATA_IND_CSCPC_RSV = 0x0,
    ///Enable Confirmation
    EACI_MSG_DATA_IND_CSCPC_ENABLE,
    ///Read Request
    EACI_MSG_DATA_IND_CSCPC_READ,
    ///Read SC Control Point CFG.
    EACI_MSG_DATA_IND_CSCPC_READ_CTNL_PT_CFG,
    ///Read CSC Measurement CFG.
    EACI_MSG_DATA_IND_CSCPC_READ_MEAS_CFG,
    ///Indicated SC Control Point
    EACI_MSG_DATA_IND_CSCPC_SC_CTNL_PT,
    ///Notify CSC meas
    EACI_MSG_DATA_IND_CSCPC_NTF_MEAS,
    ///Wait CFG. Response
    EACI_MSG_DATA_IND_CSCPC_WR_CFG_STATUS,
    ///Wait for the Write Response after having written the SC Control Point Char.
    EACI_MSG_DATA_IND_CSCPC_CFG_WR,
    ///CSC Feature
    EACI_MSG_DATA_IND_CSCPC_RD_CSC_FEAT,
    ///Sensor Location
    EACI_MSG_DATA_IND_CSCPC_RD_SENSOR_LOC,
    ///Number of Data Indication of CSC
    EACI_MSG_DATA_IND_CSCPC_MAX,
    ///Send CSC Measurement Status
    EACI_MSG_DATA_IND_CSCPS_MEAS = EACI_MSG_DATA_IND_CSCPC_MAX
};
#endif

 /**
*************************************************************************************
*** Alert Notification Profile
*************************************************************************************
*/

///EACI Data Request - AN
enum
{
    ///Reserved
    EACI_MSG_DATA_REQ_ANPC_RSV = 0x0,
    ///Enable Request
    EACI_MSG_DATA_REQ_ANPC_ENABLE,
    ///READ CFG
    EACI_MSG_DATA_REQ_ANPC_RD_CFG,
    ///Write Characteristic
    EACI_MSG_DATA_REQ_ANPC_WR_VAL,
    ///Number of Data Request of ANPC
    EACI_MSG_DATA_REQ_ANPC_MAX,
    ///Update the value of a characteristic
    EACI_MSG_DATA_REQ_ANPS_NTF_ALERT = EACI_MSG_DATA_REQ_ANPC_MAX,
};
///EACI Data Indication - AN
enum
{
    ///Reserved
    EACI_MSG_DATA_IND_ANPC_RSV = 0x0,
    ///Enable Confirmation
    EACI_MSG_DATA_IND_ANPC_ENABLE,
    ///Write attribute value Procedure
    EACI_MSG_DATA_IND_ANPC_WR_STATUS,
    ///New Alert Client Characteristic Configuration Descriptor
    EACI_MSG_DATA_IND_ANPC_NEW_ALERT_CFG,
    ///Unread Alert Status Client Characteristic Configuration Descriptor
    EACI_MSG_DATA_IND_ANPC_UNREAD_ALERT_CFG,
    ///New Alert
    EACI_MSG_DATA_IND_ANPC_NEW_ALERT,
    ///Unread Alert Status
    EACI_MSG_DATA_IND_ANPC_UNREAD_ALERT_STATUS,
    ///Number of Data Indication of AN
    EACI_MSG_DATA_IND_ANPC_MAX,
    ///Update New Alert Char. value
    EACI_MSG_DATA_IND_ANPS_UPD_NEW_ALERT = EACI_MSG_DATA_IND_ANPC_MAX,
    ///Update Unread Alert Status Char. value
    EACI_MSG_DATA_IND_ANPS_UPD_UNREAD_ALERT
};

/**
*************************************************************************************
*** Battery Profile
*************************************************************************************
*/
///EACI Data Request - BATT(Battery)
enum
{
    ///Reserved
    EACI_MSG_DATA_REQ_BATT_RSV = 0x0,
    ///Enable Request
    EACI_MSG_DATA_REQ_BATT_ENABLE,
    ///Read Characteristic Value Request
    EACI_MSG_DATA_REQ_BATT_RD_CHAR_VAL,
    ///Write Battery Level Notification
    EACI_MSG_DATA_REQ_BATT_CFG_NOTIFY,
    ///Number of Data Request of BATT
    EACI_MSG_DATA_REQ_BATT_MAX,
    ///Send the battery level update
    EACI_MSG_DATA_REQ_BASS_BATT_LEVEL_UPD = EACI_MSG_DATA_REQ_BATT_MAX,
};
///EACI Data Indication - BATT(Battery)
enum
{
    ///Reserved
    EACI_MSG_DATA_IND_BATT_RSV = 0x0,
    ///Enable Confirmation
    EACI_MSG_DATA_IND_BATT_ENABLE,
    ///Battery Level
    EACI_MSG_DATA_IND_BATT_LEVEL,
    ///Battery CFG.
    EACI_MSG_DATA_IND_BATT_RD_CFG,
    ///Write Char RSP.
    EACI_MSG_DATA_IND_BATT_WR_RSP,
    ///Number of Data Indication of BATT
    EACI_MSG_DATA_IND_BATT_MAX,
    ///Battery level update confirm
    EACI_MSG_DATA_IND_BASS_BATT_UPD_CFM = EACI_MSG_DATA_IND_BATT_MAX
};

/**
*************************************************************************************
*** Blood Pressure Profile
*************************************************************************************
*/
///EACI Data Request - BP(Blood Pressure)
enum
{
    ///Reserved
    EACI_MSG_DATA_REQ_BP_RSV = 0x0,
    ///Enable Request
    EACI_MSG_DATA_REQ_BP_ENABLE,
    ///Read BPS CHAR CODE
    EACI_MSG_DATA_REQ_BP_RD_CHAR_CODE,
    ///Configuration characteristics
    EACI_MSG_DATA_REQ_BP_CFG_INDNTF,
    ///Number of Data Request of BP
    EACI_MSG_DATA_REQ_BP_MAX,
    ///Send Pressure
    EACI_MSG_DATA_REQ_BP_SEND_PRES = EACI_MSG_DATA_REQ_BP_MAX
};
///EACI Data Indication - BP(Blood Pressure)
enum
{
    ///Reserved
    EACI_MSG_DATA_IND_BP_RSV = 0x0,
    ///Enable Confirmation
    EACI_MSG_DATA_IND_BP_ENABLE,
    ///Read Char RSP
    EACI_MSG_DATA_IND_BP_RD_RSP,
    ///Write Char RSP
    EACI_MSG_DATA_IND_BP_WR_RSP,
    ///BP Measurement
    EACI_MSG_DATA_IND_BP_MEAS,
    ///Number of Data Indication of BP
    EACI_MSG_DATA_IND_BP_MAX,
    ///BLPS CFG.
    EACI_MSG_DATA_IND_BPS_CFG = EACI_MSG_DATA_IND_BP_MAX,
    ///Blood Pressure Measurement value confirm
    EACI_MSG_DATA_IND_BPS_MEAS_SEND_CFM
};

/**
*************************************************************************************
*** Device Information Service Profile
*************************************************************************************
*/
///EACI Data Request
enum
{
    ///Reserved
    EACI_MSG_DATA_REQ_DISC_RSV = 0x0,
    ///Enable Request
    EACI_MSG_DATA_REQ_DISC_ENABLE,
    ///read char
    EACI_MSG_DATA_REQ_DISC_CHAR_RD,
    ///Number of Data Request of DIS
    EACI_MSG_DATA_REQ_DISC_MAX,
    ///write char
    EACI_MSG_DATA_REQ_DISS_CHAR_WR = EACI_MSG_DATA_REQ_DISC_MAX
};
///EACI Data Indication
enum
{
    ///Reserved
    EACI_MSG_DATA_IND_DISC_RSV = 0x0,
    ///Enable Confirmation
    EACI_MSG_DATA_IND_DISC_ENABLE,
    ///read char
    EACI_MSG_DATA_IND_DISC_CHAR_RD,
    ///Number of Data Indication of DISC
    EACI_MSG_DATA_IND_DISC_MAX
};

/**
*************************************************************************************
*** Find Me Profile
*************************************************************************************
 */
///EACI Data Request - FINDME
enum
{
    ///Reserved
    EACI_MSG_DATA_REQ_FM_RSV = 0x0,
    ///Enable Request
    EACI_MSG_DATA_REQ_FM_ENABLE,
    ///Write Alert Level
    EACI_MSG_DATA_REQ_FM_WR_ALERT,
    ///Number of Data Request of HT
    EACI_MSG_DATA_REQ_FM_MAX
};
///EACI Data Indication - FINDME
enum
{
    ///Reserved
    EACI_MSG_DATA_IND_FM_RSV = 0x0,
    ///Enable Confirmation
    EACI_MSG_DATA_IND_FM_ENABLE,
    ///Number of Data Indication of HT
    EACI_MSG_DATA_IND_FM_MAX,
    ///Alert Level
    EACI_MSG_DATA_IND_ALERT_LEV = EACI_MSG_DATA_IND_FM_MAX
};

/**
*************************************************************************************
*** Glucose Profile
*************************************************************************************
*/
///EACI Data Request - GL(Glucose Profile)
enum
{
    ///Reserved
    EACI_MSG_DATA_REQ_GL_RSV = 0x0,
    ///Enable Request
    EACI_MSG_DATA_REQ_GL_ENABLE,
    ///Register
    EACI_MSG_DATA_REQ_GL_REGISTER,
    ///Read Features
    EACI_MSG_DATA_REQ_GL_FEATURES_RD,
    ///Report Stored Records
    EACI_MSG_DATA_REQ_GL_RSR_RACP,
    ///Delete Stored Records
    EACI_MSG_DATA_REQ_GL_DSR_RACP,
    ///Abort Operation
    EACI_MSG_DATA_REQ_GL_AO_RACP,
    ///Report Number of Stored Records
    EACI_MSG_DATA_REQ_GL_RN_RACP,
    ///Number of Data Request of GL
    EACI_MSG_DATA_REQ_GL_MAX,
    ///Send Glucose measurement without context information
    EACI_MSG_DATA_REQ_GL_SEND_MEAS_WITHOUT_CTX = EACI_MSG_DATA_REQ_GL_MAX,
    ///Send Glucose measurement with context information
    EACI_MSG_DATA_REQ_GL_SEND_MEAS_WITH_CTX,
};
///EACI Data Indication - GL(Glucose Profile)
enum
{
    ///Reserved
    EACI_MSG_DATA_IND_GL_RSV = 0x0,
    ///Enable Confirmation
    EACI_MSG_DATA_IND_GL_ENABLE,
    ///Register
    EACI_MSG_DATA_IND_GL_REGISTER,
    ///Features
    EACI_MSG_DATA_IND_GL_FEATURES_RD,
    ///Glucose measurement value
    EACI_MSG_DATA_IND_GL_MEAS,
    ///Glucose measurement value with CTX
    EACI_MSG_DATA_IND_GL_MEAS_CTX,
    ///GLPC RACP response OP code
    EACI_MSG_DATA_IND_GL_RACP_RSP,
    ///Number of Data Indication of GL
    EACI_MSG_DATA_IND_GL_MAX,
    ///cfg. message
    EACI_MSG_DATA_IND_GLS_CFG = EACI_MSG_DATA_IND_GL_MAX,
};

/**
*************************************************************************************
*** Heart Rate Profile
*************************************************************************************
*/
///EACI Data Request - HRP
enum
{
    ///Reserved
    EACI_MSG_DATA_REQ_HRP_RSV = 0x0,
    ///Enable Request
    EACI_MSG_DATA_REQ_HRPC_ENABLE,
    ///Read Body Sensor Location
    EACI_MSG_DATA_READ_HRPC_BODY_SENSOR,
    ///Configure Heart Rate Measurement
    EACI_MSG_DATA_READ_HRPC_CFG_INDNTF,
    ///Number of Data Request of HRPC
    EACI_MSG_DATA_REQ_HRPC_MAX,
    ///Heart Rate measurement value
    EACI_MSG_DATA_REQ_HRPS_MEAS_VALUE = EACI_MSG_DATA_REQ_HRPC_MAX

};
///EACI Data Indication - HRP
enum
{
    ///Reserved
    EACI_MSG_DATA_IND_HRP_RSV = 0x0,
    ///Enable Confirmation
    EACI_MSG_DATA_IND_HRPC_ENABLE,
    ///Read Body Semsor Location rsp
    EACI_MSG_DATA_IND_HRPC_READ_BSL_RSP,
    ///Write RSP.
    EACI_MSG_DATA_IND_HRPC_WR_RSP,
    ///Heart Rate value send to APP
    EACI_MSG_DATA_IND_HRPC_MEAS,
    ///Number of Data Indication of HRP
    EACI_MSG_DATA_IND_HRPC_MAX,
    ///cfg send to app
    EACI_MSG_DATA_IND_HRPS_CFG = EACI_MSG_DATA_IND_HRPC_MAX,
};

/**
*************************************************************************************
*** Health Thermometer Profile
*************************************************************************************
*/
///EACI Data Request - HT
enum
{
    ///Reserved
    EACI_MSG_DATA_REQ_HT_RSV = 0x0,
    ///Enable Request
    EACI_MSG_DATA_REQ_HT_ENABLE,
    ///CFG. Temperature Measurement
    EACI_MSG_DATA_REQ_HT_TEMP_MEAS,
    ///CFG. Intermediate Temperature
    EACI_MSG_DATA_REQ_HT_INTM_TEMP,
    ///CFG. Measurement Interval
    EACI_MSG_DATA_REQ_HT_MEAS_INTV,
    ///read measurement interval 
    EACI_MSG_DATA_REQ_HT_RD_MEAS_INTV,
    ///Number of Data Request of HT
    EACI_MSG_DATA_REQ_HT_MAX,
    ///Send Measurement Interval value
    EACI_MSG_DATA_REQ_HT_SEND_INTER_VALUE = EACI_MSG_DATA_REQ_HT_MAX ,
    ///Send Temperature Measurement value
    EACI_MSG_DATA_REQ_HT_SEND_TEMP_VALUE
};
///EACI Data Indication - HT
enum
{
    ///Reserved
    EACI_MSG_DATA_IND_HT_RSV = 0x0,
    ///Enable Confirmation
    EACI_MSG_DATA_IND_HT_ENABLE,
    ///Read Char rsp
    EACI_MSG_DATA_IND_HT_RD_CHAR_RSP,
    ///Write Status
    EACI_MSG_DATA_IND_HT_WR_RSP,
    ///Temperature Measurement
    EACI_MSG_DATA_IND_HT_TEM_MEA,
    ///Measurement Interval
    EACI_MSG_DATA_IND_HT_MEAS_INTE,
    ///Number of Data Indication of HT
    EACI_MSG_DATA_IND_HT_MAX,
    ///Thermom Measurement Interval
    EACI_MSG_DATA_IND_HT_TH_TEM_MEA = EACI_MSG_DATA_IND_HT_MAX,
    ///Thermom cfg indication
    EACI_MSG_DATA_IND_HT_TH_CFG_IND    
};

/**
*************************************************************************************
*** Proximity Profile
*************************************************************************************
*/
///EACI Data Request - PROX
enum
{
    ///Reserved
    EACI_MSG_DATA_REQ_PROX_RSV = 0x0,
    ///Enable Request
    EACI_MSG_DATA_REQ_PROXM_ENABLE,
    ///Read TX Power
    EACI_MSG_DATA_READ_PROXM_TX_POWER,
    ///Write IAS data
    EACI_MSG_DATA_WRITE_PROXM_IAS,
    ///Write LLS data
    EACI_MSG_DATA_WRITE_PROXM_LLS,
    ///Number of Data Request of PROXM
    EACI_MSG_DATA_REQ_PROXM_MAX,
    ///Number of Data Request of PROX
    EACI_MSG_DATA_REQ_PROX_MAX = EACI_MSG_DATA_REQ_PROXM_MAX
};
///EACI Data Indication - PROX
enum
{
    ///Reserved
    EACI_MSG_DATA_IND_PROX_RSV = 0x0,
    ///Enable Confirmation
    EACI_MSG_DATA_IND_PROXM_ENABLE,
    ///tx power lv
    EACI_MSG_DATA_IND_PROXM_TX_POWER,
    ///Number of Data Indication of PROX
    EACI_MSG_DATA_IND_PROM_MAX,
    ///Alter Level
    EACI_MSG_DATA_IND_PROXR_ALERT = EACI_MSG_DATA_IND_PROM_MAX,
    ///Link Lost
    EACI_MSG_DATA_IND_PROXR_LINK_LOST
};

/**
*************************************************************************************
*** Scan Parameters Profile
*************************************************************************************
*/
///EACI Data Request - SP(Scan Parameters)
enum
{
    ///Reserved
    EACI_MSG_DATA_REQ_SP_RSV = 0x0,
    ///Enable Request
    EACI_MSG_DATA_REQ_SP_ENABLE,
    ///Read Scan Refresh Cfg
    EACI_MSG_DATA_REQ_SP_RD_CFG,
    ///Send Scan Interval and Win
    EACI_MSG_DATA_REQ_SP_WR_WD,
    ///Configure Scan Refresh
    EACI_MSG_DATA_REQ_SP_WR_MEAS,
    ///Number of Data Request of SP
    EACI_MSG_DATA_REQ_SP_MAX,
    ///Send the scan refresh value
    EACI_MSG_DATA_REQ_SP_SCAN_REFRESH_REQ = EACI_MSG_DATA_REQ_SP_MAX
};
///EACI Data Indication - SP(Scan Parameters)
enum
{
    ///Reserved
    EACI_MSG_DATA_IND_SP_RSV = 0x0,
    ///Enable Confirmation
    EACI_MSG_DATA_IND_SP_ENABLE,
    ///Write Status
    EACI_MSG_DATA_IND_SP_WR_RSP,
    ///Scan Refresh Cfg.
    EACI_MSG_DATA_IND_SP_RD_CFG,
    ///Number of Data Indication of SP
    EACI_MSG_DATA_IND_SP_MAX,
    ///Scan Interval and Win
    EACI_MSG_DATA_IND_SP_INTV_WD = EACI_MSG_DATA_IND_SP_MAX,
    ///Configure Scan Refresh
    EACI_MSG_DATA_IND_SP_REFRESH_NTF_CFG
};

/**
*************************************************************************************
*** Time Profile
*************************************************************************************
*/
///EACI Data Request - TIME(Time Profile)
enum
{
    ///Reserved
    EACI_MSG_DATA_REQ_TIP_RSV = 0x0,
    ///Enable Request
    EACI_MSG_DATA_REQ_TIP_ENABLE,
    ///Read a CTS or NDCS or RTUS characteristic
    EACI_MSG_DATA_REQ_TIP_CHAR_VAL_RD,
    ///Configuring the Current Time Characteristic
    EACI_MSG_DATA_REQ_TIP_CT_NTF_CFG,
    ///Writing Time Control Point
    EACI_MSG_DATA_REQ_TIP_WR_UDP_CTNL_PT,
    ///Number of Data Request of TIME
    EACI_MSG_DATA_REQ_TIP_MAX,
    ///update current time
    EACI_MSG_DATA_TIPS_UPD_CURR_TIME = EACI_MSG_DATA_REQ_TIP_MAX,
};
///EACI Data Indication - TIME(Time Profile)
enum
{
    ///Reserved
    EACI_MSG_DATA_IND_TIP_RSV = 0x0,
    ///Enable Confirmation
    EACI_MSG_DATA_IND_TIP_ENABLE,
    ///Write Status
    EACI_MSG_DATA_IND_TIP_WR_RSP,
    ///Current Time value
    EACI_MSG_DATA_IND_TIP_CT,
    ///Read Current Time Notification Configuration
    EACI_MSG_DATA_IND_TIP_NTF_CFG_RD,
    ///Reference Time Info Characteristic Structure
    EACI_MSG_DATA_IND_TIP_LTI_RD,
    ///TIPC Read Reference Time Info
    EACI_MSG_DATA_IND_TIP_RTI_RD,
    ///TIPC Read Time With DST
    EACI_MSG_DATA_IND_TIP_TDST_RD,
    ///TIPC Read Time Update State
    EACI_MSG_DATA_IND_TIP_TUS_RD,
    ///Number of Data Indication of TIME
    EACI_MSG_DATA_IND_TIP_MAX,
    ///Current time CCC Cfg.
    EACI_MSG_DATA_IND_TIP_CURRENT_CCC = EACI_MSG_DATA_IND_TIP_MAX,
    ///Time Update Control Point value
    EACI_MSG_DATA_IND_TIP_UPD_CTNL_PT
};

/**
*************************************************************************************
*** Running Speed and Cadence profile
*************************************************************************************
 */
#if BLE_RSC_COLLECTOR
void app_eaci_data_req_rscpc(uint8_t msg_id, uint8_t param_len, uint8_t const *param);
#endif
#if BLE_RSC_SENSOR
void app_eaci_data_req_rscps(uint8_t msg_id, uint8_t param_len, uint8_t const *param);
#endif

/**
*************************************************************************************
*** Phone Alert Status Profile
*************************************************************************************
 */
#if BLE_PAS_CLIENT
void app_eaci_data_req_paspc(uint8_t msg_id, uint8_t param_len, uint8_t const *param);
#endif
#if BLE_PAS_SERVER
void app_eaci_data_req_pasps(uint8_t msg_id, uint8_t param_len, uint8_t const *param);
#endif

/**
*************************************************************************************
*** Cycling Speed and Cadence profile
*************************************************************************************
 */
#if BLE_CSC_COLLECTOR
void app_eaci_data_req_cscpc(uint8_t msg_id, uint8_t param_len, uint8_t const *param);
#endif
#if BLE_CSC_SENSOR
void app_eaci_data_req_cscps(uint8_t msg_id, uint8_t param_len, uint8_t const *param);
#endif

/**
*************************************************************************************
*** Alert Notification Profile
*************************************************************************************
 */
#if BLE_AN_CLIENT
void app_eaci_data_req_anpc(uint8_t msg_id, uint8_t param_len, uint8_t const *param);
#endif
#if BLE_AN_SERVER
void app_eaci_data_req_anps(uint8_t msg_id, uint8_t param_len, uint8_t const *param);
#endif

/**
*************************************************************************************
*** Battery Profile
*************************************************************************************
 */
#if BLE_BATT_CLIENT
void app_eaci_data_req_basc(uint8_t msg_id, uint8_t param_len, uint8_t const *param);
#endif
#if BLE_BATT_SERVER
void app_eaci_data_req_bass(uint8_t msg_id, uint8_t param_len, uint8_t const *param);
#endif

/**
*************************************************************************************
*** Blood Pressure Profile
*************************************************************************************
 */
#if BLE_BP_COLLECTOR
void app_eaci_data_req_blpc(uint8_t msg_id, uint8_t param_len, uint8_t const *param);
#endif
#if BLE_BP_SENSOR
void app_eaci_data_req_blps(uint8_t msg_id, uint8_t param_len, uint8_t const *param);
#endif

/**
*************************************************************************************
*** Device Information Service Profile
*************************************************************************************
 */
#if BLE_DIS_CLIENT
void app_eaci_data_req_disc(uint8_t msg_id, uint8_t param_len, uint8_t const *param);
#endif
#if BLE_DIS_SERVER
void app_eaci_data_req_diss(uint8_t msg_id, uint8_t param_len, uint8_t const *param);
#endif

/**
*************************************************************************************
*** Find Me Profile
*************************************************************************************
 */
#if BLE_FINDME_LOCATOR
void app_eaci_data_req_findl(uint8_t msg_id, uint8_t param_len, uint8_t const *param);
#endif

/**
*************************************************************************************
*** Glucose Profile
*************************************************************************************
 */
#if BLE_GL_COLLECTOR
void app_eaci_data_req_glpc(uint8_t msg_id, uint8_t param_len, uint8_t const *param);
#endif
#if BLE_GL_SENSOR
void app_eaci_data_req_glps(uint8_t msg_id);
#endif

/**
*************************************************************************************
*** Heart Rate Profile
*************************************************************************************
 */
#if BLE_HR_COLLECTOR
void app_eaci_data_req_hrpc(uint8_t msg_id, uint8_t param_len, uint8_t const *param);
#endif
#if BLE_HR_SENSOR
void app_eaci_data_req_hrps(uint8_t msg_id, uint8_t param_len, uint8_t const *param);
#endif

/**
*************************************************************************************
*** Health Thermometer Profile
*************************************************************************************
 */
#if BLE_HT_COLLECTOR
void app_eaci_data_req_htpc(uint8_t msg_id, uint8_t param_len, uint8_t const *param);
#endif
#if BLE_HT_THERMOM
void app_eaci_data_req_htpt(uint8_t msg_id, uint8_t param_len, uint8_t const *param);
#endif

/**
*************************************************************************************
*** Proximity Profile
*************************************************************************************
 */
#if BLE_PROX_MONITOR
void app_eaci_data_req_proxm(uint8_t msg_id, uint8_t param_len, uint8_t const *param);
#endif

/**
*************************************************************************************
*** Scan Parameter Profile
*************************************************************************************
 */
#if BLE_SP_CLIENT
void app_eaci_data_req_scppc(uint8_t msg_id, uint8_t param_len, uint8_t const *param);
#endif
#if BLE_SP_SERVER
void app_eaci_data_req_scpps(uint8_t msg_id);
#endif

/**
*************************************************************************************
*** Time Profile
*************************************************************************************
 */
#if BLE_TIP_CLIENT
void app_eaci_data_req_tipc(uint8_t msg_id, uint8_t param_len, uint8_t const *param);
#endif
#if BLE_TIP_SERVER
void app_eaci_data_req_tips(uint8_t msg_id, uint8_t param_len, uint8_t const *param);
#endif


/*
 ****************************************************************************************
 * @brief EACI data indication message handler
 *
 ****************************************************************************************
 */
void app_eaci_data_ind(uint8_t msg_id, uint8_t param_len, uint8_t const *param);

/**
 ****************************************************************************************
 * @brief EACI data error indication
 *
 ****************************************************************************************
 */
void app_eaci_data_error_rsp(uint8_t msg_id, uint8_t param_len, uint8_t const *param);

#endif // _APP_DATA_H_

