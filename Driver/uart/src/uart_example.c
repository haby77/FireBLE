/****************************************************************************
 *   $Id:: uart_example.c                                                   $
 *   Project: NXP QN9020 UART example
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

/**
 * @brief Uart excample
 */
int main (void)
{
    uint8_t buffer[256];

    SystemInit();

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

    //Print out "Hello NXP!\n" thought uart.
    uart_printf(QN_UART0, (uint8_t *)"Hello NXP!\n");
//     uart_printf(QN_UART1, "Hello NXP!\n");

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

