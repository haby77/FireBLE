/****************************************************************************
 *   $Id:: analog_example.c                                                   $
 *   Project: QUINTIC QN9020 Analog modules example
 *
 *   Description:
 *     This file contains Analog driver usage.
 *
****************************************************************************/

#include "gpio.h"
#include "timer.h"
#include "sleep.h"
#include "system.h"
#include "calibration.h"

#define TEST_SLEEP_CPU_CLK_OFF          FALSE
#define TEST_SLEEP_NORMAL               TRUE
#define TEST_SLEEP_DEEP                 FALSE

static int wakeup_form_sleeptimer = 1;

void sleep_timer_set(uint32_t time)
{
    // TBC
    
    // ble interrupt mask
    *(volatile uint32_t *)(0x2f00000c) |= 0x4;
    
    // correction sleep time
    *(volatile uint32_t *)(0x2f000030) = (*(volatile uint32_t *)(0x2f000030) & (~0x8)) | 0x8;

    // sleep time
    *(volatile uint32_t *)(0x2f000034) =  time;
    
    // enable sleep
    *(volatile uint32_t *)(0x2f000030) = (*(volatile uint32_t *)(0x2f000030) & (~0x4)) | 0x4;

    // OSC SLEEP EN
    *(volatile uint32_t *)(0x2f000030) = (*(volatile uint32_t *)(0x2f000030) & (~0x3)) | 0x1;

    //delay(5000);
    while (syscon_GetBLESR(QN_SYSCON) & SYSCON_MASK_OSC_EN);
}

    
void Led_flash(void)
{
    gpio_set_direction(GPIO_P01, GPIO_OUTPUT);
    gpio_toggle_pin(GPIO_P01);
    delay(100000);
    gpio_toggle_pin(GPIO_P01);
}

void Led_toggle(void)
{
    gpio_set_direction(GPIO_P01, GPIO_OUTPUT);
    gpio_toggle_pin(GPIO_P01);
    delay(100000);
}

void cb_gpio(enum gpio_pin pin)
{

}

/**
 ****************************************************************************************
 * @brief OSC interrupt handler, BLE wakeup source
 ***************************************************************************************
 */
void OSC_EN_IRQHandler(void)
{
    NVIC->ICER[0] = 0x00000020;
    wakeup_form_sleeptimer = 1;
}

