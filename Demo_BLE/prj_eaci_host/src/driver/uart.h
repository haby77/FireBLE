/**
 ****************************************************************************************
 *
 * @file uart.h
 *
 * @brief UART interface of M3
 *
 * Copyright (C) Quintic 2009-2012
 *
 * $Rev: $
 *
 ****************************************************************************************
 */
#ifndef _UART_H_ 
#define _UART_H_

#include "LPC17xx.h"

/// Enable Mask 
#define MASK_ENABLE               (0xFFFFFFFF)
/// Disable Mask 
#define MASK_DISABLE              (0x00000000)

/*------------- Universal Asynchronous Receiver Transmitter (UART) -----------*/
typedef struct
{
  __O   uint32_t  TXD;                      /*!< Offset: 0x000 TX Data Register    ( /W) 		*/
  __I   uint32_t  RXD;                      /*!< Offset: 0x004 RX Data Register  (R/ ) 			*/
  __IO  uint32_t  BAUD;                     /*!< Offset: 0x008 Baudrate Divider Register (R/W) 	*/
  __IO  uint32_t  CR;                       /*!< Offset: 0x00C Control Register (R/W) 			*/
  __IO  uint32_t  FLAG;                     /*!< Offset: 0x010 Status Register (R/W) 			*/
} QN_UART_TypeDef;

/// UART buadrate
enum UART_BAUDRATE
{
    UART_2400   = 2400,
    UART_4800   = 4800,
    UART_9600   = 9600,
    UART_19200  = 19200,
    UART_28800  = 28800,
    UART_38400  = 38400,
    UART_57600  = 57600,
    UART_76800  = 76800,
    UART_115200 = 115200,
    UART_128000 = 128000,
    UART_153600 = 153600,
    UART_230400 = 230400,
    UART_256000 = 256000,
    UART_460800 = 460800,
    UART_691200 = 691200,
    UART_921600 = 921600
};

#define QN_UART1                            ((QN_UART_TypeDef *)    1)
#define QN_UART3                            ((QN_UART_TypeDef *)    3)

#define USARTx_CLK(div)                     (SystemFrequency / 4 / 16)

/*
 ****************************************************************************************
 * @brief Initializes the UART to default values.
 *
 *****************************************************************************************
 */
void uart_init(QN_UART_TypeDef *UART, uint32_t uartclk, enum UART_BAUDRATE baudrate);

/*
 ****************************************************************************************
 * @brief Enable/disable uart Tx and Rx.
 * 
 *****************************************************************************************
 */
void uart_tx_enable(QN_UART_TypeDef *UART, uint32_t able);

/**
 ****************************************************************************************
 * @brief Enable/disable uart Tx and Rx.
 *****************************************************************************************
 */
void uart_rx_enable(QN_UART_TypeDef *UART, uint32_t able);

/*
 ****************************************************************************************
 * @brief Enable/disable all uart Tx interrupts.
 * 
 *****************************************************************************************
 */
void uart_tx_int_enable(QN_UART_TypeDef *UART, uint32_t able);

/*
 ****************************************************************************************
 * @brief Enable/disable all uart Rx interrupts.
 * 
 *****************************************************************************************
 */
void uart_rx_int_enable(QN_UART_TypeDef *UART, uint32_t able);

/*
 ****************************************************************************************
 * @brief Starts a data reception.
 * 
 *****************************************************************************************
 */
void uart_read(QN_UART_TypeDef *UART, uint8_t *bufptr, uint32_t size, void (*rx_callback)(void));

/*
 ****************************************************************************************
 * @brief Starts a data transmission.
 *
 *****************************************************************************************
 */
void uart_write(QN_UART_TypeDef *UART, uint8_t *bufptr, uint32_t size, void (*tx_callback)(void));

/*
 ****************************************************************************************
 * @brief Send a string to Uart
 * 
 *****************************************************************************************
 */
void uart_printf(QN_UART_TypeDef *UART, uint8_t *bufptr);

#endif /* end _UART_H_ */
