/****************************************************************************
 *   $Id:: dma_example.c                                                   $
 *   Project: NXP QN9020 DMA example
 *
 *   Description:
 *     This file contains DMA driver usage.
 *
****************************************************************************/

#include "dma.h"
#include "uart.h"
#include "spi.h"
#include "system.h"


/****************************************************************************
*               DMA driver code example
* There is only one DMA channel
*
* Note: DMA don't support SPI RX in master mode.
*
****************************************************************************/



volatile uint8_t done = FALSE;

void dma_done(void)
{
    done = TRUE;
}


int main(void)
{

    uint32_t i;
    uint8_t txbuffer[256], rxbuffer[256];
    
    SystemInit();  

#if (__AHB_CLK == 32000UL)    
    uart_init(QN_UART0, __USART_CLK, UART_1200);
#else
    uart_init(QN_UART0, __USART_CLK, UART_115200);
#endif
    uart_tx_enable(QN_UART0, MASK_ENABLE);
    uart_rx_enable(QN_UART0, MASK_ENABLE);
    uart_printf(QN_UART0, (uint8_t *)"Hello NXP!!!\n");
    
    for (i = 0; i < 256; i++) {
        txbuffer[i] = 0x55 + i;
        rxbuffer[i] = 0x00;
    }
    
    dma_init();

    /* Memory to Memory */
    done = FALSE;
    dma_memory_copy((uint32_t)txbuffer, (uint32_t)rxbuffer, 256, dma_done);
    while(done == FALSE);
    
    /* Memory to Peripheral(UART0) */
    done = FALSE;
    dma_tx(DMA_TRANS_BYTE, (uint32_t)txbuffer, DMA_UART0_TX, 20, dma_done);
    while(done == FALSE);

    /* Peripheral(UART0) to Memory */
    done = FALSE;
    dma_rx(DMA_TRANS_BYTE, DMA_UART0_RX, (uint32_t)rxbuffer, 20, dma_done);
    while(done == FALSE);

    /**
     * Peripheral(UART0-RX) to Peripheral(UART0-TX)
     * Peropheral to Peripheral only support word width.
     */
    done = FALSE;
    dma_transfer(DMA_UART0_RX, DMA_UART0_TX, 40, dma_done);
#if DMA_UNDEFINE_LENGTH_EN==TRUE    
    // if undefine transfer data len, then stop by abort command
    dma_abort();
#endif
    while(done == FALSE);
    

    while(1);
}

