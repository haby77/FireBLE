/**
 ****************************************************************************************
 *
 * @file hci_uart.h
 *
 * @brief HCI UART transport module functions for application controller interface.
 *
 * Copyright (C) RivieraWaves 2009-2012
 *
 * $Rev: $
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 *
 * @file eaci_uart.h
 *
 * @brief UART transport module functions for Easy Application Controller Interface.
 *
 * Copyright (C) Quintic 2009-2012
 *
 * $Rev: $
 *
 ****************************************************************************************
 */

#ifndef EACI_UART_H_
#define EACI_UART_H_

// Field length of Message Type and Parameter Length
#define EACI_MSG_HDR_LEN    2

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
extern uint8_t payl_len;
extern uint8_t* payl_para;
extern uint8_t en_read_flag;
/*
 ****************************************************************************************
 * @brief EACI UART initialization function.
 *
 ****************************************************************************************
 */
void eaci_uart_init(void);

/*
 ****************************************************************************************
 * @brief EACI UART write function.
 *
 ****************************************************************************************
 */
void eaci_uart_write(uint8_t len, uint8_t *par);

/*
 ****************************************************************************************
 * @brief EACI Function called at each RX interrupt.
 *
 *****************************************************************************************
 */
void eaci_uart_rx_done(void);

#endif // EACI_UART_H_
