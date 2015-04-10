/**
 ****************************************************************************************
 *
 * @file app_eaci_trans.h
 *
 * @brief UART transport module functions for Easy Application Controller Interface.
 *
 * Copyright (C) Quintic 2012-2013
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */

#ifndef APP_EACI_TRANS_H_
#define APP_EACI_TRANS_H_

#include "app_env.h"
#include "lib.h"

// Field length of Message Type and Parameter Length
#define EACI_MSG_HDR_LEN        2

#if (defined(CFG_HCI_SPI))
#define EACI_MSG_BUFFER_EN      TRUE
#else
#define EACI_MSG_BUFFER_EN      FALSE
#endif

#if EACI_MSG_BUFFER_EN==TRUE
// Rx payload max size
#define EACI_RX_MAX_SIZE        32
// EACI message buffer deep
#define EACI_BUF_DEEP           4
extern uint8_t eaci_msg_buf_used;
#endif

/*
 * GLOBAL VARIABLE DECLARATIONS
 ****************************************************************************************
 */

extern struct eaci_env_tag eaci_env;

#if EACI_MSG_BUFFER_EN==TRUE
/// EACI Message structure.
struct eaci_msg
{
    struct co_list_hdr hdr;     ///< List header for chaining
    
    uint8_t         hci_type;   ///< Type of HCI data (used by the HCI only)
    int8_t          hci_off;    ///< Offset of the HCI data in the message (used by the HCI only)
    uint16_t        hci_len;    ///< Length of the HCI traffic (used by the HCI only)
    
    ke_msg_id_t     id;         ///< Message id.
    ke_task_id_t    dest_id;    ///< Destination kernel identifier.
    ke_task_id_t    src_id;     ///< Source kernel identifier.
    uint16_t        param_len;  ///< Parameter embedded struct length.
    uint8_t         param[EACI_RX_MAX_SIZE];
};
#endif

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/*
 ****************************************************************************************
 * @brief EACI Error Handling function.
 *
 ****************************************************************************************
 */
void eaci_trans_send_error(uint8_t reason);

/*
 ****************************************************************************************
 * @brief EACI UART initialization function.
 *
 ****************************************************************************************
 */
void eaci_trans_init(void);

/*
 ****************************************************************************************
 * @brief EACI UART write function.
 *
 ****************************************************************************************
 */
void eaci_trans_write(struct ke_msg *msg);

/*
 ****************************************************************************************
 * @brief EACI Function called at each RX interrupt.
 *
 *****************************************************************************************
 */
void eaci_trans_rx_done(void);

/// @} EACI_TRANS
#endif // APP_EACI_TRANS_H_
