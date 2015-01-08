/**
 ****************************************************************************************
 *
 * @file app_msg.c
 *
 * @brief Application Message functions
 *
 * Copyright (C) Quintic 2009-2012
 *
 * $Rev: $
 *
 ****************************************************************************************
 */

#include "app_env.h"

/*
 * FUNCTION DEFINTIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Check if the queue is empty.
 *
 ****************************************************************************************
 */
bool eaci_msg_que_is_empty(const struct eaci_msg_que *const que)
{
    return (que->first == NULL);
}

/*
 ****************************************************************************************
 * @brief Push one element as last in the queue.
 *
 ****************************************************************************************
 */
void eaci_msg_que_push(struct eaci_msg_que *que, void *que_elm)
{
    if (eaci_msg_que_is_empty(que))
    {
        que->first = que_elm;
    }
    else
    {
        que->last->next = que_elm;
    }

    que->last = que_elm;
    ((struct eaci_msg_que_elm *)que_elm)->next = NULL;
}

/**
 ****************************************************************************************
 * @brief Pop the first one element from the queue.
 * 
 ****************************************************************************************
 */
void *eaci_msg_que_pop(struct eaci_msg_que *que)
{
    struct eaci_msg_que_elm *elm;

    elm = que->first;
    if (elm != NULL)
    {
        que->first = que->first->next;
    }
    
    return elm;
}
