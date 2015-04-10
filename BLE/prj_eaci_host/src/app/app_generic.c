/**
 ****************************************************************************************
 *
 * @file app_generic.c
 *
 * @brief Application Command and Event functions file
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

/**
 ****************************************************************************************
 * @brief Adversting command
 *
 ****************************************************************************************
 */
void app_eaci_cmd_adv(uint8_t const start, uint16_t adv_intv_min, uint16_t adv_intv_max)
{
    uint8_t pdu[] = {EACI_MSG_TYPE_CMD, EACI_MSG_CMD_ADV, 5, 0, 0, 0, 0, 0};
    pdu[3] = start;
    pdu[4] = (uint8_t)(adv_intv_min & 0xff);
    pdu[5] = (uint8_t)(adv_intv_min >> 8 & 0xff);
    pdu[6] = (uint8_t)(adv_intv_max & 0xff);
    pdu[7] = (uint8_t)(adv_intv_max >> 8 & 0xff);
    eaci_uart_write(sizeof(pdu), pdu);
}

/**
 ****************************************************************************************
 * @brief Scan command
 *
 ****************************************************************************************
 */
void app_eaci_cmd_scan(uint8_t const start)
{
    uint8_t pdu[] = {EACI_MSG_TYPE_CMD, EACI_MSG_CMD_SCAN, 1, 0};
    pdu[3] = start;
    eaci_uart_write(sizeof(pdu), pdu);

    if (start == 1)
        app_env.inq_id = 0;
}

/**
 ****************************************************************************************
 * @brief Connection command
 *
 ****************************************************************************************
 */
void app_eaci_cmd_conn(uint8_t type, struct bd_addr *addr, uint16_t conn_intv_min,
                       uint16_t conn_intv_max, uint16_t cnnn_timeout)
{
    uint8_t pdu[] = {EACI_MSG_TYPE_CMD, EACI_MSG_CMD_CONN, 13, 0, 0, 0, 0, 0, 0, 0, 
                        0, 0, 0, 0, 0, 0};
    pdu[3] = type;
    pdu[4] = (uint8_t)(conn_intv_min & 0xff);
    pdu[5] = (uint8_t)(conn_intv_min >> 8 & 0xff);
    pdu[6] = (uint8_t)(conn_intv_max & 0xff);
    pdu[7] = (uint8_t)(conn_intv_max >> 8 & 0xff);
    pdu[8] = (uint8_t)(cnnn_timeout & 0xff);
    pdu[9] = (uint8_t)(cnnn_timeout >> 8 & 0xff);
    app_eaci_set_bd_addr_by_param(pdu+10, (const uint8_t *)addr);
    eaci_uart_write(sizeof(pdu), pdu);
}

/**
 ****************************************************************************************
 * @brief Disconnection command
 *
 ****************************************************************************************
 */
void app_eaci_cmd_disc(struct bd_addr *addr)
{
    uint8_t pdu[] = {EACI_MSG_TYPE_CMD, EACI_MSG_CMD_DISC, 6, 0, 0, 0, 0, 0, 0};
    app_eaci_set_bd_addr_by_param(pdu+3, (const uint8_t *)addr);
    eaci_uart_write(sizeof(pdu), pdu);
}

/**
 ****************************************************************************************
 * @brief Set Device Name command
 *
 ****************************************************************************************
 */
void app_eaci_set_dev_name_cmd(uint8_t val_len, uint8_t *dev_name)
{
    uint8_t *pdu;
    pdu = (uint8_t *)malloc(3 + val_len);
    if (pdu != NULL)
    {
        memset(pdu, 0, 3 + val_len);
        pdu[0] = EACI_MSG_TYPE_CMD;
        pdu[1] = EACI_MSG_CMD_SET_DEVNAME;
        pdu[2] = val_len;
    
        for (int i = 0; i< val_len; i++)
        {
            pdu[3 + i] = dev_name[i];
            QPRINTF(" %d ", dev_name[i]);
        }
        QPRINTF("\r\n");
        eaci_uart_write((3 + val_len), pdu);
        
        free(pdu);
        pdu = NULL;
    }
    else
    {
        QPRINTF("Insufficient memory available\r\n");
    }
}

/**
 ****************************************************************************************
 * @brief Master initiates a change in connection parameter.
 *
 ****************************************************************************************
 */
