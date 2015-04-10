/****************************************************************************
 *   $Id:: analog_example.c                                                 $
 *   Project: QUINTIC QN9020 Analog modules example
 *
 *   Description:
 *     This file contains Analog driver usage.
 *
****************************************************************************/

#include "analog.h"
#include "qnrf.h"
#include "system.h"


/****************************************************************************
*               ANALOG driver code example
* Analog comparator support internal or external reference(ref pin: ACMPx_P), 
* internal reference can be configured form 1/16 VDD to 15/16 VDD.
*
* Analog comparator ouput level interrupt support(high/low)
*
****************************************************************************/



#define ACMP_TEST           TRUE
#define RF_SWEEP_TEST       FALSE

#if ACMP_TEST == TRUE
volatile uint32_t acmp0_done = 0;
volatile uint32_t acmp1_done = 0;

#if CONFIG_ENABLE_DRIVER_ACMP0==TRUE 
static void acmp0_test_cb(void)
{
    //comparator_enable(ACMP0, ACMPO_1_GEN_INT, MASK_DISABLE);
    acmp0_done = 1;
}
#endif
#if CONFIG_ENABLE_DRIVER_ACMP1==TRUE 
static void acmp1_test_cb(void)
{
    //comparator_enable(ACMP1, ACMPO_1_GEN_INT, MASK_DISABLE);
    acmp1_done = 1;
}
#endif
#endif

void acmp_io_config(void)
{
    // pin mux
    syscon_SetPMCR0(QN_SYSCON, P05_ACMP1_O_PIN_CTRL
#if CONFIG_ENABLE_DRIVER_ACMP1==TRUE 
                             | P06_ACMP1_N_PIN_CTRL
                             | P07_ACMP1_P_PIN_CTRL
#else
                             | P06_SW_DAT_PIN_CTRL
                             | P07_SW_CLK_PIN_CTRL
#endif
                             );
    syscon_SetPMCR1(QN_SYSCON, P23_ACMP0_O_PIN_CTRL
                             | P27_ACMP1_O_PIN_CTRL
                             | P30_ACMP0_N_PIN_CTRL
                             | P31_ACMP0_P_PIN_CTRL
                             | P32_ACMP0_O_PIN_CTRL
                             );
}

int main (void)
{
    
    SystemInit();
    
#if ACMP_TEST == TRUE
    // --------------------------------------------
    // analog comparator 
    // --------------------------------------------
    acmp_io_config();

#if CONFIG_ENABLE_DRIVER_ACMP0==TRUE 
    analog_pin_enable(AIN0, MASK_ENABLE);
    analog_pin_enable(AIN1, MASK_ENABLE);
    
    acmp0_done = 0;
    // comparator0 test
    //comparator_init(ACMP0, VDD_1, ACMPO_0_GEN_INT, acmp0_test_cb);
    //comparator_init(ACMP0, VDD_8, ACMPO_1_GEN_INT, acmp0_test_cb);
    //comparator_init(ACMP0, VDD_15, ACMPO_0_GEN_INT, acmp0_test_cb);
    comparator_init(ACMP0, EXT_REF, ACMPO_1_GEN_INT, acmp0_test_cb);
    while (acmp0_done == 0);
#endif
    
#if CONFIG_ENABLE_DRIVER_ACMP1==TRUE 
    analog_pin_enable(AIN2, MASK_ENABLE);
    analog_pin_enable(AIN3, MASK_ENABLE);
    
    acmp1_done = 0;
    // comparator1 test
    //comparator_init(ACMP1, VDD_1, ACMPO_0_GEN_INT, acmp1_test_cb);
    comparator_init(ACMP1, VDD_8, ACMPO_0_GEN_INT, acmp1_test_cb);
    //comparator_init(ACMP1, VDD_15, ACMPO_0_GEN_INT, acmp1_test_cb);
    while (acmp1_done == 0);
#endif
#endif

#if RF_SWEEP_TEST == TRUE
    // --------------------------------------------
    // analog RF 
    // --------------------------------------------

    int mask = SYSCON_MASK_DIS_REF_PLL
             | SYSCON_MASK_DIS_LO_VCO
             | SYSCON_MASK_DIS_LO_PLL
             | SYSCON_MASK_DIS_PA;
    syscon_SetPGCR1WithMask(QN_SYSCON, mask, MASK_DISABLE);

    rf_enable(RF_TX, MASK_ENABLE, MASK_ENABLE, 0x1F);
    rf_tx_power_level_set(TX_GAIN_LEVLE10);


    for (int i = 0; i < 40; i++) {
        
        rf_set_freq(RF_TX, i);
        delay(10000);
    }
#endif


    while (1)                                /* Loop forever */
    {

    }
}


