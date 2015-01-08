/****************************************************************************
 *   $Id:: uart_example.c                                                   $
 *   Project: QUINTIC QN9020 UART example
 *
 *   Description:
 *     This file contains UART driver usage.
 *
****************************************************************************/

#include "uart.h"
#include "system.h"

volatile uint8_t rx_flag = 0;
volatile uint8_t tx_flag = 0;


void led_blink_left(void)
{
    rx_flag = 0;
}

void led_blink_right(void)
{
    tx_flag = 0;
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

/**
 * @brief Uart excample
 */
int main (void)
{
    uint8_t buffer[256];

    SystemInit();

    //UART io configurate
    uart_io_config();

#if __AHB_CLK == 32000UL
    //Initialize uart0 with 1200 baudrate, 8bit data, 1 stop bit, no parity, LSB bitorder, no HW flow control.
    uart_init(QN_UART0, __USART_CLK, UART_1200);
#else
    //Initialize uart0 with 115200 baudrate, 8bit data, 1 stop bit, no parity, LSB bitorder, no HW flow control.
    uart_init(QN_UART0, __USART_CLK, UART_115200);
    //uart_init(QN_UART0, __USART_CLK, UART_57600);
#endif
    uart_tx_enable(QN_UART0, MASK_ENABLE);
    uart_rx_enable(QN_UART0, MASK_ENABLE);

    //Initialize uart1 with 115200 baudrate, 8bit data, 1 stop bit, no parity, LSB bitorder, no HW flow control.
//     uart_init(QN_UART1, __USART_CLK, UART_115200);
//     uart_tx_enable(QN_UART1, MASK_ENABLE);
//     uart_rx_enable(QN_UART1, MASK_ENABLE);

    //Print out "Hello Quintic!\n" thought uart.
    uart_printf(QN_UART0, (uint8_t *)"Hello Quintic!\n");
//     uart_printf(QN_UART1, "Hello Quintic!\n");

    while (1)                                /* Loop forever */
    {
        //uart0 receive data
        rx_flag = 1;
        uart_read(QN_UART0, buffer, 10, led_blink_left);
        while (rx_flag == 1);

        //uart0 send data
        tx_flag = 1;
        uart_write(QN_UART0, buffer, 10, led_blink_right);
        while (tx_flag == 1);
    }
}

