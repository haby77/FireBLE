/**
 ****************************************************************************************
 *
 * @file arch_main.c
 *
 * @brief Main loop of the application.
 *
 * Copyright (C) Quintic 2012-2014
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */

/**
 * @addtogroup MAIN
 * @{
 */

/*
 * INCLUDES
 ****************************************************************************************
 */

#include "stdint.h"
#include "app_config.h"
#include "ble.h"
#include "intc.h"
#include "lib.h"
#if (QN_WORK_MODE != WORK_MODE_HCI)
    #include "prf_utils.h"
#endif
#if (QN_WORK_MODE == WORK_MODE_SOC)
    #include "app_env.h"
#endif

#include "usr_design.h"
#include "system.h"
#include "uart.h"
#include "spi.h"
#include "sleep.h"
#include "led.h"

/*
 * LOCAL VARIABLES
 ****************************************************************************************
 */

static uint8_t ble_heap[BLE_HEAP_SIZE];
#if QN_NVDS_WRITE
static uint8_t nvds_tmp_buf[NVDS_TMP_BUF_SIZE];
#endif

#ifdef CFG_DBG_PRINT
/**
 ****************************************************************************************
 * @brief Assert error 
 ****************************************************************************************
 */
void assert_err(const char *condition, const char * file, int line)
{
    QPRINTF("ASSERT_ERR(%s), in %s at line %d\r\n", condition, file, line);
    GLOBAL_INT_STOP();
    while(1);
}

void assert_param(int param0, int param1, const char * file, int line)
{
    GLOBAL_INT_STOP();
    while(1);
}

void assert_warn(const char *condition, const char * file, int line)
{
    QPRINTF("ASSERT_WARN(%s), in %s at line %d\r\n", condition, file, line);
}
#endif //CFG_DBG_PRINT


/**
 ****************************************************************************************
 * @brief Profiles register function
 *
 * This function register profile callback into BLE stack.
 *
 ****************************************************************************************
 */
#if (QN_WORK_MODE != WORK_MODE_HCI)
static void prf_register(void)
{
    prf_init_reg(prf_init);

    prf_disp_disconnect_reg(prf_dispatch_disconnect);
}
#endif

/**
 ****************************************************************************************
 * @brief BLE main function.
 *
 * This function is called right after the booting process has completed.
 ****************************************************************************************
 */