void app_eaci_cen_update_param_cmd(struct bd_addr *addr)
{
    uint8_t pdu[19];
    pdu[0] = EACI_MSG_TYPE_CMD;
    pdu[1] = EACI_MSG_CMD_CEN_UPDATE_PARAM;
    pdu[2] = 16;
    // 0x0000: accept 0x0001: reject
    pdu[3] = 0x00;
    pdu[4] = 0x00; 
    // Connection interval minimum
    pdu[5] = (uint8_t)GAP_INIT_CONN_MIN_INTV;
    pdu[6] = (uint8_t)((GAP_INIT_CONN_MIN_INTV >> 8) & 0xff);
    // Connection interval maximum
    pdu[7] = (uint8_t)GAP_INIT_CONN_MAX_INTV;
    pdu[8] = (uint8_t)((GAP_INIT_CONN_MAX_INTV >> 8) & 0xff);
    // Latency
    pdu[9] = (uint8_t)GAP_CONN_LATENCY;
    pdu[10] = (uint8_t)((GAP_CONN_LATENCY >> 8) & 0xff);
    // Supervision timeout
    pdu[11] = (uint8_t)GAP_CONN_SUPERV_TIMEOUT;
    pdu[12] = (uint8_t)((GAP_CONN_SUPERV_TIMEOUT >> 8) & 0xff);

    app_eaci_set_bd_addr_by_param(pdu+13, (const uint8_t *)addr);

    eaci_uart_write(sizeof(pdu), pdu);
}

/**
 ****************************************************************************************
 * @brief Slave request for connection parameter change.
 *
 ****************************************************************************************
 */
void app_eaci_slave_update_param_cmd(void)
{
    //uint8_t pdu[]= {EACI_MSG_TYPE_CMD,EACI_MSG_CMD_PER_UPDATE_PARAM,0x08,0x18,0x00,0x28,0x00,0x00,0x00,0xd0,0x07};
    uint8_t pdu[]= {EACI_MSG_TYPE_CMD,EACI_MSG_CMD_PER_UPDATE_PARAM,0x08,0,0,0,0,0,0,0,0};
    // Connection interval minimum
    pdu[3] = (uint8_t)GAP_INIT_CONN_MIN_INTV;
    pdu[4] = (uint8_t)((GAP_INIT_CONN_MIN_INTV >> 8) & 0xff);
    // Connection interval maximum
    pdu[5] = (uint8_t)GAP_INIT_CONN_MAX_INTV;
    pdu[6] = (uint8_t)((GAP_INIT_CONN_MAX_INTV >> 8) & 0xff);
    // Latency
    pdu[7] = (uint8_t)GAP_CONN_LATENCY;
    pdu[8] = (uint8_t)((GAP_CONN_LATENCY >> 8) & 0xff);
    // Supervision timeout
    pdu[9] = (uint8_t)GAP_CONN_SUPERV_TIMEOUT;
    pdu[10] = (uint8_t)((GAP_CONN_SUPERV_TIMEOUT >> 8) & 0xff);

    eaci_uart_write(sizeof(pdu), pdu);
}

/**
 ****************************************************************************************
 * @brief Bond command
 *
 ****************************************************************************************
 */
void app_eaci_cmd_bond(struct bd_addr *addr)
{
    uint8_t pdu[] = {EACI_MSG_TYPE_CMD, EACI_MSG_CMD_BOND, 6, 0, 0, 0, 0, 0, 0};
    app_eaci_set_bd_addr_by_param(pdu+3, (const uint8_t *)addr);
    eaci_uart_write(sizeof(pdu), pdu);
}

/**
 ****************************************************************************************
 * @brief EACI event message handler
 *
 ****************************************************************************************
 */
