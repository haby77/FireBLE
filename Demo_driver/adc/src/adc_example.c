/****************************************************************************
 *   $Id:: adc_example.c                                                   $
 *   Project: QUINTIC QN9020 ADC example
 *
 *   Description:
 *     This file contains ADC driver usage.
 *
****************************************************************************/

#include "system.h"
#include "adc.h"
#include "timer.h"
#include "pwm.h"
#include "analog.h"
#include "uart.h"
#include "stdio.h"


/****************************************************************************
*               ADC driver code example
*ADC support 4 work mode:
*  Differential with input buffer: input signal range [0.2, VDD-0.2], 
*                                  measure range [-VREF, VREF) map to [-2048, 2047].
*  Differential without input buffer: input signal range [0, VDD], 
*                                     and should have driving capability, 
*                                     measure range [-VREF, VREF) map to [-2048, 2047].
*  Single-End with input buffer: input signal 0.2 =< VIN(V) <= 1.5*VREF <= VDD-0.2, 
*                                measure range [0.2, 1.5*VREF) map to [x,2047].
*  Single-End without input buffer: input signal 0 =< VIN(V) <= VREF <= VDD, 
*                                   and should have driving capability, 
*                                   measure range [0, VREF) map to [0,2047].
*
*ADC reference voltage support 3 types:
*  Internal: 1V (calibration value stored in NVDS)
*  External REF1(with buffer and gain=2): VREF = 2*EXT_REF1 (0 < EXT_REF1 < (VDD-1.0)/2)
*  External REF2(without buffer): VERF = EXT_REF2 (0 < EXT_REF2 < VDD), EXT_REF2 should have driving capability
*
*ADC resolution support: 12/10/8 bits
*
*ADC conversion can be triggered by TIMER overflow/GPIO/software, continue conversion mode only need trigger once. 
*
****************************************************************************/


#define ADC_TRIG_BY_GPIO    FALSE
#define ADC_TRIG_BY_TOF     FALSE
#define ADC_TRIG_BY_SOFT    TRUE
#define ADC_DECIMATION_EN   FALSE
#define ADC_COMPARATOR_EN   FALSE

#define ADC_TEMP_SENSOR_EN  FALSE
#define ADC_BATT_MONITOR_EN TRUE

#define ADC_EXT_REF_EN      FALSE
#define ADC_WORKING_AT_32K  FALSE

volatile uint32_t adc_done = 0;
volatile uint32_t int_num = 0;
int16_t buf[512];

static void adc_test_cb(void)
{
    adc_done = 1;
}

#if ADC_COMPARATOR_EN == TRUE
static void adc_WCMP_cb(void)
{
    int_num++;
}
#endif

void adc_io_config(void)
{
    // pin mux
    syscon_SetPMCR0(QN_SYSCON, P00_UART0_TXD_PIN_CTRL
                             | P05_ADCT_PIN_CTRL
    #if ADC_EXT_REF_EN==TRUE
                             | P06_AIN2_PIN_CTRL
                             | P07_AIN3_PIN_CTRL
    #else
                             | P06_SW_DAT_PIN_CTRL
                             | P07_SW_CLK_PIN_CTRL
    #endif
                             | P17_UART0_RXD_PIN_CTRL
                             );
    syscon_SetPMCR1(QN_SYSCON, P27_PWM0_PIN_CTRL
                             | P26_PWM1_PIN_CTRL
                             | P31_AIN1_PIN_CTRL
                             | P30_AIN0_PIN_CTRL
                             );
}

