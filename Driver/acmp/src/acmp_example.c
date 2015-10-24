/****************************************************************************
 *   $Id: acmp_example.c
 *   $Project: NXP QN9020 Analog modules example
 *
 *   Description:
 *     This file contains Analog driver usage.
 *
****************************************************************************/

#include "analog.h"
#include "qnrf.h"
#include "system.h"

#define ACMP_TEST           TRUE
#define RF_SWEEP_TEST       FALSE

#if ACMP_TEST == TRUE
volatile uint32_t acmp0_done = 0;
volatile uint32_t acmp1_done = 0;

#if CONFIG_ENABLE_DRIVER_ACMP0==TRUE 
static void acmp0_test_cb(void)
{
    //acmp_enable(ACMP0, ACMPO_1_GEN_INT, MASK_DISABLE);
    acmp0_done = 1;
}
#endif
#if CONFIG_ENABLE_DRIVER_ACMP1==TRUE 
static void acmp1_test_cb(void)
{
    //acmp_enable(ACMP1, ACMPO_1_GEN_INT, MASK_DISABLE);
    acmp1_done = 1;
}
#endif
#endif



int main (void)
{
    
    SystemInit();
    
#if ACMP_TEST == TRUE

#if CONFIG_ENABLE_DRIVER_ACMP0==TRUE 
    /* P3.0-P  P3.1-N  P3.2-Out */
    acmp_pin_enable(ACMP0_PIN_P, MASK_ENABLE);
    acmp_pin_enable(ACMP0_PIN_N, MASK_ENABLE);
    
    acmp0_done = 0;
    // acmp0 test
    //acmp_init(ACMP0, VDD_1, ACMPO_0_GEN_INT, HYST_ENABLE, acmp0_test_cb);
    acmp_init(ACMP0, VDD_8, ACMPO_1_GEN_INT, HYST_ENABLE, acmp0_test_cb);
    //acmp_init(ACMP0, VDD_15, ACMPO_0_GEN_INT, HYST_DISABLE, acmp0_test_cb);
    //acmp_init(ACMP0, EXT_REF, ACMPO_1_GEN_INT, HYST_ENABLE, acmp0_test_cb);
    while (acmp0_done == 0);
    
#endif
    
    
#if CONFIG_ENABLE_DRIVER_ACMP1==TRUE
    /* P0.6-P P0.7-N P0.5-Out */
    acmp_pin_enable(ACMP1_PIN_P, MASK_ENABLE);
    acmp_pin_enable(ACMP1_PIN_N, MASK_ENABLE);
    
    acmp1_done = 0;
    // acmp1 test
    //acmp_init(ACMP1, VDD_1, ACMPO_0_GEN_INT, HYST_ENABLE, acmp1_test_cb);
    acmp_init(ACMP1, VDD_8, ACMPO_1_GEN_INT, HYST_ENABLE, acmp1_test_cb);
    //acmp_init(ACMP1, VDD_15, ACMPO_0_GEN_INT, HYST_ENABLE, acmp1_test_cb);
    while (acmp1_done == 0);
#endif

#endif  /* ACMP_TEST == TRUE */

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


