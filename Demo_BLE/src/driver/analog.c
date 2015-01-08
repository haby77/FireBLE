/**
 ****************************************************************************************
 *
 * @file analog.c
 *
 * @brief Analog driver for QN9020.
 *
 * Copyright (C) Quintic 2012-2013
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup  ANALOG
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "analog.h"
#if CONFIG_ENABLE_DRIVER_ANALOG==TRUE && CONFIG_ENABLE_ROM_DRIVER_ANALOG==FALSE
#include "nvds.h"

/*
 * STRUCTURE DEFINITIONS
 ****************************************************************************************
 */
#if ACMP_CALLBACK_EN==TRUE
/// Analog comparator environment parameters
struct acmp_env_tag
{
    void                (*callback)(void);
};

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

#if (CONFIG_ENABLE_DRIVER_ACMP0==TRUE && CONFIG_ACMP0_DEFAULT_IRQHANDLER==TRUE)
/// Analog comparator0 environment variable
static struct acmp_env_tag acmp_env0;
#endif

#if (CONFIG_ENABLE_DRIVER_ACMP1==TRUE && CONFIG_ACMP1_DEFAULT_IRQHANDLER==TRUE)
/// Analog comparator1 environment variable
static struct acmp_env_tag acmp_env1;
#endif
#endif

int16_t TEMP_OFFSET = DEFAULT_TEMP_OFFSET;

/*
 * LOCAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */


/*
 * EXPORTED FUNCTION DEFINITIONS
 ****************************************************************************************
 */

#if (CONFIG_ENABLE_DRIVER_ACMP0==TRUE && CONFIG_ACMP0_DEFAULT_IRQHANDLER==TRUE)
 /**
 ****************************************************************************************
 * @brief Comparator0 interrupt handler
 ****************************************************************************************
 */
void COMPARATOR0_IRQHandler(void)
{


#if ACMP_CALLBACK_EN==TRUE
    if (acmp_env0.callback != NULL)
    {
        acmp_env0.callback();
    }
#endif


}
#endif

#if (CONFIG_ENABLE_DRIVER_ACMP1==TRUE && CONFIG_ACMP1_DEFAULT_IRQHANDLER==TRUE)
 /**
 ****************************************************************************************
 * @brief Comparator1 interrupt handler
 ****************************************************************************************
 */
void COMPARATOR1_IRQHandler(void)
{


#if ACMP_CALLBACK_EN==TRUE
    if (acmp_env1.callback != NULL)
    {
        acmp_env1.callback();
    }
#endif


}
#endif

#if (CONFIG_ENABLE_DRIVER_ACMP0==TRUE || CONFIG_ENABLE_DRIVER_ACMP1==TRUE)
/**
 ****************************************************************************************
 * @brief  Initialize and enable comparator
 * @param[in]    acmpch       ACMP0 or ACMP1
 * @param[in]    acmpref      Comparator voltage: external or internal VDD (ref pin: ACMPx_P)
 * @param[in]    acmpint      Comparator interrutp condition: acmp output 1 or 0 generate interrupt
 * @param[in]    callback     Callback in interrupt handler
 * @description
 *  This function is used to initialize specified analog comparator, and to register callback function.
 *****************************************************************************************
 */
void comparator_init(enum ACMP_CH acmpch, enum ACMP_REF acmpref, enum ACMP_INT_COND acmpint, void (*callback)(void))
{
    uint32_t mask = 0;
    uint32_t reg = 0;

#if CONFIG_ENABLE_DRIVER_ACMP0==TRUE 
    if (acmpch == ACMP0) {

        acmp_env0.callback = callback;
        mask = SYSCON_MASK_ACMP0_REF
             | SYSCON_MASK_ACMP0_HYST_EN
             | SYSCON_MASK_ACMP0_VALUE
             | SYSCON_MASK_ACMP0_EN;

        // set interrupt condition
        reg = (acmpint << SYSCON_POS_ACMP0_VALUE)
            | (acmpref << SYSCON_POS_ACMP0_REF)
            //| SYSCON_MASK_ACMP0_HYST_EN  // +/- 20mv
            | SYSCON_MASK_ACMP0_EN;

#if CONFIG_ACMP0_ENABLE_INTERRUPT==TRUE
        NVIC_ClearPendingIRQ(COMPARATOR0_IRQn);
        /* Enable the comparator Interrupt */
        NVIC_EnableIRQ(COMPARATOR0_IRQn);
#endif
    }
#endif

#if CONFIG_ENABLE_DRIVER_ACMP1==TRUE 
     if (acmpch == ACMP1) {

        acmp_env1.callback = callback;
        mask = SYSCON_MASK_ACMP1_REF
             | SYSCON_MASK_ACMP1_HYST_EN
             | SYSCON_MASK_ACMP1_VALUE
             | SYSCON_MASK_ACMP1_EN;

        // set interrupt condition
        reg = (acmpint << SYSCON_POS_ACMP1_VALUE)
            | (acmpref << SYSCON_POS_ACMP1_REF)
            //| SYSCON_MASK_ACMP1_HYST_EN  // +/- 20mv
            | SYSCON_MASK_ACMP1_EN;

#if CONFIG_ACMP1_ENABLE_INTERRUPT==TRUE
        NVIC_ClearPendingIRQ(COMPARATOR1_IRQn);
        /* Enable the comparator Interrupt */
        NVIC_EnableIRQ(COMPARATOR1_IRQn);
#endif
    }
#endif
    syscon_SetAnalogCRWithMask(QN_SYSCON, mask, reg);
}

