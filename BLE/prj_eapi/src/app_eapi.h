/**
 ****************************************************************************************
 *
 * @file app_eapi.h
 *
 * @brief Application EASY API
 *
 * Copyright (C) Quintic 2012-2013
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */

#ifndef _APP_EAPI_H_
#define _APP_EAPI_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "gap.h"
#include "gap_task.h"
#include "app_gap_task.h"

enum{
    /*all profile id*/
    ///Find Me profile
    EAPI_PRF_FINDME,
    ///Health Thermometer Profile
    EAPI_PRF_THERMOMETER,
    ///Proximity
    EAPI_PRF_PROXIMITY,
    ///Blood Pressure
    EAPI_PRF_BLOODPRESSURE,
    ///Heart Rate
    EAPI_PRF_HEARTRATE,
    ///Time
    EAPI_PRF_TIME,
    ///Device Information
    EAPI_PRF_DEVICEINFO,
    ///scan
    EAPI_PRF_SCAN,
    ///Battery
    EAPI_PRF_BATTERY,
    ///HID
    EAPI_PRF_HID,
    ///Alert Notification Profile
    EAPI_PRF_ALERT,
    ///Cycling Speed and Cadence Profile
    EAPI_PRF_CSC,
    ///Phone Alert Status Profile
    EAPI_PRF_PAS,
    ///Running Speed and Cadence Profile
    EAPI_PRF_RSC,
};

#if BLE_AN_SERVER
//ANPS Profile
struct app_anps_ntf_alert_cmd
{
    /// Connection Handle
    uint16_t conhdl;
    /// Operation Code (ANPS_UPD_NEW_ALERT_OP_CODE or ANPS_UPD_UNREAD_ALERT_STATUS_OP_CODE)
    uint8_t operation;
    /// New Alert Characteristic Value or Unread Alert Status Characteristic Value
    union anps_value_tag value;
};
#endif

///CSCPS (send the value to the client)
#if BLE_CSC_SENSOR
struct app_cscps_send_value
{
    uint8_t flag;
    ///status of control point
    uint8_t status;
    union 
    {
        ///send a CSC measurement notification
        struct cscps_ntf_csc_meas_cmd csc_meas;
        ///Value
        union cscps_sc_ctnl_pt_cfm_value value;
    }cscps_send;
    
};
#endif

//pasps Profile
#if BLE_PAS_SERVER
struct app_pasps_update_val
{
    /// Operation code(PASPS_UPD_ALERT_STATUS_OP_CODE or PASPS_UPD_RINGER_SETTING_OP_CODE)
    uint16_t operation;
    /// Alert Status value or Ringer Setting value
    uint8_t value;
};
#endif

///RSCPS (send the value to the client)
#if BLE_RSC_SENSOR
struct app_rscps_send_value
{
    uint8_t flag;
    ///status of control point
    uint8_t status;
    union 
    {
        ///send a RSC measurement notification
        struct rscps_ntf_rsc_meas_cmd rsc_meas;
        ///Value
        union rscps_sc_ctnl_pt_cfm_value value;
    }rscps_send;
    
};
#endif

#if BLE_RSC_COLLECTOR
//RSCP Profile
struct app_rscpc_value_ind
{
    /// Connection handle
    uint16_t conhdl;
    /// Attribute Code
    uint8_t att_code;
    /// Value
    union app_rscpc_value_tag
    {
        /// RSC Measurement
        struct rscp_rsc_meas rsc_meas;
        /// RSC Feature
        uint16_t sensor_feat;
        /// Sensor Location
        uint8_t sensor_loc;
        /// SC Control Point Response
        struct rscp_sc_ctnl_pt_rsp ctnl_pt_rsp;
        /// Client Characteristic Configuration Descriptor Value
        uint16_t ntf_cfg;
    } value;
};
#endif

///Time profile (send the value to the client)
#if BLE_TIP_SERVER
struct app_time_send_value
{
    uint8_t flag;
    uint8_t enable_ntf_send;
    union 
    {
        ///Send update current time request - at connection
        struct tip_curr_time temp_time;  
        ///Send update local time info request - at connection
        struct tip_loc_time_info loc_time_info;
        ///Send update reference time info request - at connection
        struct tip_ref_time_info ref_time_info;
        ///Send update time DST request - at connection
        struct tip_time_with_dst time_with_dst;
        ///Send update time update state request - at connection
        struct tip_time_upd_state time_upd_state;
    }time_send;
};
#endif

/// Thermometer (send the value to the client)
#if BLE_HT_THERMOM
struct app_thermometer_send_value
{
    uint8_t flag;
    ///Send Temperature Measurement value  
    struct htp_temp_meas temp_meas;
    uint8_t flag_stable_meas;
    ///Send Measurement Interval value 
    uint16_t meas_intv;
    ///Send temperature type
    uint8_t value;
};
#endif

