/**
 ****************************************************************************************
 *
 * @file app_generic.h
 *
 * @brief Application Command and Event functions file
 *
 * Copyright (C) Quintic 2009-2012
 *
 * $Rev: $
 *
 ****************************************************************************************
 */

#ifndef _APP_GENERIC_H_
#define _APP_GENERIC_H_

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

///EACI Command
enum
{
    ///Reserved
    EACI_MSG_CMD_RSV = 0x0,
    ///Advertising
    EACI_MSG_CMD_ADV,
    ///Scan
    EACI_MSG_CMD_SCAN,
    ///Connect
    EACI_MSG_CMD_CONN,
    ///Central Disconnect
    EACI_MSG_CMD_DISC,
    ///Set Device Name
    EACI_MSG_CMD_SET_DEVNAME,
    ///Bond
    EACI_MSG_CMD_BOND,
    ///Central Update Param
    EACI_MSG_CMD_CEN_UPDATE_PARAM,
    ///Peripheral Update Param
    EACI_MSG_CMD_PER_UPDATE_PARAM,
    ///Number of Command
    EACI_MSG_CMD_MAX
};

///EACI Event
enum
{
    ///Reserved
    EACI_MSG_EVT_RSV = 0x0,
    ///Advertising Status
    EACI_MSG_EVT_ADV,
    ///Scan result
    EACI_MSG_EVT_INQ_RESULT,
    ///Scan complete
    EACI_MSG_EVT_INQ_CMP,
    ///Connect Status
    EACI_MSG_EVT_CONN,
    ///Disconnect Status
    EACI_MSG_EVT_DISC,
    ///Device Name
    EACI_MSG_EVT_SET_DEVNAME,
    ///Bond Status
    EACI_MSG_EVT_BOND,
    ///Central Update param State
    EACI_MSG_EVT_CEN_UPDATE_PARAM,
    ///SMPC Security started indication to host
    EACI_MSG_EVT_SMP_SEC,
    ///Peripheral Update param
    EACI_MSG_EVT_UPDATE_PARAM,
    ///Number of Event
    EACI_MSG_EVT_MAX
};

/*
 ****************************************************************************************
 * @brief Adversting command
 *
 ****************************************************************************************
 */
void app_eaci_cmd_adv(uint8_t const start, uint16_t adv_intv_min, uint16_t adv_intv_max);

/*
 ****************************************************************************************
 * @brief Scan command
 *
 ****************************************************************************************
 */
void app_eaci_cmd_scan(uint8_t start);

/*
 ****************************************************************************************
 * @brief Connection command
 *
 ****************************************************************************************
 */
void app_eaci_cmd_conn(uint8_t type, struct bd_addr *addr, uint16_t conn_intv_min,
                       uint16_t conn_intv_max, uint16_t cnnn_timeout);

/*
 ****************************************************************************************
 * @brief Disconnection command
 *
 ****************************************************************************************
 */
void app_eaci_cmd_disc(struct bd_addr *addr);

/**
 ****************************************************************************************
 * @brief Set Device Name command
 *
 ****************************************************************************************
 */
void app_eaci_set_dev_name_cmd(uint8_t val_len, uint8_t *dev_name);

/**
 ****************************************************************************************
 * @brief Master initiates a change in connection parameter
 *
 ****************************************************************************************
 */
void app_eaci_cen_update_param_cmd(struct bd_addr *addr);

/*
 ****************************************************************************************
 * @brief Bond command
 *
 ****************************************************************************************
 */
void app_eaci_cmd_bond(struct bd_addr *addr);

/*
 ****************************************************************************************
 * @brief EACI event message handler
 *
 ****************************************************************************************
 */
void app_eaci_evt(uint8_t msg_id, uint8_t param_len, uint8_t const *param);

/**
 ****************************************************************************************
 * @brief Slave request for connection parameter change.
 *
 ****************************************************************************************
 */
void app_eaci_slave_update_param_cmd(void);
#endif // APP_MSG_H_
