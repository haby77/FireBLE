/**
 ****************************************************************************************
 *
 * @file usr_design.c
 *
 * @brief Product related design.
 *
 * Copyright (C) Quintic 2012-2013
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup  USR
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include <stdint.h>
#include "led.h"
#include "uart.h"
#include "spi.h"
#include "gpio.h"
#include "lib.h"
#include "app_eaci_trans.h"
#include "app_eaci.h"
#include "usr_design.h"

#define EVENT_UART_DATA_ID            0

struct usr_env_tag usr_env = {0, 0xffff};

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief   Restore peripheral setting after wakeup
 ****************************************************************************************
 */
void usr_sleep_restore(void)
{
    // Restore peripheral setting   
#if (defined(CFG_HCI_UART))
    // Initialize HCI UART port
    uart_init(QN_HCI_PORT, USARTx_CLK(0), UART_9600);
    uart_tx_enable(QN_HCI_PORT, MASK_ENABLE);
    uart_rx_enable(QN_HCI_PORT, MASK_ENABLE);
#elif (defined(CFG_HCI_SPI))
    // Initialize HCI SPI port
    spi_init(QN_HCI_PORT, SPI_BITRATE(1000000), SPI_8BIT, SPI_SLAVE_MOD);
#endif
    
   
#if QN_DBG_PRINT
    uart_init(QN_DEBUG_UART, USARTx_CLK(0), UART_9600);
    uart_tx_enable(QN_DEBUG_UART, MASK_ENABLE);
    uart_rx_enable(QN_DEBUG_UART, MASK_ENABLE);
#endif

    eaci_trans_init();

}

/**
 ****************************************************************************************
 * @brief   EACI wakeup gpio callback
 ****************************************************************************************
 */
static void eaci_wakeup_gpio_cb(void)
{
    // If BLE is in the sleep mode, wakeup it.
    if(ble_ext_wakeup_allow())
    {
#if ((QN_DEEP_SLEEP_EN) && (!QN_32K_RCO))
        if (sleep_env.deep_sleep)
        {
            wakeup_32k_xtal_switch_clk();
        }
#endif

        sw_wakeup_ble_hw();
    }
    
    ke_evt_set(1UL << EVENT_UART_DATA_ID);
}

/**
 ****************************************************************************************
 * @brief   EACI wakeup gpio initilization
 ****************************************************************************************
 */
void eaci_wakeup_init(void)
{ 
    // GPIO wakeup QN MCU
    //gpio_pull_set(QN_EACI_GPIO_WAKEUP_QN_MCU, GPIO_PULL_DOWN);
    //gpio_set_direction_field(QN_EACI_GPIO_WAKEUP_QN_MCU, (uint32_t)GPIO_INPUT);
    //gpio_set_interrupt(QN_EACI_GPIO_WAKEUP_QN_MCU, GPIO_INT_RISING_EDGE);
    gpio_wakeup_config(QN_EACI_GPIO_WAKEUP_QN_MCU, GPIO_WKUP_BY_LOW);
    gpio_enable_interrupt(QN_EACI_GPIO_WAKEUP_QN_MCU);

#if (defined(QN_EACI_GPIO_WAKEUP_EX_MCU))
    // GPIO wakeup external MCU
    gpio_write_pin(QN_EACI_GPIO_WAKEUP_EX_MCU, GPIO_HIGH);
    gpio_set_direction_field(QN_EACI_GPIO_WAKEUP_EX_MCU, (uint32_t)GPIO_OUTPUT);
#endif  
}

/**
 ****************************************************************************************
 * @brief   EACI wakeup external MCU start
 ****************************************************************************************
 */
#if (defined(QN_EACI_GPIO_WAKEUP_EX_MCU))
void eaci_wakeup_ex_mcu_start(void)
{
    gpio_write_pin(QN_EACI_GPIO_WAKEUP_EX_MCU, GPIO_LOW); 
}
#endif

/**
 ****************************************************************************************
 * @brief   EACI wakeup external MCU stop
 ****************************************************************************************
 */
#if (defined(QN_EACI_GPIO_WAKEUP_EX_MCU))
void eaci_wakeup_ex_mcu_stop(void)
{
    gpio_write_pin(QN_EACI_GPIO_WAKEUP_EX_MCU, GPIO_HIGH); 
}
#endif

/**
 ****************************************************************************************
 * @brief   All GPIO interrupt callback
 ****************************************************************************************
 */
void gpio_interrupt_callback(enum gpio_pin pin)
{
    switch(pin)
    {
        case QN_EACI_GPIO_WAKEUP_QN_MCU:
            eaci_wakeup_gpio_cb();
            break;

        default:
            break;
    }
}

/**
 ****************************************************************************************
 * @brief   Led1 for BLE status
 ****************************************************************************************
 */
void usr_led1_set(uint16_t timer_on, uint16_t timer_off)
{
    usr_env.led1_on_dur = timer_on;
    usr_env.led1_off_dur = timer_off;

    if (timer_on == 0 || timer_off == 0)
    {
        if (timer_on == 0)
        {
            led_set(1, LED_OFF);
        }
        if (timer_off == 0)
        {
            led_set(1, LED_ON);
        }
        ke_timer_clear(APP_SYS_LED_1_TIMER, TASK_APP);
    }
    else
    {
        led_set(1, LED_OFF);
        ke_timer_set(APP_SYS_LED_1_TIMER, TASK_APP, timer_off);
    }
}

/**
 ****************************************************************************************
 * @brief   Led 1 flash process
 ****************************************************************************************
 */
static void usr_led1_process(void)
{
    if(led_get(1) == LED_ON)
    {
        led_set(1, LED_OFF);
        ke_timer_set(APP_SYS_LED_1_TIMER, TASK_APP, usr_env.led1_off_dur);
    }
    else
    {
        led_set(1, LED_ON);
        ke_timer_set(APP_SYS_LED_1_TIMER, TASK_APP, usr_env.led1_on_dur);
    }
}

/**
 ****************************************************************************************
 * @brief Handles LED status timer.
 *
 * @param[in] msgid      APP_SYS_LED_1_TIMER
 * @param[in] param      Pointer to struct app_uart_data_ind
 * @param[in] dest_id    TASK_APP
 * @param[in] src_id     TASK_APP
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
int app_led_timer_handler(ke_msg_id_t const msgid, void const *param,
                               ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    if(msgid == APP_SYS_LED_1_TIMER)
    {
        usr_led1_process();
    }

    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief Uart data
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
void app_event_uart_data_handler(void)
{
#if ((QN_DEEP_SLEEP_EN) && (!QN_32K_RCO))
    if (sleep_env.deep_sleep) 
    {
        sleep_env.deep_sleep = false;
        // start 32k xtal wakeup timer
        wakeup_32k_xtal_start_timer();
    }
#endif

    ke_evt_clear(1UL << EVENT_UART_DATA_ID);
}

/**
 ****************************************************************************************
 * @brief   User initialize
 ****************************************************************************************
 */
void usr_init(void)
{
    if(KE_EVENT_OK != ke_evt_callback_set(EVENT_UART_DATA_ID, 
                                            app_event_uart_data_handler))
    {
        ASSERT_ERR(0);
    }
}

/// @} USR
