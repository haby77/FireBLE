/**
 ****************************************************************************************
 *
 * @file app_eaci.h
 *
 * @brief Easy ACI interface module source file.
 *
 * Copyright (C) Quintic 2012-2013
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */

#ifndef APP_EACI_H_
#define APP_EACI_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "app_env.h"
#include "app_eaci_util.h"
#include "app_eaci_generic.h"
#if (BLE_HR_COLLECTOR || BLE_HR_SENSOR)
#include "app_eaci_hrp.h"
#endif
#if (BLE_PROX_MONITOR || BLE_PROX_REPORTER)
#include "app_eaci_prx.h"
#endif
#if (BLE_HT_COLLECTOR || BLE_HT_THERMOM)
#include "app_eaci_ht.h"
#endif
#if (BLE_BP_COLLECTOR || BLE_BP_SENSOR)
#include "app_eaci_blp.h"
#endif
#if (BLE_GL_COLLECTOR || BLE_GL_SENSOR)
#include "app_eaci_gl.h"
#endif
#if (BLE_FINDME_LOCATOR || BLE_FINDME_TARGET)
#include "app_eaci_fm.h"
#endif
#if (BLE_TIP_CLIENT || BLE_TIP_SERVER)
#include "app_eaci_time.h"
#endif
#if (BLE_SP_CLIENT || BLE_SP_SERVER)
#include "app_eaci_sp.h"
#endif
#if (BLE_DIS_CLIENT || BLE_DIS_SERVER)
#include "app_eaci_dis.h"
#endif
#if (BLE_BATT_CLIENT || BLE_BATT_SERVER)
#include "app_eaci_batt.h"
#endif
#if (BLE_AN_CLIENT || BLE_AN_SERVER)
#include "app_eaci_an.h"
#endif
#if BLE_CSC_COLLECTOR || BLE_CSC_SENSOR
#include "app_eaci_csc.h"
#endif
#if BLE_PAS_CLIENT || BLE_PAS_SERVER
#include "app_eaci_pas.h"
#endif
#if BLE_RSC_COLLECTOR || BLE_RSC_SENSOR
#include "app_eaci_rsc.h"
#endif

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

#define EACI_CMD_ERROR              1
#define EACI_MSG_REQ_MAX            10
///EACI Command
enum
{
    ///Reserved
    EACI_MSG_CMD_RSV = 0x0,
    ///Advertising
    EACI_MSG_CMD_ADV,
    ///Scan
    EACI_MSG_CMD_SCAN,
    ///Connect
    EACI_MSG_CMD_CONN,
    ///Central Disconnect
    EACI_MSG_CMD_DISC,
    ///Peripheral Set Device Name
    EACI_MSG_CMD_SET_DEVNAME,
    ///Bond
    EACI_MSG_CMD_BOND,
    ///Central Update Param
    EACI_MSG_CMD_CEN_UPDATE_PARAM,
    ///Peripheral Update Param
    EACI_MSG_CMD_PER_UPDATE_PARAM,
    ///Number of Command
    EACI_MSG_CMD_MAX
};

///EACI Event
enum
{
    ///Reserved
    EACI_MSG_EVT_RSV = 0x0,
    ///Advertising Status
    EACI_MSG_EVT_ADV,
    ///Scan result
    EACI_MSG_EVT_INQ_RESULT,
    ///Scan complete
    EACI_MSG_EVT_INQ_CMP,
    ///Connect Status
    EACI_MSG_EVT_CONN,
    ///Disconnect Status
    EACI_MSG_EVT_DISC,
    ///Peripheral Device Name
    EACI_MSG_EVT_SET_DEVNAME,
    ///Bond Status
    EACI_MSG_EVT_BOND,
    ///Central Update param State
    EACI_MSG_EVT_CEN_UPDATE_PARAM,
    ///SMPC Security started indication to host
    EACI_MSG_EVT_SMP_SEC,
    ///Peripheral Update param
    EACI_MSG_EVT_UPDATE_PARAM,
    ///Number of Event
    EACI_MSG_EVT_MAX
};

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

#if BLE_AN_CLIENT || BLE_AN_SERVER
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
    ///Update new alert
    EACI_MSG_DATA_REQ_ANPS_NTF_NEW_ALERT = EACI_MSG_DATA_REQ_ANPC_MAX,
    ///Update unread alert
    EACI_MSG_DATA_REQ_ANPS_NTF_UNREAD_ALERT
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
#endif

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
    ///Number of Data Request of DISC
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
**************************************************************************
**************************************************************************
**/
///EACI Message Type
enum
{
    ///Reserved
    EACI_MSG_TYPE_RSV = 0x0,
    ///Command
    EACI_MSG_TYPE_CMD = 0xEA,
    ///Data Request
    EACI_MSG_TYPE_DATA_REQ,
    ///Data Indication
    EACI_MSG_TYPE_DATA_IND,
    ///Event
    EACI_MSG_TYPE_EVT,
    ///Data Error
    EACI_MSG_TYPE_DATA_ERROR = 0xFA,
    ///Number of Message Type
    EACI_MSG_TYPE_MAX
};

///EACI TX/RX states
enum
{
    ///HCI TX Start State - when packet is ready to be sent
    EACI_STATE_TX_ONGOING,
    ///HCI TX Done State - TX ended with no error
    EACI_STATE_TX_IDLE,
    ///EACI RX Start State - receive message type
    EACI_STATE_RX_START,
    ///EACI RX Header State - receive message header
    EACI_STATE_RX_HDR,
    ///EACI RX Header State - receive (rest of) message payload
    EACI_STATE_RX_PAYL,
    ///Number of states
    EACI_STATE_MAX
};

///EACI Error Reason
enum
{
    ///Eaci Type error
    EACI_TYPE_ERROR,
    ///Eaci MSG Out of Range
    EACI_MSG_OOR_ERROR,
    ///Number of Error
    EACI_ERROR_MAX
};

///EACI Environment context structure
struct eaci_env_tag
{
    ///Tx state - either transmitting or done.
    uint8_t tx_state;
    ///Queue of kernel messages corresponding to packets sent through HCI
    struct co_list queue_tx;
    ///Rx state - can be receiving message type, header, payload or error
    uint8_t rx_state;
    ///Message type 0x01,0x02,0x03,0x04
    uint8_t msg_type;
    ///Message id
    uint8_t msg_id;
    ///Message parameter length
    uint8_t param_len;
    ///Pointer to current ke_msg being manipulated
    uint8_t *param;
    ///Receive error
    bool error;
};

/*
 * GLOBAL VARIABLE DECLARATIONS
 ****************************************************************************************
 */
///EACI environment structure external global variable declaration
extern struct eaci_env_tag eaci_env;

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Initialize EACI interface
 *
 ****************************************************************************************
 */
void app_eaci_init(void);

/**
 ****************************************************************************************
 * @brief EACI send PDU
 *
 ****************************************************************************************
 */
void eaci_pdu_send(uint8_t len, uint8_t *par);

/**
 ****************************************************************************************
 * @brief After-process when one PDU has been sent.
 *
 ****************************************************************************************
 */
void eaci_tx_done(void);

/**
 ****************************************************************************************
 * @brief EACI uart message handler
 *
 ****************************************************************************************
 */
void app_eaci_msg_hdl(uint8_t msg_type, uint8_t msg_id, uint8_t param_len, uint8_t const *param);

/// @} EACI
#endif // APP_EACI_H_
