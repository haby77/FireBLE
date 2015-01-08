/**
 ****************************************************************************************
 *
 * @file rtc.h
 *
 * @brief Header file of RTC for QN9020.
 *
 * Copyright (C) Quintic 2012-2013
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */
#ifndef _RTC_H_
#define _RTC_H_
#include "driver_config.h"
#if CONFIG_ENABLE_DRIVER_RTC==TRUE

/**
 ****************************************************************************************
 * @defgroup RTC RTC Driver
 * @ingroup DRIVERS
 * @brief Real time clock driver
 *
 *  QN9020 Real Time Clock (RTC) module provides user the real time and calendar message. The RTC
 *  real time is based on external or internal low power 32 KHz clock, and its features are listed as follow:
 *    - 15-bit counter to generate second with calibration function
 *    - Operate on external/internal 32 KHz clock
 *    - Provide 32-bit second counter
 *    - Programmable second and capture interrupts generation
 *    - Support input capture function with programmable noise canceller
 *    - The capture edge can be configured as positive or negative edge
 *    - Capture interrupt can be masked
 *
 * @{
 *
 ****************************************************************************************
 */

/* \example rtc_example.c
 * This is an example of how to use the RTC driver.
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */


/*
 * MACRO DEFINITIONS
 ****************************************************************************************
 */

/// Total seconds in a day
#define SECONDINDAY            86400
/// Total seconds in an hour
#define SECONDINHOUR           3600
/// Total days in a year
#define DAYINYEAR              365
/// Total days in a leap year
#define DAYINLEAPYEAR          366
/// Total days in a big month
#define DAYINBIGMONTH          31
/// Total days in a little month
#define DAYINLITTLEMONTH       30


/*
 * ENUMERATION DEFINITIONS
 *****************************************************************************************
 */


/*
 * STRUCT DEFINITIONS
 *****************************************************************************************
 */

/// RTC time
struct rtc_time
{
    uint8_t  hour;          /*!< Hour */
    uint8_t  minute;        /*!< Minute */
    uint8_t  second;        /*!< Second */
};

/// RTC date
struct rtc_date
{
    uint8_t  year;          /*!< Year, start from 2000 year */
    uint8_t  month;         /*!< Month */
    uint8_t  day;           /*!< Day */
#ifdef USE_STD_C_LIB_TIME
    uint8_t  week;          /*!< [0-6] : 0:sunday 1:monday ...... */
#endif
};

/// RTC capture edge selection
enum RTC_CAP_EDGE
{
    RTC_CAP_EDGE_POS = 0,   /*!< Rising edge is set as RTC capture edge */
    RTC_CAP_EDGE_NEG = 1,   /*!< Falling edge is set as RTC capture edge */
};

///Structure defining RTC environment parameters
struct rtc_env_tag
{
    struct rtc_time time;       /*!< RTC time structure */
    struct rtc_date date;       /*!< RTC date structure */
#if RTC_CALLBACK_EN==TRUE
    void   (*callback)(void);   /*!< The callback of RTC interrupt */
#endif
};


/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Enable RTC interrupt
 * @description
 *  This function is used to enable RTC interrupt
 ****************************************************************************************
 */
__STATIC_INLINE void rtc_int_enable(void)
{

    // wait until last time configuration synchronize
    while (rtc_rtc_GetSR(QN_RTC) & RTC_MASK_CR_SYNC_BUSY);
    rtc_rtc_SetCRWithMask(QN_RTC, RTC_MASK_SEC_IE, RTC_MASK_SEC_IE);
}

/**
 ****************************************************************************************
 * @brief Disable RTC interrupt
 * @description
 *  This function is used to disable RTC interrupt
 ****************************************************************************************
 */__STATIC_INLINE void rtc_int_disable(void)
{
    // wait until last time configuration synchronize
    while (rtc_rtc_GetSR(QN_RTC) & RTC_MASK_CR_SYNC_BUSY);
    rtc_rtc_SetCRWithMask(QN_RTC, RTC_MASK_SEC_IE, 0);
}

