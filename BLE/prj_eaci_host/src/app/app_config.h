/**
 ****************************************************************************************
 *
 * @file app_config.h
 *
 * @brief Application Configuration File
 *
 * Copyright (C) Quintic 2009-2012
 *
 * $Rev: $
 *
 ****************************************************************************************
 */

#ifndef _APP_CONFIG_H_
#define _APP_CONFIG_H_

#include "usr_config.h"

/// QN9020 Chip Version Number
#if (defined(CFG_9020_B2))
    #define QN_9020_B2
#elif (defined(CFG_9020_B1))
    #define QN_9020_B1
#elif (defined(CFG_9020_B0))
    #define QN_9020_B0
#else
    #error "No correct chip version is defined!"
#endif

/// Application Version Number
#define QN_APP_VER                  "1.0.5"

/// Set Device Name
#define DEVICE_NAME                 "Quintic_Eaci"

/// ATT parts
#define BLE_ATTS            1
#define BLE_ATTC            1

/// UART Definitions
#define QN_DEBUG_UART               QN_UART3
#define QN_HCI_UART                 QN_UART1

/// Max Connection Number
#define BLE_CONNECTION_MAX          8
/// Device Address Type
#define QN_ADDR_TYPE                0

/// Profile and Role
///Health Thermometer Profile Collector Role
#if defined(CFG_PRF_HTPC)
    #define BLE_HT_COLLECTOR    1
    #define TASK_HTPC           CFG_TASK_HTPC
#else
    #define BLE_HT_COLLECTOR    0
#endif // defined(CFG_PRF_HTPC)

///Health Thermometer Profile Thermometer Role
#if defined(CFG_PRF_HTPT)
    #define BLE_HT_THERMOM      1
    #define TASK_HTPT           CFG_TASK_HTPT
#else
    #define BLE_HT_THERMOM      0
#endif // defined(CFG_PRF_HTPT)

/// Proximity Profile Monitor Role
#if defined(CFG_PRF_PXPM)
    #define BLE_PROX_MONITOR    1
    #define TASK_PROXM          CFG_TASK_PXPM
#else
    #define BLE_PROX_MONITOR    0
#endif // defined(CFG_PRF_PXPM)

/// Proximity Profile Reporter Role
#if defined(CFG_PRF_PXPR)
    #define BLE_PROX_REPORTER   1
    #define TASK_PROXR          CFG_TASK_PXPR
#else
    #define BLE_PROX_REPORTER   0
#endif // defined(CFG_PRF_PXPR)

///Find Me Profile Locator role
#if defined(CFG_PRF_FMPL)
    #define BLE_FINDME_LOCATOR  1
    #define TASK_FINDL          CFG_TASK_FMPL
#else
    #define BLE_FINDME_LOCATOR  0
#endif // defined(CFG_PRF_FMPL)

///Find Me Profile Target role
#if defined(CFG_PRF_FMPT)
    #define BLE_FINDME_TARGET   1
    #define TASK_FINDT          CFG_TASK_FMPT
#else
    #define BLE_FINDME_TARGET   0
#endif // defined(CFG_PRF_FMPT)

///Device Information Service Client Role
#if defined(CFG_PRF_DISC)
    #define BLE_DIS_CLIENT      1
    #define TASK_DISC           CFG_TASK_DISC
#else
    #define BLE_DIS_CLIENT      0
#endif // defined(CFG_PRF_DISC)

///Device Information Service Server Role
#if defined(CFG_PRF_DISS)
    #define BLE_DIS_SERVER      1
    #define TASK_DISS           CFG_TASK_DISS
#else
    #define BLE_DIS_SERVER      0
#endif // defined(CFG_PRF_DISS)

///Blood Pressure Profile Collector Role
#if defined(CFG_PRF_BLPC)
    #define BLE_BP_COLLECTOR    1
    #define TASK_BLPC           CFG_TASK_BLPC
#else
    #define BLE_BP_COLLECTOR    0
#endif // defined(CFG_PRF_BLPC)

///Blood Pressure Profile Sensor Role
#if defined(CFG_PRF_BLPS)
    #define BLE_BP_SENSOR       1
    #define TASK_BLPS           CFG_TASK_BLPS
#else
    #define BLE_BP_SENSOR       0
#endif // defined(CFG_PRF_BLPS)

///Time Profile Client Role
#if defined(CFG_PRF_TIPC)
    #define BLE_TIP_CLIENT      1
    #define TASK_TIPC           CFG_TASK_TIPC
#else
    #define BLE_TIP_CLIENT      0
#endif // defined(CFG_PRF_TIPC)

///Time Profile Server Role
#if defined(CFG_PRF_TIPS)
    #define BLE_TIP_SERVER      1
    #define TASK_TIPS           CFG_TASK_TIPS
#else
    #define BLE_TIP_SERVER      0
#endif // defined(CFG_PRF_TIPS)

///Heart Rate Profile Collector Role
#if defined(CFG_PRF_HRPC)
    #define BLE_HR_COLLECTOR    1
    #define TASK_HRPC           CFG_TASK_HRPC
#else
    #define BLE_HR_COLLECTOR    0
#endif // defined(CFG_PRF_HRPC)

///Heart Rate Profile Sensor Role
#if defined(CFG_PRF_HRPS)
    #define BLE_HR_SENSOR       1
    #define TASK_HRPS           CFG_TASK_HRPS
