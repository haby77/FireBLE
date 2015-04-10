/**
 ****************************************************************************************
 *
 * @file system.h
 *
 * @brief User system setup and initial configuration header file.
 *
 * Copyright (C) Quintic 2012-2013
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */

#ifndef SYSTEM_H
#define SYSTEM_H


#ifdef __cplusplus
extern "C" {
#endif

#include "driver_config.h"
#include "syscon.h"

/*----------------------------------------------------------------------------
  DEFINES
 *----------------------------------------------------------------------------*/

extern void SystemInit (void);


#ifdef __cplusplus
}
#endif

/// @} SYSTEM_CONTROLLER


#endif /* SYSTEM_H */

