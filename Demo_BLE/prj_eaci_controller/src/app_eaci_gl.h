/**
 ****************************************************************************************
 *
 * @file app_eaci_gl.h
 *
 * @brief Application GL ACI
 *
 * Copyright (C) Quintic 2012-2013
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */

#ifndef APP_EACI_GL_H_
#define APP_EACI_GL_H_

/**
 ****************************************************************************************
 * @brief GLC data request API
 *
 ****************************************************************************************
 */
#if BLE_GL_COLLECTOR
void app_eaci_data_glc_hdl(uint8_t msg_id, uint8_t param_len, uint8_t const *param);
#endif
#if BLE_GL_SENSOR
void app_eaci_data_gls_hdl(uint8_t msg_id, uint8_t param_len, uint8_t const *param);
#endif

/**
 ****************************************************************************************
 * @brief EACI application task message handler
 *
 ****************************************************************************************
 */
#if BLE_GL_COLLECTOR || BLE_GL_SENSOR
void gl_app_task_msg_hdl(ke_msg_id_t const msgid, void const *param);
#endif

#endif // APP_EACI_GL_H_
