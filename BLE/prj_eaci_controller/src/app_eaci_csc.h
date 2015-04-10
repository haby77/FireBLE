/**
 ****************************************************************************************
 *
 * @file app_eaci_csc.h
 *
 * @brief Application CSC ACI
 *
 * Copyright (C) Quintic 2012-2013
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */

#ifndef APP_EACI_CSC_H_
#define APP_EACI_CSC_H_

/**
 ****************************************************************************************
 * @brief CSCPC data request API
 *
 ****************************************************************************************
 */
#if BLE_CSC_COLLECTOR
void app_eaci_data_cscpc_hdl(uint8_t msg_id, uint8_t param_len, uint8_t const *param);
#endif
/**
 ****************************************************************************************
 * @brief CSCPS data request API
 *
 ****************************************************************************************
 */
#if BLE_CSC_SENSOR
void app_eaci_data_cscps_hdl(uint8_t msg_id, uint8_t param_len, uint8_t const *param);
#endif

/**
 ****************************************************************************************
 * @brief EACI application task message handler
 *
 ****************************************************************************************
 */
#if BLE_CSC_COLLECTOR || BLE_CSC_SENSOR
void csc_app_task_msg_hdl(ke_msg_id_t const msgid, void const *param);
#endif

#endif // APP_EACI_CSC_H_
