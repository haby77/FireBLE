/**
 ****************************************************************************************
 *
 * @file uart.c
 *
 * @brief UART interface of M3
 *
 * Copyright (C) Quintic 2009-2012
 *
 * $Rev: $
 *
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include <string.h>
#include "uart.h"
#include "app_env.h"

extern uint32_t SystemFrequency;

/*
 * STRUCTURE DEFINITIONS
 ****************************************************************************************
 */
///UART tx/rx parameters
struct uart_txrxchannel
{
    uint8_t  *bufptr;
    int32_t  size;
    void     (*callback)(void);
};

///UART environment parameters
struct uart_env_tag
{
    struct uart_txrxchannel tx;
    struct uart_txrxchannel rx;
};

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

///UART1 environment variable
static struct uart_env_tag uart1_env;
///UART3 environment variable
static struct uart_env_tag uart3_env;

/**
 ****************************************************************************************
 * @brief Initializes the UART to default values.
 * @param[in]       UART          QN_UART1 or QN_UART3
 * @param[in]       uartclk       UART modules clock: USARTx_CLK(div)
 * @param[in]       baudrate      baud rate
 * @description
 *  This function is used to initialize UART, it consists of baud-rate, parity, data-bits, stop-bits, 
 *  over sample rate and bit order, the function is also used to enable specified UART interrupt, and 
 *  enable NVIC UART IRQ. 
 *****************************************************************************************
 */
void uart_init(QN_UART_TypeDef *UART, uint32_t uartclk, enum UART_BAUDRATE baudrate)
{
    uint16_t dll, fdr;
    struct uart_env_tag *uart_env = (UART == QN_UART1) ? &uart1_env : &uart3_env;
    
    switch (baudrate)
    {
        case UART_2400:
            fdr = 0x10;
            dll = uartclk/(baudrate);
            break;	
        case UART_4800:
            fdr = 0x21;
            dll = uartclk/(baudrate*1.5);
            break;
        case UART_9600:
            fdr = 0x21;
            dll = uartclk/(baudrate*1.5);
            break;
        case UART_19200:
            fdr = 0x21;
            dll = uartclk/(baudrate*1.5);
            break;
        case UART_28800:
            fdr = 0xd7;
            dll = uartclk/(baudrate*1.5);
            break;
        case UART_38400:
            fdr = 0x21;
            dll = uartclk/(baudrate*1.5);
            break;
        case UART_57600:
            fdr = 0xf8;
            dll = uartclk/(baudrate*1.5);
            break;
        case UART_76800:
            fdr = 0x21;
            dll = uartclk/(baudrate*1.5);
            break;
        case UART_115200:
            fdr = 0x85;
            dll = uartclk/(baudrate*1.5);
            break;
        case UART_128000:
            fdr = 0x32;
            dll = uartclk/(baudrate*1.5);
            break;
        case UART_153600:
            fdr = 0x85;
            dll = uartclk/(baudrate*1.5);
            break;
        case UART_230400:
            fdr = 0x85;
            dll = uartclk/(baudrate*1.5);
            break;
        case UART_256000:
            fdr = 0xfe;
            dll = uartclk/(baudrate*1.5);
            break;
        case UART_460800:
            fdr = 0x85;
            dll = uartclk/(baudrate*1.6);
            break;
        case UART_691200:
            fdr = 0xc1;
            dll = uartclk/(baudrate*1.1);
            break;
        case UART_921600:
            fdr = 0x85;
            dll = uartclk/(baudrate*1.5);
            break;
        default:
            fdr = 0x85;
            dll = uartclk/(baudrate*1.5);
            break;
    }
    
    // Configure UART1
    if (QN_UART1 == UART)
    {
        LPC_PINCON->PINSEL4 |= (2 << 0);	// Pin function select for TXD1
        LPC_PINCON->PINSEL4 |= (2 << 2);    // Pin function select for RXD1
        LPC_SC->PCONP       |= (1 << 4);	// Power Control for Peripherals UART1
        LPC_UART1->FDR  = fdr;              // Fractional Divider Register
        LPC_UART1->LCR  = 0x83;				// Enables access to registers DLL and DLM
        LPC_UART1->DLL  = dll;              // Baud Rate depending on PCLK
        LPC_UART1->DLM  = (dll >> 8);       // High divisor latch
        LPC_UART1->LCR  = 0x03;				// Disables access to registers DLL and DLM
        LPC_UART1->FCR  = 0xC7; 			// Enable Tx/Rx FIFO and 14 characters full interrupt
        NVIC_EnableIRQ(UART1_IRQn);			// UART1 Interrupt Enable
        NVIC_SetPriority(UART1_IRQn, 3);	// ACI UART have higher priority than Debug UART 
        LPC_UART1->IER  = 0x01;				// Enables the Receive Data Available interrupt for UARTn
                                            // It also controls the Character Receive Time-out interrupt
    }
    else 
    // Configure UART3
    if (QN_UART3 == UART)
    {
        LPC_PINCON->PINSEL9 |= (3 << 24);	// Pin function select for TXD3
        LPC_PINCON->PINSEL9 |= (3 << 26);	// Pin function select for RXD3
        LPC_SC->PCONP       |= (1 << 25);	// Power Control for Peripherals UART3
        LPC_UART3->FDR  = fdr;              // Fractional Divider Register
        LPC_UART3->LCR	= 0x83; 			// Enables access to registers DLL and DLM
        LPC_UART3->DLL  = dll;              // Baud Rate depending on PCLK
        LPC_UART3->DLM  = (dll >> 8);       // High divisor latch
        LPC_UART3->LCR	= 0x03; 			// Disables access to registers DLL and DLM
        LPC_UART3->FCR	= 0xC7;  			// Enable Tx/Rx FIFO and 14 characters full interrupt
        NVIC_EnableIRQ(UART3_IRQn);			// UART3 Interrupt Enable
        NVIC_SetPriority(UART3_IRQn , 5);	// Debug UART have lower priority than ACI UART 
        LPC_UART3->IER	= 0x01; 			// Enables the Receive Data Available interrupt for UARTn.
                                            // It also controls the Character Receive Time-out interrupt.
    }
    // Configure UART environment
    uart_env->rx.size = 0;
    uart_env->tx.size = 0;
    uart_env->rx.bufptr = NULL;
    uart_env->tx.bufptr = NULL;
    uart_env->rx.callback = NULL;
    uart_env->tx.callback = NULL;
}

