/**
 ****************************************************************************************
 *
 * @file lib.h
 *
 * @brief QN9020 library API header file.
 *
 * Copyright (C) Quintic 2012-2014
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */
 
#ifndef LIB_H_
#define LIB_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include <stdbool.h>
#if (QN_WORK_MODE == WORK_MODE_SOC)
#include "app_env.h"
#endif

/*
 * DEFINITIONS
 ****************************************************************************************
 */
#define QN_DBG_INFO_REG                     0x1000FFFC

#define QN_DBG_INFO_XTAL_WAKEUP_DURATION    0x00000001
#define QN_DBG_INFO_BLE_HEAP_FULL           0x00000002

/*
 * ENUMERATION DEFINITIONS
 ****************************************************************************************
 */
/// Work mode
enum WORK_MODE
{
    SOC_MODE,   // Wireless SoC
    NP_MODE,    // Network processor
    HCI_MODE    // Controller only
};

/// Power mode
enum PW_MODE
{
    NORMAL_MODE,        // Low power
    HIGH_PERFORMANCE    // High Power
};

/// Status of ke_evt
enum KE_EVENT_STATUS
{
    KE_EVENT_OK = 0,
    KE_EVENT_FAIL,
    KE_EVENT_UNKNOWN,
    KE_EVENT_CAPA_EXCEEDED,
    KE_EVENT_ALREADY_EXISTS,
};

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

typedef void (*hci_api)(void *port, uint8_t *bufptr, uint32_t size, void (*callback)(void));

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief DC-DC Enable
 * @param[in]   enable true - enable dc-dc; false - disable
 ****************************************************************************************
 */
extern void dc_dc_enable(bool enable);

/**
 ****************************************************************************************
 * @brief Set 32k xtal ppm.
 * @param[in]     ppm
 ***************************************************************************************
 */
#if (defined(QN_9020_B2) || defined(QN_9020_B1))
typedef void (*p_set_32k_ppm)(int32_t ppm);
#define set_32k_ppm ((p_set_32k_ppm)(_set_32k_ppm))
#elif defined(QN_9020_B0)
extern void set_32k_ppm(int32_t ppm);
#endif

/**
 ****************************************************************************************
 * @brief Set 32k xtal frequency.
 * @param[in]     freq  frequency (Hz)
 ***************************************************************************************
 */
#if (defined(QN_9020_B2) || defined(QN_9020_B1))
typedef void (*p_set_32k_freq)(int32_t ppm);
#define set_32k_freq ((p_set_32k_freq)(_set_32k_freq))
#elif defined(QN_9020_B0)
extern void set_32k_freq(uint32_t freq);
#endif

/**
 ****************************************************************************************
 * @brief Initilaize BLE hardware platform
 * @param[in]   pw_mode             NORMAL_MODE - low power; HIGH_PERFORMANCE - high power
 * @param[in]   xtal                which crystal is used - 16MHz or 32MHz
 * @param[in]   clk_32k             which 32k is used - 0:xtal, 1:rco
 * @param[in]   nvds_tmp_buf        pointer of nvds temp buffer
 * @param[in]   nvds_tmp_buf_len    length of nvds temp buffer
 ****************************************************************************************
 */
extern void plf_init(enum PW_MODE pw_mode, uint32_t xtal, uint8_t clk_32k, uint8_t* nvds_tmp_buf, uint32_t nvds_tmp_buf_len);

/**
 ****************************************************************************************
 * @brief Initialize BLE stack
 * @param[in]   mode            SoC, Network Processor or Controller
 * @param[in]   port            The UART/SPI interface which is used in NP or Controller mode
 * @param[in]   uart_read       The UART/SPI read API
 * @param[in]   uart_write      The UART/SPI write API
 * @param[in]   ble_heap_addr   The start address of BLE heap
 * @param[in]   ble_heap_size   The size of BLE heap
 * @param[in]   sleep_enable    BLE stack sleep enable
 ****************************************************************************************
 */