int main (void)
{

    SystemInit();

    adc_io_config();
    analog_pin_enable(AIN0, MASK_ENABLE);
    analog_pin_enable(AIN1, MASK_ENABLE);
    analog_pin_enable(AIN2, MASK_ENABLE);
    analog_pin_enable(AIN3, MASK_ENABLE);
    
#if (__AHB_CLK == 32000UL)
    uart_init(QN_UART0, __USART_CLK, UART_1200);
#else
    uart_init(QN_UART0, __USART_CLK, UART_115200);
#endif
    uart_tx_enable(QN_UART0, MASK_ENABLE);

    
    
    // ADC initialization    
#if ADC_WORKING_AT_32K==TRUE
    clk32k_enable(__32K_TYPE);
    adc_init(ADC_SINGLE_WITHOUT_BUF_DRV, ADC_CLK32K_16000, ADC_INT_REF, ADC_12BIT);
#else

#if ADC_EXT_REF_EN==TRUE
    adc_init(ADC_SINGLE_WITHOUT_BUF_DRV, ADC_CLK_1000000, ADC_EXT_REF2, ADC_12BIT);
    //adc_init(ADC_SINGLE_WITHOUT_BUF_DRV, ADC_CLK_1000000, ADC_EXT_REF1, ADC_12BIT);
#else    
    adc_init(ADC_SINGLE_WITHOUT_BUF_DRV, ADC_CLK_1000000, ADC_INT_REF, ADC_12BIT);
#endif
#endif
    
      
    
    // Read configuration
    adc_read_configuration read_cfg;

    
#if ADC_TRIG_BY_GPIO == TRUE

    read_cfg.trig_src = ADC_TRIG_GPIO;
    syscon_SetPMCR2WithMask(QN_SYSCON, SYSCON_MASK_ADCT_PIN_SEL, ADC_GPIO15_TRIG);

    // triger by gpio in burst or burst scan mode, connect PWM output to ADC trigger PIN
    pwm_init(PWM_CH0);
    pwm_config(PWM_CH0, PWM_PSCAL_DIV, PWM_COUNT_US(50, PWM_PSCAL_DIV), PWM_COUNT_US(25, PWM_PSCAL_DIV));
    pwm_enable(PWM_CH0, MASK_ENABLE);
      
#elif ADC_TRIG_BY_TOF == TRUE

    read_cfg.trig_src = ADC_TRIG_TOVF1;
    // triger by timer1 overflow
    timer_init(QN_TIMER1, NULL);
    timer_pwm_config(QN_TIMER1, TIMER_PSCAL_DIV, TIMER_COUNT_US(100, TIMER_PSCAL_DIV), TIMER_COUNT_US(50, TIMER_PSCAL_DIV));
    timer_enable(QN_TIMER1, MASK_ENABLE);

#elif ADC_TRIG_BY_SOFT == TRUE

    read_cfg.trig_src = ADC_TRIG_SOFT;

#endif


#if ADC_DECIMATION_EN == TRUE
    adc_decimation_enable(DECI_RATE_64, MASK_ENABLE);
#endif


#if ADC_COMPARATOR_EN == TRUE
    //adc_compare_init(DECI_DATA, 2500, -2000, adc_WCMP_cb);
    adc_compare_init(ADC_DATA, 600, -600, adc_WCMP_cb);
#endif


#if (ADC_TRIG_BY_GPIO==TRUE || ADC_TRIG_BY_TOF==TRUE || ADC_TRIG_BY_SOFT==TRUE)
    
    adc_done = 0;
    
    // modify here
    read_cfg.mode = BURST_MOD;
    read_cfg.start_ch = AIN0;
    read_cfg.end_ch = AIN0;
    adc_read(&read_cfg, buf, 512, adc_test_cb);
    while (adc_done == 0);
#endif


#if ADC_COMPARATOR_EN == TRUE
    int m = 0;
    int n = 0;
    for (int i = 0; i < 512; i++) {
        if (buf[i] > 600) {
            m++;
        }
        else if (buf[i] < -600) {
            n++;
        }
    }
    printf("m = %d\t n = %d\r\n", m, n);
#endif


    for (int i = 0; i < 512; i++) {
        printf("%d\t %d\r\n", buf[i], ADC_RESULT_mV(buf[i]));
    }
    int sum = 0;
    for (int i = 0; i < 10; i++) {
        sum += buf[511 - 2*i];
    }
    sum = sum / 10;
    printf("average: %d\t %d\r\n", sum, ADC_RESULT_mV(sum));
    


#if ADC_TEMP_SENSOR_EN==TRUE
    temp_sensor_enable(MASK_ENABLE);
    int16_t tempv;
    adc_init(ADC_DIFF_WITH_BUF_DRV, ADC_CLK_1000000, ADC_INT_REF, ADC_12BIT);
    adc_done = 0;
    read_cfg.trig_src = ADC_TRIG_SOFT;
    read_cfg.mode = BURST_MOD;
    read_cfg.start_ch = TEMP;
    read_cfg.end_ch = TEMP;
    adc_read(&read_cfg, &tempv, 1, adc_test_cb);
    while (adc_done == 0);
    printf("temperature: %0.1f\r\n", (float)(TEMPERATURE_X10(tempv)/10.0));
#endif


#if ADC_BATT_MONITOR_EN==TRUE
    battery_monitor_enable(MASK_ENABLE);
    int16_t battv;
    adc_init(ADC_SINGLE_WITH_BUF_DRV, ADC_CLK_1000000, ADC_INT_REF, ADC_12BIT);
    adc_done = 0;
    read_cfg.trig_src = ADC_TRIG_SOFT;
    read_cfg.mode = BURST_MOD;
    read_cfg.start_ch = BATT;
    read_cfg.end_ch = BATT;
    adc_read(&read_cfg, &battv, 1, adc_test_cb);
    while (adc_done == 0);
    printf("battery voltage: %d\r\n", 4*ADC_RESULT_mV(battv));
#endif
    
    
    while (1)                                /* Loop forever */
    {


    }
}

