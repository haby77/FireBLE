/**
 ****************************************************************************************
 *
 * @file app_eaci_generic.h
 *
 * @brief Application GENERIC ACI
 *
 * Copyright (C) Quintic 2012-2013
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */

#ifndef APP_EACI_GENERIC_ACI
#define APP_EACI_GENERIC_ACI

/**
 ****************************************************************************************
 * @brief EACI command handler
 *
 ****************************************************************************************
 */ 
void app_eaci_cmd_hdl(uint8_t msg_id, uint8_t param_len, uint8_t const *param);

/**
 ****************************************************************************************
 * @brief EACI Advertising Command handler
 *
 ****************************************************************************************
 */
void app_eaci_cmd_adv_hdl(uint8_t param_len, uint8_t const *param);

/**
 ****************************************************************************************
 * @brief EACI Scan Command handler
 *
 ****************************************************************************************
 */
void app_eaci_cmd_scan_hdl(uint8_t param_len, uint8_t const *param);

/**
 ****************************************************************************************
 * @brief EACI Connect Command handler
 *
 ****************************************************************************************
 */
void app_eaci_cmd_conn_hdl(uint8_t param_len, uint8_t const *param);

/**
 ****************************************************************************************
 * @brief EACI Disconnect Command handler
 *
 ****************************************************************************************
 */
void app_eaci_cmd_disc_hdl(uint8_t param_len, uint8_t const *param);

/**
 ****************************************************************************************
 * @brief EACI Set Device Name Command
 *
 ****************************************************************************************
 */
void app_eaci_cmd_per_set_devname_hdl(uint8_t param_len, uint8_t const *param);

/**
 ****************************************************************************************
 * @brief EACI Bond Command handler
 *
 ****************************************************************************************
 */
void app_eaci_cmd_bond_hdl(uint8_t param_len, uint8_t const *param);

/**
 ****************************************************************************************
 * @brief EACI Master initiates a change in connection parameter.
 *
 ****************************************************************************************
 */
#if (BLE_CENTRAL)
void app_eaci_cmd_cen_update_param_hdl(uint8_t param_len, uint8_t const *param);
#endif

/**
 ****************************************************************************************
 * @brief EACI Slave request for connection parameter change
 *
 ****************************************************************************************
 */
#if (BLE_PERIPHERAL)
void app_eaci_cmd_per_update_param_hdl(uint8_t param_len, uint8_t const *param);
#endif

void gap_app_task_msg_hdl(ke_msg_id_t const msgid, void const *param);

#endif // APP_EACI_GENERIC_ACI
