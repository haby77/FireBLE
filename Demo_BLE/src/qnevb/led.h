/**
 ****************************************************************************************
 *
 * @file led.h
 *
 * @brief Header file of led driver for qn evb.
 *
 * Copyright (C) Quintic 2012-2013
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */
#ifndef _LED_H_
#define _LED_H_


/*
 * MACRO DEFINITIONS
 ****************************************************************************************
 */

#if !defined(QN_9021_MINIDK)

#define LED1_PIN    (GPIO_P05)
#define LED2_PIN    (GPIO_P04)
#define LED3_PIN    (GPIO_P03)
#define LED4_PIN    (GPIO_P02)
#define LED5_PIN    (GPIO_P01)

#else

#define LED1_PIN    (GPIO_P03)
#define LED2_PIN    (GPIO_P13)
#define LED3_PIN    (GPIO_P02)  // no pin in QN9021
#define LED4_PIN    (GPIO_P02)  // no pin in QN9021
#define LED5_PIN    (GPIO_P02)  // no pin in QN9021

#endif

/*
 * ENUMERATION DEFINITIONS
 ****************************************************************************************
 */

enum led_st
{
    LED_ON = 0,
    LED_OFF = (int)0xffffffff
};

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
extern void led_init(void);
extern void led_matrix(uint32_t matrix);
extern void led_set(uint32_t idx, enum led_st enable);
extern enum led_st led_get(uint32_t idx);

#endif