/*
 ****************************************************************************************
 * @brief Enable/disable uart Tx and Rx.
 * 
 *****************************************************************************************
 */
void uart_tx_enable(QN_UART_TypeDef *UART, uint32_t able)
{
    if (MASK_ENABLE == able)
        LPC_SC->PCONP  |=  (QN_UART1 == UART ? (1<<4) : (1<<25));
    else
        LPC_SC->PCONP  &= ~(QN_UART1 == UART ? (1<<4) : (1<<25));
}

/**
 ****************************************************************************************
 * @brief Enable/disable uart Tx and Rx.
 *****************************************************************************************
 */
void uart_rx_enable(QN_UART_TypeDef *UART, uint32_t able)
{
    if (MASK_ENABLE == able)
        LPC_SC->PCONP  |=  (QN_UART1 == UART ? (1<<4) : (1<<25));
    else
        LPC_SC->PCONP  &= ~(QN_UART1 == UART ? (1<<4) : (1<<25));
}

/**
 ****************************************************************************************
 * @brief Enable/disable all uart Tx interrupts.
 * @param[in]       UART          QN_UART1 or QN_UART3
 * @param[in]       able          MASK_ENABLE or MASK_DISABLE
 * @description
 * Enable or disable specified UART RX interrupt
 *****************************************************************************************
 */
void uart_tx_int_enable(QN_UART_TypeDef *UART, uint32_t able)
{
    // Disable or enable the RDA interrupts
    if (MASK_ENABLE == able)
        NVIC_EnableIRQ(QN_UART1 == UART ? UART1_IRQn : UART3_IRQn);
    else
        NVIC_DisableIRQ(QN_UART1 == UART ? UART1_IRQn : UART3_IRQn);

//  Start or stop Tx
//	LPC_UART3->TER	= (MASK_ENABLE == able ? 0x80 : 0x00);
}