#else
    #define BLE_HR_SENSOR       0
#endif // defined(CFG_PRF_HRPS)

///Glucose Profile Client Role
#if defined(CFG_PRF_GLPC)
    #define BLE_GL_COLLECTOR    1
    #define TASK_GLPC           CFG_TASK_GLPC
#else
    #define BLE_GL_COLLECTOR    0
#endif // defined(CFG_PRF_GLPC)

///Glucose Profile Server Role
#if defined(CFG_PRF_GLPS)
    #define BLE_GL_SENSOR       1
    #define TASK_GLPS           CFG_TASK_GLPS
#else
    #define BLE_GL_SENSOR       0
#endif // defined(CFG_PRF_SCPPS)

///HID Profile Boot Host Role
#if defined(CFG_PRF_HOGPBH)
    #define BLE_HID_BOOT_HOST    1
    #define TASK_HOGPBH          CFG_TASK_HOGPBH
#else
    #define BLE_HID_BOOT_HOST    0
#endif // defined(CFG_PRF_HOGPBH)

    ///HID Profile Report Host Role
#if defined(CFG_PRF_HOGPRH)
    #define BLE_HID_REPORT_HOST  1
    #define TASK_HOGPRH          CFG_TASK_HOGPRH
#else
    #define BLE_HID_REPORT_HOST  0
#endif // defined(CFG_PRF_HOGPRH)

    ///HID Profile Device Role
#if defined(CFG_PRF_HOGPD)
    #define BLE_HID_DEVICE      1
    #define TASK_HOGPD          CFG_TASK_HOGPD
#else
    #define BLE_HID_DEVICE      0
#endif // defined(CFG_PRF_HOGPD)

///Scan Parameter Profile Client Role
#if defined(CFG_PRF_SCPPC)
    #define BLE_SP_CLIENT       1
    #define TASK_SCPPC          CFG_TASK_SCPPC
#else
    #define BLE_SP_CLIENT       0
#endif // defined(CFG_PRF_SCPPC)

///Scan Parameter Profile Server Role
#if defined(CFG_PRF_SCPPS)
    #define BLE_SP_SERVER       1
    #define TASK_SCPPS          CFG_TASK_SCPPS
#else
    #define BLE_SP_SERVER       0
#endif // defined(CFG_PRF_SCPPS)

///Battery Service Client Role
#if defined(CFG_PRF_BASC)
    #define BLE_BATT_CLIENT     1
    #define TASK_BASC           CFG_TASK_BASC
#else
    #define BLE_BATT_CLIENT     0
#endif // defined(CFG_PRF_BASC)

///Battery Service Server Role
#if defined(CFG_PRF_BASS)
    #define BLE_BATT_SERVER     1
    #define TASK_BASS           CFG_TASK_BASS
#else
    #define BLE_BATT_SERVER     0
#endif // defined(CFG_PRF_BASS)    

///Alert Notification Client Role
#if defined(CFG_PRF_ANPC)
    #define BLE_AN_CLIENT       1
    #define TASK_ANPC           CFG_TASK_ANPC
#else
    #define BLE_AN_CLIENT       0
#endif // defined(CFG_PRF_ANPC)

///Alert Notification Server Role
#if defined(CFG_PRF_ANPS)
    #define BLE_AN_SERVER       1
    #define TASK_ANPS           CFG_TASK_ANPS
#else
    #define BLE_AN_SERVER       0
#endif // defined(CFG_PRF_ANPS)

///Cycling Speed and Cadence Collector Role
#if defined(CFG_PRF_CSCPC)
    #define BLE_CSC_COLLECTOR   1
    #define TASK_CSCPC          CFG_TASK_CSCPC
#else
    #define BLE_CSC_COLLECTOR   0
#endif // defined(CFG_PRF_CSCPC)

///Cycling Speed and Cadence Sensor Role
#if defined(CFG_PRF_CSCPS)
    #define BLE_CSC_SENSOR      1
    #define TASK_CSCPS          CFG_TASK_CSCPS
#else
    #define TASK_CSCPS          0
#endif // defined(CFG_PRF_CSCPS)

///Phone Alert Status Client Role
#if defined(CFG_PRF_PASPC)
    #define BLE_PAS_CLIENT      1
    #define TASK_PASPC          CFG_TASK_PASPC
#else
    #define BLE_PAS_CLIENT      0
#endif // defined(CFG_PRF_PASPC)

///Phone Alert Status Server Role
#if defined(CFG_PRF_PASPS)
    #define BLE_PAS_SERVER      1
    #define TASK_PASPS          CFG_TASK_PASPS
#else
    #define BLE_PAS_SERVER      0
#endif // defined(CFG_PRF_PASPS)

///Running Speed and Cadence Collector Role
#if defined(CFG_PRF_RSCPC)
    #define BLE_RSC_COLLECTOR   1
    #define TASK_RSCPC          CFG_TASK_RSCPC
#else
    #define BLE_RSC_COLLECTOR   0
#endif // defined(CFG_PRF_RSCPC)

///Running Speed and Cadence Sensor Role
#if defined(CFG_PRF_RSCPS)
    #define BLE_RSC_SENSOR      1
    #define TASK_RSCPS          CFG_TASK_RSCPS
#else
    #define BLE_RSC_SENSOR      0
#endif // defined(CFG_PRF_RSCPS)

#define CFG_DBG_PRINT               1
#define CFG_STD_PRINTF              1
#endif
