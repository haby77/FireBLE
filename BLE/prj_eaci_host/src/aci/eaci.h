/**
 ****************************************************************************************
 *
 * @file hci.h
 *
 * @brief This file contains definitions related to the Fully embedded system HCI
 *        similar interface.
 *
 * Copyright (C) RivieraWaves 2009-2012
 *
 * $Rev: $
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 *
 * @file eaci.h
 *
 * @brief Easy ACI interface module source file.
 *
 * Copyright (C) Quintic 2009-2012
 *
 * $Rev: $
 *
 ****************************************************************************************
 */

#ifndef _EACI_H_
#define _EACI_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

///EACI Message Type
enum
{
    ///Reserved
    EACI_MSG_TYPE_RSV = 0x0,
    ///Command
    EACI_MSG_TYPE_CMD = 0xEA,
    ///Data Request
    EACI_MSG_TYPE_DATA_REQ,
    ///Data Indication
    EACI_MSG_TYPE_DATA_IND,
    ///Event
    EACI_MSG_TYPE_EVT,
    ///Data Error
    EACI_MSG_TYPE_DATA_ERROR = 0xFA,
    ///Number of Message Type
    EACI_MSG_TYPE_MAX
};

///EACI RX states
enum
{
    ///EACI RX Start State - receive message type
    EACI_STATE_RX_START,
    ///EACI RX Header State - receive message header
    EACI_STATE_RX_HDR,
    ///EACI RX Header State - receive (rest of) message payload
    EACI_STATE_RX_PAYL,
    ///EACI RX Header State - receive error
    EACI_STATE_RX_ERR,
    ///Number of states
    EACI_STATE_MAX
};

///EACI Environment context structure
struct eaci_env_tag
{
    ///Rx state - can be receiving message type, header, payload or error
    uint8_t rx_state;
    ///Message type 0x01,0x02,0x03,0x04
    uint8_t msg_type;
    ///Message id
    uint8_t msg_id;
    ///Message parameter length
    uint8_t param_len;
    ///Pointer to current message
    struct eaci_msg *msg;
};

///EACI Message structure
struct eaci_msg
{
    struct eaci_msg_que_elm elm;
    ///Message type 0x01,0x02,0x03,0x04
    uint8_t msg_type;
    ///Message id
    uint8_t msg_id;
    ///Message parameter length
    uint8_t param_len;
    ///Message payload
    uint8_t param[1];
};

/*
 * GLOBAL VARIABLE DECLARATIONS
 ****************************************************************************************
 */
///EACI environment structure external global variable declaration
extern struct eaci_env_tag eaci_env;

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/*
 ****************************************************************************************
 * @brief Initialize EACI interface
 *
 ****************************************************************************************
 */
void app_eaci_init(void);

/*
 ****************************************************************************************
 * @brief EACI application message handler
 *
 ****************************************************************************************
 */
void app_eaci_msg_hdl(uint8_t msg_type, uint8_t msg_id, uint8_t param_len, uint8_t const *param);

#endif // _EACI_H_
