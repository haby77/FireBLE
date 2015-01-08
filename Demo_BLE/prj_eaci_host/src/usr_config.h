/**
 ****************************************************************************************
 *
 * @file usr_config.h
 *
 * @brief User configuration file.
 *
 * Copyright (C) Quintic 2012-2013
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */

#ifndef USR_CONFIG_H_
#define USR_CONFIG_H_

/// Chip version: CFG_9020_B2, CFG_9020_B1, CFG_9020_B0
#define CFG_9020_B2


#define CFG_HCI_UART
//#define CFG_HCI_SPI 
#if defined(CFG_HCI_SPI)
    #define QN_HCI_SPI LPC_SPI
    #define HCI_SPI_CTL_PORT_NUM    0
    // SPI slave inform spi master to read
    #define HCI_SPI_RD_PIN_NUM      3
#endif
//#define MSG_BUFFER_EN

/// Profiles and services

///Alert Notification Client Role
//#define CFG_PRF_ANPC
//#define CFG_TASK_ANPC   TASK_PRF1

///Alert Notification Server Role
//#define CFG_PRF_ANPS
//#define CFG_TASK_ANPS   TASK_PRF1

///Battery Service Client Role
//#define CFG_PRF_BASC
//#define CFG_TASK_BASC   TASK_PRF2

///Battery Service Server Role
//#define CFG_PRF_BASS
//#define CFG_TASK_BASS   TASK_PRF2

///Blood Pressure Profile Collector Role
#define CFG_PRF_BLPC
#define CFG_TASK_BLPC   TASK_PRF3

///Blood Pressure Profile Sensor Role
//#define CFG_PRF_BLPS
//#define CFG_TASK_BLPS   TASK_PRF4

///Cycling Speed and Cadence Collector Role
//#define CFG_PRF_CSCPC
//#define CFG_TASK_CSCPC   TASK_PRF4

///Cycling Speed and Cadence Sensor Role
//#define CFG_PRF_CSCPS
//#define CFG_TASK_CSCPS   TASK_PRF4

///Device Information Service Client Role
//#define CFG_PRF_DISC
//#define CFG_TASK_DISC   TASK_PRF5

///Device Information Service Server Role
//#define CFG_PRF_DISS
//#define CFG_TASK_DISS   TASK_PRF5

///Find Me Profile Locator role
//#define CFG_PRF_FMPL
//#define CFG_TASK_FMPL   TASK_PRF6

///Find Me Profile Target role
//#define CFG_PRF_FMPT
//#define CFG_TASK_FMPT   TASK_PRF7

///Glucose Profile Client Role
//#define CFG_PRF_GLPC
//#define CFG_TASK_GLPC   TASK_PRF7

///Glucose Profile Server Role
//#define CFG_PRF_GLPS
//#define CFG_TASK_GLPS   TASK_PRF8

///Heart Rate Profile Collector Role
//#define CFG_PRF_HRPC
//#define CFG_TASK_HRPC   TASK_PRF8

///Heart Rate Profile Sensor Role
//#define CFG_PRF_HRPS
//#define CFG_TASK_HRPS   TASK_PRF6

///Health Thermometer Profile Collector Role
//#define CFG_PRF_HTPC
//#define CFG_TASK_HTPC   TASK_PRF1

///Health Thermometer Profile Thermometer Role
//#define CFG_PRF_HTPT
//#define CFG_TASK_HTPT   TASK_PRF2

///Phone Alert Status Client Role
//#define CFG_PRF_PASPC
//#define CFG_TASK_PASPC   TASK_PRF2

///Phone Alert Status Server Role
//#define CFG_PRF_PASPS
//#define CFG_TASK_PASPS   TASK_PRF2

///Proximity Profile Monitor Role
//#define CFG_PRF_PXPM
//#define CFG_TASK_PXPM   TASK_PRF3

///Proximity Profile Reporter Role
//#define CFG_PRF_PXPR
//#define CFG_TASK_PXPR   TASK_PRF3

///Running Speed and Cadence Collector Role
//#define CFG_PRF_RSCPC
//#define CFG_TASK_RSCPC   TASK_PRF4

///Running Speed and Cadence Sensor Role
//#define CFG_PRF_RSCPS
//#define CFG_TASK_RSCPS   TASK_PRF4

///Scan Parameter Profile Client Role
//#define CFG_PRF_SCPPC
//#define CFG_TASK_SCPPC   TASK_PRF5

///Scan Parameter Profile Server Role
//#define CFG_PRF_SCPPS
//#define CFG_TASK_SCPPS   TASK_PRF5

///Time Profile Client Role
//#define CFG_PRF_TIPC
//#define CFG_TASK_TIPC   TASK_PRF6

///Time Profile Server Role
//#define CFG_PRF_TIPS
//#define CFG_TASK_TIPS   TASK_PRF6

#endif

