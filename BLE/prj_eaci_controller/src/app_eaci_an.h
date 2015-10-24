/**
 ****************************************************************************************
 *
 * @file app_eaci_an.h
 *
 * @brief Application AN ACI
 *
 * Copyright(C) 2015 NXP Semiconductors N.V.
 * All rights reserved.
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */

#ifndef APP_EACI_AN_H_
#define APP_EACI_AN_H_

/**
 ****************************************************************************************
 * @brief ANPC data request API
 *
 ****************************************************************************************
 */
#if BLE_AN_CLIENT
void app_eaci_data_anpc_hdl(uint8_t msg_id, uint8_t param_len, uint8_t const *param);
#endif
/**
 ****************************************************************************************
 * @brief ANPS data request API
 *
 ****************************************************************************************
 */
#if BLE_AN_SERVER
void app_eaci_data_anps_hdl(uint8_t msg_id, uint8_t param_len, uint8_t const *param);
#endif

/**
 ****************************************************************************************
 * @brief EACI application task message handler
 *
 ****************************************************************************************
 */
#if BLE_AN_CLIENT || BLE_AN_SERVER
void an_app_task_msg_hdl(ke_msg_id_t const msgid, void const *param);
#endif

#endif // APP_EACI_AN_H_
