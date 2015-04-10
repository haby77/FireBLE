/**
 ****************************************************************************************
 *
 * @file lib.h
 *
 * @brief QN9020 library API header file.
 *
 * Copyright (C) Quintic 2012-2014
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */
 
#ifndef LIB_H_
#define LIB_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "driver_config.h"

/*
 * DEFINITIONS
 ****************************************************************************************
 */
#define QN_DBG_INFO_REG                     0x1000FFFC

#define QN_DBG_INFO_XTAL_WAKEUP_DURATION    0x00000001
#define QN_DBG_INFO_BLE_HEAP_FULL           0x00000002

/*
 * ENUMERATION DEFINITIONS
 ****************************************************************************************
 */

/// Power mode
enum PW_MODE
{
    NORMAL_MODE,        // Low power
    HIGH_PERFORMANCE    // High Power
};

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */


/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief DC-DC Enable
 * @param[in]   enable true - enable dc-dc; false - disable
 ****************************************************************************************
 */
extern void dc_dc_enable(bool enable);

/**
 ****************************************************************************************
 * @brief Initilaize BLE hardware platform
 * @param[in]   pw_mode             NORMAL_MODE - low power; HIGH_PERFORMANCE - high power
 * @param[in]   xtal                which crystal is used - 16MHz or 32MHz
 * @param[in]   clk_32k             which 32k is used - 0:xtal, 1:rco
 * @param[in]   nvds_tmp_buf        pointer of nvds temp buffer
 * @param[in]   nvds_tmp_buf_len    length of nvds temp buffer
 ****************************************************************************************
 */
extern void plf_init(enum PW_MODE pw_mode, uint32_t xtal, uint8_t clk_32k, uint8_t* nvds_tmp_buf, uint32_t nvds_tmp_buf_len);

extern uint32_t check_ble_wakeup(void);

#endif

