/**
 ****************************************************************************************
 *
 * @file app_eaci_util.h
 *
 * @brief Application UTIL ACI
 *
 * Copyright (C) Quintic 2012-2013
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */

#ifndef APP_EACI_UTIL_ACI
#define APP_EACI_UTIL_ACI

/// Utility function
void app_eaci_get_conhdl_by_param(uint16_t *conhdl, uint8_t const *param);
void app_eaci_get_bd_addr_by_conhdl(uint16_t conhdl, uint8_t *pdu);
void app_eaci_get_bd_addr_by_param(struct bd_addr *addr, uint8_t const *param);


#endif // APP_EACI_UTIL_ACI
