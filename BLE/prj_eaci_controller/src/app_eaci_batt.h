/**
 ****************************************************************************************
 *
 * @file app_eaci_batt.h
 *
 * @brief Application BATT ACI
 *
 * Copyright (C) Quintic 2012-2013
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */

#ifndef APP_EACI_BATT_H_
#define APP_EACI_BATT_H_

/**
 ****************************************************************************************
 * @brief BASC data request API
 *
 ****************************************************************************************
 */
#if BLE_BATT_CLIENT
void app_eaci_data_basc_hdl(uint8_t msg_id, uint8_t param_len, uint8_t const *param);
#endif

/**
 ****************************************************************************************
 * @brief BASS data request API
 *
 ****************************************************************************************
 */
#if BLE_BATT_SERVER
void app_eaci_data_bass_hdl(uint8_t msg_id, uint8_t param_len, uint8_t const *param);
#endif
/**
 ****************************************************************************************
 * @brief EACI application task message handler
 *
 ****************************************************************************************
 */
#if BLE_BATT_CLIENT || BLE_BATT_SERVER
void batt_app_task_msg_hdl(ke_msg_id_t const msgid, void const *param);
#endif

#endif // APP_EACI_HRP_H_
