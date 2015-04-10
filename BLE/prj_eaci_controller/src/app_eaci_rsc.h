/**
 ****************************************************************************************
 *
 * @file app_eaci_rsc.h
 *
 * @brief Application RSC ACI
 *
 * Copyright (C) Quintic 2012-2013
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */

#ifndef APP_EACI_RSC_H_
#define APP_EACI_RSC_H_

/**
 ****************************************************************************************
 * @brief RSCPC data request API
 *
 ****************************************************************************************
 */
#if BLE_RSC_COLLECTOR
void app_eaci_data_rscpc_hdl(uint8_t msg_id, uint8_t param_len, uint8_t const *param);
#endif
/**
 ****************************************************************************************
 * @brief RSCPS data request API
 *
 ****************************************************************************************
 */
#if BLE_RSC_SENSOR
void app_eaci_data_rscps_hdl(uint8_t msg_id, uint8_t param_len, uint8_t const *param);
#endif

/**
 ****************************************************************************************
 * @brief EACI application task message handler
 *
 ****************************************************************************************
 */
#if BLE_RSC_COLLECTOR || BLE_RSC_SENSOR
void rsc_app_task_msg_hdl(ke_msg_id_t const msgid, void const *param);
#endif

#endif // APP_EACI_RSC_H_
