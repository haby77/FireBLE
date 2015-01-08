/**
 ****************************************************************************************
 *
 * @file app_eapi.c
 *
 * @brief Application EASY API
 *
 * Copyright (C) Quintic 2012-2013
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */

#include "app_env.h"

/*
 ****************************************************************************************
 * @brief Start and Stop the device to advertising process
 *
 * @param[in] flag = FALSE;  stop advertising
 *            flag = TRUE;   start advertising
 ****************************************************************************************
 */
void eapi_app_gap_advertising(bool flag)
{
    if(flag)
    {
        // start advertising
        app_gap_adv_start_req(GAP_GEN_DISCOVERABLE|GAP_UND_CONNECTABLE,app_env.adv_data,
                                app_set_adv_data(GAP_GEN_DISCOVERABLE), app_env.scanrsp_data,
                                app_set_scan_rsp_data(app_get_local_service_flag()),
                                GAP_ADV_FAST_INTV1, GAP_ADV_FAST_INTV2); 
    }
    else
    {
        // stop advertising
        app_gap_adv_stop_req();
    }
}

/*
 ****************************************************************************************
 * @brief Scan the nearby BLE devices.   
 *
 * @param[in] flag = FALSE;  stop device scan
 *            flag = TRUE;   start device scan
 * @description
 *
 * This function is used to search devices within range or stop the search procedure.
 *
 ****************************************************************************************
 */

void eapi_app_gap_dev_scan(bool flag)
{
    if(flag)
    {
        // start device scan
        app_gap_dev_inq_req(GAP_GEN_INQ_TYPE,QN_ADDR_TYPE);
    }
    else
    {
        // stop device scan
        app_gap_dev_inq_cancel_req();
    }
}

/*
 ****************************************************************************************
 * @brief Create connection with the remote BLE device.       
 * 
 * @param[in] flag = FALSE,  disconnect ; flag= TRUE,   connect
 *
 * @param[in] addr The address of the remote device to which the connection will be created
 * @description
 *
 * This function is used to create a connection to a connectable device or disconnect a 
 * connection.
 *
 ****************************************************************************************
 */
void eapi_app_gap_conn_req(bool flag, struct bd_addr * addr)
{
    if(flag)
    {
        // connect
        for (uint8_t i = 0; i < app_env.inq_idx; i++)
        {
            if (co_bt_bdaddr_compare(&app_env.inq_addr[i], addr))
            {
                app_gap_le_create_conn_req(&app_env.inq_addr[i], 
                                            app_env.addr_type[i], 
                                            QN_ADDR_TYPE,
                                            GAP_INIT_CONN_MIN_INTV,
                                            GAP_INIT_CONN_MAX_INTV,
                                            GAP_CONN_SUPERV_TIMEOUT);
                break;
            }
        }
    }
    else
    {
        // disconnect
        uint8_t idx;
        uint16_t conhdl = app_get_conhdl_by_bdaddr(addr, &idx);
        if (conhdl != 0xFFFF)
            app_gap_discon_req(conhdl);
    }
}

/*
 ****************************************************************************************
 * @brief Initiate encryption or pairing procedure.       
 *
 * @param[in] Pointer to device address of peer
 *
 * @description
 *
 * This function is used to initiate encryption or pairing procedure.
 *
 ****************************************************************************************
 */
void eapi_app_gap_security_req(struct bd_addr *addr)
{
    app_gap_security_req(addr);
}

/*
 ****************************************************************************************    
 * 
 * @param[in] profile
 *
 * @param[in] conhdl :connection handle
 *
 * @description
 *
 * This function is used to enable the specific profile.
 *
 ****************************************************************************************
 */
