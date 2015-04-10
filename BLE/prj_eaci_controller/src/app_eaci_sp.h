/**
 ****************************************************************************************
 *
 * @file app_eaci_sp.h
 *
 * @brief Application SP ACI
 *
 * Copyright (C) Quintic 2012-2013
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */

#ifndef APP_EACI_SP_H_
#define APP_EACI_SP_H_

/**
 ****************************************************************************************
 * @brief SPC data request API
 *
 ****************************************************************************************
 */
#if BLE_SP_CLIENT
void app_eaci_data_spc_hdl(uint8_t msg_id, uint8_t param_len, uint8_t const *param);
#endif

/**
 ****************************************************************************************
 * @brief SPS data request API
 *
 ****************************************************************************************
 */
#if BLE_SP_SERVER
void app_eaci_data_sps_hdl(uint8_t msg_id);
#endif

/**
 ****************************************************************************************
 * @brief EACI application task message handler
 *
 ****************************************************************************************
 */
#if BLE_SP_CLIENT || BLE_SP_SERVER
void sp_app_task_msg_hdl(ke_msg_id_t const msgid, void const *param);
#endif

#endif // APP_EACI_SP_H_
