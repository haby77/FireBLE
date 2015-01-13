/**
 ****************************************************************************************
 *
 * @file app_task.c
 *
 * @brief APP Task implementation
 *
 * Copyright (C) Quintic 2012-2014
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */


/*
 * INCLUDE FILES
 ****************************************************************************************
 */ 
 
#include <stdint.h>
#include "ke_task.h"
#include "usr_task.h"

#include "app_env.h"

//tchip
//#include "test_timer.h"


/* default State handlers definition. */
const struct ke_msg_handler usr_default_state[USR_MSG_MAX] =
{
//tchip
    {USR_KEY_PROCESS_TIMER,						(ke_msg_func_t) usr_key_process_timer_handler},
		{USR_KEY_SCAN_TIMER,						(ke_msg_func_t) usr_key_scan_timer_handler},
};


/* Specifies the app message handlers that are common to all states. */
const struct ke_state_handler usr_state_handler[USR_STATE_MAX] =
{
    [USR_DISABLE] = KE_STATE_HANDLER_NONE,
    [USR_IDLE] = KE_STATE_HANDLER_NONE,
    [USR_CONNECT ] = KE_STATE_HANDLER_NONE,
};

const struct ke_state_handler usr_default_handler = KE_STATE_HANDLER(usr_default_state);

/* Defines the place holder for the states of all the app task instances. */
ke_state_t usr_state[USR_IDX_MAX];

// Register app task into kernel
void task_usr_desc_register(void)
{
    struct ke_task_desc task_usr_desc;
    
    task_usr_desc.state_handler = usr_state_handler;
    task_usr_desc.default_handler = &usr_default_handler;
    task_usr_desc.state = usr_state;
    task_usr_desc.state_max = USR_STATE_MAX;
    task_usr_desc.idx_max = USR_IDX_MAX;
    
    task_desc_register(TASK_USR, task_usr_desc);
}
///end of usr_task.h  
