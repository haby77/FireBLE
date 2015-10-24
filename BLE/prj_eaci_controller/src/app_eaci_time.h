/**
 ****************************************************************************************
 *
 * @file app_eaci_time.h
 *
 * @brief Application TIME ACI
 *
 * Copyright(C) 2015 NXP Semiconductors N.V.
 * All rights reserved.
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */

#ifndef APP_EACI_TIME_H_
#define APP_EACI_TIME_H_

/**
 ****************************************************************************************
 * @brief TIPC data request API
 *
 ****************************************************************************************
 */
#if BLE_TIP_CLIENT
void app_eaci_data_tipc_hdl(uint8_t msg_id, uint8_t param_len, uint8_t const *param);
#endif
/**
 ****************************************************************************************
 * @brief TIPS data request API
 *
 ****************************************************************************************
 */
#if BLE_TIP_SERVER
void app_eaci_data_tips_hdl(uint8_t msg_id, uint8_t param_len, uint8_t const *param);
#endif

/**
 ****************************************************************************************
 * @brief EACI application task message handler
 *
 ****************************************************************************************
 */
#if BLE_TIP_CLIENT || BLE_TIP_SERVER
void tip_app_task_msg_hdl(ke_msg_id_t const msgid, void const *param);
#endif

#endif // APP_EACI_TIME_H_
