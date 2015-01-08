/****************************************************************************
 *   $Id:: rtc_example.c                                                   $
 *   Project: QUINTIC QN9020 RTC example
 *
 *   Description:
 *     This file contains RTC driver usage.
 *
****************************************************************************/

#include "system.h"
#include "uart.h"
#include "calibration.h"
#include "rtc.h"
#include "gpio.h"
#include "sleep.h"


/****************************************************************************
*               RTC driver code example
*RTC module work at 32k crystal, and will be powered off when MCU enter sleep 
*
****************************************************************************/

#define LOW_POWER_MODE_EN                   FALSE

void display_date_time(void)
{
    /* 2012.12.21 00:00:00 */
    uint8_t data[21];

    data[0] = '2';
    data[1] = '0';
    data[2] = ((dec2bcd(rtc_env.date.year)&0xF0)>>4) + 48;
    data[3] = (dec2bcd(rtc_env.date.year)&0x0F) + 48;
    data[4] = '.';
    data[5] = ((dec2bcd(rtc_env.date.month)&0x10)>>4) + 48;
    data[6] = (dec2bcd(rtc_env.date.month)&0x0F) + 48;
    data[7] = '.';
    data[8] = ((dec2bcd(rtc_env.date.day)&0x30)>>4) + 48;
    data[9] = (dec2bcd(rtc_env.date.day)&0x0F) + 48;
    data[10] = ' ';
    data[11] = ((dec2bcd(rtc_env.time.hour)&0x30)>>4) + 48;
    data[12] = (dec2bcd(rtc_env.time.hour)&0x0F) + 48;
    data[13] = ':';
    data[14] = ((dec2bcd(rtc_env.time.minute)&0x70)>>4) + 48;
    data[15] = (dec2bcd(rtc_env.time.minute)&0x0F) + 48;
    data[16] = ':';
    data[17] = ((dec2bcd(rtc_env.time.second)&0x70)>>4) + 48;
    data[18] = (dec2bcd(rtc_env.time.second)&0x0F) + 48;
    data[19] = '\n';
    data[20] = '\0';
    
    uart_printf(QN_UART0, data);
#if QN_32K_RCO == TRUE
    clock_32k_correction_enable(clock_32k_correction_cb);
#endif
}

void uart_io_config(void)
{
    // pin mux
    syscon_SetPMCR0(QN_SYSCON, P07_SW_CLK_PIN_CTRL       
                             | P06_SW_DAT_PIN_CTRL       
                             | P00_UART0_TXD_PIN_CTRL    // P0.0 uart0 tx 
                             | P17_UART0_RXD_PIN_CTRL    // P1.7 uart0 rx
                             );
    syscon_SetPMCR1(QN_SYSCON, P21_UART1_TXD_PIN_CTRL    // P2.1 uart1 tx
                             | P20_UART1_RXD_PIN_CTRL    // P2.0 uart1 rx   
                             );

    // pin select
    syscon_SetPMCR2(QN_SYSCON, SYSCON_MASK_UART1_PIN_SEL);

    // pin pull ( 00 : High-Z,  01 : Pull-down,  10 : Pull-up,  11 : Reserved )
    syscon_SetPPCR0(QN_SYSCON, 0xAAAA5AAA);
    syscon_SetPPCR1(QN_SYSCON, 0x2AAAAAAA);
}

void Led_flash(void)
{
    gpio_set_direction(GPIO_P01, GPIO_OUTPUT);
    //gpio_write_pin(GPIO_P01, GPIO_LOW);
    gpio_toggle_pin(GPIO_P01);
#if LOW_POWER_MODE_EN == TRUE
    delay(100);
#else
    delay(100000);
#endif
    gpio_toggle_pin(GPIO_P01);
    
#if (QN_32K_RCO == TRUE && LOW_POWER_MODE_EN == FALSE)
    clock_32k_correction_enable(clock_32k_correction_cb);
#endif
}

