/**
 ****************************************************************************************
 *
 * @file usr_config.h
 *
 * @brief User configuration file.
 *
 * Copyright (C) Quintic 2014-2014
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */

#ifndef USR_CONFIG_H_
#define USR_CONFIG_H_

// User configuration

/// Software verion: CFG_SW_DEBUG, CFG_SW_RELEASE
// In the debug version the program will be in the infinite loop if the hardfault exception is triggered.
// In the release version the program will execute a system reboot if the hardfault exception is triggered.
#define CFG_SW_DEBUG

/// Chip version: CFG_9020_B2
#define CFG_9020_B2

/// Work mode: CFG_WM_SOC, CFG_WM_NP, CFG_WM_HCI
#define CFG_WM_SOC

/// Easy ACI
// #define CFG_EACI

/// Easy API
// #define CFG_EAPI

#if ((defined(CFG_EACI) || defined(CFG_EAPI)) && !defined(CFG_WM_SOC))
    #error "CFG_WM_SOC shall be defined when using EACI or EAPI"
#endif

/// Local name
// The local name defined by this macro is only used when the device name tag 
// is not availiable in the NVDS. Generally the local name in the advertising
// packet is obtained from device name in the NVDS.
//#define CFG_LOCAL_NAME   "Quintic BLE"

/// DC-DC enable
#define CFG_DC_DC

/// 32k RCO
// #define CFG_32K_RCO

/// NVDS WRTIE SUPPORT
// #define CFG_NVDS_WRITE

/// Test mode controll pin
//#define CFG_TEST_CTRL_PIN GPIO_P31

/// Memory retention
#define CFG_MEM_RETENTION   (MEM_BLOCK1 | MEM_BLOCK6 | MEM_BLOCK7)

/// Deep sleep support
#define CFG_DEEP_SLEEP

/// BLE Sleep
#define CFG_BLE_SLEEP

/// BLE MAX sleep duration
#define CFG_BLE_MAX_SLEEP_DUR   0x3e80  // 625us * 0x3e80 = 10s

/// Transport layer UART interface used in network processor mode and controller mode
#define CFG_HCI_UART                    QN_UART0
/// Transport layer SPI interface used in network processor mode and controller mode
//#define CFG_HCI_SPI                     QN_SPI0
/// SPI write ready, notify host to read from SPI, output
//#define CFG_HCI_SPI_WR_CTRL_PIN         GPIO_P26

/// Debug information output interface
#define CFG_DEBUG_UART  QN_UART0

// Using standard lib printf
#define CFG_STD_PRINTF

/// Debug print option
#define CFG_DBG_PRINT

/// Debug trace option
// #define CFG_DBG_TRACE_MORE

/// Debug information
#define CFG_DBG_INFO

/// Maximum connection number
#define CFG_CON 1

/// GAP role: CFG_CENTRAL, CFG_PERIPHERAL, CFG_OBSERVER, CFG_BROADCASTER, CFG_ALLROLES
#define CFG_OBSERVER

/// Local address type: CFG_ADDR_PUBLIC, CFG_ADDR_RAND
#define CFG_ADDR_PUBLIC

/// Support service discovery
// #define CFG_SVC_DISC

/// Support white list
// #define CFG_WL_SUPPORT

/// Link infomation option
// #define CFG_LINK_INFO

/// CSRK support
// #define CFG_CSRK_SUPPORT

/// Security
//#define CFG_SECURITY_ON

/// ATT parts
//#define CFG_ATTC
//#define CFG_ATTS

/// Profiles and services

///Alert Notification Client Role
//#define CFG_PRF_ANPC
//#define CFG_TASK_ANPC   TASK_PRF1

//Alert Notification Server Role
//#define CFG_PRF_ANPS
//#define CFG_TASK_ANPS   TASK_PRF2

///Battery Service Client Role
//#define CFG_PRF_BASC
//#define CFG_TASK_BASC   TASK_PRF2

///Battery Service Server Role
//#define CFG_PRF_BASS
//#define CFG_TASK_BASS   TASK_PRF1

///Blood Pressure Profile Collector Role
//#define CFG_PRF_BLPC
//#define CFG_TASK_BLPC   TASK_PRF3

///Blood Pressure Profile Sensor Role
//#define CFG_PRF_BLPS
//#define CFG_TASK_BLPS   TASK_PRF4

///Cycling Speed and Cadence Collector Role
//#define CFG_PRF_CSCPC
//#define CFG_TASK_CSCPC   TASK_PRF7

///Cycling Speed and Cadence Sensor Role
//#define CFG_PRF_CSCPS
//#define CFG_TASK_CSCPS   TASK_PRF8

///Device Information Service Client Role
//#define CFG_PRF_DISC
//#define CFG_TASK_DISC   TASK_PRF5

///Device Information Service Server Role
//#define CFG_PRF_DISS
//#define CFG_TASK_DISS   TASK_PRF6

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
//#define CFG_TASK_HRPS   TASK_PRF7

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
//#define CFG_TASK_PASPS   TASK_PRF3

///Proximity Profile Monitor Role
//#define CFG_PRF_PXPM
//#define CFG_TASK_PXPM   TASK_PRF1

///Proximity Profile Reporter Role
//#define CFG_PRF_PXPR
//#define CFG_TASK_PXPR   TASK_PRF4

///Running Speed and Cadence Collector Role
//#define CFG_PRF_RSCPC
//#define CFG_TASK_RSCPC   TASK_PRF1

///Running Speed and Cadence Sensor Role
//#define CFG_PRF_RSCPS
//#define CFG_TASK_RSCPS   TASK_PRF2

///Scan Parameter Profile Client Role
//#define CFG_PRF_SCPPC
//#define CFG_TASK_SCPPC   TASK_PRF5

///Scan Parameter Profile Server Role
//#define CFG_PRF_SCPPS
//#define CFG_TASK_SCPPS   TASK_PRF6

///Time Profile Client Role
//#define CFG_PRF_TIPC
//#define CFG_TASK_TIPC   TASK_PRF1

///Time Profile Server Role
//#define CFG_PRF_TIPS
//#define CFG_TASK_TIPS   TASK_PRF1

#endif