/**
 ****************************************************************************************
 * @brief Enable/disable all uart Rx interrupts.
 * @param[in]       UART          QN_UART1 or QN_UART3
 * @param[in]       able          MASK_ENABLE or MASK_DISABLE
 * @description
 * Enable or disable specified UART RX interrupt
 *****************************************************************************************
 */
void uart_rx_int_enable(QN_UART_TypeDef *UART, uint32_t able)
{
    // Disable or enable the RDA interrupts
    if (QN_UART1 == UART)
    {
        LPC_UART1->IER	= (MASK_ENABLE == able ? 0x01 : 0x00);
    }
    else 
    if (QN_UART3 == UART)
    {
        LPC_UART3->IER	= (MASK_ENABLE == able ? 0x01 : 0x00);
    }
}

/**
 ****************************************************************************************
 * @brief Send a byte to Uart.
 * @param[in]       UART          QN_UART1 or QN_UART3
 * @param[in]       bufptr        buffer pointer of tx data
 * @description
 * Print a byte to specified UART port
 *****************************************************************************************
 */
static void uart_send_byte(QN_UART_TypeDef *UART, uint8_t byte)
{
    if (QN_UART1 == UART)
    {
        LPC_UART1->THR = byte;
        while ((LPC_UART1->LSR & 0x20) == 0);
    }
    else 
    if (QN_UART3 == UART)
    {
        LPC_UART3->THR = byte;
        while ((LPC_UART3->LSR & 0x20) == 0);
    }
}

/**
 ****************************************************************************************
 * @brief Transmit data to UART TX FIFO.
 * @param[in]       UART          QN_UART1 or QN_UART3
 * @param[in]       uart_env      Environment Variable of specified UART port
 * @description
 * Start transmit data to specified UART port until expected tramsmit data size reduce to zero.
 ****************************************************************************************
 */
static void uart_transmit_data(QN_UART_TypeDef * UART, struct uart_env_tag *uart_env)
{
    while (uart_env->tx.size > 0)
    {
        uart_send_byte(UART, *uart_env->tx.bufptr++);
        uart_env->tx.size--;
    }
}

/**
 ****************************************************************************************
 * @brief Send a string to Uart.
 * @param[in]       UART          QN_UART1 or QN_UART3
 * @param[in]       bufptr        buffer pointer of tx data
 * @description
 * Print a string to specified UART port
 *****************************************************************************************
 */
void uart_printf(QN_UART_TypeDef *UART, uint8_t *bufptr)
{
    while (*bufptr != '\0')
        uart_send_byte(UART, *bufptr++);
}

/**
 ****************************************************************************************
 * @brief Starts a data reception.
 * @param[in]      UART           QN_UART1 or QN_UART3
 * @param[in,out]  bufptr         Pointer to the RX buffer
 * @param[in]      size           Size of the expected reception
 * @param[in]      rx_callback    Callback for end of reception
 * @description
 * This function is used to read Rx data from RX FIFO and the data will be stored in bufptr, 
 * as soon as the end of the data transfer is detected, the callback function is executed.
 *
 *****************************************************************************************
 */
void uart_read(QN_UART_TypeDef *UART, uint8_t *bufptr, uint32_t size, void (*rx_callback)(void))
{
    struct uart_env_tag *uart_env = (UART == QN_UART1) ? &uart1_env : &uart3_env;
    
    // Store environment parameters
    uart_env->rx.size = size;
    uart_env->rx.bufptr = bufptr;
    uart_env->rx.callback = rx_callback;

    // Enable UART RX int
    uart_rx_int_enable(UART, MASK_ENABLE);
}

/**
 ****************************************************************************************
 * @brief Starts a data transmission.
 * @param[in]  UART            QN_UART1 or QN_UART3
 * @param[in]  bufptr          Pointer to the TX data buffer
 * @param[in]  size            Size of the transmission
 * @param[in]  tx_callback     Callback for end of transmission
 * @description
 * This function is used to write data into TX buffer to transmit data by UART, 
 * as soon as the end of the data transfer is detected, the callback function is executed.
 *
 *****************************************************************************************
 */
