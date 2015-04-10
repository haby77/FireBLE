/**
 ****************************************************************************************
 *
 * @file app_util.c
 *
 * @brief Application Utility functions
 *
 * Copyright (C) Quintic 2009-2012
 *
 * $Rev: $
 *
 ****************************************************************************************
 */

#include "app_env.h"

/**
 ****************************************************************************************
 * @brief Compare if the two Bluetooth device address is same
 *
 ****************************************************************************************
 */
bool app_eaci_bd_addr_cmp(struct bd_addr *addr1, struct bd_addr *addr2)
{
    for (uint8_t idx=0; idx < BD_ADDR_LEN; idx++)
    {
        if (addr1->addr[idx] != addr2->addr[idx])
        {
           return (false);
        }
    }
    return(true);
}

/**
 ****************************************************************************************
 * @brief Set Bluetooth device address by paramer
 *
 ****************************************************************************************
 */
void app_eaci_set_bd_addr_by_param(uint8_t *pdu, uint8_t const *param)
{
    for (uint8_t i = 0; i < BD_ADDR_LEN; i++)
        pdu[i] = param[i];
}

/**
 ****************************************************************************************
 * @brief Get Bluetooth device address by paramer
 *
 ****************************************************************************************
 */
void app_eaci_get_bd_addr_by_param(struct bd_addr *addr, uint8_t const *param)
{
    for (uint8_t i = 0; i < BD_ADDR_LEN; i++)
        addr->addr[i] = param[i];
}

/**
 ****************************************************************************************
 * @brief Get free record index
 *
 ****************************************************************************************
 */
uint8_t app_eaci_get_free_rec_idx(void)
{
    uint8_t idx;

    for (idx = 0; idx < BLE_CONNECTION_MAX; idx++)
    {
        if (app_env.dev_rec[idx].free == true)
            return idx;
    }

    return 0xFF;
}

/**
 ****************************************************************************************
 * @brief Get device record index by Bluetooth device address
 *
 ****************************************************************************************
 */
uint8_t app_eaci_get_rec_idx(struct bd_addr *addr)
{
    uint8_t idx;

    for (idx = 0; idx < BLE_CONNECTION_MAX; idx++)
    {
        if (app_eaci_bd_addr_cmp(&app_env.dev_rec[idx].addr, addr))
            return idx;
    }

    return 0xFF;
}

/**
 ****************************************************************************************
 * @brief Set Link status
 *
 ****************************************************************************************
 */
void app_eaci_set_link_status(bool conn, struct bd_addr *addr)
{
    if (conn == true)
    {
        uint8_t idx = app_eaci_get_free_rec_idx();
        if (idx != 0xFF)
        {
            app_env.dev_rec[idx].free = false;
            app_env.dev_rec[idx].addr = *addr;
        }
        app_env.conn_nb++;
    }
    else
    {
        uint8_t idx = app_eaci_get_rec_idx(addr);
        if (idx != 0xFF)
        {
            app_env.dev_rec[idx].free = true;
        }
        app_env.conn_nb--;
    }
}
