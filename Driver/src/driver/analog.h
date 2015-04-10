/**
 ****************************************************************************************
 *
 * @file analog.h
 *
 * @brief Header file of analog for QN9020.
 *
 * Copyright (C) Quintic 2012-2013
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */
#ifndef _ANALOG_H_
#define _ANALOG_H_
#include "driver_config.h"
#if CONFIG_ENABLE_DRIVER_ANALOG==TRUE

/**
 ****************************************************************************************
 * @defgroup ANALOG Analog Driver
 * @ingroup DRIVERS
 * @brief Analog driver
 *
 *  QN9020 analog circuit contains: clock generator, two comparators, ADC, battery monitor,
 *  brown out detector, temperature sensor, RF, power and reset modules. Please refer to
 *  system controller driver for how to control clock generator, as well as power and reset modules.
 *  Also please refer to RF driver for how to set frequecny, and refer to ADC driver for how to use ADC,
 *  The other modules are described in this section as well. Their main features are listed as follow:
 *    - Two comparators with selectable reference voltage
 *    - Interrupt generate according to comparator result
 *    - Support brown out detection
 *    - Intergrated temperature sensor
 *
 * @{
 *
 ****************************************************************************************
 */

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */



/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "gpio.h"
#include "adc.h"


extern int16_t TEMP_OFFSET;

/*
 * MACRO DEFINITIONS
 ****************************************************************************************
 */
/// default temperature offset value
#define DEFAULT_TEMP_OFFSET                 (-200)
/// factory calibartion temperature
#define FAC_CAL_TEMP                        (25)
/// temperature calculated by ADC result
#define TEMPERATURE_X10(adc_data)           ((int16_t)(((((adc_data) - TEMP_OFFSET) / 3.8) + FAC_CAL_TEMP) * 10)) 


/*
 * ENUMERATION DEFINITIONS
 *****************************************************************************************
 */

/// Analog comparator channel
enum ACMP_CH
{
    ACMP0 = SYSCON_MASK_ACMP0_EN,       /*!< Analog comparator channel 0 */
    ACMP1 = SYSCON_MASK_ACMP1_EN,       /*!< Analog comparator channel 1 */
    ACMP_CH_NUM
};

/// Analog comparator reference voltage
enum ACMP_REF
{
    EXT_REF = 0,                /*!< Set reference valtage to external reference voltage */
    VDD_1,                      /*!< Set reference valtage to 1/16 VDD */
    VDD_2,                      /*!< Set reference valtage to 2/16 VDD */
    VDD_3,                      /*!< Set reference valtage to 3/16 VDD */
    VDD_4,                      /*!< Set reference valtage to 4/16 VDD */
    VDD_5,                      /*!< Set reference valtage to 5/16 VDD */
    VDD_6,                      /*!< Set reference valtage to 6/16 VDD */
    VDD_7,                      /*!< Set reference valtage to 7/16 VDD */
    VDD_8,                      /*!< Set reference valtage to 8/16 VDD */
    VDD_9,                      /*!< Set reference valtage to 9/16 VDD */
    VDD_10,                     /*!< Set reference valtage to 10/16 VDD */
    VDD_11,                     /*!< Set reference valtage to 11/16 VDD */
    VDD_12,                     /*!< Set reference valtage to 12/16 VDD */
    VDD_13,                     /*!< Set reference valtage to 13/16 VDD */
    VDD_14,                     /*!< Set reference valtage to 14/16 VDD */
    VDD_15                      /*!< Set reference valtage to 15/16 VDD */
};

/// Analog comparator interrupt condition
enum ACMP_INT_COND
{
    ACMPO_1_GEN_INT = 0,        /*!< When ACMP output is 1, generate interrupt */
    ACMPO_0_GEN_INT = 1         /*!< When ACMP output is 0, generate interrupt */
};

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Enable or disable analog input pin .
 * @param[in]   ainx        Analog pin index
 * @param[in]   able        MASK_ENABLE or MASK_DISABLE
 * @description
 *  This function is used to enable or disable analog input pin.
 *****************************************************************************************
 */
__STATIC_INLINE void analog_pin_enable(enum ADC_CH ainx, uint32_t able)
{
    if (ainx <= AIN3) {
        syscon_SetAnalogCRWithMask(QN_SYSCON, (1 << (ainx + SYSCON_POS_AINX_EN)), able);
    }
}


#if CONFIG_ENABLE_ROM_DRIVER_ANALOG==TRUE

typedef  void (*p_comparator_init)(enum ACMP_CH acmpch, enum ACMP_REF acmpref, enum ACMP_INT_COND acmpint, void (*callback)(void));
typedef  void (*p_comparator_enable)(enum ACMP_CH acmpch, enum ACMP_INT_COND acmpint, uint32_t able);
typedef  void (*p_battery_monitor_enable)(uint32_t able);
typedef  void (*p_brown_out_enable)(uint32_t able);
typedef  void (*p_temp_sensor_enable)(uint32_t able);

#define comparator_init        ((p_comparator_init)        _comparator_init)
#define comparator_enable      ((p_comparator_enable)      _comparator_enable)
#define battery_monitor_enable ((p_battery_monitor_enable) _battery_monitor_enable)
#define brown_out_enable       ((p_brown_out_enable)       _brown_out_enable)
#define temp_sensor_enable     ((p_temp_sensor_enable)     _temp_sensor_enable)
#else


#if (CONFIG_ENABLE_DRIVER_ACMP0==TRUE || CONFIG_ACMP0_DEFAULT_IRQHANDLER==TRUE)
void COMPARATOR0_IRQHandler(void);
#endif
#if (CONFIG_ENABLE_DRIVER_ACMP1==TRUE || CONFIG_ACMP1_DEFAULT_IRQHANDLER==TRUE)
void COMPARATOR1_IRQHandler(void);
#endif

#if (CONFIG_ENABLE_DRIVER_ACMP0==TRUE || CONFIG_ENABLE_DRIVER_ACMP1==TRUE)
extern void comparator_init(enum ACMP_CH acmpch, enum ACMP_REF acmpref, enum ACMP_INT_COND acmpint, void (*callback)(void));
extern void comparator_enable(enum ACMP_CH acmpch, enum ACMP_INT_COND acmpint, uint32_t able);
#endif
extern void battery_monitor_enable(uint32_t able);
extern void brown_out_enable(uint32_t able);
extern void temp_sensor_enable(uint32_t able);

#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

/// @} ANALOG
#endif /* CONFIG_ENABLE_DRIVER_ANALOG==TRUE */
#endif /* _ANALOG_H_ */
