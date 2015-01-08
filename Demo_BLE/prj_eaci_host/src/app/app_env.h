/**
 ****************************************************************************************
 *
 * @file app_env.h
 *
 * @brief Application Environment Header File
 *
 * Copyright (C) Quintic 2009-2012
 *
 * $Rev: $
 *
 ****************************************************************************************
 */

#ifndef _APP_ENV_H_
#define _APP_ENV_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "co_bt.h"
#include "co_error.h"

#include "app_config.h"
#include "attm.h"
#include "atts.h"
#include "gatt_task.h"
#include "fw_func_addr.h"
#include "gap.h"
#include "intc.h"

#include "app_sys.h"
#include "app_util.h"
#include "app_msg.h"
#include "app_generic.h"
#include "app_data.h"
#include "app_menu.h"


/*
#include "attm.h"
#include "atts.h"
#include "rscp_common.h"
#include "cscp_common.h"
#include "pasp_common.h"
#include "paspc.h"
#include "pasps_task.h"
#include "rscpc.h"
*/

#if BLE_HT_COLLECTOR
#include "app_htpc.h"
#endif
#if BLE_BP_COLLECTOR
#include "app_blpc.h"
#endif
#if BLE_HR_COLLECTOR
#include "app_hrpc.h"
#endif
#if BLE_GL_COLLECTOR
#include "app_glpc.h"
#endif
#if BLE_FINDME_LOCATOR
#include "app_findl.h"
#endif
#if BLE_PROX_MONITOR
#include "app_proxm.h"
#endif
#if BLE_TIP_CLIENT
#include "app_tipc.h"
#endif
#if BLE_HID_BOOT_HOST
#include "app_hogpbh.h"
#endif
#if BLE_HID_REPORT_HOST
#include "app_hogprh.h"
#endif
#if BLE_SP_CLIENT
#include "app_scppc.h"
#endif
#if BLE_DIS_CLIENT
#include "app_disc.h"
#endif
#if BLE_BATT_CLIENT
#include "app_basc.h"
#endif
#if BLE_CSC_COLLECTOR
#include "app_cscpc.h"
#endif
#if BLE_RSC_COLLECTOR
#include "app_rscpc.h"
#endif
#if BLE_PAS_CLIENT
#include "app_paspc.h"
#endif
#if BLE_AN_CLIENT
#include "app_anpc.h"
#endif
#if BLE_HT_THERMOM
#include "app_htpt.h"
#endif
#if BLE_BP_SENSOR
#include "app_blps.h"
#endif
#if BLE_HR_SENSOR
#include "app_hrps.h"
#endif
#if BLE_PROX_REPORTER
#include "app_proxr.h"
#endif
#if BLE_FINDME_TARGET
#include "app_findt.h"
#endif
#if BLE_TIP_SERVER
#include "app_tips.h"
#endif
#if BLE_SP_SERVER
#include "app_scpps.h"
#endif
#if BLE_DIS_SERVER
#include "app_diss.h"
#endif
#if BLE_BATT_SERVER
#include "app_bass.h"
#endif
#if BLE_GL_SENSOR
#include "app_glps.h"
#endif
#if BLE_HID_DEVICE
#include "app_hogpd.h"
#endif
#if BLE_CSC_SENSOR
#include "app_cscps.h"
#endif
#if BLE_RSC_SENSOR
#include "app_rscps.h"
#endif
#if BLE_PAS_SERVER
#include "app_pasps.h"
#endif
#if BLE_AN_SERVER
#include "app_anps.h"
#endif

#include "eaci.h"
#include "eaci_uart.h"

/*
 * GLOBAL VARIABLE DECLARATIONS
 ****************************************************************************************
 */

/// Inquiried Device Record Structure
struct app_inq_dev_record
{
    uint8_t addr_type;
    struct bd_addr addr;
};

/// Connected Device Record Structure
struct app_conn_dev_record
{
    bool free;
    struct bd_addr addr;
};

/// Application Environment Structure
struct app_env_tag
{
    // Selected Device Index
    uint8_t sel_id;
    // Inquiried Device Index
    uint8_t inq_id;
    // Connected Device Number
    uint8_t conn_nb;
    // Menu Id
    uint8_t menu_id;

    // Inquiried Device Address
    struct app_inq_dev_record inq_addr[BLE_CONNECTION_MAX];
    // Connected Device Record
    struct app_conn_dev_record dev_rec[BLE_CONNECTION_MAX];

    /// Queue of recevice messages
    struct eaci_msg_que msg_que;

    // Input Data - received from UART debug
    uint8_t input[0x0F];
    uint8_t input_len;
};

/// Application Environment
extern struct app_env_tag app_env;

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/*
 ****************************************************************************************
 * @brief Initialize the application environment.
 *
 ****************************************************************************************
 */
void app_init(void);

/*
 ****************************************************************************************
 * @brief QN printf function
 *
 ****************************************************************************************
 */
#define QPRINTF     printf

/// @} APP

#endif // APP_ENV_H_
