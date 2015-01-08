/**
 ****************************************************************************************
 *
 * @file qnrf.h
 *
 * @brief Header file of RF for QN9020.
 *
 * Copyright (C) Quintic 2012-2014
 *
 * $Rev: 1.1 $
 *
 ****************************************************************************************
 */
#ifndef _QNRF_H_
#define _QNRF_H_
#include "driver_config.h"
#if CONFIG_ENABLE_DRIVER_QNRF==TRUE

/**
 ****************************************************************************************
 * @defgroup QNRF RF Driver
 * @ingroup DRIVERS
 * @brief RF driver
 *
 *
 * @{
 *
 ****************************************************************************************
 */

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


/*
 * INCLUDE FILES
 ****************************************************************************************
 */


/*
 * MACRO DEFINITIONS
 ****************************************************************************************
 */


/*
 * ENUMERATION DEFINITIONS
 *****************************************************************************************
 */

/// RF MODE
enum RF_MODE
{
    RF_TX           = 0,        /*!< TX Mode */
    RF_RX_IMR0      = 1,        /*!< RX Mode, IMR=0 */
    RF_RX_IMR1      = 2,        /*!< RX Mode, IMR=1 */
};


/// RF TX Power
enum TX_POWER
{
    TX_GAIN_LEVEL0 = 0,         /*!< -20 dBm */
    TX_GAIN_LEVEL1,             /*!< -18 dBm */
    TX_GAIN_LEVEL2,             /*!< -16 dBm */
    TX_GAIN_LEVEL3,             /*!< -14 dBm */
    TX_GAIN_LEVEL4,             /*!< -12 dBm */
    TX_GAIN_LEVEL5,             /*!< -10 dBm */
    TX_GAIN_LEVEL6,             /*!<  -8 dBm */
    TX_GAIN_LEVEL7,             /*!<  -6 dBm */
    TX_GAIN_LEVEL8,             /*!<  -4 dBm */
    TX_GAIN_LEVEL9,             /*!<  -2 dBm */
    TX_GAIN_LEVEL10,            /*!<   0 dBm */
    TX_GAIN_LEVEL11,            /*!<   2 dBm */
    TX_GAIN_LEVEL12,            /*!<   4 dBm, Chip need calibration after entering or exiting this level */
    TX_GAIN_LEVEL_MAX,
    TX_GAIN_LEVEL_ERR
};



/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
extern void rf_tx_power_level_set(enum TX_POWER txpwr);
extern uint32_t rf_tx_power_level_get(void);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

/// @} QNRF
#endif /* CONFIG_ENABLE_DRIVER_QNRF==TRUE */
#endif /* _QNRF_H_ */