int main (void) 
{
    
    SystemInit();
    
#if 0
    if (0x00000004 & inp32(0x40000038)) {
        outp32(0x40000038, 0x80000000);
    }
    else {
        Led_flash();
        while(1);
    }
#endif
    
    /* Initialize GPIO */
    gpio_init(cb_gpio);
    
#if TEST_SLEEP_NORMAL == TRUE
    // --------------------------------------------
    // sleep wakeup
    // --------------------------------------------
    
    //set all pin to gpio
    syscon_SetPMCR0(QN_SYSCON, 0x00000000);
    syscon_SetPMCR1(QN_SYSCON, 0x00000000);
    //set all gpio input
    gpio_set_direction_field(GPIO_PIN_ALL, GPIO_INPUT); 
    gpio_write_pin_field(GPIO_PIN_ALL, (uint32_t)GPIO_HIGH);    
    // pin pull ( 00 : High-Z,  01 : Pull-down,  10 : Pull-up,  11 : Reserved )
    syscon_SetPPCR0(QN_SYSCON, 0xAAAA5AAA); // SWD pull-down save 20uA
    syscon_SetPPCR1(QN_SYSCON, 0x2AAAAAAA);  
    
    // power down BUCK needed
    syscon_SetIvrefX32WithMask(QN_SYSCON, SYSCON_MASK_BUCK_BYPASS|SYSCON_MASK_BUCK_DPD, MASK_ENABLE);    
    // power down Flash
    syscon_SetPGCR2WithMask(QN_SYSCON, SYSCON_MASK_FLASH_VCC_EN, MASK_DISABLE);
    // enable dbg power down
    syscon_SetPGCR2WithMask(QN_SYSCON, SYSCON_MASK_DBGPMUENABLE, MASK_ENABLE);
    // dis sar adc buffer
    syscon_SetPGCR1WithMask(QN_SYSCON, SYSCON_MASK_DIS_SAR_BUF, MASK_ENABLE);
    Led_flash();

    do {
        delay(10);
    } while (gpio_read_pin(GPIO_P14) == GPIO_HIGH);
    
    sleep_init();
    wakeup_by_sleep_timer(__32K_TYPE);
    wakeup_by_gpio(GPIO_P15, GPIO_WKUP_BY_LOW);
    
    do {
        gpio_set_direction(GPIO_P01, GPIO_INPUT);
        //enter_sleep(SLEEP_NORMAL, WAKEUP_BY_GPIO, Led_flash);
        if (wakeup_form_sleeptimer) {
            sleep_timer_set(32000);
            wakeup_form_sleeptimer = 0;
#if QN_32K_RCO == TRUE
            clock_32k_correction_enable(clock_32k_correction_cb);
#endif
        }

#if QN_32K_RCO == TRUE
        if (gpio_sleep_allowed() && !dev_get_bf())
#else
        if (gpio_sleep_allowed())
#endif
        enter_sleep(SLEEP_NORMAL, WAKEUP_BY_OSC_EN|WAKEUP_BY_GPIO, Led_flash);

    } while(1);
#endif
    
#if TEST_SLEEP_DEEP == TRUE
    // --------------------------------------------
    // deep sleep wakeup
    // --------------------------------------------
    
    //set all pin to gpio
    syscon_SetPMCR0(QN_SYSCON, 0x00000000);
    syscon_SetPMCR1(QN_SYSCON, 0x00000000);
    //set all gpio input
    gpio_set_direction_field(GPIO_PIN_ALL, (uint32_t)GPIO_INPUT); 
    gpio_write_pin_field(GPIO_PIN_ALL, (uint32_t)GPIO_HIGH);
    // pin pull ( 00 : High-Z,  01 : Pull-down,  10 : Pull-up,  11 : Reserved )
    syscon_SetPPCR0(QN_SYSCON, 0xAAAA5AAA); // SWD pull-down save 20uA
    syscon_SetPPCR1(QN_SYSCON, 0x2AAAAAAA);        
    
    // power down BUCK needed
    syscon_SetIvrefX32WithMask(QN_SYSCON, SYSCON_MASK_BUCK_BYPASS|SYSCON_MASK_BUCK_DPD, MASK_ENABLE);
    // power down Flash
    syscon_SetPGCR2WithMask(QN_SYSCON, SYSCON_MASK_FLASH_VCC_EN, MASK_DISABLE);
    // enable dbg power down
    syscon_SetPGCR2WithMask(QN_SYSCON, SYSCON_MASK_DBGPMUENABLE, MASK_ENABLE);
    // dis sar adc buffer
    syscon_SetPGCR1WithMask(QN_SYSCON, SYSCON_MASK_DIS_SAR_BUF, MASK_ENABLE);
    Led_flash();
    
    do {
        delay(10);
    } while (gpio_read_pin(GPIO_P14) == GPIO_HIGH);
    
    sleep_init();
    
    do {
        gpio_set_direction(GPIO_P01, GPIO_INPUT);
        wakeup_by_gpio(GPIO_P15, GPIO_WKUP_BY_CHANGE);
        enter_sleep(SLEEP_DEEP, WAKEUP_BY_GPIO, Led_flash);
                
    } while(1);
#endif

#if TEST_SLEEP_CPU_CLK_OFF == TRUE
    // --------------------------------------------
    // clock gating 
    // --------------------------------------------
    // Set timer 0 wakeup
    timer_init(QN_TIMER0, NULL);
    timer_config(QN_TIMER0, TIMER_PSCAL_DIV, TIMER_COUNT_MS(1000, TIMER_PSCAL_DIV));
    timer_enable(QN_TIMER0, MASK_ENABLE);  

    sleep_init();
    
    do {

        enter_sleep(SLEEP_CPU_CLK_OFF, WAKEUP_BY_TIMER0, NULL);
        Led_flash();
    
    }
    while(1);
#endif
    
    
}
