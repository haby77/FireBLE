/**
 ****************************************************************************************
 *
 * @file app_eaci_fm.h
 *
 * @brief Application FM ACI
 *
 * Copyright (C) Quintic 2012-2013
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */

#ifndef APP_EACI_FM_H_
#define APP_EACI_FM_H_

/**
 ****************************************************************************************
 * @brief FM data request API
 *
 ****************************************************************************************
 */
#if BLE_FINDME_LOCATOR
void app_eaci_data_fm_hdl(uint8_t msg_id, uint8_t param_len, uint8_t const *param);
#endif

/**
 ****************************************************************************************
 * @brief EACI application task message handler
 *
 ****************************************************************************************
 */
#if BLE_FINDME_LOCATOR || BLE_FINDME_TARGET
void findme_app_task_msg_hdl(ke_msg_id_t const msgid, void const *param);
#endif

#endif // APP_EACI_FM_H_