void eapi_prf_client_enable_req(uint16_t prf_id, uint16_t conhdl)
{
    switch(prf_id)
    {
        //Find Me profile
#if BLE_FINDME_LOCATOR
        case EAPI_PRF_FINDME:
            app_findl_enable_req(NULL, conhdl);
            break;
#endif
        //Thermometer profile
#if BLE_HT_COLLECTOR
        case EAPI_PRF_THERMOMETER:
            app_htpc_enable_req(NULL, conhdl);
            break;
#endif
        //Proximity profile
#if BLE_PROX_MONITOR
        case EAPI_PRF_PROXIMITY:
            app_proxm_enable_req(NULL, NULL, NULL, conhdl);
            break;
#endif
        //Blood Pressure profile
#if BLE_BP_COLLECTOR
        case EAPI_PRF_BLOODPRESSURE:
            app_blpc_enable_req(NULL, conhdl);
            break;
#endif
        //Heart Rate profile
#if BLE_HR_COLLECTOR
        case EAPI_PRF_HEARTRATE:
            app_hrpc_enable_req(NULL, conhdl);
            break;
#endif
        //Time profile
#if BLE_TIP_CLIENT
        case EAPI_PRF_TIME:
            app_tipc_enable_req(NULL, NULL, NULL, conhdl);
            break;
#endif
        //Device Information profile
#if BLE_DIS_CLIENT
        case EAPI_PRF_DEVICEINFO:
            app_disc_enable_req(NULL, conhdl);
            break;
#endif
        //Scan profile
#if BLE_SP_CLIENT
        case EAPI_PRF_SCAN:
            app_scppc_enable_req(0x0012, 0x0012, NULL, conhdl);
            break;
#endif
        //Battery profile
#if BLE_BATT_CLIENT
        case EAPI_PRF_BATTERY:
            app_basc_enable_req(0, NULL, conhdl);
            break;
#endif
        //Alert Notification profile
#if BLE_AN_CLIENT
        case EAPI_PRF_ALERT:
            app_anpc_enable_req(NULL, NULL, NULL, conhdl);
            break;
#endif
        //Cycling Speed and Cadence Profile
#if BLE_CSC_COLLECTOR
        case EAPI_PRF_CSC:
            app_cscpc_enable_req(NULL, conhdl);
            break;
#endif
        //Phone Alert Status Profile
#if BLE_PAS_CLIENT
        case EAPI_PRF_PAS:
            app_paspc_enable_req(NULL, conhdl);
            break;
#endif
        //Running Speed and Cadence Profile
#if BLE_RSC_COLLECTOR
        case EAPI_PRF_RSC:
            app_rscpc_enable_req(NULL, conhdl);
            break;
#endif
        default:
            break;
    }
}

/*
 ****************************************************************************************    
 * 
 * @description
 *
 * This function is used to read the specific characteristic.
 *
 ****************************************************************************************
 */
void eapi_prf_client_rd_char_req(uint16_t prf_id, uint16_t conhdl, uint8_t char_code)
{
    switch(prf_id)
    {
        //Thermometer profile
#if BLE_HT_COLLECTOR
        case EAPI_PRF_THERMOMETER:
            //Generic message to read a HTs or DIS characteristic value
            app_htpc_rd_char_req(char_code, conhdl);
            break;
#endif
        //Proximity profile
#if BLE_PROX_MONITOR
        case EAPI_PRF_PROXIMITY:
            if (char_code)
            {
                ///Generic message to Read TX Power Level
                app_proxm_rd_txpw_lvl_req(conhdl);
            }
            else
            {
                ///Generic message to Read LLS Alert level
                app_proxm_rd_alert_lvl_req(conhdl);
            }
            break;
#endif
        //Blood Pressure profile
#if BLE_BP_COLLECTOR
        case EAPI_PRF_BLOODPRESSURE:
            //read a BPS characteristic value
            app_blpc_rd_char_req(char_code, conhdl);
            break;
#endif
        //Heart Rate profile
#if BLE_HR_COLLECTOR
        case EAPI_PRF_HEARTRATE:
            //Generic message to read a HRS characteristic value
            app_hrpc_rd_char_req(char_code, conhdl);
            break;
#endif     
        //Time profile
#if BLE_TIP_CLIENT    
        case EAPI_PRF_TIME:
            //read a CTS or NDCS or RTUS characteristic value
            app_tipc_rd_char_req(char_code, conhdl);
            break;
#endif
        //Device Information profile
#if BLE_DIS_CLIENT    
        case EAPI_PRF_DEVICEINFO:
            app_disc_rd_char_req(char_code, conhdl);
            break;
#endif
        //Scan profile
#if BLE_SP_CLIENT    
        case EAPI_PRF_SCAN:
            //Read Scan Refresh Notification Configuration Value request
            app_scppc_scan_refresh_ntf_cfg_rd_req(conhdl);
            break;
#endif          
        //Battery profile
#if BLE_BATT_CLIENT        
        case EAPI_PRF_BATTERY:
            app_basc_rd_char_req(char_code, 0, conhdl);
            break;
#endif
        //Alert Notification profile
#if BLE_AN_CLIENT
        case EAPI_PRF_ALERT:
            app_anpc_rd_char_req(char_code, conhdl);
            break;
#endif
        //Cycling Speed and Cadence Profile
#if BLE_CSC_COLLECTOR
        case EAPI_PRF_CSC:
            app_cscpc_read_req(char_code, conhdl);
            break;
#endif
        //Phone Alert Status Profile
#if BLE_PAS_CLIENT
        case EAPI_PRF_PAS:
            app_paspc_read_req(char_code, conhdl);
            break;
#endif
        //Running Speed and Cadence Profile
#if BLE_RSC_COLLECTOR
        case EAPI_PRF_RSC:
            app_rscpc_read_req(char_code, conhdl);
            break;
#endif
        default:
            break;
    }
}