extern void ble_init(enum WORK_MODE mode, void *port, hci_api hci_read, hci_api hci_write, uint8_t *ble_heap_addr, uint32_t ble_heap_size, bool sleep_enable);

/**
 ****************************************************************************************
 * @brief Configure work mode 
 * @param[in]   mode            Work mode (Soc, Network Processor, Controller)
 * @param[in]   port            The UART/SPI interface which is used in NP or Controller mode
 * @param[in]   uart_read       The UART/SPI read API
 * @param[in]   uart_write      The UART/SPI write API
 ****************************************************************************************
 */
extern void config_work_mode(enum WORK_MODE mode, void *port, hci_api hci_read, hci_api hci_write);

/**
 ****************************************************************************************
 * @brief Register EACI tx done function
 * @param[in]   p_eaci_tx_done  The pointer of eaci_tx_done()
 ****************************************************************************************
 */
extern void reg_eaci_tx_done(void (*p_eaci_tx_done)(void));

/**
 ****************************************************************************************
 * @brief Scheduler
 ****************************************************************************************
 */
extern void ke_schedule(void);

/**
 ****************************************************************************************
 * @brief Allow or prevent from the BLE hardware going to sleep
 * @param[in]   enable  ture - allow; false - prevent
 ****************************************************************************************
 */
typedef void (*p_enable_ble_sleep)(bool enable);
#define enable_ble_sleep ((p_enable_ble_sleep)(_enable_ble_sleep))

/**
 ****************************************************************************************
 * @brief Set maximum sleep duration of BLE sleep timer. If the BLE stack works, it will
 *        revises the sleep duration based on BLE stack's requirement, otherwise this
 *        sleep duration will be the setting value.
 * @param[in]   duration    unit 625us, maximum is 209715199(36hours16mins)
 ****************************************************************************************
 */
#if (defined(QN_9020_B2) || defined(QN_9020_B1))
typedef bool (*p_set_max_sleep_duration)(uint32_t duration);
#define set_max_sleep_duration ((p_set_max_sleep_duration)(_set_max_sleep_duration))
#elif defined(QN_9020_B0)
extern bool set_max_sleep_duration(uint32_t duration);
#endif

/**
 ****************************************************************************************
 * @brief Set BLE program latency. The software should program BLE hardware before 
 *        the BLE hardware handle BLE event. This value specifices this duration.
 * @param[in]   latency    0 < latency <= 8, unit 625us, default value is 4
 ****************************************************************************************
 */
#if (defined(QN_9020_B2) || defined(QN_9020_B1))
extern void set_ble_program_latency(uint8_t latency);
#endif

/**
 ***************************************************************
 * @brief Check the sleep status of ble hardware.
 ***************************************************************
*/
extern bool ble_hw_sleep(void);

/**
 ***************************************************************
 * @brief Check whether to do BLE external wakeup.
 ***************************************************************
*/
extern bool ble_ext_wakeup_allow(void);

/**
 ***************************************************************
 * @brief Wakeup BLE hardware by software.
 ***************************************************************
*/
#if (defined(QN_9020_B2) || defined(QN_9020_B1))
typedef void (*p_sw_wakeup_ble_hw)(void);
#define sw_wakeup_ble_hw ((p_sw_wakeup_ble_hw)(_sw_wakeup_ble_hw))
#elif defined(QN_9020_B0)
extern void sw_wakeup_ble_hw(void);
#endif

/**
 ****************************************************************************************
 * @brief Register sleep callback.
 *        enter_cb is invoked before BLE entering sleep mode. If the return of
 *        callback function is FALSE, the BLE hardware will not enter into sleep mode.
 *        exit_cb provides user a way to do sth after ble hardware wakeup.
 * @param[in]     enter_cb   Callback function 
 * @param[in]     exit_cb    Callback function
 ***************************************************************************************
 */
