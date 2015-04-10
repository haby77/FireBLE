/****************************************************************************
 *   $Id:: dma_example.c                                                   $
 *   Project: QUINTIC QN9020 DMA example
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

void uart_spi_io_config(void)
{
    // pin mux
    syscon_SetPMCR0(QN_SYSCON, P07_SW_CLK_PIN_CTRL       
                             | P06_SW_DAT_PIN_CTRL       
                             | P00_UART0_TXD_PIN_CTRL    // P0.0 uart0 tx 
                             | P17_UART0_RXD_PIN_CTRL    // P1.7 uart0 rx
                             | P13_SPI1_CLK_PIN_CTRL     // P1.3 spi1 clk
                             | P12_SPI1_CS0_PIN_CTRL     // P1.2 spi1 cs
                             | P11_SPI1_DAT_PIN_CTRL     // P1.1 spi1 data out 
                             | P10_SPI1_DIN_PIN_CTRL     // P1.0 spi1 data in
                             );
    syscon_SetPMCR1(QN_SYSCON, P21_UART1_TXD_PIN_CTRL    // P2.1 uart1 tx
                             | P20_UART1_RXD_PIN_CTRL    // P2.0 uart1 rx   
                             | P35_SPI0_CS0_PIN_CTRL     // P3.5 spi0 cs 
                             | P34_SPI0_CLK_PIN_CTRL     // P3.4 spi0 clk
                             | P33_SPI0_DAT_PIN_CTRL     // P3.3 spi0 data out
                             | P32_SPI0_DIN_PIN_CTRL     // P3.2 spi0 data in
                             );

    // pin select
    syscon_SetPMCR2(QN_SYSCON, SYSCON_MASK_UART1_PIN_SEL|SYSCON_MASK_SPI0_PIN_SEL);   

    // pin pull ( 00 : High-Z,  01 : Pull-down,  10 : Pull-up,  11 : Reserved )
    syscon_SetPPCR0(QN_SYSCON, 0xAAAA5AAA);
    syscon_SetPPCR1(QN_SYSCON, 0x2AAAAAAA);
}


int main(void)
{

    uint32_t i;
    uint8_t txbuffer[256], rxbuffer[256];
    
    SystemInit();  


    uart_spi_io_config();
#if (__AHB_CLK == 32000UL)    
    uart_init(QN_UART0, __USART_CLK, UART_1200);
#else
    uart_init(QN_UART0, __USART_CLK, UART_115200);
#endif
    uart_tx_enable(QN_UART0, MASK_ENABLE);
    uart_rx_enable(QN_UART0, MASK_ENABLE);
    uart_printf(QN_UART0, (uint8_t *)"Hello Quintic!!!\n");

    spi_init(QN_SPI1, SPI_BITRATE(1000000), SPI_32BIT, SPI_MASTER_MOD);

    for (i = 0; i < 256; i++) {
        txbuffer[i] = 0x55 + i;
        rxbuffer[i] = 0x00;
    }
        
    
    dma_init();

    done = FALSE;
    dma_memory_copy((uint32_t)txbuffer, (uint32_t)rxbuffer, 256, dma_done);
    while(done == FALSE);

    done = FALSE;
    dma_memory_copy((uint32_t)(txbuffer+1), (uint32_t)rxbuffer, 256, dma_done);
    while(done == FALSE);

    done = FALSE;
    dma_memory_copy((uint32_t)(txbuffer+2), (uint32_t)rxbuffer, 256, dma_done);
    while(done == FALSE);

    done = FALSE;
    dma_memory_copy((uint32_t)txbuffer, (uint32_t)(rxbuffer+3), 256, dma_done);
    while(done == FALSE);

    done = FALSE;
    dma_tx(DMA_TRANS_BYTE, (uint32_t)txbuffer, DMA_UART0_TX, 20, dma_done);
    while(done == FALSE);

    done = FALSE;
    dma_tx(DMA_TRANS_BYTE, (uint32_t)txbuffer, DMA_SPI1_TX, 20, dma_done);
    while(done == FALSE);

    done = FALSE;
    dma_rx(DMA_TRANS_HALF_WORD, DMA_UART0_RX, (uint32_t)rxbuffer, 20, dma_done);
    //dma_rx(DMA_TRANS_WORD, DMA_UART0_RX, (uint32_t)rxbuffer, 20, dma_done);
    while(done == FALSE);


    done = FALSE;
    dma_transfer(DMA_UART0_RX, DMA_UART0_TX, 40, dma_done);
#if DMA_UNDEFINE_LENGTH_EN==TRUE    
    // if undefine transfer data len, then stop by abort command
    dma_abort();
#endif
    while(done == FALSE);
    

    while(1);
}

