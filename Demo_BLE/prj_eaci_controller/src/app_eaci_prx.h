/**
 ****************************************************************************************
 *
 * @file app_eaci_prx.h
 *
 * @brief Application PRX ACI
 *
 * Copyright (C) Quintic 2012-2013
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */

#ifndef APP_EACI_PRX_H_
#define APP_EACI_PRX_H_

/**
 ****************************************************************************************
 * @brief PRX data request API
 *
 ****************************************************************************************
 */
#if BLE_PROX_MONITOR
void app_eaci_data_proxm_hdl(uint8_t msg_id, uint8_t param_len, uint8_t const *param);
#endif

/**
 ****************************************************************************************
 * @brief EACI application task message handler
 *
 ****************************************************************************************
 */
#if BLE_PROX_MONITOR || BLE_PROX_REPORTER
void prox_app_task_msg_hdl(ke_msg_id_t const msgid, void const *param);
#endif

#endif // APP_EACI_PRX_H_
