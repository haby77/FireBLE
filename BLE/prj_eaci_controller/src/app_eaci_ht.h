/**
 ****************************************************************************************
 *
 * @file app_eaci_ht.h
 *
 * @brief Application HT ACI
 *
 * Copyright (C) Quintic 2012-2013
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */

#ifndef APP_EACI_HT_H_
#define APP_EACI_HT_H_

/**
 ****************************************************************************************
 * @brief HTC data request API
 *
 ****************************************************************************************
 */
#if BLE_HT_COLLECTOR
void app_eaci_data_htpc_hdl(uint8_t msg_id, uint8_t param_len, uint8_t const *param);
#endif
/**
 ****************************************************************************************
 * @brief HTT data request API
 *
 ****************************************************************************************
 */
#if BLE_HT_THERMOM
void app_eaci_data_htps_hdl(uint8_t msg_id, uint8_t param_len, uint8_t const *param);
#endif

/**
 ****************************************************************************************
 * @brief EACI application task message handler
 *
 ****************************************************************************************
 */
#if BLE_HT_COLLECTOR || BLE_HT_THERMOM
void ht_app_task_msg_hdl(ke_msg_id_t const msgid, void const *param);
#endif

#endif // APP_EACI_HT_H_
