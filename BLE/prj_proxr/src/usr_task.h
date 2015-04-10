#ifndef __USR_TASK_H__
#define __USR_TASK_H__

#include "app_env.h"
#include "usr_button.h"

#undef TASK_USR
#define TASK_USR    TASK_USER

#define USR_IDX_MAX                                 0x01

/// states of USR task
enum
{
    /// Init state
    USR_DISABLE,
    /// Idle state
    USR_IDLE,
    /// Scan state
    USR_CONNECT,
    /// Number of defined states.
    USR_STATE_MAX
};

/// Usr Message Interface
enum
{
    	//tchip
    USR_KEY_PROCESS_TIMER,
		USR_KEY_SCAN_TIMER,
    USR_MSG_MAX,
};

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
extern const struct ke_state_handler usr_state_handler[USR_STATE_MAX];

extern const struct ke_state_handler usr_default_handler;

extern ke_state_t usr_state[USR_IDX_MAX];

extern void task_usr_desc_register(void);

#endif  
//end of usr_task.h
