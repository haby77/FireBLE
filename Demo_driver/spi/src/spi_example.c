/****************************************************************************
 *   $Id:: spi_example.c                                                   $
 *   Project: QUINTIC QN9020 SPI example
 *
 *   Description:
 *     This file contains SPI driver usage.
 *
****************************************************************************/

#include "system.h"
#include "spi.h"


/****************************************************************************
*               SPI driver code example
*SPI driver has the master and slave mode, which are use the same API.
*
*
*Note: 
*   The max speed of SPI should no more than (__USART_CLK/4)
*   The size of transmission should be multiple of 4 at 32 bits mode.
*
****************************************************************************/


volatile uint8_t rx_flag = 0;
volatile uint8_t tx_flag = 0;

// configure SPI test mode: 1: master, 0: slave
#define CFG_SPI_TEST_MODE_MASTER_EN     1
// configure SPI test port: 1: QN_SPI1, 0: QN_SPI0
#define CFG_SPI_TEST_PORT_NUM           0

#if CFG_SPI_TEST_PORT_NUM
#define SPI_TEST_PORT               QN_SPI1
#else 
#define SPI_TEST_PORT               QN_SPI0
#endif

void spi_rx_cb(void)
{
    rx_flag = 0;
}

void spi_tx_cb(void)
{
    tx_flag = 0;
}

/*  
 * Main Program
 */
int main (void)
{
    uint32_t j;
    uint8_t txbuffer[40];
    uint8_t rxbuffer[40];
    
    SystemInit();
    
    for (j = 0; j < 40; j++) {
        rxbuffer[j] = 0;
        txbuffer[j] = 0x55 + j;
    }

#if CFG_SPI_TEST_MODE_MASTER_EN    
    
    //Initialize SPI
    spi_init(SPI_TEST_PORT, SPI_BITRATE(1000000), SPI_8BIT, SPI_MASTER_MOD);

    
    #if 1  // full-duplex, interrupt should be enabled
    rx_flag = 1;
    
    spi_write(SPI_TEST_PORT, txbuffer, 8, spi_tx_cb);
    spi_read(SPI_TEST_PORT,  rxbuffer, 8, spi_rx_cb);
    spi_int_enable(SPI_TEST_PORT, SPI_TX_INT|SPI_RX_INT, MASK_ENABLE); 
    
    while(rx_flag);
    
    #else  // half-duplex
    
    //Write 8byte data
    tx_flag = 1;
    spi_write(SPI_TEST_PORT, txbuffer, 8, spi_tx_cb);
    spi_int_enable(SPI_TEST_PORT, SPI_TX_INT, MASK_ENABLE); 
    while(tx_flag);
    
    // wait for slave ready
    delay(100); 
    
    //Read 8byte data
    rx_flag = 1;
    spi_read(SPI_TEST_PORT,  rxbuffer, 8, spi_rx_cb);
    spi_int_enable(SPI_TEST_PORT, SPI_RX_INT, MASK_ENABLE); 
    while(rx_flag);
    #endif
    
#else

    //Initialize SPI
    spi_init(SPI_TEST_PORT, SPI_BITRATE(1000000), SPI_8BIT, SPI_SLAVE_MOD);
    tx_flag = 1;

    spi_read(SPI_TEST_PORT,  rxbuffer, 8, spi_rx_cb);
    spi_write(SPI_TEST_PORT, txbuffer, 8, spi_tx_cb);
    spi_int_enable(SPI_TEST_PORT, SPI_TX_INT|SPI_RX_INT, MASK_ENABLE);

    while(tx_flag);
#endif
    
    spi_clock_off(SPI_TEST_PORT);
    
    while (1)                             /* Loop forever */
    {
        
    }
}

