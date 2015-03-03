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