/**
 ****************************************************************************************
 * @brief   Enable RTC module clock
 * @description
 *  This function is used to enable RTC module clock
 *
 *****************************************************************************************
 */
__STATIC_INLINE void rtc_clock_on(void)
{
    // enable 32K clock
    syscon_SetCRSC(QN_SYSCON, SYSCON_MASK_GATING_32K_CLK);
}

/**
 ****************************************************************************************
 * @brief   Disable RTC module clock
 * @description
 *  This function is used to disable RTC module clock
 *
 *****************************************************************************************
 */
__STATIC_INLINE void rtc_clock_off(void)
{
    // disable 32K clock
    syscon_SetCRSS(QN_SYSCON, SYSCON_MASK_GATING_32K_CLK);
}

/**
 ****************************************************************************************
 * @brief   Reset RTC module
 * @description
 *  This function is used to reset RTC module
 *
 *****************************************************************************************
 */
__STATIC_INLINE void rtc_reset(void)
{
    // Reset RTC module
    syscon_SetCRSS(QN_SYSCON, SYSCON_MASK_32K_RST);
    syscon_SetCRSC(QN_SYSCON, SYSCON_MASK_32K_RST);
}

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
#if CONFIG_ENABLE_ROM_DRIVER_RTC==TRUE

#define rtc_env (*(struct rtc_env_tag *)_rtc_env)

typedef void    (*p_rtc_void)(void);
typedef void    (*p_rtc_calibration)(uint8_t dir, uint16_t ppm);
typedef void    (*p_rtc_correction)(uint32_t sleep_count);
typedef void    (*p_rtc_time_set)(uint8_t year, uint8_t month,  uint8_t day, uint8_t hour, uint8_t minute, uint8_t second, void (*callback)(void));
typedef void    (*p_rtc_capture_enable)(enum RTC_CAP_EDGE edge, void (*callback)(void));
typedef uint8_t (*p_dec2bcd)(uint8_t decade);
typedef uint8_t (*p_bcd2dec)(uint8_t bcd);

#define rtc_init             ((p_rtc_void)            _rtc_init)
#define rtc_calibration      ((p_rtc_calibration)     _rtc_calibration)
#define rtc_correction       ((p_rtc_correction)      _rtc_correction)
#define rtc_time_set         ((p_rtc_time_set)        _rtc_time_set)
#define rtc_time_get         ((p_rtc_void)            _rtc_time_get)
#define rtc_capture_enable   ((p_rtc_capture_enable)  _rtc_capture_enable)
#define rtc_capture_disable  ((p_rtc_void)            _rtc_capture_disable)
#define dec2bcd              ((p_dec2bcd)             _dec2bcd)
#define bcd2dec              ((p_bcd2dec)             _bcd2dec)


#else

extern struct rtc_env_tag rtc_env;

#if CONFIG_RTC_DEFAULT_IRQHANDLER==TRUE
void RTC_IRQHandler(void);
void RTC_CAP_IRQHandler(void);
#endif

extern void rtc_init(void);
extern void rtc_calibration(uint8_t dir, uint16_t ppm);
extern void rtc_correction(uint32_t sleep_count);
extern void rtc_time_set(uint8_t year, uint8_t month,  uint8_t day,
                         uint8_t hour, uint8_t minute, uint8_t second, void (*callback)(void));

extern void rtc_capture_enable(enum RTC_CAP_EDGE edge, void (*callback)(void));
extern void rtc_capture_disable(void);

extern uint8_t dec2bcd(uint8_t decade);
extern uint8_t bcd2dec(uint8_t bcd);

#if 0
extern uint32_t rtc_time_get(void);
#else
extern void rtc_time_get(void);
#endif
#endif

/// @} RTC
#endif /* CONFIG_ENABLE_DRIVER_RTC==TRUE */
#endif /* _RTC_H_ */
