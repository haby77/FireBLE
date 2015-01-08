/**
 ****************************************************************************************
 *
 * @file app_eaci_dis.h
 *
 * @brief Application DIS ACI
 *
 * Copyright (C) Quintic 2012-2013
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */

#ifndef APP_EACI_DIS_H_
#define APP_EACI_DIS_H_

/**
 ****************************************************************************************
 * @brief DISC data request API
 *
 ****************************************************************************************
 */
#if BLE_DIS_CLIENT
void app_eaci_data_disc_hdl(uint8_t msg_id, uint8_t param_len, uint8_t const *param);
#endif

/**
 ****************************************************************************************
 * @brief DISS data request API
 *
 ****************************************************************************************
 */
#if BLE_DIS_SERVER
void app_eaci_data_diss_hdl(uint8_t msg_id, uint8_t param_len, uint8_t const *param);
#endif

/**
 ****************************************************************************************
 * @brief EACI application task message handler
 *
 ****************************************************************************************
 */
#if BLE_DIS_CLIENT
void dis_app_task_msg_hdl(ke_msg_id_t const msgid, void const *param);
#endif

#endif // APP_EACI_DIS_H_
