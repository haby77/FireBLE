/**
 ****************************************************************************************
 *
 * @file app_sys.h
 *
 * @brief Application System Functioins
 *
 * Copyright (C) Quintic 2009-2012
 *
 * $Rev: $
 *
 ****************************************************************************************
 */

#ifndef _APP_SYS_H_
#define _APP_SYS_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#define QN_DEBUG_RX_LEN     0x10

/// Application Debug environment context structure
struct app_debug_env_tag
{
    uint8_t len;
    uint8_t buf_rx[QN_DEBUG_RX_LEN];
};

/// Message of UART data request send to application
struct app_uart_data_req
{
    uint8_t len;
    uint8_t data[1];
};

/// Application handle message of UART received data
struct app_uart_data_ind
{
    uint8_t len;
    uint8_t data[1];
};

/*
 ****************************************************************************************
 * @brief Debug initialization.
 *
 ****************************************************************************************
 */
void app_debug_init(void);

#endif // _APP_SYS_H_

