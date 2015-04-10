/**
 ****************************************************************************************
 *
 * @file app_eaci_hrp.h
 *
 * @brief Application HRPC API
 *
 * Copyright (C) Quintic 2012-2013
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */

#ifndef APP_EACI_HRP_H_
#define APP_EACI_HRP_H_

/**
 ****************************************************************************************
 * @brief HRPC data request API
 *
 ****************************************************************************************
 */
#if BLE_HR_COLLECTOR
void app_eaci_data_hrpc_hdl(uint8_t msg_id, uint8_t param_len, uint8_t const *param);
#endif
/**
 ****************************************************************************************
 * @brief HRPS data request API
 *
 ****************************************************************************************
 */
#if BLE_HR_SENSOR
void app_eaci_data_hrps_hdl(uint8_t msg_id, uint8_t param_len, uint8_t const *param);
#endif

/**
 ****************************************************************************************
 * @brief EACI application task message handler
 *
 ****************************************************************************************
 */
#if BLE_HR_COLLECTOR || BLE_HR_SENSOR
void hr_app_task_msg_hdl(ke_msg_id_t const msgid, void const *param);
#endif

#endif // APP_EACI_HRP_H_