int main (void)
{
    SystemInit();
    
#if (LOW_POWER_MODE_EN == FALSE)
    clock_32k_correction_init();
    clock_32k_correction_enable(clock_32k_correction_cb);
#endif
    
    /* Initialize GPIO */
    gpio_init(NULL);
    //UART io configurate
    uart_io_config();
    
    uart_init(QN_UART0, __USART_CLK, UART_115200);
    uart_tx_enable(QN_UART0, MASK_ENABLE);
    uart_printf(QN_UART0, (uint8_t *)("Hello Quintic!!!\n"));
    Led_flash();
    
    do {
        delay(10);
    } while (gpio_read_pin(GPIO_P14) == GPIO_HIGH);

    //Initialize RTC
    rtc_init();
    
    /* 
     * Set the current time. If the interrupt is enabled, the callback function
     * will be called every one second.
     */
#if LOW_POWER_MODE_EN==FALSE
    rtc_time_set(12,12,21, 0,0,0, display_date_time);
    //rtc_time_set(12,12,21, 0,0,0, Led_flash);
#else
    rtc_time_set(12,12,21, 0,0,0, Led_flash);
#endif

#if LOW_POWER_MODE_EN == TRUE
    int mask;
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
    
    syscon_SetPGCR2WithMask(QN_SYSCON, SYSCON_MASK_PD_STATE|SYSCON_MASK_PMUENABLE, MASK_DISABLE);
    
    // set system clock to 32K
    syscon_set_sysclk_src(CLK_LOW_32K, __32K_TYPE);
    delay(3000); 
    
    // power off all not needed modules
    mask = SYSCON_MASK_DIS_SAR_BUF
         | SYSCON_MASK_DIS_OSC
         | SYSCON_MASK_DIS_BG
         | SYSCON_MASK_DIS_V2I
         | SYSCON_MASK_DIS_BUCK
         | SYSCON_MASK_DIS_VREG_A
         | SYSCON_MASK_DIS_VREG_D
         | SYSCON_MASK_DIS_XTAL
#if QN_32K_RCO == TRUE
         | SYSCON_MASK_DIS_XTAL32
#endif
         | SYSCON_MASK_DIS_REF_PLL
         | SYSCON_MASK_DIS_LO_VCO
         | SYSCON_MASK_DIS_LO_PLL
         | SYSCON_MASK_DIS_PA
         | SYSCON_MASK_DIS_LNA
         | SYSCON_MASK_DIS_LNA_PKDET
         | SYSCON_MASK_DIS_MIXER
         | SYSCON_MASK_DIS_PPF_PKDET
         | SYSCON_MASK_DIS_PPF
         | SYSCON_MASK_DIS_RX_PKDET
         | SYSCON_MASK_DIS_RX_ADC
         | SYSCON_MASK_DIS_SAR_ADC
#if (QN_32K_RCO == FALSE)
         | SYSCON_MASK_DIS_RCO
#endif
         | SYSCON_MASK_DIS_MEM7
         | SYSCON_MASK_DIS_MEM6
         | SYSCON_MASK_DIS_MEM5
         | SYSCON_MASK_DIS_MEM4
         | SYSCON_MASK_DIS_MEM3
         | SYSCON_MASK_DIS_MEM2
         //| SYSCON_MASK_DIS_MEM1
         ;
    syscon_SetPGCR1WithMask(QN_SYSCON, mask, MASK_ENABLE);

    // gating all not needed modules
    mask = SYSCON_MASK_GATING_TIMER3
         | SYSCON_MASK_GATING_TIMER2
#if QN_32K_RCO == FALSE   
         | SYSCON_MASK_GATING_TIMER1
#endif
         | SYSCON_MASK_GATING_TIMER0
         | SYSCON_MASK_GATING_UART1
         | SYSCON_MASK_GATING_UART0
         | SYSCON_MASK_GATING_SPI1
         | SYSCON_MASK_GATING_SPI0
         //| SYSCON_MASK_GATING_32K_CLK
         | SYSCON_MASK_GATING_SPI_AHB
         //| SYSCON_MASK_GATING_GPIO
         | SYSCON_MASK_GATING_ADC
         | SYSCON_MASK_GATING_DMA
         | SYSCON_MASK_GATING_BLE_AHB
         | SYSCON_MASK_GATING_PWM
         ;
    syscon_SetCRSS(QN_SYSCON, mask);
#endif

#if 0
    uint32_t *psrc = (uint32_t *)0x10004000;
    uint32_t *pdst = (uint32_t *)0x10004000;
    
    gpio_set_direction(GPIO_P03, GPIO_OUTPUT);
    gpio_set_direction(GPIO_P02, GPIO_OUTPUT);
    gpio_write_pin(GPIO_P03, GPIO_HIGH);
    for (int i = 0; i < 0x2000; i++) {
        *psrc++ = i;
    }
    for (int i = 0; i < 0x2000; i++) {
        
        if (*pdst++ != i) {
            gpio_write_pin(GPIO_P03, GPIO_LOW);
        }
    }
    gpio_write_pin(GPIO_P02, GPIO_LOW);
#endif
    syscon_SetPGCR2WithMask(QN_SYSCON, SYSCON_MASK_DVDD12_PMU_SET, MASK_ENABLE);

    while (1)                                /* Loop forever */
    {
        gpio_set_direction(GPIO_P01, GPIO_INPUT);
        enter_sleep(SLEEP_CPU_CLK_OFF, WAKEUP_BY_RTC|WAKEUP_BY_TIMER1, NULL);
    }
}