/**
 ****************************************************************************************
 * @brief  Enable comparator with interrupt condition
 * @param[in]    acmpch       ACMP0 or ACMP0
 * @param[in]    acmpint      Comparator interrutp condition: acmp output 1 or 0 generate interrupt
 * @param[in]    able         MASK_ENABLE or MASK_DISABLE
 * @description
 *  This function is used to enable or disable specified analog comparator with interrupt condition.
 *  If Comparators aren't used during sleep, please set ACMP0 and ACMP1 to disabled for lower sleep
 *  leakage current.
 *****************************************************************************************
 */
void comparator_enable(enum ACMP_CH acmpch, enum ACMP_INT_COND acmpint, uint32_t able)
{
    uint32_t mask = 0;
    uint32_t reg = 0;

#if CONFIG_ENABLE_DRIVER_ACMP0==TRUE 
    if (acmpch == ACMP0) {
        mask = SYSCON_MASK_ACMP0_EN|SYSCON_MASK_ACMP0_VALUE;
        if (able == MASK_DISABLE) {
            mask |= SYSCON_MASK_ACMP0_REF;
        }
        reg =  (SYSCON_MASK_ACMP0_EN & able) | (acmpint << SYSCON_POS_ACMP0_VALUE);
    }
#endif

#if CONFIG_ENABLE_DRIVER_ACMP1==TRUE 
    if (acmpch == ACMP1) {
        mask = SYSCON_MASK_ACMP1_EN|SYSCON_MASK_ACMP1_VALUE;
        if (able == MASK_DISABLE) {
            mask |= SYSCON_MASK_ACMP1_REF;
        }
        reg =  (SYSCON_MASK_ACMP1_EN & able) | (acmpint << SYSCON_POS_ACMP1_VALUE);
    }
#endif
    syscon_SetAnalogCRWithMask(QN_SYSCON, mask, reg);
}
#endif

/**
 ****************************************************************************************
 * @brief  Enable/Disable battery monitor
 * @param[in]    able         MASK_ENABLE or MASK_DISABLE
 * @description
 *  This function is used to enable or disable battery monitor.
 *****************************************************************************************
 */
void battery_monitor_enable(uint32_t able)
{
    syscon_SetAnalogCRWithMask(QN_SYSCON, SYSCON_MASK_BT_EN, able);
}


/**
 ****************************************************************************************
 * @brief  Enable/Disable brown out detector
 * @param[in]    able         MASK_ENABLE or MASK_DISABLE
 * @description
 *  This function is used to enable or disable brown out detector.
 *****************************************************************************************
 */
void brown_out_enable(uint32_t able)
{
    syscon_SetAnalogCRWithMask(QN_SYSCON, SYSCON_MASK_BD_EN, able);
}

/**
 ****************************************************************************************
 * @brief  Enable/Disable temperature sensor
 * @param[in]    able         MASK_ENABLE or MASK_DISABLE
 * @description
 *  This function is used to enable or disable temperature sensor.
 *****************************************************************************************
 */
void temp_sensor_enable(uint32_t able)
{
    syscon_SetAnalogCRWithMask(QN_SYSCON, SYSCON_MASK_TS_EN, able);

    if (able) {
        // read from NVDS
        uint16_t len = 4;
        uint32_t data = 0;
        TEMP_OFFSET = DEFAULT_TEMP_OFFSET;
        if (NVDS_OK == nvds_get(NVDS_TAG_TEMPERATURE_OFFSET, &len, (uint8_t *)&data)) {
            if ((data>0xFD80) && (data<0xFFFF)) { // -640 ~ 0
                TEMP_OFFSET = data;
            }
        }
    }
}

#endif /* CONFIG_ENABLE_DRIVER_ANALOG==TRUE */
/// @} ANALOG
