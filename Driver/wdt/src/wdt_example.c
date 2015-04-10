/****************************************************************************
 *   $Id:: wdt_example.c                                        $
 *   Project: QUINTIC QN9020 watchdog timer example
 *
 *   Description:
 *     This file contains watchdog timer driver usage.
 *
****************************************************************************/


#include "wdt.h"
#include "gpio.h"
#include "system.h"

#define SYS_CLK_32K_EN              FALSE

int main (void)
{
    SystemInit();

    gpio_init(NULL);
    gpio_set_direction(GPIO_P01, GPIO_OUTPUT); // P0.1 output
    gpio_write_pin(GPIO_P01, GPIO_LOW);
    delay(10000);

#if SYS_CLK_32K_EN==TRUE
    syscon_set_sysclk_src(CLK_LOW_32K, __32K_TYPE);
    // APB == AHB == 32K
    delay(177);
#endif

    //wdt_init(0xFFF, WDT_NO_ACTION_MOD);
    //wdt_init(0xFFF, WDT_INT_MOD);
    wdt_init(0x7F, WDT_RESET_MOD);
    gpio_write_pin(GPIO_P01, GPIO_HIGH);

    while (1)                                /* Loop forever */
    {

        //wdt_set(0x7F);
        delay(2);

        // test lock function
        //QN_WDT->LOAD = 0x7F;

        //while(1);
    }
}