/*
 ****************************************************************************************    
 * 
 * @description
 *
 * This function is used to write the specific characteristic.
 *
 ****************************************************************************************
 */
void eapi_prf_client_write_char_req(uint16_t prf_id, uint16_t conhdl, uint16_t char_code, uint32_t data)
{
    switch(prf_id)
    {
        //Find Me profile
#if BLE_FINDME_LOCATOR
        case EAPI_PRF_FINDME:
            //Alert level set
            //FINDL_ALERT_NONE  FINDL_ALERT_MILD  FINDL_ALERT_HIGH
            app_findl_set_alert_req(char_code, conhdl);
            break;
#endif
        //Thermometer profile
#if BLE_HT_COLLECTOR        
        case EAPI_PRF_THERMOMETER:
            if (char_code > APP_HTPC_CHAR_HTS_MEAS_INTV)
            {
                app_htpc_wr_meas_intv_req((uint16_t)data, conhdl);
            }
            else
            {
                app_htpc_cfg_indntf_req(char_code, (uint16_t)data, conhdl);
            }
            break;      
#endif
        //Proximity profile
#if BLE_PROX_MONITOR            
        case EAPI_PRF_PROXIMITY:
            app_proxm_wr_alert_lvl_req(char_code, (uint16_t)data, conhdl);
            break;
#endif
        //Blood Pressure profile
#if BLE_BP_COLLECTOR        
        case EAPI_PRF_BLOODPRESSURE:
            //Generic message for configuring the 2 characteristics that can be handled.
            app_blpc_cfg_indntf_req(char_code, (uint16_t)data, conhdl);
            break;
#endif
        //Heart Rate profile
#if BLE_HR_COLLECTOR        
        case EAPI_PRF_HEARTRATE:
            if (char_code)
            {
                app_hrpc_wr_cntl_point_req((uint16_t)data, conhdl);
            }
            else
            {
                app_hrpc_cfg_indntf_req((uint16_t)data, conhdl);
            }
            break;
#endif
#if BLE_TIP_CLIENT
        //Time profile
        case EAPI_PRF_TIME:
        {
            if (char_code)
            {
                app_tipc_wr_time_udp_ctnl_pt_req((uint16_t)data, conhdl);
            }
            else
            {
                app_tipc_ct_ntf_cfg_req((uint16_t)data, conhdl);
            }
        }
            break;
#endif
#if BLE_SP_CLIENT
        //Scan profile
        case EAPI_PRF_SCAN:
        {
            if (char_code)
            {
                ///scan_intv = char_code     scan_window = cfg_val
                app_scppc_scan_intv_wd_wr_req(char_code, (uint16_t)data, conhdl);
            }
            else
            {
                app_scppc_wr_meas_intv_req((uint16_t)data, conhdl);
            }
        }
            break;
#endif
#if BLE_BATT_CLIENT
        //Battery profile
        case EAPI_PRF_BATTERY:
        {
            ///Write Battery Level Notification Configuration Value request
            app_basc_cfg_indntf_req((uint16_t)data, 0, conhdl);
        }
            break;
#endif
        //Alert Notification profile
#if BLE_AN_CLIENT
        case EAPI_PRF_ALERT:
        {
            union anpc_write_value_tag value;
            if (char_code == ANPC_RD_WR_NEW_ALERT_CFG)
            {
                value.new_alert_ntf_cfg = (uint16_t)data; // Notify
            }
            else if (char_code == ANPC_RD_WR_UNREAD_ALERT_STATUS_CFG)
            {
                value.unread_alert_status_ntf_cfg = (uint16_t)data; // Notify
            }
            app_anpc_wr_char_req(char_code, &value, conhdl);
        }
            break;
#endif
        //Cycling Speed and Cadence Profile
#if BLE_CSC_COLLECTOR
        case EAPI_PRF_CSC:
        {
            app_cscpc_cfg_ntfind_req(char_code, (uint16_t)data, conhdl);
        }
            break;
#endif
        //Phone Alert Status Profile
#if BLE_PAS_CLIENT
        case EAPI_PRF_PAS:
        {
            union write_value_tag value;
            value.alert_status_ntf_cfg = (uint16_t)data;
            app_paspc_wr_char_req(char_code, &value, conhdl);
        }
            break;
#endif
        //Running Speed and Cadence Profile
#if BLE_RSC_COLLECTOR
        case EAPI_PRF_RSC:
            if (char_code > RSCP_CTNL_PT_OP_REQ_SUPP_LOC)
            {
                app_rscpc_cfg_ntfind_req(char_code, (uint16_t)data, conhdl);
            }
            else
            {
                struct rscp_sc_ctnl_pt_req sc_ctnl_pt;
                
                sc_ctnl_pt.op_code = char_code;
                switch (char_code)
                {
                    case RSCP_CTNL_PT_OP_SET_CUMUL_VAL:
                        sc_ctnl_pt.value.cumul_val = data;
                        break;
                    case RSCP_CTNL_PT_OP_START_CALIB:
                        sc_ctnl_pt.value.cumul_val = data;
                        break;
                    case RSCP_CTNL_PT_OP_UPD_LOC:
                        sc_ctnl_pt.value.sensor_loc = (uint16_t)data;
                        break;
                    case RSCP_CTNL_PT_OP_REQ_SUPP_LOC:
                        sc_ctnl_pt.value.sensor_loc = (uint16_t)data;
                        break;
                    default:
                        break;
                }

                app_rscpc_ctnl_pt_cfg_req(&sc_ctnl_pt, conhdl);
            }
            break;
#endif
        default:
            break;
    }
}

