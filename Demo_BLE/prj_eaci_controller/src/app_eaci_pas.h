/**
 ****************************************************************************************
 *
 * @file app_eaci_pas.h
 *
 * @brief Application PAS ACI
 *
 * Copyright (C) Quintic 2012-2013
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */

#ifndef APP_EACI_PAS_H_
#define APP_EACI_PAS_H_

/**
 ****************************************************************************************
 * @brief PASPC data request API
 *
 ****************************************************************************************
 */
#if BLE_PAS_CLIENT
void app_eaci_data_paspc_hdl(uint8_t msg_id, uint8_t param_len, uint8_t const *param);
#endif
/**
 ****************************************************************************************
 * @brief PASPS data request API
 *
 ****************************************************************************************
 */
#if BLE_PAS_SERVER
void app_eaci_data_pasps_hdl(uint8_t msg_id, uint8_t param_len, uint8_t const *param);
#endif

/**
 ****************************************************************************************
 * @brief EACI application task message handler
 *
 ****************************************************************************************
 */
#if BLE_PAS_CLIENT || BLE_PAS_SERVER
void pas_app_task_msg_hdl(ke_msg_id_t const msgid, void const *param);
#endif

#endif // APP_EACI_PAS_H_
