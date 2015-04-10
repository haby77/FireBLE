
/**
 ****************************************************************************************
 *
 * @file app_otas_task.h
 *
 * @brief Application otas implementation
 *
 * Copyright (C) Quintic 2014-2014
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */
 
#ifndef APP_OTAS_TASK_H_
#define APP_OTAS_TASK_H_

/**
 ****************************************************************************************
 * @addtogroup APP_OTAS_TASK OTA Task API
 * @ingroup APP_OTAS
 * @brief OTAS Task API
 *
 * OTAS Task APIs are used to handle the message from OTA or APP.
 * @{
 ****************************************************************************************
 */
 
#if BLE_OTA_SERVER

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "otas_task.h"


/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/*
 ****************************************************************************************
 * @brief  This handler is used to inform the application of the ota transimition status now 
 *
 ****************************************************************************************
 */ 

int app_otas_start_handler(ke_msg_id_t const msgid, struct otas_transimit_status_ind const * param,
                           ke_task_id_t const dest_id, ke_task_id_t const src_id);

#endif // BLE_OTA_SERVER


/// @} APP_PROXR_TASK

#endif // APP_OTAS_TASK_H_
