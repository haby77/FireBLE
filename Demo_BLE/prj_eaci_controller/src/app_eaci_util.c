/**
 ****************************************************************************************
 *
 * @file app_eaci_util.c
 *
 * @brief Application UTIL ACI
 *
 * Copyright (C) Quintic 2012-2013
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup APP_EACI_UTIL_ACI
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
 
#include "app_eaci.h"
#include "app_eaci_util.h"
/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
void app_eaci_get_bd_addr_by_param(struct bd_addr *addr, uint8_t const *param)
{
    for (uint8_t i = 0; i < BD_ADDR_LEN; i++)
        addr->addr[i] = param[i];
}

void app_eaci_get_conhdl_by_param(uint16_t *conhdl, uint8_t const *param)
{
    struct bd_addr addr;
    uint8_t idx;
    app_eaci_get_bd_addr_by_param(&addr, param);
    *conhdl = app_get_conhdl_by_bdaddr(&addr, &idx);
}

void app_eaci_get_bd_addr_by_conhdl(uint16_t conhdl, uint8_t *pdu)
{
    // BD address
    struct bd_addr addr;
    if (app_get_bd_addr_by_conhdl(conhdl, &addr) != GAP_INVALID_CONIDX)
        for (uint8_t i = 0; i < BD_ADDR_LEN; i++)
            pdu[i] = addr.addr[i];
}


/// @} APP_EACI_UTIL_ACI