int main(void)
{
    int ble_sleep_st, usr_sleep_st;

    // DC-DC
    dc_dc_enable(QN_DC_DC_ENABLE);

    // QN platform initialization
#if QN_NVDS_WRITE
    plf_init(QN_POWER_MODE, __XTAL, QN_32K_RCO, nvds_tmp_buf, NVDS_TMP_BUF_SIZE);
#else
    plf_init(QN_POWER_MODE, __XTAL, QN_32K_RCO, NULL, 0);
#endif

#if (defined(QN_9020_B1) && (!QN_PMU_VOLTAGE))
    disable_patch_b1();
#endif

    // System initialization, user configuration
    SystemInit();

    // Profiles register
#if (QN_WORK_MODE != WORK_MODE_HCI)
    prf_register();
#endif

    // BLE stack initialization
    // Notes:
    // 1. When the chip works on Network Processor Mode, UART flow control signal is used to implement sleep mode.
    // UART 's flow control feature shall be enabled. Enable this feature in the uart.c file.
    // 2. Controller mode does not support sleep mode.
    // 3. So far client example project does not support sleep mode. It will be implemented later.

    // Check to go normal work mode or test mode.
    // If the input of test control pin is low level, the program will enter into test mode, otherwise the program will
    // enter into work mode which is defined in the user configuration file.
#if (defined(QN_TEST_CTRL_PIN))
    if(gpio_read_pin(QN_TEST_CTRL_PIN) == GPIO_HIGH)
    {
#endif
        // Work mode defined in the usr_config.h
        ble_init((enum WORK_MODE)QN_WORK_MODE, QN_HCI_PORT, QN_HCI_RD, QN_HCI_WR, ble_heap, BLE_HEAP_SIZE, QN_BLE_SLEEP);
#if (defined(QN_TEST_CTRL_PIN))
    }
    else
    {
        // Test mode (controller mode)
        ble_init((enum WORK_MODE)WORK_MODE_HCI, QN_HCI_PORT, QN_HCI_RD, QN_HCI_WR, ble_heap, BLE_HEAP_SIZE, false);
        // In the test mode, the program moniter test control pin. If the input of test control ping changes to low level,
        // it means work mode should be switched to the mode defined in the user configuration file.
        gpio_set_interrupt(QN_TEST_CTRL_PIN, GPIO_INT_HIGH_LEVEL);
        gpio_enable_interrupt(QN_TEST_CTRL_PIN);
    }
#endif

    set_max_sleep_duration(QN_BLE_MAX_SLEEP_DUR);

    // If QN902x works on wireless SoC mode, initialize APP task
#if (QN_WORK_MODE == WORK_MODE_SOC)
    app_init();
#endif

    usr_init();

    sleep_init();
    wakeup_by_sleep_timer(__32K_TYPE);

    GLOBAL_INT_START();

    while(1)
    {
        ke_schedule();

        // Checks for sleep have to be done with interrupt disabled
        GLOBAL_INT_DISABLE_WITHOUT_TUNER();

        // Check whether the chip can enters sleep mode
        //
        // Chip enter sleep condition:
        // +--------+--------+--------+--------+--------+
        // |    USR |        |        |        |        |
        // | BLE    | ACTIVE | IDLE   | SLEEP  | DEEP   |
        // +--------+--------+--------+--------+--------+
        // | ACTIVE | active | active | active | active |
        // | IDLE   | active | idle   | idle   | idle   |
        // | SLEEP  | active | idle   | sleep  | deep   |
        // +--------+--------+--------+--------+--------+

        // Obtain the status of the user program
        usr_sleep_st = usr_sleep();

        // If the user program can be sleep or deep sleep then check ble status
        if(usr_sleep_st != PM_ACTIVE)
        {
            // Obtain the status of ble sleep mode
            ble_sleep_st = ble_sleep(usr_sleep_st);

            // Check if the processor clock can be gated
            if(((ble_sleep_st == PM_IDLE) || (usr_sleep_st == PM_IDLE))
             && (ble_sleep_st != PM_ACTIVE))
            {
                // Debug
                led_set(5, LED_OFF);
                led_set(4, LED_ON);  // led4 is on when enter into gating mode

                enter_sleep(SLEEP_CPU_CLK_OFF,
                            WAKEUP_BY_ALL_IRQ_SOURCE,
                            NULL);

                // Debug
                led_set(4, LED_OFF);
                led_set(5, LED_ON);  // led5 is on when enter into active mode
            }

            // Check if the processor can be power down
            else if((ble_sleep_st == PM_SLEEP) && (usr_sleep_st == PM_SLEEP))
            {
                // Debug
                led_set(5, LED_OFF);
                led_set(3, LED_ON);  // led3 is on when enter into sleep mode

                enter_sleep(SLEEP_NORMAL,
                            (WAKEUP_BY_OSC_EN | WAKEUP_BY_GPIO),
                            sleep_cb);

                // Debug
                led_set(3, LED_OFF);
                led_set(5, LED_ON);  // led5 is on when enter into active mode
            }

            // Check if the system can be deep sleep
            else if((ble_sleep_st == PM_SLEEP) && (usr_sleep_st == PM_DEEP_SLEEP))
            {
                // Debug
                led_set(5, LED_OFF);
                led_set(2, LED_ON);  // led2 is on when enter into deep sleep mode

                enter_sleep(SLEEP_DEEP,
                            WAKEUP_BY_GPIO,
                            sleep_cb);

                // Debug
                led_set(2, LED_OFF);
                led_set(5, LED_ON);  // led5 is on when enter into active mode
            }
        }

        // Checks for sleep have to be done with interrupt disabled
        GLOBAL_INT_RESTORE_WITHOUT_TUNER();
    }
}

/// @} MAIN


