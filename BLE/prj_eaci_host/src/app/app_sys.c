/**
 ****************************************************************************************
 *
 * @file app_sys.c
 *
 * @brief Application System Functions
 *
 * Copyright (C) Quintic 2009-2012
 *
 * $Rev: $
 *
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "app_env.h" 
#include "uart.h"

struct app_debug_env_tag app_debug_env;
static void app_debug_uart_rx_done(void);

/**
 ****************************************************************************************
 * @brief Debug initialization.
 *
 ****************************************************************************************
 */
void app_debug_init(void)
{
    app_debug_env.len = 0;
    uart_read(QN_DEBUG_UART, app_debug_env.buf_rx, 1, app_debug_uart_rx_done);
}

/**
 ****************************************************************************************
 * @brief UART receive call back function, input string should end with '\r''\n'.
 *
 ****************************************************************************************
 */
void app_debug_uart_rx_done(void)
{
    if (app_debug_env.buf_rx[app_debug_env.len] == 0x0A)
    {
        app_env.input_len = app_debug_env.len;
        app_debug_env.len = 0;
    }
    else
    {
        app_env.input[app_debug_env.len] = app_debug_env.buf_rx[app_debug_env.len];
        if (app_debug_env.len == QN_DEBUG_RX_LEN-1)
            app_debug_env.len = 0;
        else
            app_debug_env.len += 1;
    }

    uart_read(QN_DEBUG_UART, app_debug_env.buf_rx+app_debug_env.len, 1, app_debug_uart_rx_done);
}