/*
 ****************************************************************************************
 * 
 * @param[in] profile
 *
 * @param[in] connect handle
 *
 * @description
 *
 * This function is used to send the value to the client.
 *
 ****************************************************************************************
 */
#if BLE_HT_THERMOM|BLE_BP_SENSOR|BLE_HR_SENSOR|BLE_TIP_SERVER|BLE_BATT_SERVER|BLE_AN_SERVER
void eapi_prf_server_value_send(uint16_t prf_id, uint16_t conhdl, void *par)
{
    switch(prf_id)
    {
#if BLE_HT_THERMOM
        //Thermometer profile
        case EAPI_PRF_THERMOMETER:
        {
            uint8_t i = ((struct app_thermometer_send_value *)par)->flag;
            switch(i)
            {
                case 0:
                    app_htpt_temp_send(conhdl, &(((struct app_thermometer_send_value *)par)->temp_meas),
                                        ((struct app_thermometer_send_value *)par)->flag_stable_meas);                 
                    break;
                case 1:
                    app_htpt_measurement_intv_send(conhdl, ((struct app_thermometer_send_value *)par)->meas_intv);
                    break;
                case 2:
                    app_htpt_temp_type_send(((struct app_thermometer_send_value *)par)->value);
                    break;
                default:
                    break;
            }
         }
            break;
#endif
#if BLE_BP_SENSOR
        //Blood Pressure profile
        case EAPI_PRF_BLOODPRESSURE:
        {
            app_blps_pressure_send_req(conhdl, ((struct app_bp_send_value *)par)->flag_interm,
                                                &(((struct app_bp_send_value *)par)->bp_val));
        }
            break;
#endif
#if BLE_HR_SENSOR
        //Heart Rate profile
        case EAPI_PRF_HEARTRATE:
        {
            app_hrps_measurement_send(conhdl, (struct hrs_hr_meas *)&par);
        }
            break;
#endif
#if BLE_TIP_SERVER
        //Time profile
        case EAPI_PRF_TIME:
        {
            uint8_t time_par = ((struct app_time_send_value *)&par)->flag;
            switch(time_par)
            {
                case 0:
                    app_tips_upd_curr_time_req(conhdl, &(((struct app_time_send_value *)&par)->time_send.temp_time),
                                                ((struct app_time_send_value *)&par)->enable_ntf_send);
                    break;
                case 1:
                    app_tips_upd_local_time_info_req(conhdl, &(((struct app_time_send_value *)&par)->time_send.loc_time_info));
                    break;
                case 2:
                    app_tips_upd_ref_time_info_req(conhdl, &(((struct app_time_send_value *)&par)->time_send.ref_time_info));
                    break;
                case 3:
                    app_tips_upd_time_dst_req(conhdl, &(((struct app_time_send_value *)&par)->time_send.time_with_dst));
                    break;
                case 4:
                    app_tips_upd_time_upd_state_req(conhdl, &(((struct app_time_send_value *)&par)->time_send.time_upd_state));
                    break;
                default :
                    break;
            }
        }
            break;
#endif
#if BLE_SP_SERVER
        //Scan profile
        case EAPI_PRF_SCAN:
        {
            app_scpps_scan_refresh_req(conhdl, 0);
        }
            break;
#endif
#if BLE_BATT_SERVER
        //Battery profile
        case EAPI_PRF_BATTERY:
        {
            app_bass_batt_level_upd_req(conhdl, ((struct app_battery_send_value *)par)->bas_instance,
                                        ((struct app_battery_send_value *)par)->batt_lever);
        }
            break;
#endif
#if BLE_AN_SERVER
        //Alert Notification Profile
        case EAPI_PRF_ALERT:
        {
            app_anps_ntf_alert_cmd(conhdl, ((struct app_anps_ntf_alert_cmd *)par)->operation,
                                    (&((struct app_anps_ntf_alert_cmd *)par)->value));
        }
            break;
#endif
#if BLE_CSC_SENSOR
        case EAPI_PRF_CSC:
        {
            uint8_t flag = ((struct app_cscps_send_value *)par)->flag;
            switch(flag)
            {
                case 1:
                    app_cscps_ntf_csc_meas_req(conhdl, ((struct app_cscps_send_value *)par)->cscps_send.csc_meas.flags,
                                                        ((struct app_cscps_send_value *)par)->cscps_send.csc_meas.cumul_crank_rev,
                                                        ((struct app_cscps_send_value *)par)->cscps_send.csc_meas.last_crank_evt_time,
                                                        ((struct app_cscps_send_value *)par)->cscps_send.csc_meas.last_wheel_evt_time,
                                                        ((struct app_cscps_send_value *)par)->cscps_send.csc_meas.wheel_rev);
                    break;
                case 2:
                    app_cscps_sc_ctnl_pt_cfm(conhdl, ((struct app_cscps_send_value *)par)->status,
                                                        &((struct app_cscps_send_value *)par)->cscps_send.value);
                    break;
                default:
                    break;
            }
        }
            break;
#endif
#if BLE_PAS_SERVER
        case EAPI_PRF_PAS:
        {
            app_pasps_update_char_val_req(conhdl, ((struct app_pasps_update_val *)par)->operation,
                                                    ((struct app_pasps_update_val *)par)->value);
        }
            break;
#endif
#if BLE_RSC_SENSOR
        case EAPI_PRF_RSC:
        {
            uint8_t flag = ((struct app_rscps_send_value *)par)->flag;
            switch(flag)
            {
                case 1:
                    app_rscps_ntf_rsc_meas_req(conhdl, ((struct app_rscps_send_value *)par)->rscps_send.rsc_meas.flags,
                                                        ((struct app_rscps_send_value *)par)->rscps_send.rsc_meas.inst_cad,
                                                        ((struct app_rscps_send_value *)par)->rscps_send.rsc_meas.inst_speed,
                                                        ((struct app_rscps_send_value *)par)->rscps_send.rsc_meas.inst_stride_len,
                                                        ((struct app_rscps_send_value *)par)->rscps_send.rsc_meas.total_dist);
                    break;
                case 2:
                    app_rscps_sc_ctnl_pt_cfm(conhdl, ((struct app_rscps_send_value *)par)->status,
                                                        &((struct app_rscps_send_value *)par)->rscps_send.value);
                    break;
                default:
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

/*
 ****************************************************************************************    
 * 
 * @param[in] message id
 *
 * @param[in] data
 *
 * @description
 *
 * This function is used to send the value to the client.
 *
 ****************************************************************************************
 */

void app_task_msg_hdl(ke_msg_id_t msgid, void const *param)
{
    switch(msgid)
    {
        case GAP_DEV_INQ_RESULT_EVT:
            QPRINTF("* Message ID: GAP_DEV_INQ_RESULT_EVT \r\n");
            break;
        case GAP_DEV_INQ_REQ_CMP_EVT:
            QPRINTF("* Message ID: GAP_DEV_INQ_REQ_CMP_EVT \r\n");
            break;
        case GAP_SET_MODE_REQ_CMP_EVT:
            QPRINTF("* Message ID: GAP_SET_MODE_REQ_CMP_EVT \r\n");
            break;
        case GAP_ADV_REQ_CMP_EVT:
            QPRINTF("* Message ID: GAP_ADV_REQ_CMP_EVT \r\n");
            break;
        case GAP_LE_CREATE_CONN_REQ_CMP_EVT:
            QPRINTF("* Message ID: GAP_LE_CREATE_CONN_REQ_CMP_EVT \r\n");
            break;
        case GAP_DISCON_CMP_EVT:
            QPRINTF("* Message ID: GAP_DISCON_CMP_EVT \r\n");
            break;
        case GAP_BOND_REQ_CMP_EVT:
            QPRINTF("* Message ID: GAP_BOND_REQ_CMP_EVT \r\n");
            break;
#if BLE_HT_COLLECTOR
        case HTPC_ENABLE_CFM:
            QPRINTF("* Message ID: HTPC_ENABLE_CFM \r\n");
            break;
        case HTPC_RD_CHAR_RSP:
            QPRINTF("* Message ID: HTPC_RD_CHAR_RSP \r\n");
            break;
        case HTPC_TEMP_IND:
            QPRINTF("* Message ID: HTPC_TEMP_IND \r\n");
            break;
        case HTPC_MEAS_INTV_IND:
            QPRINTF("* Message ID: HTPC_MEAS_INTV_IND \r\n");
            break;
#endif
#if BLE_HT_THERMOM
        case HTPT_MEAS_INTV_CHG_IND:
            QPRINTF("* Message ID: GAP_BOND_REQ_CMP_EVT \r\n");
            break;
        case HTPT_CFG_INDNTF_IND:
            QPRINTF("* Message ID: HTPT_CFG_INDNTF_IND \r\n");
            break;
#endif
#if BLE_BP_COLLECTOR
        case BLPC_ENABLE_CFM:
            QPRINTF("* Message ID: BLPC_ENABLE_CFM \r\n");
            break;
        case BLPC_RD_CHAR_RSP:
            QPRINTF("* Message ID: BLPC_RD_CHAR_RSP \r\n");
            break;
        case BLPC_BP_MEAS_IND:
            QPRINTF("* Message ID: BLPC_BP_MEAS_IND \r\n");
            break;
#endif
#if BLE_BP_SENSOR
        case BLPS_CFG_INDNTF_IND:
            QPRINTF("* Message ID: BLPS_CFG_INDNTF_IND \r\n");
            break;
#endif
#if BLE_HR_COLLECTOR
        case HRPC_ENABLE_CFM:
            QPRINTF("* Message ID: HRPC_ENABLE_CFM \r\n");
            break;
        case HRPC_RD_CHAR_RSP:
            QPRINTF("* Message ID: HRPC_RD_CHAR_RSP \r\n");
            break;
        case HRPC_HR_MEAS_IND:
            QPRINTF("* Message ID: HRPC_HR_MEAS_IND \r\n");
            break;
#endif
#if BLE_HR_SENSOR
        case HRPS_CFG_INDNTF_IND:
            QPRINTF("* Message ID: HRPS_CFG_INDNTF_IND \r\n");
            break;
        case HRPS_ENERGY_EXP_RESET_IND:
            QPRINTF("* Message ID: HRPS_ENERGY_EXP_RESET_IND \r\n");
            break;
#endif
#if BLE_GL_COLLECTOR
        case GLPC_ENABLE_CFM:
            QPRINTF("* Message ID: GLPC_ENABLE_CFM \r\n");
            break;
        case GLPC_REGISTER_CFM:
            QPRINTF("* Message ID: GLPC_REGISTER_CFM \r\n");
            break;
        case GLPC_READ_FEATURES_RSP:
            QPRINTF("* Message ID: GLPC_READ_FEATURES_RSP \r\n");
            break;
        case GLPC_RACP_RSP:
            QPRINTF("* Message ID: GLPC_RACP_RSP \r\n");
            break;
        case GLPC_MEAS_IND:
            QPRINTF("* Message ID: GLPC_MEAS_IND \r\n");
            break;
        case GLPC_MEAS_CTX_IND:
            QPRINTF("* Message ID: GLPC_MEAS_CTX_IND \r\n");
            break;
#endif
#if BLE_GL_SENSOR
        case GLPS_CFG_INDNTF_IND:
            QPRINTF("* Message ID: GLPS_CFG_INDNTF_IND \r\n");
            break;
        case GLPS_RACP_REQ_IND:
            QPRINTF("* Message ID: GLPS_RACP_REQ_IND \r\n");
            break;
#endif
#if BLE_FINDME_LOCATOR
        case FINDL_ENABLE_CFM:
            QPRINTF("* Message ID: FINDL_ENABLE_CFM \r\n");
            break;
#endif
#if BLE_FINDME_TARGET
        case FINDT_ALERT_IND:
            QPRINTF("* Message ID: FINDT_ALERT_IND \r\n");
            break;
#endif
#if BLE_PROX_MONITOR
        case PROXM_ENABLE_CFM:
            QPRINTF("* Message ID: PROXM_ENABLE_CFM \r\n");
            break;
        case PROXM_RD_CHAR_RSP:
            QPRINTF("* Message ID: PROXM_RD_CHAR_RSP \r\n");
            break;
#endif
#if BLE_PROX_REPORTER
        case PROXR_ALERT_IND:
            QPRINTF("* Message ID: PROXR_ALERT_IND \r\n");
            break;
#endif
#if BLE_TIP_CLIENT
        case TIPC_ENABLE_CFM:
            QPRINTF("* Message ID: TIPC_ENABLE_CFM \r\n");
            break;
        case TIPC_CT_IND:
            QPRINTF("* Message ID: TIPC_CT_IND \r\n");
            break;
        case TIPC_CT_NTF_CFG_RD_RSP:
            QPRINTF("* Message ID: TIPC_CT_NTF_CFG_RD_RSP \r\n");
            break;
        case TIPC_LTI_RD_RSP:
            QPRINTF("* Message ID: TIPC_LTI_RD_RSP \r\n");
            break;
        case TIPC_RTI_RD_RSP:
            QPRINTF("* Message ID: TIPC_RTI_RD_RSP \r\n");
            break;
        case TIPC_TDST_RD_RSP:
            QPRINTF("* Message ID: TIPC_TDST_RD_RSP \r\n");
            break;
        case TIPC_TUS_RD_RSP:
            QPRINTF("* Message ID: TIPC_TUS_RD_RSP \r\n");
            break;
#endif
#if BLE_TIP_SERVER
        case TIPS_CURRENT_TIME_CCC_IND:
            QPRINTF("* Message ID: TIPS_CURRENT_TIME_CCC_IND \r\n");
            break;
        case TIPS_TIME_UPD_CTNL_PT_IND:
            QPRINTF("* Message ID: TIPS_TIME_UPD_CTNL_PT_IND \r\n");
            break;
#endif
#if BLE_SP_CLIENT
        case SCPPC_ENABLE_CFM:
            QPRINTF("* Message ID: SCPPC_ENABLE_CFM \r\n");
            break;
        case SCPPC_SCAN_REFRESH_NTF_CFG_RD_RSP:
            QPRINTF("* Message ID: SCPPC_SCAN_REFRESH_NTF_CFG_RD_RSP \r\n");
            break;
#endif
#if BLE_SP_SERVER
        case SCPPS_SCAN_INTV_WD_IND:
            QPRINTF("* Message ID: SCPPS_SCAN_INTV_WD_IND \r\n");
            break;
        case SCPPS_SCAN_REFRESH_NTF_CFG_IND:
            QPRINTF("* Message ID: SCPPS_SCAN_REFRESH_NTF_CFG_IND \r\n");
            break;
#endif
#if BLE_DIS_CLIENT
        case DISC_ENABLE_CFM:
            QPRINTF("* Message ID: DISC_ENABLE_CFM \r\n");
            break;
        case DISC_RD_CHAR_RSP:
            QPRINTF("* Message ID: DISC_RD_CHAR_RSP \r\n");
            break;
#endif
#if BLE_BATT_CLIENT
        case BASC_ENABLE_CFM:
            QPRINTF("* Message ID: BASC_ENABLE_CFM \r\n");
            break;
        case BASC_BATT_LEVEL_NTF_CFG_RD_RSP:
            QPRINTF("* Message ID: BASC_BATT_LEVEL_NTF_CFG_RD_RSP \r\n");
            break;
        case BASC_BATT_LEVEL_PRES_FORMAT_RD_RSP:
            QPRINTF("* Message ID: BASC_BATT_LEVEL_PRES_FORMAT_RD_RSP \r\n");
            break;
        case BASC_BATT_LEVEL_IND:
            QPRINTF("* Message ID: BASC_BATT_LEVEL_IND \r\n");
            break;
#endif
#if BLE_BATT_SERVER
        case BASS_BATT_LEVEL_NTF_CFG_IND:
            QPRINTF("* Message ID: BASS_BATT_LEVEL_NTF_CFG_IND \r\n");
            break;
#endif
#if BLE_AN_CLIENT
        case ANPC_CMP_EVT:
            QPRINTF("* Message ID: ANPC_CMP_EVT \r\n");
            break;
        case ANPC_VALUE_IND:
            QPRINTF("* Message ID: ANPC_VALUE_IND \r\n");
            break;
#endif
#if BLE_AN_SERVER
        case ANPS_NTF_IMMEDIATE_REQ_IND:
            QPRINTF("* Message ID: ANPS_NTF_IMMEDIATE_REQ_IND \r\n");
            break;
        case ANPS_NTF_STATUS_UPDATE_IND:
            QPRINTF("* Message ID: ANPS_NTF_STATUS_UPDATE_IND \r\n");
            break;
        case ANPS_CMP_EVT:
            QPRINTF("* Message ID: ANPS_CMP_EVT \r\n");
            break;
#endif
#if BLE_CSC_COLLECTOR
        case CSCPC_CMP_EVT:
            QPRINTF("* Message ID: CSCPC_CMP_EVT \r\n");
            break;
        case CSCPC_VALUE_IND:
            QPRINTF("* Message ID: CSCPC_VALUE_IND \r\n");
            break;
#endif
#if BLE_CSC_SENSOR
        case CSCPS_SC_CTNL_PT_REQ_IND:
            QPRINTF("* Message ID: CSCPS_SC_CTNL_PT_REQ_IND \r\n");
            break;
        case CSCPS_NTF_IND_CFG_IND:
            QPRINTF("* Message ID: CSCPS_NTF_IND_CFG_IND \r\n");
            break;
        case CSCPS_CMP_EVT:
            QPRINTF("* Message ID: CSCPS_CMP_EVT \r\n");
            break;
#endif
#if BLE_PAS_CLIENT
        case PASPC_CMP_EVT:
            QPRINTF("* Message ID: PASPC_CMP_EVT \r\n");
            break;
        case PASPC_VALUE_IND:
            QPRINTF("* Message ID: PASPC_VALUE_IND \r\n");
            break;
#endif
#if BLE_PAS_SERVER
        case PASPS_WRITTEN_CHAR_VAL_IND:
            QPRINTF("* Message ID: PASPS_WRITTEN_CHAR_VAL_IND \r\n");
            break;
        case PASPS_CMP_EVT:
            QPRINTF("* Message ID: PASPS_CMP_EVT \r\n");
            break;
#endif
#if BLE_RSC_COLLECTOR
        case RSCPC_CMP_EVT:
            QPRINTF("* Message ID: RSCPC_CMP_EVT \r\n");
            break;
        case RSCPC_VALUE_IND:
        {
            uint8_t rsc_meas = ((struct app_rscpc_value_ind *)param)->value.rsc_meas.inst_cad;
            QPRINTF(" RSC Measurement %d\r\n", rsc_meas);
        }
            QPRINTF("* Message ID: RSCPC_VALUE_IND \r\n");
            break;
#endif
#if BLE_RSC_SENSOR
        case RSCPS_SC_CTNL_PT_REQ_IND:
            QPRINTF("* Message ID: RSCPS_SC_CTNL_PT_REQ_IND \r\n");
            break;
        case RSCPS_NTF_IND_CFG_IND:
            QPRINTF("* Message ID: RSCPS_NTF_IND_CFG_IND \r\n");
            break;
        case RSCPS_CMP_EVT:
            QPRINTF("* Message ID: RSCPS_CMP_EVT \r\n");
            break;
#endif
        default:
            break;
    }
}


