/**
 ****************************************************************************************
 *
 * @file button.h
 *
 * @brief Header file of button driver for qn evb.
 *
 * Copyright (C) Quintic 2012-2013
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */
#ifndef _BUTTON_H_
#define _BUTTON_H_

/*
 * MACRO DEFINITIONS
 ****************************************************************************************
 */
#if !defined(QN_9021_MINIDK)

#if !defined(FireBLE_platform)

#if	defined(CFG_FCC_CE_TEST)
#define	BUTTON1_PIN			CFG_FCC_CE_CTRL_PIN
#else
#define BUTTON1_PIN    (GPIO_P14)
#endif
#define BUTTON2_PIN    (GPIO_P15)

#else

#if	defined(CFG_FCC_CE_TEST)
#define	BUTTON1_PIN			CFG_FCC_CE_CTRL_PIN
#else
#define BUTTON1_PIN    (GPIO_P12)
#endif
#define	BUTTON2_PIN	 (GPIO_P02)  // no pin in QN9021

#endif

#else

#if	defined(CFG_FCC_CE_TEST)
#define	BUTTON1_PIN			CFG_FCC_CE_CTRL_PIN
#else
#define BUTTON1_PIN    (GPIO_P12)
#endif
#define BUTTON2_PIN    (GPIO_P10)

#endif

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

extern void button_init(void);
extern int check_button_state(int btn);

#endif