void uart_write(QN_UART_TypeDef *UART, uint8_t *bufptr, uint32_t size, void (*tx_callback)(void))
{
    struct uart_env_tag *uart_env = (UART == QN_UART1) ? &uart1_env : &uart3_env;

    //Store environment parameters
    uart_env->tx.size = size;
    uart_env->tx.bufptr = bufptr;
    uart_env->tx.callback = tx_callback;

    if (UART == QN_UART1 || UART == QN_UART3) 
    {
        // Start data transmission
        uart_transmit_data(UART, uart_env);
        if (uart_env->tx.callback != NULL)
        {
            uart_env->tx.callback();
        }
    }
}

/**
 ****************************************************************************************
 * @brief UART1 RX interrupt handler.
 * @description
 * Receive data from port UART1 until expected receive data size reduce to zero.
 ****************************************************************************************
 */
void UART1_IRQHandler(void) 
{
    while ((LPC_UART1->IIR & 0x01) == 0)
    {
        switch (LPC_UART1->IIR & 0x0E)
        {
        case 0x04:	// Receive Data Available (RDA)
        case 0x0C:	// Character Time-out Indicator (CTI)
            while ((LPC_UART1->LSR & 0x01) == 0x01)
            {
                if (uart1_env.rx.size > 0)
                {
                    *uart1_env.rx.bufptr++ = LPC_UART1->RBR;
                    uart1_env.rx.size--;
                }
                
                if (uart1_env.rx.size == 0)
                {
                    // Disable UART all rx int
                    uart_rx_int_enable(QN_UART1, MASK_DISABLE);
                    // Call end of reception callback
                    if(uart1_env.rx.callback != NULL)
                    {
                        uart1_env.rx.callback();
                    }
                }	
            }
            break;
        default:
            break;
        }
    }
}

/**
 ****************************************************************************************
 * @brief UART3 RX interrupt handler.
 * @description
 * Receive data from port UART3 until expected receive data size reduce to zero.
 ****************************************************************************************
 */
void UART3_IRQHandler(void) 
{
    while ((LPC_UART3->IIR & 0x01) == 0)
    {
        switch (LPC_UART3->IIR & 0x0E)
        {
        case 0x04:	// Receive Data Available (RDA)
        case 0x0C:	// Character Time-out Indicator (CTI)
            while ((LPC_UART3->LSR & 0x01) == 0x01)
            {
                if (uart3_env.rx.size > 0)
                {
                    *uart3_env.rx.bufptr++ = LPC_UART3->RBR;
                    uart3_env.rx.size--;
                }
                
                if (uart3_env.rx.size == 0)
                {
                    // Disable UART all rx int
                    uart_rx_int_enable(QN_UART3, MASK_DISABLE);
                    // Call end of reception callback
                    if(uart3_env.rx.callback != NULL)
                    {
                        uart3_env.rx.callback();
                    }
                }	
            }
            break;
        default:
            break;
        }
    }
}

#ifdef CFG_STD_PRINTF
/**
 ****************************************************************************************
 * @brief Receive a byte from Uart.
 * @param[in]       UART          QN_UART1 or QN_UART3
 * @description
 * Read a byte from specified UART port
 *****************************************************************************************
 */
static unsigned char uart_recv_byte(QN_UART_TypeDef *UART)
{
    unsigned char c = 0;

    if (QN_UART1 == UART)
    {
        while (!((LPC_UART1->LSR & 0x01) == 0x01));
        c = (LPC_UART1->RBR);        
    }
    else 
    if (QN_UART3 == UART)
    {
        while (!((LPC_UART3->LSR & 0x01) == 0x01));
        c = (LPC_UART3->RBR);   
    }

    return c;
}

/**
 ****************************************************************************************
 * @brief Output a character.
 * @description
 * Output a character to uart debug port.
 ****************************************************************************************
 */
unsigned char UartPutc(unsigned char my_ch)
{
    uart_printf(QN_DEBUG_UART, (uint8_t *)&my_ch);
    return (my_ch);
}

/**
 ****************************************************************************************
 * @brief Get a character.
 * @description
 * Get a character form uart debug port.
 ****************************************************************************************
 */
unsigned char UartGetc(void)
{
    return uart_recv_byte(QN_DEBUG_UART);
}
#endif

