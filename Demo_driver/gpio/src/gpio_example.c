/****************************************************************************
 *   $Id:: gpio_example.c                                                   $
 *   Project: QUINTIC QN9020 GPIO example
 *
 *   Description:
 *     This file contains GPIO driver usage.
 *
****************************************************************************/

#include "gpio.h"
#include "system.h"


/****************************************************************************
*               GPIO driver code example
* All the 31 GPIO pins can be configured to input or output, and support interrupt.
*
****************************************************************************/


#define GPIO_INTERRUPT_EXAMPLE      FALSE
#define GPIO_INPUT_EXAMPLE          FALSE
#define GPIO_OUTPUT_EXAMPLE         TRUE

void gpio_io_config(void)
{
    // pin mux
    syscon_SetPMCR0(QN_SYSCON, P00_GPIO_0_PIN_CTRL
                             | P01_GPIO_1_PIN_CTRL
                             | P02_GPIO_2_PIN_CTRL
                             | P03_GPIO_3_PIN_CTRL
                             | P04_GPIO_4_PIN_CTRL
                             | P05_GPIO_5_PIN_CTRL
                             | P06_SW_DAT_PIN_CTRL
                             | P07_SW_CLK_PIN_CTRL
                             | P10_GPIO_8_PIN_CTRL
                             | P11_GPIO_9_PIN_CTRL
                             | P12_GPIO_10_PIN_CTRL
                             | P13_GPIO_11_PIN_CTRL
                             | P14_GPIO_12_PIN_CTRL
                             | P15_GPIO_13_PIN_CTRL
                             | P16_GPIO_14_PIN_CTRL
                             | P17_GPIO_15_PIN_CTRL
                             );
    syscon_SetPMCR1(QN_SYSCON, P20_GPIO_16_PIN_CTRL
                             | P21_GPIO_17_PIN_CTRL
                             | P22_GPIO_18_PIN_CTRL
                             | P23_GPIO_19_PIN_CTRL
                             | P24_GPIO_20_PIN_CTRL
                             | P25_GPIO_21_PIN_CTRL
                             | P26_GPIO_22_PIN_CTRL
                             | P27_GPIO_23_PIN_CTRL
                             | P30_GPIO_24_PIN_CTRL
                             | P31_GPIO_25_PIN_CTRL
                             | P32_GPIO_26_PIN_CTRL
                             | P33_GPIO_27_PIN_CTRL
                             | P34_GPIO_28_PIN_CTRL
                             | P35_GPIO_29_PIN_CTRL
                             | P36_GPIO_30_PIN_CTRL
                             );
}

void cb_gpio_p14(void)
{
    gpio_toggle_pin(GPIO_P01);
    //delay(50000);
}
void cb_gpio_p15(void)
{
    gpio_toggle_pin(GPIO_P02);
    //delay(50000);
}

void cb_gpio(enum gpio_pin pin)
{
    if (pin == GPIO_P14) {
        cb_gpio_p14();
    }
    else if (pin == GPIO_P15) {
        cb_gpio_p15();
    }
}

uint32_t result;
enum gpio_level result_level;

/* Main Program */
int main (void)
{   

    SystemInit();
    gpio_io_config();
    
    /* Initialize GPIO */
    gpio_init(cb_gpio);
    
#if GPIO_INTERRUPT_EXAMPLE == TRUE
    /* set P1.4 and P1.5 to pull up */
    gpio_pull_set(GPIO_P14, GPIO_PULL_UP);
    gpio_pull_set(GPIO_P15, GPIO_PULL_UP);
    
    /* set P1.4 and P1.5 direction to input*/
    gpio_set_direction_field(GPIO_P14 | GPIO_P15, (uint32_t)GPIO_INPUT);

    /* set P0.1 and P0.2 direction to output*/
    gpio_set_direction_field(GPIO_P01 | GPIO_P02, (uint32_t)GPIO_OUTPUT);
    
    /* set P0.1 and P0.2 to output low level*/
    gpio_write_pin_field(GPIO_P01 | GPIO_P02, GPIO_LOW);
    
    
    /* set P1.5 interrupt to rising edge*/
    gpio_set_interrupt(GPIO_P14, GPIO_INT_RISING_EDGE);

    /* set P1.5 interrupt to falling edge*/
    gpio_set_interrupt(GPIO_P15, GPIO_INT_FALLING_EDGE);
    
    /* enable P1.5 and P1.5 interrupt and set the callback function*/
    gpio_enable_interrupt(GPIO_P14);
    gpio_enable_interrupt(GPIO_P15);
    
    while(1);
#endif    
    
#if GPIO_INPUT_EXAMPLE == TRUE

    /* Set all pin to input */
    gpio_set_direction_field(GPIO_PIN_ALL, (uint32_t)GPIO_INPUT);
    while(1)
    {
        /* read all pins input level */
        result = gpio_read_pin_field(GPIO_PIN_ALL);
        
        /* read P0.0 input level*/
        result_level = gpio_read_pin(GPIO_P00);
    }
    
#endif

#if GPIO_OUTPUT_EXAMPLE == TRUE
    
    /* Set all pin to output */
    gpio_set_direction_field(GPIO_PIN_ALL, (uint32_t)GPIO_OUTPUT);
    while(1)
    {
        int i;
        for(i=0; i<31; ++i)
        {
            /* Polling all pins to output low level, and the order is from P0.0 to P3.6 */
            gpio_write_pin_field(GPIO_PIN_ALL, ~(1<<i));
            delay(10000);
        }
    }
#endif
}