///Blood Pressure(send the value to the client)
#if BLE_BP_SENSOR
struct app_bp_send_value
{
    uint8_t flag_interm;
    struct bps_bp_meas bp_val;
};
#endif

///Battery (send the value to the client)
#if BLE_BATT_SERVER
struct app_battery_send_value
{
    ///The instances of battery service
    uint8_t bas_instance;
    ///battery level
    uint8_t batt_lever;
};
#endif

///used to send the value to the client.
#if BLE_HT_THERMOM|BLE_BP_SENSOR|BLE_HR_SENSOR|BLE_TIP_SERVER|BLE_BATT_SERVER
union prf_server_value
{
    /*all profile id*/
    ///Health Thermometer Profile
#if BLE_HT_THERMOM
    struct app_thermometer_send_value ht_par;
#endif
    ///Blood Pressure profile
#if BLE_BP_SENSOR
    struct app_bp_send_value bp_par;
#endif
    ///Heart Rate profile
#if BLE_HR_SENSOR
    struct hrs_hr_meas hr_val;
#endif
    ///Time profile
#if BLE_TIP_SERVER
    struct app_time_send_value time_par;
#endif
    ///Battery profile
#if BLE_BATT_SERVER
    struct app_battery_send_value batt_val;
#endif
};
#endif

/// prf_client_write_char_req()
/// Health Thermometer Service Characteristics - Char. Code
enum
{
    /// Temperature Measurement
    APP_HTPC_CHAR_HTS_TEMP_MEAS,
    /// Temperature Type
    APP_HTPC_CHAR_HTS_TEMP_TYPE,
    /// Intermediate Temperature
    APP_HTPC_CHAR_HTS_INTM_TEMP,
    /// Measurement Interval
    APP_HTPC_CHAR_HTS_MEAS_INTV,
    /// Measurement interval write
    APP_HTPC_WR_MEAS_INTV_REQ,

    APP_HTPC_CHAR_HTS_MAX,
};

enum
{
    ///Read LLS Alert level
    APP_PROXM_RD_ALERT_LVL,
    ///Read TX Power Level
    APP_PROXM_RD_TXPW_LVL,
};

///prf_client_write_char_req
///Heart Rate profile
enum
{
    APP_HRPC_CFG_INDNTF,
    APP_HRPC_WR_CNTL_POINT,
};

///Time profile
enum
{
    APP_TIPC_CT_NTF_CFG,
    APP_TIPC_WR_TIME_UDP_CTNL,
};

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/*
 ****************************************************************************************
 * @brief Start and Stop the device to advertising process
 *
 ****************************************************************************************
 */
void eapi_app_gap_advertising(bool flag);

/*
 ****************************************************************************************
 * @brief This function is used to search devices within range or stop the search procedure.
 *
 ****************************************************************************************
 */
void eapi_app_gap_dev_scan(bool flag);

/*
 ****************************************************************************************
 * @brief This function is used to create a connection to a connectable device or 
 *        disconnect a connection.
 *
 ****************************************************************************************
 */
void eapi_app_gap_conn_req(bool flag, struct bd_addr * addr);

/*
 ****************************************************************************************
 * @brief Initiate encryption or pairing procedure.
 *
 ****************************************************************************************
 */
void eapi_app_gap_security_req(struct bd_addr * addr);

/*
 ****************************************************************************************
 * @brief This function is used to enable the specific profile.
 *
 ****************************************************************************************
 */
void eapi_prf_client_enable_req(uint16_t prf_id, uint16_t conhdl);

/*
 ****************************************************************************************    
 * 
 * This function is used to read the specific characteristic.
 *
 ****************************************************************************************
 */
void eapi_prf_client_rd_char_req(uint16_t prf_id, uint16_t conhdl, uint8_t char_code);

/*
 ****************************************************************************************    
 *
 * This function is used to write the specific characteristic.
 *
 ****************************************************************************************
 */
void eapi_prf_client_write_char_req(uint16_t prf_id, uint16_t conhdl, uint16_t char_code, uint32_t data);

/*
 ****************************************************************************************    
 *
 * This function is used to send the value to the client.
 *
 ****************************************************************************************
 */
#if BLE_HT_THERMOM|BLE_BP_SENSOR|BLE_HR_SENSOR|BLE_TIP_SERVER|BLE_BATT_SERVER
void eapi_prf_server_value_send(uint16_t prf_id, uint16_t conhdl, void *par);
#endif

/*
 ****************************************************************************************    
 *
 * This is the function of user.
 *
 ****************************************************************************************
 */
void app_task_msg_hdl(ke_msg_id_t msgid, void const *param);

#endif  /// APP_EAPI_H

