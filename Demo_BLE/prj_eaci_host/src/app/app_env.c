/**
 ****************************************************************************************
 *
 * @file app_env.c
 *
 * @brief Application Environment C file
 *
 * Copyright (C) Quintic 2009-2012
 *
 * $Rev: $
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup APP
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "app_env.h"

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */
struct app_env_tag app_env;

static void app_env_init(void)
{
    memset(&app_env, 0, sizeof(app_env));
    for (uint8_t idx = 0; idx < BLE_CONNECTION_MAX; idx++)
    {
        app_env.dev_rec[idx].free = true;
    }
}

/**
 ****************************************************************************************
 * @brief Initialize the application environment.
 *
 ****************************************************************************************
 */
void app_init(void)
{
    app_env_init();
    app_debug_init();
    app_eaci_init();
    
    app_env.menu_id = menu_main;
    app_menu_hdl();
}

/// @} APP