void app_eaci_evt(uint8_t msg_id, uint8_t param_len, uint8_t const *param)
{
    switch (msg_id)
    {
        case EACI_MSG_EVT_ADV:
            if (param_len == 1)
            {
                // Adverstising
                if (param[0] == 1)
                    QPRINTF("Adverstising started.\r\n");
                else
                    QPRINTF("Adverstising stopped.\r\n");
            }
            break;

        case EACI_MSG_EVT_INQ_RESULT:
            if (param_len >= 7)
            {
                // Scan result
                app_env.inq_addr[app_env.inq_id].addr_type = param[0];
                app_eaci_get_bd_addr_by_param(&app_env.inq_addr[app_env.inq_id].addr, param+1);
                QPRINTF("%d. %c %02X%02X:%02X:%02X%02X%02X ",
                    app_env.inq_id, 
                    app_env.inq_addr[app_env.inq_id].addr_type ? 'R' : 'P', 
                    app_env.inq_addr[app_env.inq_id].addr.addr[5],
                    app_env.inq_addr[app_env.inq_id].addr.addr[4],
                    app_env.inq_addr[app_env.inq_id].addr.addr[3],
                    app_env.inq_addr[app_env.inq_id].addr.addr[2],
                    app_env.inq_addr[app_env.inq_id].addr.addr[1],
                    app_env.inq_addr[app_env.inq_id].addr.addr[0]);

                if (param_len > 7)
                {
                    for (uint8_t i = 0; i < param_len - 7; i++)
                        QPRINTF("%c", param[7+i]);
                    QPRINTF("\r\n");
                }
                else
                {
                    QPRINTF("N/A\r\n");
                }
                app_env.inq_id++;
            }
            break;

        case EACI_MSG_EVT_INQ_CMP:
            if (param_len == 1)
            {
                // Scan completed
                QPRINTF("Total %d devices found.\r\n", param[0]);
            }
            break;

        case EACI_MSG_EVT_CONN:
            if (param_len == 7)
            {
                struct bd_addr peer_addr;
                app_eaci_get_bd_addr_by_param(&peer_addr, param+1);
                // Connection completed
                if (param[0] == 0)
                {
                    app_eaci_set_link_status(true, &peer_addr);
                }
                QPRINTF("Connection with %02X%02X:%02X:%02X%02X%02X result is 0x%x.\r\n", 
                    peer_addr.addr[5],
                    peer_addr.addr[4],
                    peer_addr.addr[3],
                    peer_addr.addr[2],
                    peer_addr.addr[1],
                    peer_addr.addr[0],
                    param[0]);
            }
            break;

        case EACI_MSG_EVT_DISC:
            if (param_len == 7)
            {
            // Connection disconnected
            struct bd_addr peer_addr;
            app_eaci_get_bd_addr_by_param(&peer_addr, param+1);
            app_eaci_set_link_status(false, &peer_addr); 
            QPRINTF("Disconnect with %02X%02X:%02X:%02X%02X%02X reason is 0x%x.\r\n", 
                    peer_addr.addr[5],
                    peer_addr.addr[4],
                    peer_addr.addr[3],
                    peer_addr.addr[2],
                    peer_addr.addr[1],
                    peer_addr.addr[0],
                    param[0]);
            }
            break;

        case EACI_MSG_EVT_BOND:
            if (param_len == 8)
            {
                // Bond completed
                struct bd_addr peer_addr;
                app_eaci_get_bd_addr_by_param(&peer_addr, param+2);
                QPRINTF("Bond complete with %02X%02X:%02X:%02X%02X%02X, bonded: %01X, status: 0x%02X.\r\n", 
                        peer_addr.addr[5],
                        peer_addr.addr[4],
                        peer_addr.addr[3],
                        peer_addr.addr[2],
                        peer_addr.addr[1],
                        peer_addr.addr[0],
                        param[1],
                        param[0]);
            }
            break;

        case EACI_MSG_EVT_CEN_UPDATE_PARAM:
            if (param_len == 1)
            {
                // Update Param
                if (param[0] == 0)
                    QPRINTF("Master update parameter complete.\r\n");
                else
                    QPRINTF("Master update parameter failed.\r\n");
            }
            break;

        case EACI_MSG_EVT_SMP_SEC:
            if (param_len == 1)
            {
                // Update Param
                if (param[0] == 0)
                    QPRINTF("SMPC Security complete.\r\n");
                else
                    QPRINTF("SMPC Security failed.\r\n");
            }
            break;

        case EACI_MSG_EVT_SET_DEVNAME:
            if (param_len == 1)
            {
                // Update Param
                if (param[0] == 0)
                    QPRINTF("Set Device Name complete.\r\n");
                else
                    QPRINTF("Set Device Name failed.\r\n");
            }
            break;

        case EACI_MSG_EVT_UPDATE_PARAM:
            if (param_len == 1)
            {
                // Update Param
                if (param[0] == 0)
                    QPRINTF("Slave update success.\r\n");
                else
                    QPRINTF("Slave update failed.\r\n");
            }
            break;

        default:
            break;
    }
}
