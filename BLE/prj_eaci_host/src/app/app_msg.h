/**
 ****************************************************************************************
 *
 * @file app_msg.h
 *
 * @brief Application Message functions
 *
 * Copyright (C) Quintic 2009-2012
 *
 * $Rev: $
 *
 ****************************************************************************************
 */

#ifndef _APP_MSG_H_
#define _APP_MSG_H_

/*
 * GLOBAL VARIABLE DECLARATIONS
 ****************************************************************************************
 */

/// structure of a queue element
struct eaci_msg_que_elm
{
    struct eaci_msg_que_elm *next;
};

/// structure of a queue
struct eaci_msg_que
{
    /// pointer to first element
    struct eaci_msg_que_elm *first;
    /// pointer to the last element
    struct eaci_msg_que_elm *last;
};

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/*
 ****************************************************************************************
 * @brief Check if the queue is empty.
 *
 ****************************************************************************************
 */
bool eaci_msg_que_is_empty(const struct eaci_msg_que *const que);

/*
 ****************************************************************************************
 * @brief Push one element as last in the queue.
 *
 ****************************************************************************************
 */
void eaci_msg_que_push(struct eaci_msg_que *que, void *msg);

/*
 ****************************************************************************************
 * @brief Pop the first one element from the queue.
 * 
 ****************************************************************************************
 */
void *eaci_msg_que_pop(struct eaci_msg_que *que);

/*
 ****************************************************************************************
 * @brief Malloc one EACI message.
 *
 ****************************************************************************************
 */
inline void *MSG_MALLOC(uint32_t size)  { return malloc(size); }

/*
 ****************************************************************************************
 * @brief Free one EACI message.
 *
 ****************************************************************************************
 */
inline void MSG_FREE(void * mem_ptr)    { free(mem_ptr); }

#endif

