/**
 ****************************************************************************************
 *
 * @file app_eaci_blp.h
 *
 * @brief Application BLP ACI
 *
 * Copyright (C) Quintic 2012-2013
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */

#ifndef APP_EACI_BLP_H_
#define APP_EACI_BLP_H_

/**
 ****************************************************************************************
 * @brief BLPC data request API
 *
 ****************************************************************************************
 */
#if BLE_BP_COLLECTOR
void app_eaci_data_bpc_hdl(uint8_t msg_id, uint8_t param_len, uint8_t const *param);
#endif
/**
 ****************************************************************************************
 * @brief BLPS data request API
 *
 ****************************************************************************************
 */
#if BLE_BP_SENSOR
void app_eaci_data_bps_hdl(uint8_t msg_id, uint8_t param_len, uint8_t const *param);
#endif

/**
 ****************************************************************************************
 * @brief EACI application task message handler
 *
 ****************************************************************************************
 */
#if BLE_BP_COLLECTOR || BLE_BP_SENSOR
void bp_app_task_msg_hdl(ke_msg_id_t const msgid, void const *param);
#endif

#endif // APP_EACI_BLP_H_