extern void reg_ble_sleep_cb(bool (*enter_cb)(void), void (*exit_cb)(void));

/**
 ****************************************************************************************
 * @brief Save configuration which will lose in sleep mode
 ****************************************************************************************
 */
#if (defined(QN_9020_B2) || defined(QN_9020_B1))
typedef void (*p_save_ble_setting)(void);
#define save_ble_setting ((p_save_ble_setting)(_save_ble_setting))
#elif defined(QN_9020_B0)
extern void save_ble_setting(void);
#endif

/**
 ****************************************************************************************
 * @brief Restore configuration which is saved before entering sleep mode
 ****************************************************************************************
 */
extern void restore_ble_setting(void);

/**
 ****************************************************************************************
 * @brief Sleep post process
 ****************************************************************************************
 */
extern void sleep_post_process(void);

/**
 ****************************************************************************************
 * @brief Check that BLE has already waked-up
 ****************************************************************************************
 */
extern uint32_t check_ble_wakeup(void);

/**
 ****************************************************************************************
 * @brief Set events
 *
 * This primitive sets one or more events in the event field variable.
 * The corresponding event handler set by ke_evt_callback_set() will be invoked
 * in the scheduler.
 *
 * @param[in]  event       Events that have to be set (bit field).
 *
 ****************************************************************************************
 */
typedef void (*p_ke_evt_set)(uint32_t const);
#define ke_evt_set ((p_ke_evt_set)(_ke_evt_set))

/**
 ****************************************************************************************
 * @brief Clear events
 *
 * This primitive clears one or more events in the event field variable.
 * Generally when the event handler is invoked, the corresponding event bit should
 * be clear in the event handler to prevent reentry.
 *
 * @param[in]  event       Events that have to be cleared (bit field).
 *
 ****************************************************************************************
 */
typedef void (*p_ke_evt_clear)(uint32_t const);
#define ke_evt_clear ((p_ke_evt_clear)(_ke_evt_clear))

/**
 ****************************************************************************************
 * @brief Register an event callback.
 *
 * When the scheduler is called in the main loop of the background, the kernel
 * checks if the event field is non-null, gets the one with highest priority and
 * executes the event handlers for which the corresponding event bit is set.
 *
 * There are total 32 events, and the highest priority events are used by BLE stack.
 * So users have 24 events could be used in the application. The most significant bit
 * has the highest priority. The LSB has the lowest priority.
 *
 * @param[in]  event_type       Event type. (user can use only 0 ~ 23)
 * @param[in]  p_callback       Pointer to callback function.
 *
 * @return                      Status
 ****************************************************************************************
 */
#if (defined(QN_9020_B2) || defined(QN_9020_B1))
typedef enum KE_EVENT_STATUS (*p_ke_evt_callback_set)(uint8_t event_type, void (*p_callback)(void));
#define ke_evt_callback_set ((p_ke_evt_callback_set)(_ke_evt_callback_set))
#elif defined(QN_9020_B0)
extern enum KE_EVENT_STATUS ke_evt_callback_set(uint8_t event_type, void (*p_callback)(void));
#endif

/**
 ****************************************************************************************
 * @brief Get system tick count, unit is 10ms
 ****************************************************************************************
 */
extern uint32_t ke_time(void);

/**
 ****************************************************************************************
 * @brief Get ke timer queue status, TRUE means empty
 ****************************************************************************************
 */
extern bool ke_timer_empty(void);

/**
 ****************************************************************************************
 * @brief set debug bit, the debug information is saved at 0x1000fffc.
 * @param[in]  dbg_info_bit     Debug information
 ****************************************************************************************
 */
extern void set_dbg_info(uint32_t dbg_info_bit);

/**
 ****************************************************************************************
 * @brief Disable B1 patch
 ****************************************************************************************
 */
void disable_patch_b1(void);

#endif

