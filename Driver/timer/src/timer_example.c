/****************************************************************************
 *   $Id:: timer_example.c                                                   $
 *   Project: QUINTIC QN9020 Timer example
 *
 *   Description:
 *     This file contains Timer driver usage. 
 *     
 *     1. timer interrupt example;
 *     2. timer pwm output example;
 *     3. timer capture timer mode example;
 *     4. timer capture counter mode example;
 *     5. timer capture event mode example.
 *     
****************************************************************************/

#include "gpio.h"
#include "timer.h"
#include "system.h"

/*
 * Excample switch
 */
#define TIMER_INTERRUPT_EXAMPLE            TRUE  // timer interrupt example;
#define TIMER_PWM_EXAMPLE                  FALSE // timer pwm output example;
#define TIMER_CAPTURE_TIMER_MOD_EXAMPLE    FALSE // timer capture timer mode example;
#define TIMER_CAPTURE_COUNTER_MOD_EXAMPLE  FALSE // timer capture counter mode example;
#define TIMER_CAPTURE_EVENT_MOD_EXAMPLE    FALSE // timer capture event mode example.

#define LED_PINS         (GPIO_P32|GPIO_P31|GPIO_P05|GPIO_P04)

void timer_io_config(void)
{
    // pin mux
    syscon_SetPMCR0(QN_SYSCON, P07_SW_CLK_PIN_CTRL       
                             | P06_SW_DAT_PIN_CTRL
                             | P00_UART0_TXD_PIN_CTRL      
                             | P17_UART0_RXD_PIN_CTRL    
                             | P11_TIMER1_ICP0_PIN_CTRL //P1.1 timer1, used for pwm and caputure
                             );
    syscon_SetPMCR1(QN_SYSCON, P35_TIMER0_ICP0_PIN_CTRL //P3.5 timer0, used for pwm and caputure    
                             | P26_TIMER2_ICP0_PIN_CTRL //P2.6 timer2, used for pwm and caputure         
                             | P23_TIMER3_ICP0_PIN_CTRL //P2.3 timer3, used for pwm and caputure      
                             );

    // pin pull ( 00 : High-Z,  01 : Pull-down,  10 : Pull-up,  11 : Reserved )
    syscon_SetPPCR0(QN_SYSCON, 0xAAAA5AAA);
    syscon_SetPPCR1(QN_SYSCON, 0x2AAAAAAA);
}

void led0_link(void)
{
    gpio_toggle_pin(GPIO_P32);
}
void led1_link(void)
{
    gpio_toggle_pin(GPIO_P31);
}
void led2_link(void)
{
    gpio_toggle_pin(GPIO_P05);
}
void led3_link(void)
{
    gpio_toggle_pin(GPIO_P04);
}

void timer0_callback(void)
{
    //int timer0_count = timer0_env.count;
}

int main (void) 
{

    SystemInit();

    /* Initialize GPIO (sets up clock) */
    gpio_init(NULL);
    /* Set all pin to output */
    gpio_set_direction_field(LED_PINS, (uint32_t)GPIO_OUTPUT);
    gpio_write_pin_field(LED_PINS, (uint32_t)GPIO_HIGH);
    
    
    // Timer io configurate
    timer_io_config();
    
#if TIMER_INTERRUPT_EXAMPLE
    timer_init(QN_TIMER0, led0_link);
    timer_config(QN_TIMER0, TIMER_PSCAL_DIV, TIMER_COUNT_US(1000, TIMER_PSCAL_DIV));
    timer_enable(QN_TIMER0, MASK_ENABLE);

    timer_init(QN_TIMER1, led1_link);
    timer_config(QN_TIMER1, TIMER_PSCAL_DIV, TIMER_COUNT_US(2000, TIMER_PSCAL_DIV));
    timer_enable(QN_TIMER1, MASK_ENABLE);

    timer_init(QN_TIMER2, led2_link);
    timer_config(QN_TIMER2, 10, TIMER_COUNT_MS(3, 10));
    timer_enable(QN_TIMER2, MASK_ENABLE);

    timer_init(QN_TIMER3, led3_link);
    timer_config(QN_TIMER3, 10, TIMER_COUNT_MS(4, 10));
    timer_enable(QN_TIMER3, MASK_ENABLE);
#endif

#if TIMER_PWM_EXAMPLE
    //P3.6 will output pwm wave with period for 1000us and pulse for 500us 
    timer_init(QN_TIMER0, NULL);
    timer_pwm_config(QN_TIMER0, TIMER_PSCAL_DIV, TIMER_COUNT_US(1000, TIMER_PSCAL_DIV), TIMER_COUNT_US(500, TIMER_PSCAL_DIV));
    timer_enable(QN_TIMER0, MASK_ENABLE);
    
    //P1.1 will output pwm wave with period for 2000us and pulse for 1000us
    timer_init(QN_TIMER1, NULL);
    timer_pwm_config(QN_TIMER1, TIMER_PSCAL_DIV, TIMER_COUNT_US(2000, TIMER_PSCAL_DIV), TIMER_COUNT_US(1000, TIMER_PSCAL_DIV));
    timer_enable(QN_TIMER1, MASK_ENABLE);
    
    //P2.6 will output pwm wave with period for 4ms and pulse for 2ms
    timer_init(QN_TIMER2, NULL);
    timer_pwm_config(QN_TIMER2, 10, TIMER_COUNT_MS(4, 10),  TIMER_COUNT_MS(2, 10));
    timer_enable(QN_TIMER2, MASK_ENABLE);
    
    //P2.3 will output pwm wave with period for 6ms and pulse for 3ms
    timer_init(QN_TIMER3, NULL);
    timer_pwm_config(QN_TIMER3, 10, TIMER_COUNT_MS(6, 10),  TIMER_COUNT_MS(3, 10));
    timer_enable(QN_TIMER3, MASK_ENABLE);
#endif

#if TIMER_CAPTURE_TIMER_MOD_EXAMPLE
    /*
     * Capture timer mode is to capture trigger event, and record the time-stamp.
     *
     * Make sure the macro TIMER0_CAP_MODE is INCAP_TIMER_MOD. 
     *
     * Timer0 will capture the P3.6 rising edge, and each rising edge will trigger 
     * the callback function, as the same time the counter value will transmit to 
     * timer0_env.count.
     * 
     * Other timers are similar to this.
     */
    timer_init(QN_TIMER0, timer0_callback);
    timer_capture_config(QN_TIMER0, INCAP_TIMER_MOD, 0, 0, 0);
    timer_enable(QN_TIMER0, MASK_ENABLE);
#endif

#if TIMER_CAPTURE_COUNTER_MOD_EXAMPLE
    /*
     * Capture event mode is to calculate the time of happened specified event with 
     * specified numbers.
     *
     * Make sure the macro TIMER0_CAP_MODE is INCAP_COUNTER_MOD. 
     *
     * Timer0 will count the time during 5 times rising edge of P3.6. After that, the 
     * callback function will be called, as the same time the time value will transmit to 
     * timer0_env.count.
     * 
     * Other timers are similar to this.
     */
    timer_init(QN_TIMER0, timer0_callback);
    timer_capture_config(QN_TIMER0, INCAP_COUNTER_MOD, 100, 0, 5);
    timer_enable(QN_TIMER0, MASK_ENABLE);
#endif

#if TIMER_CAPTURE_EVENT_MOD_EXAMPLE
    /*
     * Capture conter mode is used to count the numbers of a special event during a specified period.
     * 
     * Make sure the macro TIMER0_CAP_MODE is INCAP_EVENT_MOD. 
     *
     * Timer0 will count the numbers of rising edge during 1000ms with P3.6, and each rising 
     * edge will trigger the callback function, as the same time the counter value will transmit 
     * to timer0_env.count.
     * 
     * Other timers are similar to this.
     */
    timer_init(QN_TIMER0, timer0_callback);
    timer_capture_config(QN_TIMER0, INCAP_EVENT_MOD, 100, TIMER_COUNT_MS(1000, 100), 0);
    timer_enable(QN_TIMER0, MASK_ENABLE);
#endif

    while (1)                                /* Loop forever */
    {        
       
    }
}
